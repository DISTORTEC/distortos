/**
 * \file
 * \brief STM32F4 chip vector table and default weak handlers
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 * - RM0383 reference manual (STM32F411xC/E), Revision 1, 2014-07-24
 * - RM0390 reference manual (STM32F446xx), Revision 2, 2016-01-12
 * - RM0386 reference manual (STM32F469xx and STM32F479xx), Revision 2, 2015-11-19
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#include <cstddef>

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
#elif defined(CONFIG_CHIP_STM32F411)
#	define STM32F411_VECTORS
#elif defined(CONFIG_CHIP_STM32F412)
#	define STM32F412_VECTORS
#elif defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43)
#	define STM32F42_STM32F43_VECTORS
#elif defined(CONFIG_CHIP_STM32F446)
#	define STM32F446_VECTORS
#elif defined(CONFIG_CHIP_STM32F46) || defined(CONFIG_CHIP_STM32F47)
#	define STM32F46_STM32F47_VECTORS
#else
#	error "Wrong configuration or unsupported STM32F4 chip!"
#endif

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| provide default weak handlers
+---------------------------------------------------------------------------------------------------------------------*/

// 0x040, 0: Window WatchDog
__attribute__ ((weak)) void WWDG_IRQHandler()
{
	while (1);
}

// 0x044, 1: PVD through EXTI Line detection
__attribute__ ((weak)) void PVD_IRQHandler()
{
	while (1);
}

// 0x048, 2: Tamper and TimeStamp through the EXTI line
__attribute__ ((weak)) void TAMP_STAMP_IRQHandler()
{
	while (1);
}

// 0x04c, 3: RTC Wakeup through the EXTI line
__attribute__ ((weak)) void RTC_WKUP_IRQHandler()
{
	while (1);
}

// 0x050, 4: FLASH
__attribute__ ((weak)) void FLASH_IRQHandler()
{
	while (1);
}

// 0x054, 5: RCC
__attribute__ ((weak)) void RCC_IRQHandler()
{
	while (1);
}

// 0x058, 6: EXTI Line 0
__attribute__ ((weak)) void EXTI0_IRQHandler()
{
	while (1);
}

// 0x05c, 7: EXTI Line 1
__attribute__ ((weak)) void EXTI1_IRQHandler()
{
	while (1);
}

// 0x060, 8: EXTI Line 2
__attribute__ ((weak)) void EXTI2_IRQHandler()
{
	while (1);
}

// 0x064, 9: EXTI Line 3
__attribute__ ((weak)) void EXTI3_IRQHandler()
{
	while (1);
}

// 0x068, 10: EXTI Line 4
__attribute__ ((weak)) void EXTI4_IRQHandler()
{
	while (1);
}

// 0x06c, 11: DMA1 Stream 0
__attribute__ ((weak)) void DMA1_Stream0_IRQHandler()
{
	while (1);
}

// 0x070, 12: DMA1 Stream 1
__attribute__ ((weak)) void DMA1_Stream1_IRQHandler()
{
	while (1);
}

// 0x074, 13: DMA1 Stream 2
__attribute__ ((weak)) void DMA1_Stream2_IRQHandler()
{
	while (1);
}

// 0x078, 14: DMA1 Stream 3
__attribute__ ((weak)) void DMA1_Stream3_IRQHandler()
{
	while (1);
}

// 0x07c, 15: DMA1 Stream 4
__attribute__ ((weak)) void DMA1_Stream4_IRQHandler()
{
	while (1);
}

// 0x080, 16: DMA1 Stream 5
__attribute__ ((weak)) void DMA1_Stream5_IRQHandler()
{
	while (1);
}

// 0x084, 17: DMA1 Stream 6
__attribute__ ((weak)) void DMA1_Stream6_IRQHandler()
{
	while (1);
}

// 0x088, 18: ADC1, ADC2 and ADC3
__attribute__ ((weak)) void ADC_IRQHandler()
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

// 0x08c, 19: Reserved
__attribute__ ((weak)) void __Reserved_0x8c_Handler()
{
	while (1);
}

// 0x090, 20: Reserved
__attribute__ ((weak)) void __Reserved_0x90_Handler()
{
	while (1);
}

// 0x094, 21: Reserved
__attribute__ ((weak)) void __Reserved_0x94_Handler()
{
	while (1);
}

// 0x098, 22: Reserved
__attribute__ ((weak)) void __Reserved_0x98_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x08c, 19: CAN1 TX
__attribute__ ((weak)) void CAN1_TX_IRQHandler()
{
	while (1);
}

// 0x090, 20: CAN1 RX0
__attribute__ ((weak)) void CAN1_RX0_IRQHandler()
{
	while (1);
}

// 0x094, 21: CAN1 RX1
__attribute__ ((weak)) void CAN1_RX1_IRQHandler()
{
	while (1);
}

// 0x098, 22: CAN1 SCE
__attribute__ ((weak)) void CAN1_SCE_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x09c, 23: EXTI Lines [9:5]
__attribute__ ((weak)) void EXTI9_5_IRQHandler()
{
	while (1);
}

