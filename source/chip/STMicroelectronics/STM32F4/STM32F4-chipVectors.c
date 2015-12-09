/**
 * \file
 * \brief STM32F4 chip vector table and default weak handlers.
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-09
 */

#include "distortos/distortosConfiguration.h"

/*---------------------------------------------------------------------------------------------------------------------+
| vectors' configuration
+---------------------------------------------------------------------------------------------------------------------*/

#if defined(CONFIG_CHIP_STM32F401)
#	define STM32F401_VECTORS
#elif defined(CONFIG_CHIP_STM32F405) || defined(CONFIG_CHIP_STM32F407) || defined(CONFIG_CHIP_STM32F415) || \
	defined(CONFIG_CHIP_STM32F417)
#	define STM32F405_STM32F407_STM32F415_STM32F417_VECTORS
#elif defined(CONFIG_CHIP_STM32F410)
#	define STM32F410_VECTORS
#elif defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43)
#	define STM32F42_STM32F43_VECTORS
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

// Tamper and TimeStamp through the EXTI line
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

// EXTI Line 0
__attribute__ ((weak)) void EXTI0_IRQHandler(void)
{
	while (1);
}

// EXTI Line 1
__attribute__ ((weak)) void EXTI1_IRQHandler(void)
{
	while (1);
}

// EXTI Line 2
__attribute__ ((weak)) void EXTI2_IRQHandler(void)
{
	while (1);
}

// EXTI Line 3
__attribute__ ((weak)) void EXTI3_IRQHandler(void)
{
	while (1);
}

// EXTI Line 4
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

// ADC1, ADC2 and ADC3
__attribute__ ((weak)) void ADC_IRQHandler(void)
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

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

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// EXTI Lines [9:5]
__attribute__ ((weak)) void EXTI9_5_IRQHandler(void)
{
	while (1);
}

// TIM1 Break and TIM9
__attribute__ ((weak)) void TIM1_BRK_TIM9_IRQHandler(void)
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// TIM1 Update
__attribute__ ((weak)) void TIM1_UP_IRQHandler(void)
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// TIM1 Update and TIM10
__attribute__ ((weak)) void TIM1_UP_TIM10_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

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

#if defined(STM32F410_VECTORS)

// Reserved 0xB0
__attribute__ ((weak)) void __Reserved_0xB0_Handler(void)
{
	while (1);
}

// Reserved 0xB4
__attribute__ ((weak)) void __Reserved_0xB4_Handler(void)
{
	while (1);
}

// Reserved 0xB8
__attribute__ ((weak)) void __Reserved_0xB8_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F410_VECTORS)

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

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

// Reserved 0xDC
__attribute__ ((weak)) void __Reserved_0xDC_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// USART3
__attribute__ ((weak)) void USART3_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// EXTI Lines [15:10]
__attribute__ ((weak)) void EXTI15_10_IRQHandler(void)
{
	while (1);
}

// RTC Alarm (A and B) through the EXTI line
__attribute__ ((weak)) void RTC_Alarm_IRQHandler(void)
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// Reserved 0xE8
__attribute__ ((weak)) void __Reserved_0xE8_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// USB OTG FS Wakeup through the EXTI line
__attribute__ ((weak)) void OTG_FS_WKUP_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

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

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// DMA1 Stream7
__attribute__ ((weak)) void DMA1_Stream7_IRQHandler(void)
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

// Reserved 0x100
__attribute__ ((weak)) void __Reserved_0x100_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// FSMC
__attribute__ ((weak)) void FSMC_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS)

// Reserved 0x104
__attribute__ ((weak)) void __Reserved_0x104_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// SDIO
__attribute__ ((weak)) void SDIO_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

// TIM5
__attribute__ ((weak)) void TIM5_IRQHandler(void)
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// Reserved 0x10C
__attribute__ ((weak)) void __Reserved_0x10C_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// SPI3
__attribute__ ((weak)) void SPI3_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

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

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

// Reserved 0x118
__attribute__ ((weak)) void __Reserved_0x118_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS)

