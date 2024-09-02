/**
 * \file
 * \brief ST,NUCLEO-U575ZI-Q (ST,STM32U575ZI chip) vector table, default handlers and NVIC low-level initializer
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/chip/CMSIS-proxy.h"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// single exception vector - pointer to function with no arguments and no return value
using ExceptionVector = void(*)();

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// priority of interrupts
#if defined(DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI};
#else	// !defined(DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {};
#endif	// !defined(DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI)

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Low-level peripheral initializer for NVIC
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void nvicLowLevelInitializer()
{
#ifdef DISTORTOS_ARCHITECTURE_NVIC_WWDG_ENABLE
	NVIC_SetPriority(WWDG_IRQn, interruptPriority);
	NVIC_EnableIRQ(WWDG_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_WWDG_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_PVD_PVM_ENABLE
	NVIC_SetPriority(PVD_PVM_IRQn, interruptPriority);
	NVIC_EnableIRQ(PVD_PVM_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_PVD_PVM_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RTC_ENABLE
	NVIC_SetPriority(RTC_IRQn, interruptPriority);
	NVIC_EnableIRQ(RTC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RTC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RTC_S_ENABLE
	NVIC_SetPriority(RTC_S_IRQn, interruptPriority);
	NVIC_EnableIRQ(RTC_S_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RTC_S_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TAMP_ENABLE
	NVIC_SetPriority(TAMP_IRQn, interruptPriority);
	NVIC_EnableIRQ(TAMP_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TAMP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RAMCFG_ENABLE
	NVIC_SetPriority(RAMCFG_IRQn, interruptPriority);
	NVIC_EnableIRQ(RAMCFG_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RAMCFG_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FLASH_ENABLE
	NVIC_SetPriority(FLASH_IRQn, interruptPriority);
	NVIC_EnableIRQ(FLASH_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FLASH_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FLASH_S_ENABLE
	NVIC_SetPriority(FLASH_S_IRQn, interruptPriority);
	NVIC_EnableIRQ(FLASH_S_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FLASH_S_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GTZC_ENABLE
	NVIC_SetPriority(GTZC_IRQn, interruptPriority);
	NVIC_EnableIRQ(GTZC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GTZC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RCC_ENABLE
	NVIC_SetPriority(RCC_IRQn, interruptPriority);
	NVIC_EnableIRQ(RCC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RCC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RCC_S_ENABLE
	NVIC_SetPriority(RCC_S_IRQn, interruptPriority);
	NVIC_EnableIRQ(RCC_S_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RCC_S_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI0_ENABLE
	NVIC_SetPriority(EXTI0_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI0_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI0_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI1_ENABLE
	NVIC_SetPriority(EXTI1_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI2_ENABLE
	NVIC_SetPriority(EXTI2_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI3_ENABLE
	NVIC_SetPriority(EXTI3_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI4_ENABLE
	NVIC_SetPriority(EXTI4_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI5_ENABLE
	NVIC_SetPriority(EXTI5_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI6_ENABLE
	NVIC_SetPriority(EXTI6_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI6_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI6_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI7_ENABLE
	NVIC_SetPriority(EXTI7_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI7_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI7_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI8_ENABLE
	NVIC_SetPriority(EXTI8_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI8_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI8_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI9_ENABLE
	NVIC_SetPriority(EXTI9_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI9_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI9_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI10_ENABLE
	NVIC_SetPriority(EXTI10_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI10_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI10_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI11_ENABLE
	NVIC_SetPriority(EXTI11_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI11_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI11_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI12_ENABLE
	NVIC_SetPriority(EXTI12_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI12_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI12_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI13_ENABLE
	NVIC_SetPriority(EXTI13_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI13_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI13_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI14_ENABLE
	NVIC_SetPriority(EXTI14_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI14_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI14_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI15_ENABLE
	NVIC_SetPriority(EXTI15_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI15_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI15_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_IWDG_ENABLE
	NVIC_SetPriority(IWDG_IRQn, interruptPriority);
	NVIC_EnableIRQ(IWDG_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_IWDG_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL0_ENABLE
	NVIC_SetPriority(GPDMA1_Channel0_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL0_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL1_ENABLE
	NVIC_SetPriority(GPDMA1_Channel1_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL2_ENABLE
	NVIC_SetPriority(GPDMA1_Channel2_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL3_ENABLE
	NVIC_SetPriority(GPDMA1_Channel3_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL4_ENABLE
	NVIC_SetPriority(GPDMA1_Channel4_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL5_ENABLE
	NVIC_SetPriority(GPDMA1_Channel5_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL6_ENABLE
	NVIC_SetPriority(GPDMA1_Channel6_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel6_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL6_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL7_ENABLE
	NVIC_SetPriority(GPDMA1_Channel7_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel7_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL7_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_ADC1_ENABLE
	NVIC_SetPriority(ADC1_IRQn, interruptPriority);
	NVIC_EnableIRQ(ADC1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_ADC1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DAC1_ENABLE
	NVIC_SetPriority(DAC1_IRQn, interruptPriority);
	NVIC_EnableIRQ(DAC1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DAC1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FDCAN1_IT0_ENABLE
	NVIC_SetPriority(FDCAN1_IT0_IRQn, interruptPriority);
	NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FDCAN1_IT0_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FDCAN1_IT1_ENABLE
	NVIC_SetPriority(FDCAN1_IT1_IRQn, interruptPriority);
	NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FDCAN1_IT1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_BRK_ENABLE
	NVIC_SetPriority(TIM1_BRK_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_BRK_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_BRK_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_UP_ENABLE
	NVIC_SetPriority(TIM1_UP_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_UP_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_UP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_TRG_COM_ENABLE
	NVIC_SetPriority(TIM1_TRG_COM_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_TRG_COM_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_CC_ENABLE
	NVIC_SetPriority(TIM1_CC_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_CC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_CC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM2_ENABLE
	NVIC_SetPriority(TIM2_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM3_ENABLE
	NVIC_SetPriority(TIM3_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM4_ENABLE
	NVIC_SetPriority(TIM4_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM5_ENABLE
	NVIC_SetPriority(TIM5_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM6_ENABLE
	NVIC_SetPriority(TIM6_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM6_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM6_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM7_ENABLE
	NVIC_SetPriority(TIM7_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM7_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM7_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_BRK_ENABLE
	NVIC_SetPriority(TIM8_BRK_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_BRK_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_BRK_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_UP_ENABLE
	NVIC_SetPriority(TIM8_UP_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_UP_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_UP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_TRG_COM_ENABLE
	NVIC_SetPriority(TIM8_TRG_COM_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_TRG_COM_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_TRG_COM_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_CC_ENABLE
	NVIC_SetPriority(TIM8_CC_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_CC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_CC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C1_EV_ENABLE
	NVIC_SetPriority(I2C1_EV_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C1_EV_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C1_EV_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C1_ER_ENABLE
	NVIC_SetPriority(I2C1_ER_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C1_ER_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C2_EV_ENABLE
	NVIC_SetPriority(I2C2_EV_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C2_EV_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C2_EV_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C2_ER_ENABLE
	NVIC_SetPriority(I2C2_ER_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C2_ER_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C2_ER_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SPI1_ENABLE
	NVIC_SetPriority(SPI1_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SPI1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SPI2_ENABLE
	NVIC_SetPriority(SPI2_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SPI2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USART1_ENABLE
	NVIC_SetPriority(USART1_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USART1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USART2_ENABLE
	NVIC_SetPriority(USART2_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USART2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USART3_ENABLE
	NVIC_SetPriority(USART3_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USART3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_UART4_ENABLE
	NVIC_SetPriority(UART4_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_UART4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_UART5_ENABLE
	NVIC_SetPriority(UART5_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_UART5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPUART1_ENABLE
	NVIC_SetPriority(LPUART1_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPUART1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPUART1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPTIM1_ENABLE
	NVIC_SetPriority(LPTIM1_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPTIM1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPTIM1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPTIM2_ENABLE
	NVIC_SetPriority(LPTIM2_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPTIM2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPTIM2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM15_ENABLE
	NVIC_SetPriority(TIM15_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM15_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM15_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM16_ENABLE
	NVIC_SetPriority(TIM16_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM16_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM16_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM17_ENABLE
	NVIC_SetPriority(TIM17_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM17_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM17_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_COMP_ENABLE
	NVIC_SetPriority(COMP_IRQn, interruptPriority);
	NVIC_EnableIRQ(COMP_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_COMP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_OTG_FS_ENABLE
	NVIC_SetPriority(OTG_FS_IRQn, interruptPriority);
	NVIC_EnableIRQ(OTG_FS_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_OTG_FS_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_CRS_ENABLE
	NVIC_SetPriority(CRS_IRQn, interruptPriority);
	NVIC_EnableIRQ(CRS_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_CRS_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FMC_ENABLE
	NVIC_SetPriority(FMC_IRQn, interruptPriority);
	NVIC_EnableIRQ(FMC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FMC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_OCTOSPI1_ENABLE
	NVIC_SetPriority(OCTOSPI1_IRQn, interruptPriority);
	NVIC_EnableIRQ(OCTOSPI1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_OCTOSPI1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_PWR_S3WU_ENABLE
	NVIC_SetPriority(PWR_S3WU_IRQn, interruptPriority);
	NVIC_EnableIRQ(PWR_S3WU_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_PWR_S3WU_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SDMMC1_ENABLE
	NVIC_SetPriority(SDMMC1_IRQn, interruptPriority);
	NVIC_EnableIRQ(SDMMC1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SDMMC1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SDMMC2_ENABLE
	NVIC_SetPriority(SDMMC2_IRQn, interruptPriority);
	NVIC_EnableIRQ(SDMMC2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SDMMC2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL8_ENABLE
	NVIC_SetPriority(GPDMA1_Channel8_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel8_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL8_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL9_ENABLE
	NVIC_SetPriority(GPDMA1_Channel9_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel9_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL9_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL10_ENABLE
	NVIC_SetPriority(GPDMA1_Channel10_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel10_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL10_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL11_ENABLE
	NVIC_SetPriority(GPDMA1_Channel11_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel11_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL11_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL12_ENABLE
	NVIC_SetPriority(GPDMA1_Channel12_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL12_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL13_ENABLE
	NVIC_SetPriority(GPDMA1_Channel13_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel13_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL13_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL14_ENABLE
	NVIC_SetPriority(GPDMA1_Channel14_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel14_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL14_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL15_ENABLE
	NVIC_SetPriority(GPDMA1_Channel15_IRQn, interruptPriority);
	NVIC_EnableIRQ(GPDMA1_Channel15_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_GPDMA1_CHANNEL15_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C3_EV_ENABLE
	NVIC_SetPriority(I2C3_EV_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C3_EV_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C3_EV_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C3_ER_ENABLE
	NVIC_SetPriority(I2C3_ER_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C3_ER_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C3_ER_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SAI1_ENABLE
	NVIC_SetPriority(SAI1_IRQn, interruptPriority);
	NVIC_EnableIRQ(SAI1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SAI1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SAI2_ENABLE
	NVIC_SetPriority(SAI2_IRQn, interruptPriority);
	NVIC_EnableIRQ(SAI2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SAI2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TSC_ENABLE
	NVIC_SetPriority(TSC_IRQn, interruptPriority);
	NVIC_EnableIRQ(TSC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TSC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RNG_ENABLE
	NVIC_SetPriority(RNG_IRQn, interruptPriority);
	NVIC_EnableIRQ(RNG_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RNG_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FPU_ENABLE
	NVIC_SetPriority(FPU_IRQn, interruptPriority);
	NVIC_EnableIRQ(FPU_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FPU_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_HASH_ENABLE
	NVIC_SetPriority(HASH_IRQn, interruptPriority);
	NVIC_EnableIRQ(HASH_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_HASH_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPTIM3_ENABLE
	NVIC_SetPriority(LPTIM3_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPTIM3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPTIM3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SPI3_ENABLE
	NVIC_SetPriority(SPI3_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SPI3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C4_ER_ENABLE
	NVIC_SetPriority(I2C4_ER_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C4_ER_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C4_ER_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C4_EV_ENABLE
	NVIC_SetPriority(I2C4_EV_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C4_EV_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C4_EV_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT0_ENABLE
	NVIC_SetPriority(MDF1_FLT0_IRQn, interruptPriority);
	NVIC_EnableIRQ(MDF1_FLT0_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT0_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT1_ENABLE
	NVIC_SetPriority(MDF1_FLT1_IRQn, interruptPriority);
	NVIC_EnableIRQ(MDF1_FLT1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT2_ENABLE
	NVIC_SetPriority(MDF1_FLT2_IRQn, interruptPriority);
	NVIC_EnableIRQ(MDF1_FLT2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT3_ENABLE
	NVIC_SetPriority(MDF1_FLT3_IRQn, interruptPriority);
	NVIC_EnableIRQ(MDF1_FLT3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_UCPD1_ENABLE
	NVIC_SetPriority(UCPD1_IRQn, interruptPriority);
	NVIC_EnableIRQ(UCPD1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_UCPD1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_ICACHE_ENABLE
	NVIC_SetPriority(ICACHE_IRQn, interruptPriority);
	NVIC_EnableIRQ(ICACHE_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_ICACHE_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPTIM4_ENABLE
	NVIC_SetPriority(LPTIM4_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPTIM4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPTIM4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DCACHE1_ENABLE
	NVIC_SetPriority(DCACHE1_IRQn, interruptPriority);
	NVIC_EnableIRQ(DCACHE1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DCACHE1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_ADF1_ENABLE
	NVIC_SetPriority(ADF1_IRQn, interruptPriority);
	NVIC_EnableIRQ(ADF1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_ADF1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_ADC4_ENABLE
	NVIC_SetPriority(ADC4_IRQn, interruptPriority);
	NVIC_EnableIRQ(ADC4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_ADC4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL0_ENABLE
	NVIC_SetPriority(LPDMA1_Channel0_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL0_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL1_ENABLE
	NVIC_SetPriority(LPDMA1_Channel1_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPDMA1_Channel1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL2_ENABLE
	NVIC_SetPriority(LPDMA1_Channel2_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPDMA1_Channel2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL3_ENABLE
	NVIC_SetPriority(LPDMA1_Channel3_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPDMA1_Channel3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPDMA1_CHANNEL3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA2D_ENABLE
	NVIC_SetPriority(DMA2D_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA2D_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA2D_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DCMI_PSSI_ENABLE
	NVIC_SetPriority(DCMI_PSSI_IRQn, interruptPriority);
	NVIC_EnableIRQ(DCMI_PSSI_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DCMI_PSSI_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_OCTOSPI2_ENABLE
	NVIC_SetPriority(OCTOSPI2_IRQn, interruptPriority);
	NVIC_EnableIRQ(OCTOSPI2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_OCTOSPI2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT4_ENABLE
	NVIC_SetPriority(MDF1_FLT4_IRQn, interruptPriority);
	NVIC_EnableIRQ(MDF1_FLT4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT5_ENABLE
	NVIC_SetPriority(MDF1_FLT5_IRQn, interruptPriority);
	NVIC_EnableIRQ(MDF1_FLT5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_MDF1_FLT5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_CORDIC_ENABLE
	NVIC_SetPriority(CORDIC_IRQn, interruptPriority);
	NVIC_EnableIRQ(CORDIC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_CORDIC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FMAC_ENABLE
	NVIC_SetPriority(FMAC_IRQn, interruptPriority);
	NVIC_EnableIRQ(FMAC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FMAC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LSECSSD_ENABLE
	NVIC_SetPriority(LSECSSD_IRQn, interruptPriority);
	NVIC_EnableIRQ(LSECSSD_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LSECSSD_ENABLE
}

BIND_LOW_LEVEL_INITIALIZER(50, nvicLowLevelInitializer);

}	// namespace

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| default handlers
+---------------------------------------------------------------------------------------------------------------------*/