// 0x0a0, 24: TIM1 Break and TIM9
__attribute__ ((weak)) void TIM1_BRK_TIM9_IRQHandler()
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// 0x0a4, 25: TIM1 Update
__attribute__ ((weak)) void TIM1_UP_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// 0x0a4, 25: TIM1 Update and TIM10
__attribute__ ((weak)) void TIM1_UP_TIM10_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

// 0x0a8, 26: TIM1 Trigger and Commutation and TIM11
__attribute__ ((weak)) void TIM1_TRG_COM_TIM11_IRQHandler()
{
	while (1);
}

// 0x0ac, 27: TIM1 Capture Compare
__attribute__ ((weak)) void TIM1_CC_IRQHandler()
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// 0x0b0, 28: Reserved
__attribute__ ((weak)) void __Reserved_0xb0_Handler()
{
	while (1);
}

// 0x0b4, 29: Reserved
__attribute__ ((weak)) void __Reserved_0xb4_Handler()
{
	while (1);
}

// 0x0b8, 30: Reserved
__attribute__ ((weak)) void __Reserved_0xb8_Handler()
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// 0x0b0, 28: TIM2
__attribute__ ((weak)) void TIM2_IRQHandler()
{
	while (1);
}

// 0x0b4, 29: TIM3
__attribute__ ((weak)) void TIM3_IRQHandler()
{
	while (1);
}

// 0x0b8, 30: TIM4
__attribute__ ((weak)) void TIM4_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

// 0x0bc, 31: I2C1 Event
__attribute__ ((weak)) void I2C1_EV_IRQHandler()
{
	while (1);
}

// 0x0c0, 32: I2C1 Error
__attribute__ ((weak)) void I2C1_ER_IRQHandler()
{
	while (1);
}

// 0x0c4, 33: I2C2 Event
__attribute__ ((weak)) void I2C2_EV_IRQHandler()
{
	while (1);
}

// 0x0c8, 34: I2C2 Error
__attribute__ ((weak)) void I2C2_ER_IRQHandler()
{
	while (1);
}

// 0x0cc, 35: SPI1
__attribute__ ((weak)) void SPI1_IRQHandler()
{
	while (1);
}

// 0x0d0, 36: SPI2
__attribute__ ((weak)) void SPI2_IRQHandler()
{
	while (1);
}

// 0x0d4, 37: USART1
__attribute__ ((weak)) void USART1_IRQHandler()
{
	while (1);
}

// 0x0d8, 38: USART2
__attribute__ ((weak)) void USART2_IRQHandler()
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

// 0x0dc, 39: Reserved
__attribute__ ((weak)) void __Reserved_0xdc_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x0dc, 39: USART3
__attribute__ ((weak)) void USART3_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x0e0, 40: EXTI Lines [15:10]
__attribute__ ((weak)) void EXTI15_10_IRQHandler()
{
	while (1);
}

// 0x0e4, 41: RTC Alarm (A and B) through the EXTI line
__attribute__ ((weak)) void RTC_Alarm_IRQHandler()
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// 0x0e8, 42: Reserved
__attribute__ ((weak)) void __Reserved_0xe8_Handler()
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// 0x0e8, 42: USB OTG FS Wakeup through the EXTI line
__attribute__ ((weak)) void OTG_FS_WKUP_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

// 0x0ec, 43: Reserved
__attribute__ ((weak)) void __Reserved_0xec_Handler()
{
	while (1);
}

// 0x0f0, 44: Reserved
__attribute__ ((weak)) void __Reserved_0xf0_Handler()
{
	while (1);
}

// 0x0f4, 45: Reserved
__attribute__ ((weak)) void __Reserved_0xf4_Handler()
{
	while (1);
}

// 0x0f8, 46: Reserved
__attribute__ ((weak)) void __Reserved_0xf8_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x0ec, 43: TIM8 Break and TIM12
__attribute__ ((weak)) void TIM8_BRK_TIM12_IRQHandler()
{
	while (1);
}

// 0x0f0, 44: TIM8 Update and TIM13
__attribute__ ((weak)) void TIM8_UP_TIM13_IRQHandler()
{
	while (1);
}

// 0x0f4, 45: TIM8 Trigger and Commutation and TIM14
__attribute__ ((weak)) void TIM8_TRG_COM_TIM14_IRQHandler()
{
	while (1);
}

// 0x0f8, 46: TIM8 Capture Compare
__attribute__ ((weak)) void TIM8_CC_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x0fc, 47: DMA1 Stream7
__attribute__ ((weak)) void DMA1_Stream7_IRQHandler()
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

// 0x100, 48: Reserved
__attribute__ ((weak)) void __Reserved_0x100_Handler()
{
	while (1);
}

#elif defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

// 0x100, 48: FMC
__attribute__ ((weak)) void FMC_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F446_VECTORS) && !defined(STM32F46_STM32F47_VECTORS)

