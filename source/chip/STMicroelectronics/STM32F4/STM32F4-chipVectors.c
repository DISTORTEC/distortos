/**
 * \file
 * \brief STM32F4 chip vector table and default weak handlers.
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-28
 */

#include "distortos/distortosConfiguration.h"

/*---------------------------------------------------------------------------------------------------------------------+
| vectors' configuration
+---------------------------------------------------------------------------------------------------------------------*/

#if defined(CONFIG_CHIP_STM32F401)
#	define STM32F401_VECTORS_
#elif defined(CONFIG_CHIP_STM32F405) || defined(CONFIG_CHIP_STM32F407) || defined(CONFIG_CHIP_STM32F415) || \
	defined(CONFIG_CHIP_STM32F417)
#	define STM32F405_STM32F407_STM32F415_STM32F417_VECTORS_
#elif defined(CONFIG_CHIP_STM32F427) || defined(CONFIG_CHIP_STM32F429) || defined(CONFIG_CHIP_STM32F437) || \
	defined(CONFIG_CHIP_STM32F439)
#	define STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_
#else
#	error "Wrong configuration or unsupported STM32F4 chip!"
#endif

/*---------------------------------------------------------------------------------------------------------------------+
| provide default weak handlers
+---------------------------------------------------------------------------------------------------------------------*/

// Window WatchDog
__attribute__ ((weak)) void WWDG_IRQHandler(void)
{
	while (1);
}

// PVD through EXTI Line detection
__attribute__ ((weak)) void PVD_IRQHandler(void)
{
	while (1);
}

// Tamper and TimeStamps through the EXTI line
__attribute__ ((weak)) void TAMP_STAMP_IRQHandler(void)
{
	while (1);
}

// RTC Wakeup through the EXTI line
__attribute__ ((weak)) void RTC_WKUP_IRQHandler(void)
{
	while (1);
}

// FLASH
__attribute__ ((weak)) void FLASH_IRQHandler(void)
{
	while (1);
}

// RCC
__attribute__ ((weak)) void RCC_IRQHandler(void)
{
	while (1);
}

// EXTI Line0
__attribute__ ((weak)) void EXTI0_IRQHandler(void)
{
	while (1);
}

// EXTI Line1
__attribute__ ((weak)) void EXTI1_IRQHandler(void)
{
	while (1);
}

// EXTI Line2
__attribute__ ((weak)) void EXTI2_IRQHandler(void)
{
	while (1);
}

// EXTI Line3
__attribute__ ((weak)) void EXTI3_IRQHandler(void)
{
	while (1);
}

// EXTI Line4
__attribute__ ((weak)) void EXTI4_IRQHandler(void)
{
	while (1);
}

// DMA1 Stream 0
__attribute__ ((weak)) void DMA1_Stream0_IRQHandler(void)
{
	while (1);
}

// DMA1 Stream 1
__attribute__ ((weak)) void DMA1_Stream1_IRQHandler(void)
{
	while (1);
}

// DMA1 Stream 2
__attribute__ ((weak)) void DMA1_Stream2_IRQHandler(void)
{
	while (1);
}

// DMA1 Stream 3
__attribute__ ((weak)) void DMA1_Stream3_IRQHandler(void)
{
	while (1);
}

// DMA1 Stream 4
__attribute__ ((weak)) void DMA1_Stream4_IRQHandler(void)
{
	while (1);
}

// DMA1 Stream 5
__attribute__ ((weak)) void DMA1_Stream5_IRQHandler(void)
{
	while (1);
}

// DMA1 Stream 6
__attribute__ ((weak)) void DMA1_Stream6_IRQHandler(void)
{
	while (1);
}

// ADC1, ADC2 and ADC3s
__attribute__ ((weak)) void ADC_IRQHandler(void)
{
	while (1);
}

#ifdef STM32F401_VECTORS_

// Reserved 0x8C
__attribute__ ((weak)) void __Reserved_0x8C_Handler(void)
{
	while (1);
}