// NMI, address 0x8, exception number 2, IRQ number -14
__attribute__ ((weak)) void NMI_Handler()
{
	while (1);
}

// HardFault, address 0xc, exception number 3, IRQ number -13
__attribute__ ((weak)) void HardFault_Handler()
{
	while (1);
}

// MemManage, address 0x10, exception number 4, IRQ number -12
__attribute__ ((weak)) void MemManage_Handler()
{
	while (1);
}

// BusFault, address 0x14, exception number 5, IRQ number -11
__attribute__ ((weak)) void BusFault_Handler()
{
	while (1);
}

// UsageFault, address 0x18, exception number 6, IRQ number -10
__attribute__ ((weak)) void UsageFault_Handler()
{
	while (1);
}

// SecureFault, address 0x1c, exception number 7, IRQ number -9
__attribute__ ((weak)) void SecureFault_Handler()
{
	while (1);
}

// SVC, address 0x2c, exception number 11, IRQ number -5
__attribute__ ((weak)) void SVC_Handler()
{
	while (1);
}

// DebugMon, address 0x30, exception number 12, IRQ number -4
__attribute__ ((weak)) void DebugMon_Handler()
{
	while (1);
}

// PendSV, address 0x38, exception number 14, IRQ number -2
__attribute__ ((weak)) void PendSV_Handler()
{
	while (1);
}