// 0x100, 48: FSMC
__attribute__ ((weak)) void FSMC_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F446_VECTORS) && !defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS)

// 0x104, 49: Reserved
__attribute__ ((weak)) void __Reserved_0x104_Handler()
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// 0x104, 49: SDIO
__attribute__ ((weak)) void SDIO_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

// 0x108, 50: TIM5
__attribute__ ((weak)) void TIM5_IRQHandler()
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// 0x10c, 51: Reserved
__attribute__ ((weak)) void __Reserved_0x10c_Handler()
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// 0x10c, 51: SPI3
__attribute__ ((weak)) void SPI3_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

// 0x110, 52: Reserved
__attribute__ ((weak)) void __Reserved_0x110_Handler()
{
	while (1);
}

// 0x114, 53: Reserved
__attribute__ ((weak)) void __Reserved_0x114_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

// 0x110, 52: UART4
__attribute__ ((weak)) void UART4_IRQHandler()
{
	while (1);
}

// 0x114, 53: UART5
__attribute__ ((weak)) void UART5_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS)

// 0x118, 54: Reserved
__attribute__ ((weak)) void __Reserved_0x118_Handler()
{
	while (1);
}

#elif defined(STM32F412_VECTORS)

// 0x118, 54: TIM6
__attribute__ ((weak)) void TIM6_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F411_VECTORS) && !defined(STM32F412_VECTORS)

// 0x118, 54: TIM6 and DAC1&2 underrun error
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F411_VECTORS) && !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

// 0x11c, 55: Reserved
__attribute__ ((weak)) void __Reserved_0x11c_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x11c, 55: TIM7
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x120, 56: DMA2 Stream 0
__attribute__ ((weak)) void DMA2_Stream0_IRQHandler()
{
	while (1);
}

// 0x124, 57: DMA2 Stream 1
__attribute__ ((weak)) void DMA2_Stream1_IRQHandler()
{
	while (1);
}

// 0x128, 58: DMA2 Stream 2
__attribute__ ((weak)) void DMA2_Stream2_IRQHandler()
{
	while (1);
}

// 0x12c, 59: DMA2 Stream 3
__attribute__ ((weak)) void DMA2_Stream3_IRQHandler()
{
	while (1);
}

// 0x130, 60: DMA2 Stream 4
__attribute__ ((weak)) void DMA2_Stream4_IRQHandler()
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F446_VECTORS)

// 0x134, 61: Reserved
__attribute__ ((weak)) void __Reserved_0x134_Handler()
{
	while (1);
}

#elif defined(STM32F412_VECTORS)

// 0x134, 61: SD filter 0
__attribute__ ((weak)) void DFSDM1_FLT0_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

// 0x134, 61: Ethernet
__attribute__ ((weak)) void ETH_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F446_VECTORS)

// 0x138, 62: Reserved
__attribute__ ((weak)) void __Reserved_0x138_Handler()
{
	while (1);
}

#elif defined(STM32F410_VECTORS)

// 0x138, 62: EXTI Line 19
__attribute__ ((weak)) void EXTI19_IRQHandler()
{
	while (1);
}

#elif defined(STM32F412_VECTORS)

// 0x138, 62: SD filter 1
__attribute__ ((weak)) void DFSDM1_FLT1_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

// 0x138, 62: Ethernet Wakeup through the EXTI line
__attribute__ ((weak)) void ETH_WKUP_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

// 0x13c, 63: Reserved
__attribute__ ((weak)) void __Reserved_0x13c_Handler()
{
	while (1);
}

// 0x140, 64: Reserved
__attribute__ ((weak)) void __Reserved_0x140_Handler()
{
	while (1);
}

// 0x144, 65: Reserved
__attribute__ ((weak)) void __Reserved_0x144_Handler()
{
	while (1);
}

// 0x148, 66: Reserved
__attribute__ ((weak)) void __Reserved_0x148_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

// 0x13c, 63: CAN2 TX
__attribute__ ((weak)) void CAN2_TX_IRQHandler()
{
	while (1);
}

// 0x140, 64: CAN2 RX0
__attribute__ ((weak)) void CAN2_RX0_IRQHandler()
{
	while (1);
}

// 0x144, 65: CAN2 RX1
__attribute__ ((weak)) void CAN2_RX1_IRQHandler()
{
	while (1);
}

// 0x148, 66: CAN2 SCE
__attribute__ ((weak)) void CAN2_SCE_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

#if defined(STM32F410_VECTORS)

// 0x14c, 67: Reserved
__attribute__ ((weak)) void __Reserved_0x14c_Handler()
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// 0x14c, 67: USB OTG FS
__attribute__ ((weak)) void OTG_FS_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

// 0x150, 68: DMA2 Stream 5
__attribute__ ((weak)) void DMA2_Stream5_IRQHandler()
{
	while (1);
}

// 0x154, 69: DMA2 Stream 6
__attribute__ ((weak)) void DMA2_Stream6_IRQHandler()
{
	while (1);
}

