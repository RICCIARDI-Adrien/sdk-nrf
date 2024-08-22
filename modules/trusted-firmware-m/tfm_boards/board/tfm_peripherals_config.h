/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef TFM_PERIPHERALS_CONFIG_H__
#define TFM_PERIPHERALS_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <autoconf.h>
#include <nrfx.h>

#define TFM_PERIPHERAL_DCNF_SECURE CONFIG_NRF_DCNF_SECURE

#define TFM_PERIPHERAL_FPU_SECURE CONFIG_NRF_FPU_SECURE

#define TFM_PERIPHERAL_OSCILLATORS_SECURE CONFIG_NRF_OSCILLATORS_SECURE

#define TFM_PERIPHERAL_REGULATORS_SECURE CONFIG_NRF_REGULATORS_SECURE

#define TFM_PERIPHERAL_CLOCK_SECURE CONFIG_NRF_CLOCK_SECURE

#define TFM_PERIPHERAL_POWER_SECURE CONFIG_NRF_POWER_SECURE

#define TFM_PERIPHERAL_RESET_SECURE CONFIG_NRF_RESET_SECURE

#define TFM_PERIPHERAL_SPIM0_SECURE CONFIG_NRF_SPIM0_SECURE
#define TFM_PERIPHERAL_SPIM00_SECURE CONFIG_NRF_SPIM00_SECURE
#define TFM_PERIPHERAL_SPIM20_SECURE CONFIG_NRF_SPIM20_SECURE
#define TFM_PERIPHERAL_SPIM21_SECURE CONFIG_NRF_SPIM21_SECURE
#define TFM_PERIPHERAL_SPIM22_SECURE CONFIG_NRF_SPIM22_SECURE
#define TFM_PERIPHERAL_SPIM30_SECURE CONFIG_NRF_SPIM30_SECURE

#define TFM_PERIPHERAL_SPIS0_SECURE CONFIG_NRF_SPIS0_SECURE

#define TFM_PERIPHERAL_TWIM0_SECURE CONFIG_NRF_TWIM0_SECURE

#define TFM_PERIPHERAL_TWIS0_SECURE CONFIG_NRF_TWIS0_SECURE

#define TFM_PERIPHERAL_UARTE0_SECURE CONFIG_NRF_UARTE0_SECURE

#define TFM_PERIPHERAL_SPIM1_SECURE CONFIG_NRF_SPIM1_SECURE

#define TFM_PERIPHERAL_SPIS1_SECURE CONFIG_NRF_SPIS1_SECURE

#define TFM_PERIPHERAL_TWIM1_SECURE CONFIG_NRF_TWIM1_SECURE

#define TFM_PERIPHERAL_TWIS1_SECURE CONFIG_NRF_TWIS1_SECURE

#define TFM_PERIPHERAL_UARTE1_SECURE CONFIG_NRF_UARTE1_SECURE

#define TFM_PERIPHERAL_SPIM4_SECURE CONFIG_NRF_SPIM4_SECURE

#define TFM_PERIPHERAL_SPIM2_SECURE CONFIG_NRF_SPIM2_SECURE

#define TFM_PERIPHERAL_SPIS2_SECURE CONFIG_NRF_SPIS2_SECURE

#define TFM_PERIPHERAL_TWIM2_SECURE CONFIG_NRF_TWIM2_SECURE

#define TFM_PERIPHERAL_TWIS2_SECURE CONFIG_NRF_TWIS2_SECURE

#define TFM_PERIPHERAL_UARTE2_SECURE CONFIG_NRF_UARTE2_SECURE

#define TFM_PERIPHERAL_SPIM3_SECURE CONFIG_NRF_SPIM3_SECURE

#define TFM_PERIPHERAL_SPIS3_SECURE CONFIG_NRF_SPIS3_SECURE

#define TFM_PERIPHERAL_TWIM3_SECURE CONFIG_NRF_TWIM3_SECURE

#define TFM_PERIPHERAL_TWIS3_SECURE CONFIG_NRF_TWIS3_SECURE