// SysTick, address 0x3c, exception number 15, IRQ number -1
__attribute__ ((weak)) void SysTick_Handler()
{
	while (1);
}

// WWDG, address 0x40, exception number 16, IRQ number 0
__attribute__ ((weak)) void WWDG_IRQHandler()
{
	while (1);
}

// PVD_PVM, address 0x44, exception number 17, IRQ number 1
__attribute__ ((weak)) void PVD_PVM_IRQHandler()
{
	while (1);
}

// RTC, address 0x48, exception number 18, IRQ number 2
__attribute__ ((weak)) void RTC_IRQHandler()
{
	while (1);
}

// RTC_S, address 0x4c, exception number 19, IRQ number 3
__attribute__ ((weak)) void RTC_S_IRQHandler()
{
	while (1);
}

// TAMP, address 0x50, exception number 20, IRQ number 4
__attribute__ ((weak)) void TAMP_IRQHandler()
{
	while (1);
}

// RAMCFG, address 0x54, exception number 21, IRQ number 5
__attribute__ ((weak)) void RAMCFG_IRQHandler()
{
	while (1);
}

// FLASH, address 0x58, exception number 22, IRQ number 6
__attribute__ ((weak)) void FLASH_IRQHandler()
{
	while (1);
}

// FLASH_S, address 0x5c, exception number 23, IRQ number 7
__attribute__ ((weak)) void FLASH_S_IRQHandler()
{
	while (1);
}

// GTZC, address 0x60, exception number 24, IRQ number 8
__attribute__ ((weak)) void GTZC_IRQHandler()
{
	while (1);
}

// RCC, address 0x64, exception number 25, IRQ number 9
__attribute__ ((weak)) void RCC_IRQHandler()
{
	while (1);
}

// RCC_S, address 0x68, exception number 26, IRQ number 10
__attribute__ ((weak)) void RCC_S_IRQHandler()
{
	while (1);
}

// EXTI0, address 0x6c, exception number 27, IRQ number 11
__attribute__ ((weak)) void EXTI0_IRQHandler()
{
	while (1);
}

// EXTI1, address 0x70, exception number 28, IRQ number 12
__attribute__ ((weak)) void EXTI1_IRQHandler()
{
	while (1);
}

// EXTI2, address 0x74, exception number 29, IRQ number 13
__attribute__ ((weak)) void EXTI2_IRQHandler()
{
	while (1);
}