// 0x158, 70: DMA2 Stream 7
__attribute__ ((weak)) void DMA2_Stream7_IRQHandler()
{
	while (1);
}

// 0x15c, 71: USART6
__attribute__ ((weak)) void USART6_IRQHandler()
{
	while (1);
}

#if defined(STM32F410_VECTORS)

// 0x160, 72: Reserved
__attribute__ ((weak)) void __Reserved_0x160_Handler()
{
	while (1);
}

// 0x164, 73: Reserved
__attribute__ ((weak)) void __Reserved_0x164_Handler()
{
	while (1);
}

#else	// !defined(STM32F410_VECTORS)

// 0x160, 72: I2C3 event
__attribute__ ((weak)) void I2C3_EV_IRQHandler()
{
	while (1);
}

// 0x164, 73: I2C3 error
__attribute__ ((weak)) void I2C3_ER_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

// 0x168, 74: Reserved
__attribute__ ((weak)) void __Reserved_0x168_Handler()
{
	while (1);
}

// 0x16c, 75: Reserved
__attribute__ ((weak)) void __Reserved_0x16c_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

// 0x168, 74: USB OTG HS End Point 1 Out
__attribute__ ((weak)) void OTG_HS_EP1_OUT_IRQHandler()
{
	while (1);
}

// 0x16c, 75: USB OTG HS End Point 1 In
__attribute__ ((weak)) void OTG_HS_EP1_IN_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

// 0x170, 76: Reserved
__attribute__ ((weak)) void __Reserved_0x170_Handler()
{
	while (1);
}

#elif defined(STM32F410_VECTORS)

// 0x170, 76: EXTI Line 20
__attribute__ ((weak)) void EXTI20_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

// 0x170, 76: USB OTG HS Wakeup through the EXTI line
__attribute__ ((weak)) void OTG_HS_WKUP_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

// 0x174, 77: Reserved
__attribute__ ((weak)) void __Reserved_0x174_Handler()
{
	while (1);
}

// 0x178, 78: Reserved
__attribute__ ((weak)) void __Reserved_0x178_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

// 0x174, 77: USB OTG HS
__attribute__ ((weak)) void OTG_HS_IRQHandler()
{
	while (1);
}

// 0x178, 78: DCMI
__attribute__ ((weak)) void DCMI_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || \
		defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

// 0x17c, 79: Reserved
__attribute__ ((weak)) void __Reserved_0x17c_Handler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

// 0x17c, 79: CRYP crypto
__attribute__ ((weak)) void CRYP_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F446_VECTORS)

// 0x180, 80: Reserved
__attribute__ ((weak)) void __Reserved_0x180_Handler()
{
	while (1);
}

#elif defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

// 0x180, 80: RNG
__attribute__ ((weak)) void RNG_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

// 0x180, 80: Hash and RNG
__attribute__ ((weak)) void HASH_RNG_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

// 0x184, 81: FPU
__attribute__ ((weak)) void FPU_IRQHandler()
{
	while (1);
}

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || \
defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

// 0x188, 82: Reserved
__attribute__ ((weak)) void __Reserved_0x188_Handler()
{
	while (1);
}

// 0x18c, 83: Reserved
__attribute__ ((weak)) void __Reserved_0x18c_Handler()
{
	while (1);
}

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

// 0x188, 82: UART7
__attribute__ ((weak)) void UART7_IRQHandler()
{
	while (1);
}

// 0x18c, 83: UART8
__attribute__ ((weak)) void UART8_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS)

// 0x190, 84: Reserved
__attribute__ ((weak)) void __Reserved_0x190_Handler()
{
	while (1);
}

#elif defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS) || \
		defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

// 0x190, 84: SPI4
__attribute__ ((weak)) void SPI4_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS) ||
		// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS) || \
		defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

// 0x194, 85: SPI5
__attribute__ ((weak)) void SPI5_IRQHandler()
{
	while (1);
}

#elif defined(STM32F446_VECTORS)

// 0x194, 85: Reserved
__attribute__ ((weak)) void __Reserved_0x194_Handler()
{
	while (1);
}

#endif	// defined(STM32F446_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

// 0x198, 86: Reserved
__attribute__ ((weak)) void __Reserved_0x198_Handler()
{
	while (1);
}

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

// 0x198, 86: SPI6
__attribute__ ((weak)) void SPI6_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

// 0x19c, 87: Reserved
__attribute__ ((weak)) void __Reserved_0x19c_Handler()
{
	while (1);
}

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

// 0x19c, 87: SAI1
__attribute__ ((weak)) void SAI1_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

// 0x1a0, 88: Reserved
__attribute__ ((weak)) void __Reserved_0x1a0_Handler()
{
	while (1);
}

// 0x1a4, 89: Reserved
__attribute__ ((weak)) void __Reserved_0x1a4_Handler()
{
	while (1);
}

// 0x1a8, 90: Reserved
__attribute__ ((weak)) void __Reserved_0x1a8_Handler()
{
	while (1);
}

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