// Reserved 0x90
__attribute__ ((weak)) void __Reserved_0x90_Handler(void)
{
	while (1);
}

// Reserved 0x94
__attribute__ ((weak)) void __Reserved_0x94_Handler(void)
{
	while (1);
}

// Reserved 0x98
__attribute__ ((weak)) void __Reserved_0x98_Handler(void)
{
	while (1);
}

#else	// !def STM32F401_VECTORS_

// CAN1 TX
__attribute__ ((weak)) void CAN1_TX_IRQHandler(void)
{
	while (1);
}

// CAN1 RX0
__attribute__ ((weak)) void CAN1_RX0_IRQHandler(void)
{
	while (1);
}

// CAN1 RX1
__attribute__ ((weak)) void CAN1_RX1_IRQHandler(void)
{
	while (1);
}

// CAN1 SCE
__attribute__ ((weak)) void CAN1_SCE_IRQHandler(void)
{
	while (1);
}

#endif	// !def STM32F401_VECTORS_

// External Line[9:5]
__attribute__ ((weak)) void EXTI9_5_IRQHandler(void)
{
	while (1);
}

// TIM1 Break and TIM9
__attribute__ ((weak)) void TIM1_BRK_TIM9_IRQHandler(void)
{
	while (1);
}

// TIM1 Update and TIM10
__attribute__ ((weak)) void TIM1_UP_TIM10_IRQHandler(void)
{
	while (1);
}

// TIM1 Trigger and Commutation and TIM11
__attribute__ ((weak)) void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	while (1);
}

// TIM1 Capture Compare
__attribute__ ((weak)) void TIM1_CC_IRQHandler(void)
{
	while (1);
}

// TIM2
__attribute__ ((weak)) void TIM2_IRQHandler(void)
{
	while (1);
}

// TIM3
__attribute__ ((weak)) void TIM3_IRQHandler(void)
{
	while (1);
}

// TIM4
__attribute__ ((weak)) void TIM4_IRQHandler(void)
{
	while (1);
}

// I2C1 Event
__attribute__ ((weak)) void I2C1_EV_IRQHandler(void)
{
	while (1);
}

// I2C1 Error
__attribute__ ((weak)) void I2C1_ER_IRQHandler(void)
{
	while (1);
}

// I2C2 Event
__attribute__ ((weak)) void I2C2_EV_IRQHandler(void)
{
	while (1);
}

// I2C2 Error
__attribute__ ((weak)) void I2C2_ER_IRQHandler(void)
{
	while (1);
}

// SPI1
__attribute__ ((weak)) void SPI1_IRQHandler(void)
{
	while (1);
}

// SPI2
__attribute__ ((weak)) void SPI2_IRQHandler(void)
{
	while (1);
}

// USART1
__attribute__ ((weak)) void USART1_IRQHandler(void)
{
	while (1);
}

// USART2
__attribute__ ((weak)) void USART2_IRQHandler(void)
{
	while (1);
}

#ifdef STM32F401_VECTORS_

// Reserved 0xDC
__attribute__ ((weak)) void __Reserved_0xDC_Handler(void)
{
	while (1);
}

#else	// !def STM32F401_VECTORS_

// USART3
__attribute__ ((weak)) void USART3_IRQHandler(void)
{
	while (1);
}

#endif	// !def STM32F401_VECTORS_

// External Line[15:10]s
__attribute__ ((weak)) void EXTI15_10_IRQHandler(void)
{
	while (1);
}

// RTC Alarm (A and B) through EXTI Line
__attribute__ ((weak)) void RTC_Alarm_IRQHandler(void)
{
	while (1);
}

// USB OTG FS Wakeup through EXTI line
__attribute__ ((weak)) void OTG_FS_WKUP_IRQHandler(void)
{
	while (1);
}

#ifdef STM32F401_VECTORS_

// Reserved 0xEC
__attribute__ ((weak)) void __Reserved_0xEC_Handler(void)
{
	while (1);
}