// TIM6 and DAC1&2 underrun error
__attribute__ ((weak)) void TIM6_DAC_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

// Reserved 0x11C
__attribute__ ((weak)) void __Reserved_0x11C_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// TIM7
__attribute__ ((weak)) void TIM7_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

// Reserved 0x134
__attribute__ ((weak)) void __Reserved_0x134_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// Ethernet
__attribute__ ((weak)) void ETH_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

// Reserved 0x138
__attribute__ ((weak)) void __Reserved_0x138_Handler(void)
{
	while (1);
}

#elif defined(STM32F410_VECTORS)

// EXTI Line 19
__attribute__ ((weak)) void EXTI19_IRQHandler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// Ethernet Wakeup through the EXTI line
__attribute__ ((weak)) void ETH_WKUP_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

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

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS)

// Reserved 0x14C
__attribute__ ((weak)) void __Reserved_0x14C_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// USB OTG FS
__attribute__ ((weak)) void OTG_FS_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

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

#if defined(STM32F410_VECTORS)

// Reserved 0x160
__attribute__ ((weak)) void __Reserved_0x160_Handler(void)
{
	while (1);
}

// Reserved 0x164
__attribute__ ((weak)) void __Reserved_0x164_Handler(void)
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

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

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

// Reserved 0x170
__attribute__ ((weak)) void __Reserved_0x170_Handler(void)
{
	while (1);
}

#elif defined(STM32F410_VECTORS)

// EXTI Line 20
__attribute__ ((weak)) void EXTI20_IRQHandler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// USB OTG HS Wakeup through the EXTI line
__attribute__ ((weak)) void OTG_HS_WKUP_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

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

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

// Reserved 0x180
__attribute__ ((weak)) void __Reserved_0x180_Handler(void)
{
	while (1);
}

#elif defined(STM32F410_VECTORS)

// RNG
__attribute__ ((weak)) void RNG_IRQHandler(void)
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// Hash and RNG
__attribute__ ((weak)) void HASH_RNG_IRQHandler(void)
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

// FPU
__attribute__ ((weak)) void FPU_IRQHandler(void)
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

#	if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

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

#	else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

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

#	endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#	if defined(STM32F410_VECTORS)

// Reserved 0x190
__attribute__ ((weak)) void __Reserved_0x190_Handler(void)
{
	while (1);
}

#	else	// !defined(STM32F410_VECTORS)

// SPI4
__attribute__ ((weak)) void SPI4_IRQHandler(void)
{
	while (1);
}

#	endif	// !defined(STM32F410_VECTORS)

#endif	// defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

// SPI5
__attribute__ ((weak)) void SPI5_IRQHandler(void)
{
	while (1);
}

#	if defined(STM32F410_VECTORS)

// Reserved 0x198
__attribute__ ((weak)) void __Reserved_0x198_Handler(void)
{
	while (1);
}

// Reserved 0x19C
__attribute__ ((weak)) void __Reserved_0x19C_Handler(void)
{
	while (1);
}

// Reserved 0x1A0
__attribute__ ((weak)) void __Reserved_0x1A0_Handler(void)
{
	while (1);
}

// Reserved 0x1A4
__attribute__ ((weak)) void __Reserved_0x1A4_Handler(void)
{
	while (1);
}

// Reserved 0x1A8
__attribute__ ((weak)) void __Reserved_0x1A8_Handler(void)
{
	while (1);
}

#	else	// !defined(STM32F410_VECTORS)

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

#	endif	// !defined(STM32F410_VECTORS)

#endif	// defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

#if defined(STM32F410_VECTORS)

// Reserved 0x1AC
__attribute__ ((weak)) void __Reserved_0x1AC_Handler(void)
{
	while (1);
}

// Reserved 0x1B0
__attribute__ ((weak)) void __Reserved_0x1B0_Handler(void)
{
	while (1);
}

// Reserved 0x1B4
__attribute__ ((weak)) void __Reserved_0x1B4_Handler(void)
{
	while (1);
}