#define TFM_PERIPHERAL_UARTE3_SECURE CONFIG_NRF_UARTE3_SECURE

#define TFM_PERIPHERAL_UARTE00_SECURE CONFIG_NRF_UARTE00_SECURE

#define TFM_PERIPHERAL_UARTE20_SECURE CONFIG_NRF_UARTE20_SECURE

#define TFM_PERIPHERAL_UARTE21_SECURE CONFIG_NRF_UARTE21_SECURE

#define TFM_PERIPHERAL_UARTE22_SECURE CONFIG_NRF_UARTE22_SECURE

#define TFM_PERIPHERAL_UARTE30_SECURE CONFIG_NRF_UARTE30_SECURE

#define TFM_PERIPHERAL_SAADC_SECURE CONFIG_NRF_SAADC_SECURE

#define TFM_PERIPHERAL_TIMER0_SECURE CONFIG_NRF_TIMER0_SECURE
#define TFM_PERIPHERAL_TIMER1_SECURE CONFIG_NRF_TIMER1_SECURE
#define TFM_PERIPHERAL_TIMER2_SECURE CONFIG_NRF_TIMER2_SECURE
#define TFM_PERIPHERAL_TIMER00_SECURE CONFIG_NRF_TIMER00_SECURE
#define TFM_PERIPHERAL_TIMER10_SECURE CONFIG_NRF_TIMER10_SECURE
#define TFM_PERIPHERAL_TIMER20_SECURE CONFIG_NRF_TIMER20_SECURE
#define TFM_PERIPHERAL_TIMER21_SECURE CONFIG_NRF_TIMER21_SECURE
#define TFM_PERIPHERAL_TIMER22_SECURE CONFIG_NRF_TIMER22_SECURE
#define TFM_PERIPHERAL_TIMER23_SECURE CONFIG_NRF_TIMER23_SECURE
#define TFM_PERIPHERAL_TIMER24_SECURE CONFIG_NRF_TIMER24_SECURE

#define TFM_PERIPHERAL_RTC0_SECURE CONFIG_NRF_RTC0_SECURE

#define TFM_PERIPHERAL_RTC1_SECURE CONFIG_NRF_RTC1_SECURE

#define TFM_PERIPHERAL_DPPI_SECURE CONFIG_NRF_DPPI_SECURE

#ifdef NRF_WDT0
#define TFM_PERIPHERAL_WDT0_SECURE CONFIG_NRF_WDT0_SECURE
#else
#define TFM_PERIPHERAL_WDT_SECURE CONFIG_NRF_WDT0_SECURE
#endif

#define TFM_PERIPHERAL_WDT1_SECURE CONFIG_NRF_WDT1_SECURE

#define TFM_PERIPHERAL_COMP_SECURE CONFIG_NRF_COMP_SECURE

#define TFM_PERIPHERAL_LPCOMP_SECURE CONFIG_NRF_LPCOMP_SECURE

#define TFM_PERIPHERAL_EGU0_SECURE CONFIG_NRF_EGU0_SECURE
#define TFM_PERIPHERAL_EGU1_SECURE CONFIG_NRF_EGU1_SECURE
#define TFM_PERIPHERAL_EGU2_SECURE CONFIG_NRF_EGU2_SECURE
#define TFM_PERIPHERAL_EGU3_SECURE CONFIG_NRF_EGU3_SECURE
#define TFM_PERIPHERAL_EGU4_SECURE CONFIG_NRF_EGU4_SECURE
#define TFM_PERIPHERAL_EGU5_SECURE CONFIG_NRF_EGU5_SECURE
#define TFM_PERIPHERAL_EGU10_SECURE CONFIG_NRF_EGU10_SECURE
#define TFM_PERIPHERAL_EGU20_SECURE CONFIG_NRF_EGU20_SECURE