// 0x1a0, 88: LTDC
__attribute__ ((weak)) void LTDC_IRQHandler()
{
	while (1);
}

// 0x1a4, 89: LTDC error
__attribute__ ((weak)) void LTDC_ER_IRQHandler()
{
	while (1);
}

// 0x1a8, 90: DMA2D
__attribute__ ((weak)) void DMA2D_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

// 0x1ac, 91: Reserved
__attribute__ ((weak)) void __Reserved_0x1ac_Handler()
{
	while (1);
}

#elif defined(STM32F446_VECTORS)

// 0x1ac, 91: SAI2
__attribute__ ((weak)) void SAI2_IRQHandler()
{
	while (1);
}

#elif defined(STM32F46_STM32F47_VECTORS)

// 0x1ac, 91: QuadSPI
__attribute__ ((weak)) void QUADSPI_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS)

// 0x1b0, 92: Reserved
__attribute__ ((weak)) void __Reserved_0x1b0_Handler()
{
	while (1);
}

#elif defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

// 0x1b0, 92: QuadSPI
__attribute__ ((weak)) void QUADSPI_IRQHandler()
{
	while (1);
}

#elif defined(STM32F46_STM32F47_VECTORS)

// 0x1b0, 92: DSI
__attribute__ ((weak)) void DSI_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

// 0x1b4, 93: Reserved
__attribute__ ((weak)) void __Reserved_0x1b4_Handler()
{
	while (1);
}

// 0x1b8, 94: Reserved
__attribute__ ((weak)) void __Reserved_0x1b8_Handler()
{
	while (1);
}

#elif defined(STM32F446_VECTORS)

// 0x1b4, 93: CEC
__attribute__ ((weak)) void CEC_IRQHandler()
{
	while (1);
}

// 0x1b8, 94: SPDIF RX
__attribute__ ((weak)) void SPDIF_RX_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

// 0x1bc, 95: FMPI2C1 Event
__attribute__ ((weak)) void FMPI2C1_EV_IRQHandler()
{
	while (1);
}

// 0x1c0, 96: FMPI2C1 Error
__attribute__ ((weak)) void FMPI2C1_ER_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

#if defined(STM32F410_VECTORS)

// 0x1c4, 97: LPTIM1 through the EXTI line
__attribute__ ((weak)) void LPTIM1_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F410_VECTORS)

}	// extern "C"

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// single interrupt vector - pointer to function with no arguments and no return value
using InterruptVector = void(*)();

/*---------------------------------------------------------------------------------------------------------------------+
| global variables
+---------------------------------------------------------------------------------------------------------------------*/