// Reserved 0xF0
__attribute__ ((weak)) void __Reserved_0xF0_Handler(void)
{
	while (1);
}

// Reserved 0xF4
__attribute__ ((weak)) void __Reserved_0xF4_Handler(void)
{
	while (1);
}

// Reserved 0xF8
__attribute__ ((weak)) void __Reserved_0xF8_Handler(void)
{
	while (1);
}

#else	// !def STM32F401_VECTORS_

// TIM8 Break and TIM12
__attribute__ ((weak)) void TIM8_BRK_TIM12_IRQHandler(void)
{
	while (1);
}

// TIM8 Update and TIM13
__attribute__ ((weak)) void TIM8_UP_TIM13_IRQHandler(void)
{
	while (1);
}

// TIM8 Trigger and Commutation and TIM14
__attribute__ ((weak)) void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	while (1);
}

// TIM8 Capture Compare
__attribute__ ((weak)) void TIM8_CC_IRQHandler(void)
{
	while (1);
}

#endif	// !def STM32F401_VECTORS_

// DMA1 Stream7
__attribute__ ((weak)) void DMA1_Stream7_IRQHandler(void)
{
	while (1);
}

#ifdef STM32F401_VECTORS_

// Reserved 0x100
__attribute__ ((weak)) void __Reserved_0x100_Handler(void)
{
	while (1);
}

#else	// !def STM32F401_VECTORS_

// FSMC
__attribute__ ((weak)) void FSMC_IRQHandler(void)
{
	while (1);
}

#endif	// !def STM32F401_VECTORS_

// SDIO
__attribute__ ((weak)) void SDIO_IRQHandler(void)
{
	while (1);
}

// TIM5
__attribute__ ((weak)) void TIM5_IRQHandler(void)
{
	while (1);
}

// SPI3
__attribute__ ((weak)) void SPI3_IRQHandler(void)
{
	while (1);
}

#ifdef STM32F401_VECTORS_

// Reserved 0x110
__attribute__ ((weak)) void __Reserved_0x110_Handler(void)
{
	while (1);
}

// Reserved 0x114
__attribute__ ((weak)) void __Reserved_0x114_Handler(void)
{
	while (1);
}

// Reserved 0x118
__attribute__ ((weak)) void __Reserved_0x118_Handler(void)
{
	while (1);
}

// Reserved 0x11C
__attribute__ ((weak)) void __Reserved_0x11C_Handler(void)
{
	while (1);
}

#else	// !def STM32F401_VECTORS_

// UART4
__attribute__ ((weak)) void UART4_IRQHandler(void)
{
	while (1);
}

// UART5
__attribute__ ((weak)) void UART5_IRQHandler(void)
{
	while (1);
}

// TIM6 and DAC1&2 underrun errors
__attribute__ ((weak)) void TIM6_DAC_IRQHandler(void)
{
	while (1);
}

// TIM7
__attribute__ ((weak)) void TIM7_IRQHandler(void)
{
	while (1);
}

#endif	// !def STM32F401_VECTORS_

// DMA2 Stream 0
__attribute__ ((weak)) void DMA2_Stream0_IRQHandler(void)
{
	while (1);
}

// DMA2 Stream 1
__attribute__ ((weak)) void DMA2_Stream1_IRQHandler(void)
{
	while (1);
}

// DMA2 Stream 2
__attribute__ ((weak)) void DMA2_Stream2_IRQHandler(void)
{
	while (1);
}

// DMA2 Stream 3
__attribute__ ((weak)) void DMA2_Stream3_IRQHandler(void)
{
	while (1);
}

// DMA2 Stream 4
__attribute__ ((weak)) void DMA2_Stream4_IRQHandler(void)
{
	while (1);
}

#ifdef STM32F401_VECTORS_

// Reserved 0x134
__attribute__ ((weak)) void __Reserved_0x134_Handler(void)
{
	while (1);
}

// Reserved 0x138
__attribute__ ((weak)) void __Reserved_0x138_Handler(void)
{
	while (1);
}