// EXTI3, address 0x78, exception number 30, IRQ number 14
__attribute__ ((weak)) void EXTI3_IRQHandler()
{
	while (1);
}

// EXTI4, address 0x7c, exception number 31, IRQ number 15
__attribute__ ((weak)) void EXTI4_IRQHandler()
{
	while (1);
}

// EXTI5, address 0x80, exception number 32, IRQ number 16
__attribute__ ((weak)) void EXTI5_IRQHandler()
{
	while (1);
}

// EXTI6, address 0x84, exception number 33, IRQ number 17
__attribute__ ((weak)) void EXTI6_IRQHandler()
{
	while (1);
}

// EXTI7, address 0x88, exception number 34, IRQ number 18
__attribute__ ((weak)) void EXTI7_IRQHandler()
{
	while (1);
}

// EXTI8, address 0x8c, exception number 35, IRQ number 19
__attribute__ ((weak)) void EXTI8_IRQHandler()
{
	while (1);
}

// EXTI9, address 0x90, exception number 36, IRQ number 20
__attribute__ ((weak)) void EXTI9_IRQHandler()
{
	while (1);
}

// EXTI10, address 0x94, exception number 37, IRQ number 21
__attribute__ ((weak)) void EXTI10_IRQHandler()
{
	while (1);
}

// EXTI11, address 0x98, exception number 38, IRQ number 22
__attribute__ ((weak)) void EXTI11_IRQHandler()
{
	while (1);
}

// EXTI12, address 0x9c, exception number 39, IRQ number 23
__attribute__ ((weak)) void EXTI12_IRQHandler()
{
	while (1);
}

// EXTI13, address 0xa0, exception number 40, IRQ number 24
__attribute__ ((weak)) void EXTI13_IRQHandler()
{
	while (1);
}

// EXTI14, address 0xa4, exception number 41, IRQ number 25
__attribute__ ((weak)) void EXTI14_IRQHandler()
{
	while (1);
}

// EXTI15, address 0xa8, exception number 42, IRQ number 26
__attribute__ ((weak)) void EXTI15_IRQHandler()
{
	while (1);
}

// IWDG, address 0xac, exception number 43, IRQ number 27
__attribute__ ((weak)) void IWDG_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel0, address 0xb4, exception number 45, IRQ number 29
__attribute__ ((weak)) void GPDMA1_Channel0_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel1, address 0xb8, exception number 46, IRQ number 30
__attribute__ ((weak)) void GPDMA1_Channel1_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel2, address 0xbc, exception number 47, IRQ number 31
__attribute__ ((weak)) void GPDMA1_Channel2_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel3, address 0xc0, exception number 48, IRQ number 32
__attribute__ ((weak)) void GPDMA1_Channel3_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel4, address 0xc4, exception number 49, IRQ number 33
__attribute__ ((weak)) void GPDMA1_Channel4_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel5, address 0xc8, exception number 50, IRQ number 34
__attribute__ ((weak)) void GPDMA1_Channel5_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel6, address 0xcc, exception number 51, IRQ number 35
__attribute__ ((weak)) void GPDMA1_Channel6_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel7, address 0xd0, exception number 52, IRQ number 36
__attribute__ ((weak)) void GPDMA1_Channel7_IRQHandler()
{
	while (1);
}

// ADC1, address 0xd4, exception number 53, IRQ number 37
__attribute__ ((weak)) void ADC1_IRQHandler()
{
	while (1);
}

// DAC1, address 0xd8, exception number 54, IRQ number 38
__attribute__ ((weak)) void DAC1_IRQHandler()
{
	while (1);
}

// FDCAN1_IT0, address 0xdc, exception number 55, IRQ number 39
__attribute__ ((weak)) void FDCAN1_IT0_IRQHandler()
{
	while (1);
}

// FDCAN1_IT1, address 0xe0, exception number 56, IRQ number 40
__attribute__ ((weak)) void FDCAN1_IT1_IRQHandler()
{
	while (1);
}

// TIM1_BRK, address 0xe4, exception number 57, IRQ number 41
__attribute__ ((weak)) void TIM1_BRK_IRQHandler()
{
	while (1);
}

// TIM1_UP, address 0xe8, exception number 58, IRQ number 42
__attribute__ ((weak)) void TIM1_UP_IRQHandler()
{
	while (1);
}

// TIM1_TRG_COM, address 0xec, exception number 59, IRQ number 43
__attribute__ ((weak)) void TIM1_TRG_COM_IRQHandler()
{
	while (1);
}

// TIM1_CC, address 0xf0, exception number 60, IRQ number 44
__attribute__ ((weak)) void TIM1_CC_IRQHandler()
{
	while (1);
}

// TIM2, address 0xf4, exception number 61, IRQ number 45
__attribute__ ((weak)) void TIM2_IRQHandler()
{
	while (1);
}

// TIM3, address 0xf8, exception number 62, IRQ number 46
__attribute__ ((weak)) void TIM3_IRQHandler()
{
	while (1);
}

// TIM4, address 0xfc, exception number 63, IRQ number 47
__attribute__ ((weak)) void TIM4_IRQHandler()
{
	while (1);
}

// TIM5, address 0x100, exception number 64, IRQ number 48
__attribute__ ((weak)) void TIM5_IRQHandler()
{
	while (1);
}

// TIM6, address 0x104, exception number 65, IRQ number 49
__attribute__ ((weak)) void TIM6_IRQHandler()
{
	while (1);
}

// TIM7, address 0x108, exception number 66, IRQ number 50
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

// TIM8_BRK, address 0x10c, exception number 67, IRQ number 51
__attribute__ ((weak)) void TIM8_BRK_IRQHandler()
{
	while (1);
}

// TIM8_UP, address 0x110, exception number 68, IRQ number 52
__attribute__ ((weak)) void TIM8_UP_IRQHandler()
{
	while (1);
}

// TIM8_TRG_COM, address 0x114, exception number 69, IRQ number 53
__attribute__ ((weak)) void TIM8_TRG_COM_IRQHandler()
{
	while (1);
}

// TIM8_CC, address 0x118, exception number 70, IRQ number 54
__attribute__ ((weak)) void TIM8_CC_IRQHandler()
{
	while (1);
}

// I2C1_EV, address 0x11c, exception number 71, IRQ number 55
__attribute__ ((weak)) void I2C1_EV_IRQHandler()
{
	while (1);
}

// I2C1_ER, address 0x120, exception number 72, IRQ number 56
__attribute__ ((weak)) void I2C1_ER_IRQHandler()
{
	while (1);
}

// I2C2_EV, address 0x124, exception number 73, IRQ number 57
__attribute__ ((weak)) void I2C2_EV_IRQHandler()
{
	while (1);
}