/// chip vector table
extern "C" const InterruptVector chipVectors[] __attribute__ ((section(".chipVectors")))
{
		WWDG_IRQHandler,					// 0x040, 0: Window WatchDog
		PVD_IRQHandler,						// 0x044, 1: PVD through EXTI Line detection
		TAMP_STAMP_IRQHandler,				// 0x048, 2: Tamper and TimeStamp through the EXTI line
		RTC_WKUP_IRQHandler,				// 0x04c, 3: RTC Wakeup through the EXTI line
		FLASH_IRQHandler,					// 0x050, 4: FLASH
		RCC_IRQHandler,						// 0x054, 5: RCC
		EXTI0_IRQHandler,					// 0x058, 6: EXTI Line 0
		EXTI1_IRQHandler,					// 0x05c, 7: EXTI Line 1
		EXTI2_IRQHandler,					// 0x060, 8: EXTI Line 2
		EXTI3_IRQHandler,					// 0x064, 9: EXTI Line 3
		EXTI4_IRQHandler,					// 0x068, 10: EXTI Line 4
		DMA1_Stream0_IRQHandler,			// 0x06c, 11: DMA1 Stream 0
		DMA1_Stream1_IRQHandler,			// 0x070, 12: DMA1 Stream 1
		DMA1_Stream2_IRQHandler,			// 0x074, 13: DMA1 Stream 2
		DMA1_Stream3_IRQHandler,			// 0x078, 14: DMA1 Stream 3
		DMA1_Stream4_IRQHandler,			// 0x07c, 15: DMA1 Stream 4
		DMA1_Stream5_IRQHandler,			// 0x080, 16: DMA1 Stream 5
		DMA1_Stream6_IRQHandler,			// 0x084, 17: DMA1 Stream 6
		ADC_IRQHandler,						// 0x088, 18: ADC1, ADC2 and ADC3

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

		__Reserved_0x8c_Handler,			// 0x08c, 19: Reserved
		__Reserved_0x90_Handler,			// 0x090, 20: Reserved
		__Reserved_0x94_Handler,			// 0x094, 21: Reserved
		__Reserved_0x98_Handler,			// 0x098, 22: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		CAN1_TX_IRQHandler,					// 0x08c, 19: CAN1 TX
		CAN1_RX0_IRQHandler,				// 0x090, 20: CAN1 RX0
		CAN1_RX1_IRQHandler,				// 0x094, 21: CAN1 RX1
		CAN1_SCE_IRQHandler,				// 0x098, 22: CAN1 SCE

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		EXTI9_5_IRQHandler,					// 0x09c, 23: EXTI Lines [9:5]
		TIM1_BRK_TIM9_IRQHandler,			// 0x0a0, 24: TIM1 Break and TIM9

#if defined(STM32F410_VECTORS)

		TIM1_UP_IRQHandler,					// 0x0a4, 25: TIM1 Update

#else	// !defined(STM32F410_VECTORS)

		TIM1_UP_TIM10_IRQHandler,			// 0x0a4, 25: TIM1 Update and TIM10

#endif	// !defined(STM32F410_VECTORS)

		TIM1_TRG_COM_TIM11_IRQHandler,		// 0x0a8, 26: TIM1 Trigger and Commutation and TIM11
		TIM1_CC_IRQHandler,					// 0x0ac, 27: TIM1 Capture Compare

#if defined(STM32F410_VECTORS)

		__Reserved_0xb0_Handler,			// 0x0b0, 28: Reserved
		__Reserved_0xb4_Handler,			// 0x0b4, 29: Reserved
		__Reserved_0xb8_Handler,			// 0x0b8, 30: Reserved

#else	// !defined(STM32F410_VECTORS)

		TIM2_IRQHandler,					// 0x0b0, 28: TIM2
		TIM3_IRQHandler,					// 0x0b4, 29: TIM3
		TIM4_IRQHandler,					// 0x0b8, 30: TIM4

#endif	// !defined(STM32F410_VECTORS)

		I2C1_EV_IRQHandler,					// 0x0bc, 31: I2C1 Event
		I2C1_ER_IRQHandler,					// 0x0c0, 32: I2C1 Error
		I2C2_EV_IRQHandler,					// 0x0c4, 33: I2C2 Event
		I2C2_ER_IRQHandler,					// 0x0c8, 34: I2C2 Error
		SPI1_IRQHandler,					// 0x0cc, 35: SPI1
		SPI2_IRQHandler,					// 0x0d0, 36: SPI2
		USART1_IRQHandler,					// 0x0d4, 37: USART1
		USART2_IRQHandler,					// 0x0d8, 38: USART2

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

		__Reserved_0xdc_Handler,			// 0x0dc, 39: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		USART3_IRQHandler,					// 0x0dc, 39: USART3

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		EXTI15_10_IRQHandler,				// 0x0e0, 40: EXTI Lines [15:10]
		RTC_Alarm_IRQHandler,				// 0x0e4, 41: RTC Alarm (A and B) through the EXTI line

#if defined(STM32F410_VECTORS)

		__Reserved_0xe8_Handler,			// 0x0e8, 42: Reserved

#else	// !defined(STM32F410_VECTORS)

		OTG_FS_WKUP_IRQHandler,				// 0x0e8, 42: USB OTG FS Wakeup through the EXTI line

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

		__Reserved_0xec_Handler,			// 0x0ec, 43: Reserved
		__Reserved_0xf0_Handler,			// 0x0f0, 44: Reserved
		__Reserved_0xf4_Handler,			// 0x0f4, 45: Reserved
		__Reserved_0xf8_Handler,			// 0x0f8, 46: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		TIM8_BRK_TIM12_IRQHandler,			// 0x0ec, 43: TIM8 Break and TIM12
		TIM8_UP_TIM13_IRQHandler,			// 0x0f0, 44: TIM8 Update and TIM13
		TIM8_TRG_COM_TIM14_IRQHandler,		// 0x0f4, 45: TIM8 Trigger and Commutation and TIM14
		TIM8_CC_IRQHandler,					// 0x0f8, 46: TIM8 Capture Compare

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		DMA1_Stream7_IRQHandler,			// 0x0fc, 47: DMA1 Stream7

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

		__Reserved_0x100_Handler,			// 0x100, 48: Reserved

#elif defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

		FMC_IRQHandler,						// 0x100, 48: FMC

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F446_VECTORS) && !defined(STM32F46_STM32F47_VECTORS)

		FSMC_IRQHandler,					// 0x100, 48: FSMC

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F446_VECTORS) && !defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS)

		__Reserved_0x104_Handler,			// 0x104, 49: Reserved

#else	// !defined(STM32F410_VECTORS)

		SDIO_IRQHandler,					// 0x104, 49: SDIO

#endif	// !defined(STM32F410_VECTORS)

		TIM5_IRQHandler,					// 0x108, 50: TIM5

#if defined(STM32F410_VECTORS)

		__Reserved_0x10c_Handler,			// 0x10c, 51: Reserved

#else	// !defined(STM32F410_VECTORS)

		SPI3_IRQHandler,					// 0x10c, 51: SPI3

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

		__Reserved_0x110_Handler,			// 0x110, 52: Reserved
		__Reserved_0x114_Handler,			// 0x114, 53: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

		UART4_IRQHandler,					// 0x110, 52: UART4
		UART5_IRQHandler,					// 0x114, 53: UART5

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS)

		__Reserved_0x118_Handler,			// 0x118, 54: Reserved