// Reserved 0x13C
__attribute__ ((weak)) void __Reserved_0x13C_Handler(void)
{
	while (1);
}

// Reserved 0x140
__attribute__ ((weak)) void __Reserved_0x140_Handler(void)
{
	while (1);
}

// Reserved 0x144
__attribute__ ((weak)) void __Reserved_0x144_Handler(void)
{
	while (1);
}

// Reserved 0x148
__attribute__ ((weak)) void __Reserved_0x148_Handler(void)
{
	while (1);
}

#else	// !def STM32F401_VECTORS_

// Ethernet
__attribute__ ((weak)) void ETH_IRQHandler(void)
{
	while (1);
}

// Ethernet Wakeup through EXTI line
__attribute__ ((weak)) void ETH_WKUP_IRQHandler(void)
{
	while (1);
}

// CAN2 TX
__attribute__ ((weak)) void CAN2_TX_IRQHandler(void)
{
	while (1);
}

// CAN2 RX0
__attribute__ ((weak)) void CAN2_RX0_IRQHandler(void)
{
	while (1);
}

// CAN2 RX1
__attribute__ ((weak)) void CAN2_RX1_IRQHandler(void)
{
	while (1);
}

// CAN2 SCE
__attribute__ ((weak)) void CAN2_SCE_IRQHandler(void)
{
	while (1);
}

#endif	// !def STM32F401_VECTORS_

// USB OTG FS
__attribute__ ((weak)) void OTG_FS_IRQHandler(void)
{
	while (1);
}

// DMA2 Stream 5
__attribute__ ((weak)) void DMA2_Stream5_IRQHandler(void)
{
	while (1);
}

// DMA2 Stream 6
__attribute__ ((weak)) void DMA2_Stream6_IRQHandler(void)
{
	while (1);
}

// DMA2 Stream 7
__attribute__ ((weak)) void DMA2_Stream7_IRQHandler(void)
{
	while (1);
}

// USART6
__attribute__ ((weak)) void USART6_IRQHandler(void)
{
	while (1);
}

// I2C3 event
__attribute__ ((weak)) void I2C3_EV_IRQHandler(void)
{
	while (1);
}

// I2C3 error
__attribute__ ((weak)) void I2C3_ER_IRQHandler(void)
{
	while (1);
}

#ifdef STM32F401_VECTORS_

// Reserved 0x168
__attribute__ ((weak)) void __Reserved_0x168_Handler(void)
{
	while (1);
}

// Reserved 0x16C
__attribute__ ((weak)) void __Reserved_0x16C_Handler(void)
{
	while (1);
}

// Reserved 0x170
__attribute__ ((weak)) void __Reserved_0x170_Handler(void)
{
	while (1);
}

// Reserved 0x174
__attribute__ ((weak)) void __Reserved_0x174_Handler(void)
{
	while (1);
}

// Reserved 0x178
__attribute__ ((weak)) void __Reserved_0x178_Handler(void)
{
	while (1);
}

// Reserved 0x17C
__attribute__ ((weak)) void __Reserved_0x17C_Handler(void)
{
	while (1);
}

// Reserved 0x180
__attribute__ ((weak)) void __Reserved_0x180_Handler(void)
{
	while (1);
}

#else	// !def STM32F401_VECTORS_

// USB OTG HS End Point 1 Out
__attribute__ ((weak)) void OTG_HS_EP1_OUT_IRQHandler(void)
{
	while (1);
}

// USB OTG HS End Point 1 In
__attribute__ ((weak)) void OTG_HS_EP1_IN_IRQHandler(void)
{
	while (1);
}

// USB OTG HS Wakeup through EXTI
__attribute__ ((weak)) void OTG_HS_WKUP_IRQHandler(void)
{
	while (1);
}

// USB OTG HS
__attribute__ ((weak)) void OTG_HS_IRQHandler(void)
{
	while (1);
}

// DCMI
__attribute__ ((weak)) void DCMI_IRQHandler(void)
{
	while (1);
}

