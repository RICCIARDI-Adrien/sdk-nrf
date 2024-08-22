/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <nrf_rpc.h>
#include <nrf_rpc_tr.h>
#include <nrf_rpc/nrf_rpc_uart.h>
#include <nrf_rpc_errno.h>

#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/crc.h>

LOG_MODULE_REGISTER(nrf_rpc_uart, CONFIG_NRF_RPC_TR_LOG_LEVEL);

enum {
	HDLC_CHAR_ESCAPE = 0x7d,
	HDLC_CHAR_DELIMITER = 0x7e,
};

typedef enum {
	HDLC_STATE_UNSYNC,
	HDLC_STATE_FRAME_START,
	HDLC_STATE_FRAME_FOUND,
	HDLC_STATE_ESCAPE,
} hdlc_state_t;

struct nrf_rpc_uart {
	const struct device *uart;
	nrf_rpc_tr_receive_handler_t receive_callback;
	void *receive_ctx;
	const struct nrf_rpc_tr *transport;

	/* RX ring buffer populated by UART ISR */
	uint8_t rx_buffer[CONFIG_NRF_RPC_UART_RX_RINGBUF_SIZE];
	struct ring_buf rx_ringbuf;

	/* RX work to consume and decode bytes from RX ring buffer */
	struct k_work rx_work;
	struct k_work_q rx_workq;

	K_KERNEL_STACK_MEMBER(rx_workq_stack, CONFIG_NRF_RPC_UART_RX_THREAD_STACK_SIZE);

	/* HDLC frame parsing state */
	hdlc_state_t hdlc_state;
	uint8_t rx_packet[CONFIG_NRF_RPC_UART_MAX_PACKET_SIZE];
	size_t rx_packet_len;

	/* TX lock */
	struct k_mutex tx_lock;
};

#define CRC_SIZE sizeof(uint16_t)

static int hdlc_decode_byte(struct nrf_rpc_uart *uart_tr, uint8_t byte)
{
	/* TODO: handle frame buffer overflow */

	if (uart_tr->hdlc_state == HDLC_STATE_ESCAPE) {
		uart_tr->rx_packet[uart_tr->rx_packet_len++] = byte ^ 0x20;
		uart_tr->hdlc_state = HDLC_STATE_FRAME_START;
	} else if (byte == HDLC_CHAR_ESCAPE) {
		uart_tr->hdlc_state = HDLC_STATE_ESCAPE;
	} else if (byte == HDLC_CHAR_DELIMITER) {
		uart_tr->hdlc_state = (uart_tr->hdlc_state == HDLC_STATE_FRAME_START)
					      ? HDLC_STATE_FRAME_FOUND
					      : HDLC_STATE_UNSYNC;
	} else {
		uart_tr->rx_packet[uart_tr->rx_packet_len++] = byte;
		uart_tr->hdlc_state = HDLC_STATE_FRAME_START;
	}

	if (uart_tr->hdlc_state == HDLC_STATE_FRAME_FOUND) {
		LOG_HEXDUMP_DBG(uart_tr->rx_packet, uart_tr->rx_packet_len, "Received frame");
		if (uart_tr->rx_packet_len > CRC_SIZE) {
			uart_tr->rx_packet_len -= CRC_SIZE;
		} else {
			uart_tr->rx_packet_len = 0;
			uart_tr->hdlc_state = HDLC_STATE_UNSYNC;
		}
	}

	return 0;
}

static void work_handler(struct k_work *work)
{
	struct nrf_rpc_uart *uart_tr = CONTAINER_OF(work, struct nrf_rpc_uart, rx_work);
	const struct nrf_rpc_tr *transport = uart_tr->transport;
	uint8_t *data;
	size_t len;
	int ret;
	uint16_t crc_received = 0;
	uint16_t crc_calculated = 0;

	while (!ring_buf_is_empty(&uart_tr->rx_ringbuf)) {
		len = ring_buf_get_claim(&uart_tr->rx_ringbuf, &data,
					 CONFIG_NRF_RPC_UART_MAX_PACKET_SIZE);
		for (size_t i = 0; i < len; i++) {
			if (hdlc_decode_byte(uart_tr, data[i]) != 0) {
				continue;
			}

			if (uart_tr->hdlc_state != HDLC_STATE_FRAME_FOUND) {
				continue;
			}

			crc_received = sys_get_le16(uart_tr->rx_packet + uart_tr->rx_packet_len);
			crc_calculated =
				crc16_ccitt(0xffff, uart_tr->rx_packet, uart_tr->rx_packet_len);
			if (crc_calculated != crc_received) {
				LOG_ERR("Invalid CRC, calculated %x received %x", crc_calculated,
					crc_received);
				uart_tr->rx_packet_len = 0;
				uart_tr->hdlc_state = HDLC_STATE_UNSYNC;
				return;
			}

			uart_tr->receive_callback(transport, uart_tr->rx_packet,
						  uart_tr->rx_packet_len, uart_tr->receive_ctx);
			uart_tr->rx_packet_len = 0;
			uart_tr->hdlc_state = HDLC_STATE_UNSYNC;
		}

		ret = ring_buf_get_finish(&uart_tr->rx_ringbuf, len);
		if (ret < 0) {
			LOG_DBG("Cannot flush ring buffer (%d)", ret);
		}
	}
}