#elif defined(STM32F412_VECTORS)

		TIM6_IRQHandler,					// 0x118, 54: TIM6

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F411_VECTORS) && !defined(STM32F412_VECTORS)

		TIM6_DAC_IRQHandler,				// 0x118, 54: TIM6 and DAC1&2 underrun error

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F411_VECTORS) && !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

		__Reserved_0x11c_Handler,			// 0x11c, 55: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		TIM7_IRQHandler,					// 0x11c, 55: TIM7

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		DMA2_Stream0_IRQHandler,			// 0x120, 56: DMA2 Stream 0
		DMA2_Stream1_IRQHandler,			// 0x124, 57: DMA2 Stream 1
		DMA2_Stream2_IRQHandler,			// 0x128, 58: DMA2 Stream 2
		DMA2_Stream3_IRQHandler,			// 0x12c, 59: DMA2 Stream 3
		DMA2_Stream4_IRQHandler,			// 0x130, 60: DMA2 Stream 4

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F446_VECTORS)

		__Reserved_0x134_Handler,			// 0x134, 61: Reserved

#elif defined(STM32F412_VECTORS)

		DFSDM1_FLT0_IRQHandler,				// 0x134, 61: SD filter 0

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

		ETH_IRQHandler,						// 0x134, 61: Ethernet

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F446_VECTORS)

		__Reserved_0x138_Handler,			// 0x138, 62: Reserved

#elif defined(STM32F410_VECTORS)

		EXTI19_IRQHandler,					// 0x138, 62: EXTI Line 19

#elif defined(STM32F412_VECTORS)

		DFSDM1_FLT1_IRQHandler,				// 0x138, 62: SD filter 1

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

		ETH_WKUP_IRQHandler,				// 0x138, 62: Ethernet Wakeup through the EXTI line

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) && !defined(STM32F446_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS)

		__Reserved_0x13c_Handler,			// 0x13c, 63: Reserved
		__Reserved_0x140_Handler,			// 0x140, 64: Reserved
		__Reserved_0x144_Handler,			// 0x144, 65: Reserved
		__Reserved_0x148_Handler,			// 0x148, 66: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

		CAN2_TX_IRQHandler,					// 0x13c, 63: CAN2 TX
		CAN2_RX0_IRQHandler,				// 0x140, 64: CAN2 RX0
		CAN2_RX1_IRQHandler,				// 0x144, 65: CAN2 RX1
		CAN2_SCE_IRQHandler,				// 0x148, 66: CAN2 SCE

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS)

#if defined(STM32F410_VECTORS)

		__Reserved_0x14c_Handler,			// 0x14c, 67: Reserved

#else	// !defined(STM32F410_VECTORS)

		OTG_FS_IRQHandler,					// 0x14c, 67: USB OTG FS

#endif	// !defined(STM32F410_VECTORS)

		DMA2_Stream5_IRQHandler,			// 0x150, 68: DMA2 Stream 5
		DMA2_Stream6_IRQHandler,			// 0x154, 69: DMA2 Stream 6
		DMA2_Stream7_IRQHandler,			// 0x158, 70: DMA2 Stream 7
		USART6_IRQHandler,					// 0x15c, 71: USART6

#if defined(STM32F410_VECTORS)

		__Reserved_0x160_Handler,			// 0x160, 72: Reserved
		__Reserved_0x164_Handler,			// 0x164, 73: Reserved

#else	// !defined(STM32F410_VECTORS)

		I2C3_EV_IRQHandler,					// 0x160, 72: I2C3 event
		I2C3_ER_IRQHandler,					// 0x164, 73: I2C3 error

#endif	// !defined(STM32F410_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

		__Reserved_0x168_Handler,			// 0x168, 74: Reserved
		__Reserved_0x16c_Handler,			// 0x16c, 75: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

		OTG_HS_EP1_OUT_IRQHandler,			// 0x168, 74: USB OTG HS End Point 1 Out
		OTG_HS_EP1_IN_IRQHandler,			// 0x16c, 75: USB OTG HS End Point 1 In

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

		__Reserved_0x170_Handler,			// 0x170, 76: Reserved

#elif defined(STM32F410_VECTORS)

		EXTI20_IRQHandler,					// 0x170, 76: EXTI Line 20

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

		OTG_HS_WKUP_IRQHandler,				// 0x170, 76: USB OTG HS Wakeup through the EXTI line

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS)

		__Reserved_0x174_Handler,			// 0x174, 77: Reserved
		__Reserved_0x178_Handler,			// 0x178, 78: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

		OTG_HS_IRQHandler,					// 0x174, 77: USB OTG HS
		DCMI_IRQHandler,					// 0x178, 78: DCMI

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || \
		defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

		__Reserved_0x17c_Handler,			// 0x17c, 79: Reserved

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

		CRYP_IRQHandler,					// 0x17c, 79: CRYP crypto

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

#if defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F446_VECTORS)

		__Reserved_0x180_Handler,			// 0x180, 80: Reserved