// CRYP crypto
__attribute__ ((weak)) void CRYP_IRQHandler(void)
{
	while (1);
}

// Hash and Rng
__attribute__ ((weak)) void HASH_RNG_IRQHandler(void)
{
	while (1);
}

#endif	// !def STM32F401_VECTORS_

// FPU
__attribute__ ((weak)) void FPU_IRQHandler(void)
{
	while (1);
}

#if defined(STM32F401_VECTORS_) || defined(STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_)

#	ifdef STM32F401_VECTORS_

// Reserved 0x188
__attribute__ ((weak)) void __Reserved_0x188_Handler(void)
{
	while (1);
}

// Reserved 0x18C
__attribute__ ((weak)) void __Reserved_0x18C_Handler(void)
{
	while (1);
}

#	else	// !def STM32F401_VECTORS_

// UART7
__attribute__ ((weak)) void UART7_IRQHandler(void)
{
	while (1);
}

// UART8
__attribute__ ((weak)) void UART8_IRQHandler(void)
{
	while (1);
}

#	endif	// !def STM32F401_VECTORS_

// SPI4
__attribute__ ((weak)) void SPI4_IRQHandler(void)
{
	while (1);
}

#endif	// defined(STM32F401_VECTORS_) || defined(STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_)

#ifdef STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_

// SPI5
__attribute__ ((weak)) void SPI5_IRQHandler(void)
{
	while (1);
}

// SPI6
__attribute__ ((weak)) void SPI6_IRQHandler(void)
{
	while (1);
}

// SAI1
__attribute__ ((weak)) void SAI1_IRQHandler(void)
{
	while (1);
}

// LTDC
__attribute__ ((weak)) void LTDC_IRQHandler(void)
{
	while (1);
}

// LTDC error
__attribute__ ((weak)) void LTDC_ER_IRQHandler(void)
{
	while (1);
}

// DMA2D
__attribute__ ((weak)) void DMA2D_IRQHandler(void)
{
	while (1);
}

#endif	// def STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// single interrupt vector - pointer to function with no arguments and no return value
typedef void (*InterruptVector)(void);

/*---------------------------------------------------------------------------------------------------------------------+
| global variables
+---------------------------------------------------------------------------------------------------------------------*/