// I2C2_ER, address 0x128, exception number 74, IRQ number 58
__attribute__ ((weak)) void I2C2_ER_IRQHandler()
{
	while (1);
}

// SPI1, address 0x12c, exception number 75, IRQ number 59
__attribute__ ((weak)) void SPI1_IRQHandler()
{
	while (1);
}

// SPI2, address 0x130, exception number 76, IRQ number 60
__attribute__ ((weak)) void SPI2_IRQHandler()
{
	while (1);
}

// USART1, address 0x134, exception number 77, IRQ number 61
__attribute__ ((weak)) void USART1_IRQHandler()
{
	while (1);
}

// USART2, address 0x138, exception number 78, IRQ number 62
__attribute__ ((weak)) void USART2_IRQHandler()
{
	while (1);
}

// USART3, address 0x13c, exception number 79, IRQ number 63
__attribute__ ((weak)) void USART3_IRQHandler()
{
	while (1);
}

// UART4, address 0x140, exception number 80, IRQ number 64
__attribute__ ((weak)) void UART4_IRQHandler()
{
	while (1);
}

// UART5, address 0x144, exception number 81, IRQ number 65
__attribute__ ((weak)) void UART5_IRQHandler()
{
	while (1);
}

// LPUART1, address 0x148, exception number 82, IRQ number 66
__attribute__ ((weak)) void LPUART1_IRQHandler()
{
	while (1);
}

// LPTIM1, address 0x14c, exception number 83, IRQ number 67
__attribute__ ((weak)) void LPTIM1_IRQHandler()
{
	while (1);
}

// LPTIM2, address 0x150, exception number 84, IRQ number 68
__attribute__ ((weak)) void LPTIM2_IRQHandler()
{
	while (1);
}

// TIM15, address 0x154, exception number 85, IRQ number 69
__attribute__ ((weak)) void TIM15_IRQHandler()
{
	while (1);
}

// TIM16, address 0x158, exception number 86, IRQ number 70
__attribute__ ((weak)) void TIM16_IRQHandler()
{
	while (1);
}

// TIM17, address 0x15c, exception number 87, IRQ number 71
__attribute__ ((weak)) void TIM17_IRQHandler()
{
	while (1);
}

// COMP, address 0x160, exception number 88, IRQ number 72
__attribute__ ((weak)) void COMP_IRQHandler()
{
	while (1);
}

// OTG_FS, address 0x164, exception number 89, IRQ number 73
__attribute__ ((weak)) void OTG_FS_IRQHandler()
{
	while (1);
}

// CRS, address 0x168, exception number 90, IRQ number 74
__attribute__ ((weak)) void CRS_IRQHandler()
{
	while (1);
}

// FMC, address 0x16c, exception number 91, IRQ number 75
__attribute__ ((weak)) void FMC_IRQHandler()
{
	while (1);
}

// OCTOSPI1, address 0x170, exception number 92, IRQ number 76
__attribute__ ((weak)) void OCTOSPI1_IRQHandler()
{
	while (1);
}

// PWR_S3WU, address 0x174, exception number 93, IRQ number 77
__attribute__ ((weak)) void PWR_S3WU_IRQHandler()
{
	while (1);
}

// SDMMC1, address 0x178, exception number 94, IRQ number 78
__attribute__ ((weak)) void SDMMC1_IRQHandler()
{
	while (1);
}

// SDMMC2, address 0x17c, exception number 95, IRQ number 79
__attribute__ ((weak)) void SDMMC2_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel8, address 0x180, exception number 96, IRQ number 80
__attribute__ ((weak)) void GPDMA1_Channel8_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel9, address 0x184, exception number 97, IRQ number 81
__attribute__ ((weak)) void GPDMA1_Channel9_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel10, address 0x188, exception number 98, IRQ number 82
__attribute__ ((weak)) void GPDMA1_Channel10_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel11, address 0x18c, exception number 99, IRQ number 83
__attribute__ ((weak)) void GPDMA1_Channel11_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel12, address 0x190, exception number 100, IRQ number 84
__attribute__ ((weak)) void GPDMA1_Channel12_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel13, address 0x194, exception number 101, IRQ number 85
__attribute__ ((weak)) void GPDMA1_Channel13_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel14, address 0x198, exception number 102, IRQ number 86
__attribute__ ((weak)) void GPDMA1_Channel14_IRQHandler()
{
	while (1);
}

// GPDMA1_Channel15, address 0x19c, exception number 103, IRQ number 87
__attribute__ ((weak)) void GPDMA1_Channel15_IRQHandler()
{
	while (1);
}

// I2C3_EV, address 0x1a0, exception number 104, IRQ number 88
__attribute__ ((weak)) void I2C3_EV_IRQHandler()
{
	while (1);
}

// I2C3_ER, address 0x1a4, exception number 105, IRQ number 89
__attribute__ ((weak)) void I2C3_ER_IRQHandler()
{
	while (1);
}

// SAI1, address 0x1a8, exception number 106, IRQ number 90
__attribute__ ((weak)) void SAI1_IRQHandler()
{
	while (1);
}

// SAI2, address 0x1ac, exception number 107, IRQ number 91
__attribute__ ((weak)) void SAI2_IRQHandler()
{
	while (1);
}

// TSC, address 0x1b0, exception number 108, IRQ number 92
__attribute__ ((weak)) void TSC_IRQHandler()
{
	while (1);
}

// RNG, address 0x1b8, exception number 110, IRQ number 94
__attribute__ ((weak)) void RNG_IRQHandler()
{
	while (1);
}

// FPU, address 0x1bc, exception number 111, IRQ number 95
__attribute__ ((weak)) void FPU_IRQHandler()
{
	while (1);
}

// HASH, address 0x1c0, exception number 112, IRQ number 96
__attribute__ ((weak)) void HASH_IRQHandler()
{
	while (1);
}

// LPTIM3, address 0x1c8, exception number 114, IRQ number 98
__attribute__ ((weak)) void LPTIM3_IRQHandler()
{
	while (1);
}

// SPI3, address 0x1cc, exception number 115, IRQ number 99
__attribute__ ((weak)) void SPI3_IRQHandler()
{
	while (1);
}

// I2C4_ER, address 0x1d0, exception number 116, IRQ number 100
__attribute__ ((weak)) void I2C4_ER_IRQHandler()
{
	while (1);
}

// I2C4_EV, address 0x1d4, exception number 117, IRQ number 101
__attribute__ ((weak)) void I2C4_EV_IRQHandler()
{
	while (1);
}