static void serial_cb(const struct device *uart, void *user_data)
{
	struct nrf_rpc_uart *uart_tr = user_data;
	uint32_t rx_len;
	uint8_t *rx_buffer;
	bool new_data = false;

	while (uart_irq_update(uart) && uart_irq_rx_ready(uart)) {
		rx_len = ring_buf_put_claim(&uart_tr->rx_ringbuf, &rx_buffer,
					    uart_tr->rx_ringbuf.size);
		if (rx_len > 0) {
			rx_len = uart_fifo_read(uart, rx_buffer, rx_len);
			int err = ring_buf_put_finish(&uart_tr->rx_ringbuf, rx_len);
			(void)err; /*silence the compiler*/
			__ASSERT_NO_MSG(err == 0);
			if (rx_len <= 0) {
				continue;
			} else {
				new_data = true;
			}
		} else {
			uint8_t dummy;

			/* No space in the ring buffer - consume byte. */
			LOG_WRN("RX ring buffer full.");

			rx_len = uart_fifo_read(uart, &dummy, 1);
		}
	}

	if (new_data) {
		k_work_submit_to_queue(&uart_tr->rx_workq, &uart_tr->rx_work);
	}
}

static int init(const struct nrf_rpc_tr *transport, nrf_rpc_tr_receive_handler_t receive_cb,
		void *context)
{
	struct nrf_rpc_uart *uart_tr = transport->ctx;

	if (uart_tr->transport != NULL) {
		LOG_DBG("init not needed");
		return 0;
	}

	uart_tr->transport = transport;

	LOG_DBG("init called");

	if (receive_cb == NULL) {
		LOG_ERR("No transport receive callback");
		return -NRF_EINVAL;
	}
	uart_tr->receive_callback = receive_cb;
	uart_tr->receive_ctx = context;

	if (!device_is_ready(uart_tr->uart)) {
		LOG_ERR("UART device not found!");
		return -NRF_ENOENT;
	}

	/* configure interrupt and callback to receive data */
	int ret = uart_irq_callback_user_data_set(uart_tr->uart, serial_cb, uart_tr);

	if (ret < 0) {
		if (ret == -ENOTSUP) {
			LOG_ERR("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			LOG_ERR("UART device does not support interrupt-driven API\n");
		} else {
			LOG_ERR("Error setting UART callback: %d\n", ret);
		}
		return 0;
	}

	k_mutex_init(&uart_tr->tx_lock);

	k_work_queue_init(&uart_tr->rx_workq);
	k_work_queue_start(&uart_tr->rx_workq, uart_tr->rx_workq_stack,
			   K_THREAD_STACK_SIZEOF(uart_tr->rx_workq_stack), K_PRIO_PREEMPT(0), NULL);

	k_work_init(&uart_tr->rx_work, work_handler);
	ring_buf_init(&uart_tr->rx_ringbuf, sizeof(uart_tr->rx_buffer), uart_tr->rx_buffer);

	uart_tr->hdlc_state = HDLC_STATE_UNSYNC;
	uart_tr->rx_packet_len = 0;
	uart_irq_rx_enable(uart_tr->uart);

	return 0;
}

static void send_byte(const struct device *dev, uint8_t byte)
{
	if (byte == HDLC_CHAR_DELIMITER || byte == HDLC_CHAR_ESCAPE) {
		uart_poll_out(dev, HDLC_CHAR_ESCAPE);
		byte ^= 0x20;
	}

	uart_poll_out(dev, byte);
}

static int send(const struct nrf_rpc_tr *transport, const uint8_t *data, size_t length)
{
	uint8_t crc[2];
	struct nrf_rpc_uart *uart_tr = transport->ctx;

	LOG_HEXDUMP_DBG(data, length, "Sending frame");

	k_mutex_lock(&uart_tr->tx_lock, K_FOREVER);

	uart_poll_out(uart_tr->uart, HDLC_CHAR_DELIMITER);

	for (size_t i = 0; i < length; i++) {
		send_byte(uart_tr->uart, data[i]);
	}

	sys_put_le16(crc16_ccitt(0xffff, data, length), crc);
	send_byte(uart_tr->uart, crc[0]);
	send_byte(uart_tr->uart, crc[1]);

	uart_poll_out(uart_tr->uart, HDLC_CHAR_DELIMITER);

	k_free((void *)data);

	k_mutex_unlock(&uart_tr->tx_lock);

	return 0;
}

static void *tx_buf_alloc(const struct nrf_rpc_tr *transport, size_t *size)
{
	void *data = NULL;

	data = k_malloc(*size);
	if (!data) {
		LOG_ERR("Failed to allocate Tx buffer.");
		goto error;
	}

	return data;

error:
	/* It should fail to avoid writing to NULL buffer. */
	k_oops();
	*size = 0;
	return NULL;
}

static void tx_buf_free(const struct nrf_rpc_tr *transport, void *buf)
{
	ARG_UNUSED(transport);

	k_free(buf);
}

const struct nrf_rpc_tr_api nrf_rpc_uart_service_api = {
	.init = init,
	.send = send,
	.tx_buf_alloc = tx_buf_alloc,
	.tx_buf_free = tx_buf_free,
};

#define NRF_RPC_UART_INSTANCE(node_id) _CONCAT(nrf_rpc_inst_, DT_DEP_ORD(node_id))

#define NRF_RPC_UART_TRANSPORT_DEFINE(node_id)                                                     \
	struct nrf_rpc_uart NRF_RPC_UART_INSTANCE(node_id) = {                                     \
		.uart = DEVICE_DT_GET(node_id),                                                    \
		.receive_callback = NULL,                                                          \
		.transport = NULL,                                                                 \
	};                                                                                         \
	const struct nrf_rpc_tr NRF_RPC_UART_TRANSPORT(node_id) = {                                \
		.api = &nrf_rpc_uart_service_api,                                                  \
		.ctx = &NRF_RPC_UART_INSTANCE(node_id),                                            \
	};

DT_FOREACH_STATUS_OKAY(nordic_nrf_uarte, NRF_RPC_UART_TRANSPORT_DEFINE);