// Reserved 0x1B8
__attribute__ ((weak)) void __Reserved_0x1B8_Handler(void)
{
	while (1);
}

// FMPI2C1 Event
__attribute__ ((weak)) void FMPI2C1_EV_IRQHandler(void)
{
	while (1);
}

// FMPI2C1 Error
__attribute__ ((weak)) void FMPI2C1_ER_IRQHandler(void)
{
	while (1);
}

// LPTIM1 through the EXTI line
__attribute__ ((weak)) void LPTIM1_IRQHandler(void)
{
	while (1);
}

#endif	// defined(STM32F410_VECTORS)

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
		TAMP_STAMP_IRQHandler,				// Tamper and TimeStamp through the EXTI line
		RTC_WKUP_IRQHandler,				// RTC Wakeup through the EXTI line
		FLASH_IRQHandler,					// FLASH
		RCC_IRQHandler,						// RCC
		EXTI0_IRQHandler,					// EXTI Line 0
		EXTI1_IRQHandler,					// EXTI Line 1
		EXTI2_IRQHandler,					// EXTI Line 2
		EXTI3_IRQHandler,					// EXTI Line 3
		EXTI4_IRQHandler,					// EXTI Line 4
		DMA1_Stream0_IRQHandler,			// DMA1 Stream 0
		DMA1_Stream1_IRQHandler,			// DMA1 Stream 1
		DMA1_Stream2_IRQHandler,			// DMA1 Stream 2
		DMA1_Stream3_IRQHandler,			// DMA1 Stream 3
		DMA1_Stream4_IRQHandler,			// DMA1 Stream 4
		DMA1_Stream5_IRQHandler,			// DMA1 Stream 5
		DMA1_Stream6_IRQHandler,			// DMA1 Stream 6
		ADC_IRQHandler,						// ADC1, ADC2 and ADC3

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x8C_Handler,			// Reserved 0x8C
		__Reserved_0x90_Handler,			// Reserved 0x90
		__Reserved_0x94_Handler,			// Reserved 0x94
		__Reserved_0x98_Handler,			// Reserved 0x98

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		CAN1_TX_IRQHandler,					// CAN1 TX
		CAN1_RX0_IRQHandler,				// CAN1 RX0
		CAN1_RX1_IRQHandler,				// CAN1 RX1
		CAN1_SCE_IRQHandler,				// CAN1 SCE

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		EXTI9_5_IRQHandler,					// EXTI Lines [9:5]
		TIM1_BRK_TIM9_IRQHandler,			// TIM1 Break and TIM9

#if defined(STM32F410_VECTORS)

		TIM1_UP_IRQHandler,					// TIM1 Update

#else	// !defined(STM32F410_VECTORS)

		TIM1_UP_TIM10_IRQHandler,			// TIM1 Update and TIM10

#endif	// !defined(STM32F410_VECTORS)

		TIM1_TRG_COM_TIM11_IRQHandler,		// TIM1 Trigger and Commutation and TIM11
		TIM1_CC_IRQHandler,					// TIM1 Capture Compare

#if defined(STM32F410_VECTORS)

		__Reserved_0xB0_Handler,			// Reserved 0xB0
		__Reserved_0xB4_Handler,			// Reserved 0xB4
		__Reserved_0xB8_Handler,			// Reserved 0xB8

#else	// !defined(STM32F410_VECTORS)

		TIM2_IRQHandler,					// TIM2
		TIM3_IRQHandler,					// TIM3
		TIM4_IRQHandler,					// TIM4

#endif	// !defined(STM32F410_VECTORS)

		I2C1_EV_IRQHandler,					// I2C1 Event
		I2C1_ER_IRQHandler,					// I2C1 Error
		I2C2_EV_IRQHandler,					// I2C2 Event
		I2C2_ER_IRQHandler,					// I2C2 Error
		SPI1_IRQHandler,					// SPI1
		SPI2_IRQHandler,					// SPI2
		USART1_IRQHandler,					// USART1
		USART2_IRQHandler,					// USART2

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0xDC_Handler,			// Reserved 0xDC

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		USART3_IRQHandler,					// USART3

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		EXTI15_10_IRQHandler,				// EXTI Lines [15:10]
		RTC_Alarm_IRQHandler,				// RTC Alarm (A and B) through the EXTI line