/// chip vector table
const InterruptVector chipVectors[] __attribute__ ((section(".chipVectors"))) =
{
		WWDG_IRQHandler,					// Window WatchDog
		PVD_IRQHandler,						// PVD through EXTI Line detection
		TAMP_STAMP_IRQHandler,				// Tamper and TimeStamps through the EXTI line
		RTC_WKUP_IRQHandler,				// RTC Wakeup through the EXTI line
		FLASH_IRQHandler,					// FLASH
		RCC_IRQHandler,						// RCC
		EXTI0_IRQHandler,					// EXTI Line0
		EXTI1_IRQHandler,					// EXTI Line1
		EXTI2_IRQHandler,					// EXTI Line2
		EXTI3_IRQHandler,					// EXTI Line3
		EXTI4_IRQHandler,					// EXTI Line4
		DMA1_Stream0_IRQHandler,			// DMA1 Stream 0
		DMA1_Stream1_IRQHandler,			// DMA1 Stream 1
		DMA1_Stream2_IRQHandler,			// DMA1 Stream 2
		DMA1_Stream3_IRQHandler,			// DMA1 Stream 3
		DMA1_Stream4_IRQHandler,			// DMA1 Stream 4
		DMA1_Stream5_IRQHandler,			// DMA1 Stream 5
		DMA1_Stream6_IRQHandler,			// DMA1 Stream 6
		ADC_IRQHandler,						// ADC1, ADC2 and ADC3s

#ifdef STM32F401_VECTORS_

		__Reserved_0x8C_Handler,			// Reserved 0x8C
		__Reserved_0x90_Handler,			// Reserved 0x90
		__Reserved_0x94_Handler,			// Reserved 0x94
		__Reserved_0x98_Handler,			// Reserved 0x98

#else	// !def STM32F401_VECTORS_

		CAN1_TX_IRQHandler,					// CAN1 TX
		CAN1_RX0_IRQHandler,				// CAN1 RX0
		CAN1_RX1_IRQHandler,				// CAN1 RX1
		CAN1_SCE_IRQHandler,				// CAN1 SCE

#endif	// !def STM32F401_VECTORS_

		EXTI9_5_IRQHandler,					// External Line[9:5]s
		TIM1_BRK_TIM9_IRQHandler,			// TIM1 Break and TIM9
		TIM1_UP_TIM10_IRQHandler,			// TIM1 Update and TIM10
		TIM1_TRG_COM_TIM11_IRQHandler,		// TIM1 Trigger and Commutation and TIM11
		TIM1_CC_IRQHandler,					// TIM1 Capture Compare
		TIM2_IRQHandler,					// TIM2
		TIM3_IRQHandler,					// TIM3
		TIM4_IRQHandler,					// TIM4
		I2C1_EV_IRQHandler,					// I2C1 Event
		I2C1_ER_IRQHandler,					// I2C1 Error
		I2C2_EV_IRQHandler,					// I2C2 Event
		I2C2_ER_IRQHandler,					// I2C2 Error
		SPI1_IRQHandler,					// SPI1
		SPI2_IRQHandler,					// SPI2
		USART1_IRQHandler,					// USART1
		USART2_IRQHandler,					// USART2

#ifdef STM32F401_VECTORS_

		__Reserved_0xDC_Handler,			// Reserved 0xDC

#else	// !def STM32F401_VECTORS_

		USART3_IRQHandler,					// USART3

#endif	// !def STM32F401_VECTORS_

		EXTI15_10_IRQHandler,				// External Line[15:10]s
		RTC_Alarm_IRQHandler,				// RTC Alarm (A and B) through EXTI Line
		OTG_FS_WKUP_IRQHandler,				// USB OTG FS Wakeup through EXTI line

#ifdef STM32F401_VECTORS_

		__Reserved_0xEC_Handler,			// Reserved 0xEC
		__Reserved_0xF0_Handler,			// Reserved 0xF0
		__Reserved_0xF4_Handler,			// Reserved 0xF4
		__Reserved_0xF8_Handler,			// Reserved 0xF8

#else	// !def STM32F401_VECTORS_

		TIM8_BRK_TIM12_IRQHandler,			// TIM8 Break and TIM12
		TIM8_UP_TIM13_IRQHandler,			// TIM8 Update and TIM13
		TIM8_TRG_COM_TIM14_IRQHandler,		// TIM8 Trigger and Commutation and TIM14
		TIM8_CC_IRQHandler,					// TIM8 Capture Compare

#endif	// !def STM32F401_VECTORS_

		DMA1_Stream7_IRQHandler,			// DMA1 Stream7

#ifdef STM32F401_VECTORS_

		__Reserved_0x100_Handler,			// Reserved 0x100

#else	// !def STM32F401_VECTORS_

		FSMC_IRQHandler,					// FSMC

#endif	// !def STM32F401_VECTORS_

		SDIO_IRQHandler,					// SDIO
		TIM5_IRQHandler,					// TIM5
		SPI3_IRQHandler,					// SPI3

#ifdef STM32F401_VECTORS_

		__Reserved_0x110_Handler,			// Reserved 0x110
		__Reserved_0x114_Handler,			// Reserved 0x114
		__Reserved_0x118_Handler,			// Reserved 0x118
		__Reserved_0x11C_Handler,			// Reserved 0x11C

#else	// !def STM32F401_VECTORS_

		UART4_IRQHandler,					// UART4
		UART5_IRQHandler,					// UART5
		TIM6_DAC_IRQHandler,				// TIM6 and DAC1&2 underrun errors
		TIM7_IRQHandler,					// TIM7

#endif	// !def STM32F401_VECTORS_

		DMA2_Stream0_IRQHandler,			// DMA2 Stream 0
		DMA2_Stream1_IRQHandler,			// DMA2 Stream 1
		DMA2_Stream2_IRQHandler,			// DMA2 Stream 2
		DMA2_Stream3_IRQHandler,			// DMA2 Stream 3
		DMA2_Stream4_IRQHandler,			// DMA2 Stream 4

#ifdef STM32F401_VECTORS_

		__Reserved_0x134_Handler,			// Reserved 0x134
		__Reserved_0x138_Handler,			// Reserved 0x138
		__Reserved_0x13C_Handler,			// Reserved 0x13C
		__Reserved_0x140_Handler,			// Reserved 0x140
		__Reserved_0x144_Handler,			// Reserved 0x144
		__Reserved_0x148_Handler,			// Reserved 0x148

#else	// !def STM32F401_VECTORS_

		ETH_IRQHandler,						// Ethernet
		ETH_WKUP_IRQHandler,				// Ethernet Wakeup through EXTI line
		CAN2_TX_IRQHandler,					// CAN2 TX
		CAN2_RX0_IRQHandler,				// CAN2 RX0
		CAN2_RX1_IRQHandler,				// CAN2 RX1
		CAN2_SCE_IRQHandler,				// CAN2 SCE

#endif	// !def STM32F401_VECTORS_

		OTG_FS_IRQHandler,					// USB OTG FS
		DMA2_Stream5_IRQHandler,			// DMA2 Stream 5
		DMA2_Stream6_IRQHandler,			// DMA2 Stream 6
		DMA2_Stream7_IRQHandler,			// DMA2 Stream 7
		USART6_IRQHandler,					// USART6
		I2C3_EV_IRQHandler,					// I2C3 event
		I2C3_ER_IRQHandler,					// I2C3 error

#ifdef STM32F401_VECTORS_

		__Reserved_0x168_Handler,			// Reserved 0x168
		__Reserved_0x16C_Handler,			// Reserved 0x16C
		__Reserved_0x170_Handler,			// Reserved 0x170
		__Reserved_0x174_Handler,			// Reserved 0x174
		__Reserved_0x178_Handler,			// Reserved 0x178
		__Reserved_0x17C_Handler,			// Reserved 0x17C
		__Reserved_0x180_Handler,			// Reserved 0x180

#else	// !def STM32F401_VECTORS_

		OTG_HS_EP1_OUT_IRQHandler,			// USB OTG HS End Point 1 Out
		OTG_HS_EP1_IN_IRQHandler,			// USB OTG HS End Point 1 In
		OTG_HS_WKUP_IRQHandler,				// USB OTG HS Wakeup through EXTI
		OTG_HS_IRQHandler,					// USB OTG HS
		DCMI_IRQHandler,					// DCMI
		CRYP_IRQHandler,					// CRYP crypto
		HASH_RNG_IRQHandler,				// Hash and Rng

#endif	// !def STM32F401_VECTORS_

		FPU_IRQHandler,						// FPU

#if defined(STM32F401_VECTORS_) || defined(STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_)

#	ifdef STM32F401_VECTORS_

		__Reserved_0x188_Handler,			// Reserved 0x188
		__Reserved_0x18C_Handler,			// Reserved 0x18C

#	else	// !def STM32F401_VECTORS_

		UART7_IRQHandler,					// UART7
		UART8_IRQHandler,					// UART8

#	endif	// !def STM32F401_VECTORS_

		SPI4_IRQHandler,					// SPI4

#endif	// defined(STM32F401_VECTORS_) || defined(STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_)

#ifdef STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_

		SPI5_IRQHandler,					// SPI5
		SPI6_IRQHandler,					// SPI6
		SAI1_IRQHandler,					// SAI1
		LTDC_IRQHandler,					// LTDC
		LTDC_ER_IRQHandler,					// LTDC error
		DMA2D_IRQHandler,					// DMA2D

#endif	// def STM32F427_STM32F429_STM32F437_STM32F439_VECTORS_

};