// MDF1_FLT0, address 0x1d8, exception number 118, IRQ number 102
__attribute__ ((weak)) void MDF1_FLT0_IRQHandler()
{
	while (1);
}

// MDF1_FLT1, address 0x1dc, exception number 119, IRQ number 103
__attribute__ ((weak)) void MDF1_FLT1_IRQHandler()
{
	while (1);
}

// MDF1_FLT2, address 0x1e0, exception number 120, IRQ number 104
__attribute__ ((weak)) void MDF1_FLT2_IRQHandler()
{
	while (1);
}

// MDF1_FLT3, address 0x1e4, exception number 121, IRQ number 105
__attribute__ ((weak)) void MDF1_FLT3_IRQHandler()
{
	while (1);
}

// UCPD1, address 0x1e8, exception number 122, IRQ number 106
__attribute__ ((weak)) void UCPD1_IRQHandler()
{
	while (1);
}

// ICACHE, address 0x1ec, exception number 123, IRQ number 107
__attribute__ ((weak)) void ICACHE_IRQHandler()
{
	while (1);
}

// LPTIM4, address 0x1f8, exception number 126, IRQ number 110
__attribute__ ((weak)) void LPTIM4_IRQHandler()
{
	while (1);
}

// DCACHE1, address 0x1fc, exception number 127, IRQ number 111
__attribute__ ((weak)) void DCACHE1_IRQHandler()
{
	while (1);
}

// ADF1, address 0x200, exception number 128, IRQ number 112
__attribute__ ((weak)) void ADF1_IRQHandler()
{
	while (1);
}

// ADC4, address 0x204, exception number 129, IRQ number 113
__attribute__ ((weak)) void ADC4_IRQHandler()
{
	while (1);
}

// LPDMA1_Channel0, address 0x208, exception number 130, IRQ number 114
__attribute__ ((weak)) void LPDMA1_Channel0_IRQHandler()
{
	while (1);
}

// LPDMA1_Channel1, address 0x20c, exception number 131, IRQ number 115
__attribute__ ((weak)) void LPDMA1_Channel1_IRQHandler()
{
	while (1);
}

// LPDMA1_Channel2, address 0x210, exception number 132, IRQ number 116
__attribute__ ((weak)) void LPDMA1_Channel2_IRQHandler()
{
	while (1);
}

// LPDMA1_Channel3, address 0x214, exception number 133, IRQ number 117
__attribute__ ((weak)) void LPDMA1_Channel3_IRQHandler()
{
	while (1);
}

// DMA2D, address 0x218, exception number 134, IRQ number 118
__attribute__ ((weak)) void DMA2D_IRQHandler()
{
	while (1);
}

// DCMI_PSSI, address 0x21c, exception number 135, IRQ number 119
__attribute__ ((weak)) void DCMI_PSSI_IRQHandler()
{
	while (1);
}

// OCTOSPI2, address 0x220, exception number 136, IRQ number 120
__attribute__ ((weak)) void OCTOSPI2_IRQHandler()
{
	while (1);
}

// MDF1_FLT4, address 0x224, exception number 137, IRQ number 121
__attribute__ ((weak)) void MDF1_FLT4_IRQHandler()
{
	while (1);
}

// MDF1_FLT5, address 0x228, exception number 138, IRQ number 122
__attribute__ ((weak)) void MDF1_FLT5_IRQHandler()
{
	while (1);
}

// CORDIC, address 0x22c, exception number 139, IRQ number 123
__attribute__ ((weak)) void CORDIC_IRQHandler()
{
	while (1);
}

// FMAC, address 0x230, exception number 140, IRQ number 124
__attribute__ ((weak)) void FMAC_IRQHandler()
{
	while (1);
}

// LSECSSD, address 0x234, exception number 141, IRQ number 125
__attribute__ ((weak)) void LSECSSD_IRQHandler()
{
	while (1);
}

/*---------------------------------------------------------------------------------------------------------------------+
| global symbols' declarations
+---------------------------------------------------------------------------------------------------------------------*/

extern const char __main_stack_end[];		// main stack end - imported from linker script