#define TFM_PERIPHERAL_PWM0_SECURE CONFIG_NRF_PWM0_SECURE
#define TFM_PERIPHERAL_PWM1_SECURE CONFIG_NRF_PWM1_SECURE
#define TFM_PERIPHERAL_PWM2_SECURE CONFIG_NRF_PWM2_SECURE
#define TFM_PERIPHERAL_PWM3_SECURE CONFIG_NRF_PWM3_SECURE
#define TFM_PERIPHERAL_PWM20_SECURE CONFIG_NRF_PWM20_SECURE
#define TFM_PERIPHERAL_PWM21_SECURE CONFIG_NRF_PWM21_SECURE
#define TFM_PERIPHERAL_PWM22_SECURE CONFIG_NRF_PWM22_SECURE

#ifdef NRF_PDM0
#define TFM_PERIPHERAL_PDM0_SECURE CONFIG_NRF_PDM_SECURE
#else
#define TFM_PERIPHERAL_PDM_SECURE CONFIG_NRF_PDM_SECURE
#endif

#ifdef NRF_I2S0
#define TFM_PERIPHERAL_I2S0_SECURE CONFIG_NRF_I2S_SECURE
#else
#define TFM_PERIPHERAL_I2S_SECURE CONFIG_NRF_I2S_SECURE
#endif

#define TFM_PERIPHERAL_IPC_SECURE CONFIG_NRF_IPC_SECURE

#define TFM_PERIPHERAL_QSPI_SECURE CONFIG_NRF_QSPI_SECURE

#define TFM_PERIPHERAL_NFCT_SECURE CONFIG_NRF_NFCT_SECURE

#define TFM_PERIPHERAL_MUTEX_SECURE CONFIG_NRF_MUTEX_SECURE

#define TFM_PERIPHERAL_QDEC0_SECURE CONFIG_NRF_QDEC0_SECURE

#define TFM_PERIPHERAL_QDEC1_SECURE CONFIG_NRF_QDEC1_SECURE

#define TFM_PERIPHERAL_USBD_SECURE CONFIG_NRF_USBD_SECURE

#define TFM_PERIPHERAL_USBREG_SECURE CONFIG_NRF_USBREG_SECURE

#define TFM_PERIPHERAL_NVMC_SECURE CONFIG_NRF_NVMC_SECURE

#define TFM_PERIPHERAL_GPIOTE0_SECURE CONFIG_NRF_GPIOTE0_SECURE

#define TFM_PERIPHERAL_GPIO0_SECURE CONFIG_NRF_GPIO0_SECURE
#define TFM_PERIPHERAL_GPIO1_SECURE CONFIG_NRF_GPIO1_SECURE
#define TFM_PERIPHERAL_GPIO2_SECURE CONFIG_NRF_GPIO2_SECURE

#define TFM_PERIPHERAL_VMC_SECURE CONFIG_NRF_VMC_SECURE

#if defined(CONFIG_NRF_GPIO0_PIN_MASK_SECURE)
#define TFM_PERIPHERAL_GPIO0_PIN_MASK_SECURE CONFIG_NRF_GPIO0_PIN_MASK_SECURE
#if ENABLE_TRACE
#if CONFIG_NRF_GPIO0_PIN_MASK_SECURE &                                                             \
	(TAD_PSEL_TRACECLK_PIN_Msk | TAD_PSEL_TRACEDATA0_PIN_Msk | TAD_PSEL_TRACEDATA1_PIN_Msk |   \
	 TAD_PSEL_TRACEDATA2_PIN_Msk | TAD_PSEL_TRACEDATA3_PIN_Msk)
#error Trace pins must be configured as non-secure.
#endif
#endif
#endif

#if defined(CONFIG_NRF_GPIO1_PIN_MASK_SECURE)
#define TFM_PERIPHERAL_GPIO1_PIN_MASK_SECURE CONFIG_NRF_GPIO1_PIN_MASK_SECURE
#endif

#if defined(CONFIG_NRF_DPPI_CHANNEL_MASK_SECURE)
#define TFM_PERIPHERAL_DPPI_CHANNEL_MASK_SECURE CONFIG_NRF_DPPI_CHANNEL_MASK_SECURE
#endif

#ifdef __cplusplus
}
#endif

#endif /* TFM_PERIPHERAL_CONFIG_H__ */