#if defined(STM32F410_VECTORS)

		__Reserved_0xE8_Handler,			// Reserved 0xE8

#else	// !defined(STM32F410_VECTORS)

		OTG_FS_WKUP_IRQHandler,				// USB OTG FS Wakeup through the EXTI line

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0xEC_Handler,			// Reserved 0xEC
		__Reserved_0xF0_Handler,			// Reserved 0xF0
		__Reserved_0xF4_Handler,			// Reserved 0xF4
		__Reserved_0xF8_Handler,			// Reserved 0xF8

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		TIM8_BRK_TIM12_IRQHandler,			// TIM8 Break and TIM12
		TIM8_UP_TIM13_IRQHandler,			// TIM8 Update and TIM13
		TIM8_TRG_COM_TIM14_IRQHandler,		// TIM8 Trigger and Commutation and TIM14
		TIM8_CC_IRQHandler,					// TIM8 Capture Compare

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		DMA1_Stream7_IRQHandler,			// DMA1 Stream7

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x100_Handler,			// Reserved 0x100

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		FSMC_IRQHandler,					// FSMC

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS)

		__Reserved_0x104_Handler,			// Reserved 0x104

#else	// !defined(STM32F410_VECTORS)

		SDIO_IRQHandler,					// SDIO

#endif	// !defined(STM32F410_VECTORS)

		TIM5_IRQHandler,					// TIM5

#if defined(STM32F410_VECTORS)

		__Reserved_0x10C_Handler,			// Reserved 0x10C

#else	// !defined(STM32F410_VECTORS)

		SPI3_IRQHandler,					// SPI3

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x110_Handler,			// Reserved 0x110
		__Reserved_0x114_Handler,			// Reserved 0x114

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		UART4_IRQHandler,					// UART4
		UART5_IRQHandler,					// UART5

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

		__Reserved_0x118_Handler,			// Reserved 0x118

#else	// !defined(STM32F401_VECTORS)

		TIM6_DAC_IRQHandler,				// TIM6 and DAC1&2 underrun error

#endif	// !defined(STM32F401_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x11C_Handler,			// Reserved 0x11C

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		TIM7_IRQHandler,					// TIM7

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		DMA2_Stream0_IRQHandler,			// DMA2 Stream 0
		DMA2_Stream1_IRQHandler,			// DMA2 Stream 1
		DMA2_Stream2_IRQHandler,			// DMA2 Stream 2
		DMA2_Stream3_IRQHandler,			// DMA2 Stream 3
		DMA2_Stream4_IRQHandler,			// DMA2 Stream 4

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x134_Handler,			// Reserved 0x134

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		ETH_IRQHandler,						// Ethernet

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

		__Reserved_0x138_Handler,			// Reserved 0x138

#elif defined(STM32F410_VECTORS)

		EXTI19_IRQHandler,					// EXTI Line 19

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		ETH_WKUP_IRQHandler,				// Ethernet Wakeup through the EXTI line

#endif	// !defined(STM32F401_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x13C_Handler,			// Reserved 0x13C
		__Reserved_0x140_Handler,			// Reserved 0x140
		__Reserved_0x144_Handler,			// Reserved 0x144
		__Reserved_0x148_Handler,			// Reserved 0x148

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		CAN2_TX_IRQHandler,					// CAN2 TX
		CAN2_RX0_IRQHandler,				// CAN2 RX0
		CAN2_RX1_IRQHandler,				// CAN2 RX1
		CAN2_SCE_IRQHandler,				// CAN2 SCE

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS)

		__Reserved_0x14C_Handler,			// Reserved 0x14C

#else	// !defined(STM32F410_VECTORS)

		OTG_FS_IRQHandler,					// USB OTG FS