void Reset_Handler();		// import the address of Reset_Handler()

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// vector table
const ExceptionVector vectorTable[] __attribute__ ((section(".vectorTable"), used))
{
		reinterpret_cast<ExceptionVector>(__main_stack_end),	// 0x0: Main stack end address
		Reset_Handler,		// Reset, address 0x4, exception number 1, IRQ number -15
		NMI_Handler,		// NMI, address 0x8, exception number 2, IRQ number -14
		HardFault_Handler,		// HardFault, address 0xc, exception number 3, IRQ number -13
		MemManage_Handler,		// MemManage, address 0x10, exception number 4, IRQ number -12
		BusFault_Handler,		// BusFault, address 0x14, exception number 5, IRQ number -11
		UsageFault_Handler,		// UsageFault, address 0x18, exception number 6, IRQ number -10
		SecureFault_Handler,		// SecureFault, address 0x1c, exception number 7, IRQ number -9
		nullptr,		// Reserved, address 0x20, exception number 8, IRQ number -8
		nullptr,		// Reserved, address 0x24, exception number 9, IRQ number -7
		nullptr,		// Reserved, address 0x28, exception number 10, IRQ number -6
		SVC_Handler,		// SVC, address 0x2c, exception number 11, IRQ number -5
		DebugMon_Handler,		// DebugMon, address 0x30, exception number 12, IRQ number -4
		nullptr,		// Reserved, address 0x34, exception number 13, IRQ number -3
		PendSV_Handler,		// PendSV, address 0x38, exception number 14, IRQ number -2
		SysTick_Handler,		// SysTick, address 0x3c, exception number 15, IRQ number -1
		WWDG_IRQHandler,		// WWDG, address 0x40, exception number 16, IRQ number 0
		PVD_PVM_IRQHandler,		// PVD_PVM, address 0x44, exception number 17, IRQ number 1
		RTC_IRQHandler,		// RTC, address 0x48, exception number 18, IRQ number 2
		RTC_S_IRQHandler,		// RTC_S, address 0x4c, exception number 19, IRQ number 3
		TAMP_IRQHandler,		// TAMP, address 0x50, exception number 20, IRQ number 4
		RAMCFG_IRQHandler,		// RAMCFG, address 0x54, exception number 21, IRQ number 5
		FLASH_IRQHandler,		// FLASH, address 0x58, exception number 22, IRQ number 6
		FLASH_S_IRQHandler,		// FLASH_S, address 0x5c, exception number 23, IRQ number 7
		GTZC_IRQHandler,		// GTZC, address 0x60, exception number 24, IRQ number 8
		RCC_IRQHandler,		// RCC, address 0x64, exception number 25, IRQ number 9
		RCC_S_IRQHandler,		// RCC_S, address 0x68, exception number 26, IRQ number 10
		EXTI0_IRQHandler,		// EXTI0, address 0x6c, exception number 27, IRQ number 11
		EXTI1_IRQHandler,		// EXTI1, address 0x70, exception number 28, IRQ number 12
		EXTI2_IRQHandler,		// EXTI2, address 0x74, exception number 29, IRQ number 13
		EXTI3_IRQHandler,		// EXTI3, address 0x78, exception number 30, IRQ number 14
		EXTI4_IRQHandler,		// EXTI4, address 0x7c, exception number 31, IRQ number 15
		EXTI5_IRQHandler,		// EXTI5, address 0x80, exception number 32, IRQ number 16
		EXTI6_IRQHandler,		// EXTI6, address 0x84, exception number 33, IRQ number 17
		EXTI7_IRQHandler,		// EXTI7, address 0x88, exception number 34, IRQ number 18
		EXTI8_IRQHandler,		// EXTI8, address 0x8c, exception number 35, IRQ number 19
		EXTI9_IRQHandler,		// EXTI9, address 0x90, exception number 36, IRQ number 20
		EXTI10_IRQHandler,		// EXTI10, address 0x94, exception number 37, IRQ number 21
		EXTI11_IRQHandler,		// EXTI11, address 0x98, exception number 38, IRQ number 22
		EXTI12_IRQHandler,		// EXTI12, address 0x9c, exception number 39, IRQ number 23
		EXTI13_IRQHandler,		// EXTI13, address 0xa0, exception number 40, IRQ number 24
		EXTI14_IRQHandler,		// EXTI14, address 0xa4, exception number 41, IRQ number 25
		EXTI15_IRQHandler,		// EXTI15, address 0xa8, exception number 42, IRQ number 26
		IWDG_IRQHandler,		// IWDG, address 0xac, exception number 43, IRQ number 27
		nullptr,		// Reserved, address 0xb0, exception number 44, IRQ number 28
		GPDMA1_Channel0_IRQHandler,		// GPDMA1_Channel0, address 0xb4, exception number 45, IRQ number 29
		GPDMA1_Channel1_IRQHandler,		// GPDMA1_Channel1, address 0xb8, exception number 46, IRQ number 30
		GPDMA1_Channel2_IRQHandler,		// GPDMA1_Channel2, address 0xbc, exception number 47, IRQ number 31
		GPDMA1_Channel3_IRQHandler,		// GPDMA1_Channel3, address 0xc0, exception number 48, IRQ number 32
		GPDMA1_Channel4_IRQHandler,		// GPDMA1_Channel4, address 0xc4, exception number 49, IRQ number 33
		GPDMA1_Channel5_IRQHandler,		// GPDMA1_Channel5, address 0xc8, exception number 50, IRQ number 34
		GPDMA1_Channel6_IRQHandler,		// GPDMA1_Channel6, address 0xcc, exception number 51, IRQ number 35
		GPDMA1_Channel7_IRQHandler,		// GPDMA1_Channel7, address 0xd0, exception number 52, IRQ number 36
		ADC1_IRQHandler,		// ADC1, address 0xd4, exception number 53, IRQ number 37
		DAC1_IRQHandler,		// DAC1, address 0xd8, exception number 54, IRQ number 38
		FDCAN1_IT0_IRQHandler,		// FDCAN1_IT0, address 0xdc, exception number 55, IRQ number 39
		FDCAN1_IT1_IRQHandler,		// FDCAN1_IT1, address 0xe0, exception number 56, IRQ number 40
		TIM1_BRK_IRQHandler,		// TIM1_BRK, address 0xe4, exception number 57, IRQ number 41
		TIM1_UP_IRQHandler,		// TIM1_UP, address 0xe8, exception number 58, IRQ number 42
		TIM1_TRG_COM_IRQHandler,		// TIM1_TRG_COM, address 0xec, exception number 59, IRQ number 43
		TIM1_CC_IRQHandler,		// TIM1_CC, address 0xf0, exception number 60, IRQ number 44
		TIM2_IRQHandler,		// TIM2, address 0xf4, exception number 61, IRQ number 45
		TIM3_IRQHandler,		// TIM3, address 0xf8, exception number 62, IRQ number 46
		TIM4_IRQHandler,		// TIM4, address 0xfc, exception number 63, IRQ number 47
		TIM5_IRQHandler,		// TIM5, address 0x100, exception number 64, IRQ number 48
		TIM6_IRQHandler,		// TIM6, address 0x104, exception number 65, IRQ number 49
		TIM7_IRQHandler,		// TIM7, address 0x108, exception number 66, IRQ number 50
		TIM8_BRK_IRQHandler,		// TIM8_BRK, address 0x10c, exception number 67, IRQ number 51
		TIM8_UP_IRQHandler,		// TIM8_UP, address 0x110, exception number 68, IRQ number 52
		TIM8_TRG_COM_IRQHandler,		// TIM8_TRG_COM, address 0x114, exception number 69, IRQ number 53
		TIM8_CC_IRQHandler,		// TIM8_CC, address 0x118, exception number 70, IRQ number 54
		I2C1_EV_IRQHandler,		// I2C1_EV, address 0x11c, exception number 71, IRQ number 55
		I2C1_ER_IRQHandler,		// I2C1_ER, address 0x120, exception number 72, IRQ number 56
		I2C2_EV_IRQHandler,		// I2C2_EV, address 0x124, exception number 73, IRQ number 57
		I2C2_ER_IRQHandler,		// I2C2_ER, address 0x128, exception number 74, IRQ number 58
		SPI1_IRQHandler,		// SPI1, address 0x12c, exception number 75, IRQ number 59
		SPI2_IRQHandler,		// SPI2, address 0x130, exception number 76, IRQ number 60
		USART1_IRQHandler,		// USART1, address 0x134, exception number 77, IRQ number 61
		USART2_IRQHandler,		// USART2, address 0x138, exception number 78, IRQ number 62
		USART3_IRQHandler,		// USART3, address 0x13c, exception number 79, IRQ number 63
		UART4_IRQHandler,		// UART4, address 0x140, exception number 80, IRQ number 64
		UART5_IRQHandler,		// UART5, address 0x144, exception number 81, IRQ number 65
		LPUART1_IRQHandler,		// LPUART1, address 0x148, exception number 82, IRQ number 66
		LPTIM1_IRQHandler,		// LPTIM1, address 0x14c, exception number 83, IRQ number 67
		LPTIM2_IRQHandler,		// LPTIM2, address 0x150, exception number 84, IRQ number 68
		TIM15_IRQHandler,		// TIM15, address 0x154, exception number 85, IRQ number 69
		TIM16_IRQHandler,		// TIM16, address 0x158, exception number 86, IRQ number 70
		TIM17_IRQHandler,		// TIM17, address 0x15c, exception number 87, IRQ number 71
		COMP_IRQHandler,		// COMP, address 0x160, exception number 88, IRQ number 72
		OTG_FS_IRQHandler,		// OTG_FS, address 0x164, exception number 89, IRQ number 73
		CRS_IRQHandler,		// CRS, address 0x168, exception number 90, IRQ number 74
		FMC_IRQHandler,		// FMC, address 0x16c, exception number 91, IRQ number 75
		OCTOSPI1_IRQHandler,		// OCTOSPI1, address 0x170, exception number 92, IRQ number 76
		PWR_S3WU_IRQHandler,		// PWR_S3WU, address 0x174, exception number 93, IRQ number 77
		SDMMC1_IRQHandler,		// SDMMC1, address 0x178, exception number 94, IRQ number 78
		SDMMC2_IRQHandler,		// SDMMC2, address 0x17c, exception number 95, IRQ number 79
		GPDMA1_Channel8_IRQHandler,		// GPDMA1_Channel8, address 0x180, exception number 96, IRQ number 80
		GPDMA1_Channel9_IRQHandler,		// GPDMA1_Channel9, address 0x184, exception number 97, IRQ number 81
		GPDMA1_Channel10_IRQHandler,		// GPDMA1_Channel10, address 0x188, exception number 98, IRQ number 82
		GPDMA1_Channel11_IRQHandler,		// GPDMA1_Channel11, address 0x18c, exception number 99, IRQ number 83
		GPDMA1_Channel12_IRQHandler,		// GPDMA1_Channel12, address 0x190, exception number 100, IRQ number 84
		GPDMA1_Channel13_IRQHandler,		// GPDMA1_Channel13, address 0x194, exception number 101, IRQ number 85
		GPDMA1_Channel14_IRQHandler,		// GPDMA1_Channel14, address 0x198, exception number 102, IRQ number 86
		GPDMA1_Channel15_IRQHandler,		// GPDMA1_Channel15, address 0x19c, exception number 103, IRQ number 87
		I2C3_EV_IRQHandler,		// I2C3_EV, address 0x1a0, exception number 104, IRQ number 88
		I2C3_ER_IRQHandler,		// I2C3_ER, address 0x1a4, exception number 105, IRQ number 89
		SAI1_IRQHandler,		// SAI1, address 0x1a8, exception number 106, IRQ number 90
		SAI2_IRQHandler,		// SAI2, address 0x1ac, exception number 107, IRQ number 91
		TSC_IRQHandler,		// TSC, address 0x1b0, exception number 108, IRQ number 92
		nullptr,		// Reserved, address 0x1b4, exception number 109, IRQ number 93
		RNG_IRQHandler,		// RNG, address 0x1b8, exception number 110, IRQ number 94
		FPU_IRQHandler,		// FPU, address 0x1bc, exception number 111, IRQ number 95
		HASH_IRQHandler,		// HASH, address 0x1c0, exception number 112, IRQ number 96
		nullptr,		// Reserved, address 0x1c4, exception number 113, IRQ number 97
		LPTIM3_IRQHandler,		// LPTIM3, address 0x1c8, exception number 114, IRQ number 98
		SPI3_IRQHandler,		// SPI3, address 0x1cc, exception number 115, IRQ number 99
		I2C4_ER_IRQHandler,		// I2C4_ER, address 0x1d0, exception number 116, IRQ number 100
		I2C4_EV_IRQHandler,		// I2C4_EV, address 0x1d4, exception number 117, IRQ number 101
		MDF1_FLT0_IRQHandler,		// MDF1_FLT0, address 0x1d8, exception number 118, IRQ number 102
		MDF1_FLT1_IRQHandler,		// MDF1_FLT1, address 0x1dc, exception number 119, IRQ number 103
		MDF1_FLT2_IRQHandler,		// MDF1_FLT2, address 0x1e0, exception number 120, IRQ number 104
		MDF1_FLT3_IRQHandler,		// MDF1_FLT3, address 0x1e4, exception number 121, IRQ number 105
		UCPD1_IRQHandler,		// UCPD1, address 0x1e8, exception number 122, IRQ number 106
		ICACHE_IRQHandler,		// ICACHE, address 0x1ec, exception number 123, IRQ number 107
		nullptr,		// Reserved, address 0x1f0, exception number 124, IRQ number 108
		nullptr,		// Reserved, address 0x1f4, exception number 125, IRQ number 109
		LPTIM4_IRQHandler,		// LPTIM4, address 0x1f8, exception number 126, IRQ number 110
		DCACHE1_IRQHandler,		// DCACHE1, address 0x1fc, exception number 127, IRQ number 111
		ADF1_IRQHandler,		// ADF1, address 0x200, exception number 128, IRQ number 112
		ADC4_IRQHandler,		// ADC4, address 0x204, exception number 129, IRQ number 113
		LPDMA1_Channel0_IRQHandler,		// LPDMA1_Channel0, address 0x208, exception number 130, IRQ number 114
		LPDMA1_Channel1_IRQHandler,		// LPDMA1_Channel1, address 0x20c, exception number 131, IRQ number 115
		LPDMA1_Channel2_IRQHandler,		// LPDMA1_Channel2, address 0x210, exception number 132, IRQ number 116
		LPDMA1_Channel3_IRQHandler,		// LPDMA1_Channel3, address 0x214, exception number 133, IRQ number 117
		DMA2D_IRQHandler,		// DMA2D, address 0x218, exception number 134, IRQ number 118
		DCMI_PSSI_IRQHandler,		// DCMI_PSSI, address 0x21c, exception number 135, IRQ number 119
		OCTOSPI2_IRQHandler,		// OCTOSPI2, address 0x220, exception number 136, IRQ number 120
		MDF1_FLT4_IRQHandler,		// MDF1_FLT4, address 0x224, exception number 137, IRQ number 121
		MDF1_FLT5_IRQHandler,		// MDF1_FLT5, address 0x228, exception number 138, IRQ number 122
		CORDIC_IRQHandler,		// CORDIC, address 0x22c, exception number 139, IRQ number 123
		FMAC_IRQHandler,		// FMAC, address 0x230, exception number 140, IRQ number 124
		LSECSSD_IRQHandler,		// LSECSSD, address 0x234, exception number 141, IRQ number 125
};

}	// extern "C"