#elif defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

		RNG_IRQHandler,						// 0x180, 80: RNG

#else	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

		HASH_RNG_IRQHandler,				// 0x180, 80: Hash and RNG

#endif	// !defined(STM32F401_VECTORS) && !defined(STM32F410_VECTORS) && !defined(STM32F411_VECTORS) &&
		// !defined(STM32F412_VECTORS) &&!defined(STM32F446_VECTORS)

		FPU_IRQHandler,						// 0x184, 81: FPU

#if defined(STM32F401_VECTORS) || defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || \
		defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

		__Reserved_0x188_Handler,			// 0x188, 82: Reserved
		__Reserved_0x18c_Handler,			// 0x18c, 83: Reserved

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

		UART7_IRQHandler,					// 0x188, 82: UART7
		UART8_IRQHandler,					// 0x18c, 83: UART8

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS)

		__Reserved_0x190_Handler,			// 0x190, 84: Reserved

#elif defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS) || \
		defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

		SPI4_IRQHandler,					// 0x190, 84: SPI4

#endif	// defined(STM32F401_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS) ||
		// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F411_VECTORS) || defined(STM32F412_VECTORS) || \
		defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

		SPI5_IRQHandler,					// 0x194, 85: SPI5

#elif defined(STM32F446_VECTORS)

		__Reserved_0x194_Handler,			// 0x194, 85: Reserved

#endif	// defined(STM32F446_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

		__Reserved_0x198_Handler,			// 0x198, 86: Reserved

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

		SPI6_IRQHandler,					// 0x198, 86: SPI6

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

		__Reserved_0x19c_Handler,			// 0x19c, 87: Reserved

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

		SAI1_IRQHandler,					// 0x19c, 87: SAI1

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F446_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

		__Reserved_0x1a0_Handler,			// 0x1a0, 88: Reserved
		__Reserved_0x1a4_Handler,			// 0x1a4, 89: Reserved
		__Reserved_0x1a8_Handler,			// 0x1a8, 90: Reserved

#elif defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

		LTDC_IRQHandler,					// 0x1a0, 88: LTDC
		LTDC_ER_IRQHandler,					// 0x1a4, 89: LTDC error
		DMA2D_IRQHandler,					// 0x1a8, 90: DMA2D

#endif	// defined(STM32F42_STM32F43_VECTORS) || defined(STM32F46_STM32F47_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

		__Reserved_0x1ac_Handler,			// 0x1ac, 91: Reserved

#elif defined(STM32F446_VECTORS)

		SAI2_IRQHandler,					// 0x1ac, 91: SAI2

#elif defined(STM32F46_STM32F47_VECTORS)

		QUADSPI_IRQHandler,					// 0x1ac, 91: QuadSPI

#endif	// defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS)

		__Reserved_0x1b0_Handler,			// 0x1b0, 92: Reserved

#elif defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

		QUADSPI_IRQHandler,					// 0x1b0, 92: QuadSPI

#elif defined(STM32F46_STM32F47_VECTORS)

		DSI_IRQHandler,						// 0x1b0, 92: DSI

#endif	// defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS)

		__Reserved_0x1b4_Handler,			// 0x1b4, 93: Reserved
		__Reserved_0x1b8_Handler,			// 0x1b8, 94: Reserved

#elif defined(STM32F446_VECTORS)

		CEC_IRQHandler,						// 0x1b4, 93: CEC
		SPDIF_RX_IRQHandler,				// 0x1b8, 94: SPDIF RX

#endif	// defined(STM32F410_VECTORS)

#if defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

		FMPI2C1_EV_IRQHandler,				// 0x1bc, 95: FMPI2C1 Event
		FMPI2C1_ER_IRQHandler,				// 0x1c0, 96: FMPI2C1 Error

#endif	// defined(STM32F410_VECTORS) || defined(STM32F412_VECTORS) || defined(STM32F446_VECTORS)

#if defined(STM32F410_VECTORS)

		LPTIM1_IRQHandler,					// 0x1c4, 97: LPTIM1 through the EXTI line

#endif	// defined(STM32F410_VECTORS)

};

namespace
{

/// expected number of chip vectors
constexpr size_t expectedChipVectorsSize
{
#if defined(STM32F401_VECTORS)
		85
#elif defined(STM32F405_STM32F407_STM32F415_STM32F417_VECTORS)
		82
#elif defined(STM32F410_VECTORS)
		98
#elif defined(STM32F411_VECTORS)
		86
#elif defined(STM32F412_VECTORS)
		97
#elif defined(STM32F42_STM32F43_VECTORS)
		91
#elif defined(STM32F446_VECTORS)
		97
#elif defined(STM32F46_STM32F47_VECTORS)
		93
#else
#	error "Wrong configuration or unsupported STM32F4 chip!"
#endif
};

static_assert(sizeof(chipVectors) / sizeof(*chipVectors) == expectedChipVectorsSize, "Invalid size of chipVectors[]!");

}	// namespace