#endif	// !defined(STM32F410_VECTORS)

		DMA2_Stream5_IRQHandler,			// DMA2 Stream 5
		DMA2_Stream6_IRQHandler,			// DMA2 Stream 6
		DMA2_Stream7_IRQHandler,			// DMA2 Stream 7
		USART6_IRQHandler,					// USART6

#if defined(STM32F410_VECTORS)

		__Reserved_0x160_Handler,			// Reserved 0x160
		__Reserved_0x164_Handler,			// Reserved 0x164

#else	// !defined(STM32F410_VECTORS)

		I2C3_EV_IRQHandler,					// I2C3 event
		I2C3_ER_IRQHandler,					// I2C3 error

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x168_Handler,			// Reserved 0x168
		__Reserved_0x16C_Handler,			// Reserved 0x16C

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		OTG_HS_EP1_OUT_IRQHandler,			// USB OTG HS End Point 1 Out
		OTG_HS_EP1_IN_IRQHandler,			// USB OTG HS End Point 1 In

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

		__Reserved_0x170_Handler,			// Reserved 0x170

#elif defined(STM32F410_VECTORS)

		EXTI20_IRQHandler,					// EXTI Line 20

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		OTG_HS_WKUP_IRQHandler,				// USB OTG HS Wakeup through the EXTI line

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x174_Handler,			// Reserved 0x174
		__Reserved_0x178_Handler,			// Reserved 0x178
		__Reserved_0x17C_Handler,			// Reserved 0x17C

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		OTG_HS_IRQHandler,					// USB OTG HS
		DCMI_IRQHandler,					// DCMI
		CRYP_IRQHandler,					// CRYP crypto

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS)

		__Reserved_0x180_Handler,			// Reserved 0x180

#elif defined(STM32F410_VECTORS)

		RNG_IRQHandler,						// RNG

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		HASH_RNG_IRQHandler,				// Hash and RNG

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		FPU_IRQHandler,						// FPU

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

#	if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS)

		__Reserved_0x188_Handler,			// Reserved 0x188
		__Reserved_0x18C_Handler,			// Reserved 0x18C

#	else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

		UART7_IRQHandler,					// UART7
		UART8_IRQHandler,					// UART8

#	endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS)

#	if defined(STM32F410_VECTORS)

		__Reserved_0x190_Handler,			// Reserved 0x190

#	else	// !defined(STM32F410_VECTORS)

		SPI4_IRQHandler,					// SPI4

#	endif	// !defined(STM32F410_VECTORS)

#endif	// defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

		SPI5_IRQHandler,					// SPI5

#	if defined(STM32F410_VECTORS)

		__Reserved_0x198_Handler,			// Reserved 0x198
		__Reserved_0x19C_Handler,			// Reserved 0x19C
		__Reserved_0x1A0_Handler,			// Reserved 0x1A0
		__Reserved_0x1A4_Handler,			// Reserved 0x1A4
		__Reserved_0x1A8_Handler,			// Reserved 0x1A8

#	else	// !defined(STM32F410_VECTORS)

		SPI6_IRQHandler,					// SPI6
		SAI1_IRQHandler,					// SAI1
		LTDC_IRQHandler,					// LTDC
		LTDC_ER_IRQHandler,					// LTDC error
		DMA2D_IRQHandler,					// DMA2D

#	endif	// !defined(STM32F410_VECTORS)

#endif	// defined(STM32F410_VECTORS) || defined(STM32F42_STM32F43_VECTORS)

#if defined(STM32F410_VECTORS)

		__Reserved_0x1AC_Handler,			// Reserved 0x1AC
		__Reserved_0x1B0_Handler,			// Reserved 0x1B0
		__Reserved_0x1B4_Handler,			// Reserved 0x1B4
		__Reserved_0x1B8_Handler,			// Reserved 0x1B8
		FMPI2C1_EV_IRQHandler,				// FMPI2C1 Event
		FMPI2C1_ER_IRQHandler,				// FMPI2C1 Error
		LPTIM1_IRQHandler,					// LPTIM1 through the EXTI line

#endif	// defined(STM32F410_VECTORS)

};
