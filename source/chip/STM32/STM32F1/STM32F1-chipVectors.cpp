/**
 * \file
 * \brief STM32F1 chip vector table and default weak handlers
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

#if defined(CONFIG_CHIP_STM32F100)
	#define STM32F100_VECTORS
#elif defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)
	#define STM32F1_CONNECTIVITY_LINE_VECTORS
#elif defined(CONFIG_CHIP_STM32F101RF) || defined(CONFIG_CHIP_STM32F101VF) || defined(CONFIG_CHIP_STM32F101ZF) || \
		defined(CONFIG_CHIP_STM32F103RF) || defined(CONFIG_CHIP_STM32F103VF) || defined(CONFIG_CHIP_STM32F103ZF) || \
		defined(CONFIG_CHIP_STM32F101RG) || defined(CONFIG_CHIP_STM32F101VG) || defined(CONFIG_CHIP_STM32F101ZG) || \
		defined(CONFIG_CHIP_STM32F103RG) || defined(CONFIG_CHIP_STM32F103VG) || defined(CONFIG_CHIP_STM32F103ZG)
	#define STM32F1_XL_DENSITY_VECTORS
#else
	#define STM32F1_STANDARD_VECTORS
#endif

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| provide default weak handlers
+---------------------------------------------------------------------------------------------------------------------*/

// 0x040, 0: Window watchdog
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

// 0x050, 4: Flash
__attribute__ ((weak)) void FLASH_IRQHandler()
{
	while (1);
}

// 0x054, 5: RCC
__attribute__ ((weak)) void RCC_IRQHandler()
{
	while (1);
}

// 0x058, 6: EXTI Line0
__attribute__ ((weak)) void EXTI0_IRQHandler()
{
	while (1);
}

// 0x05c, 7: EXTI Line1
__attribute__ ((weak)) void EXTI1_IRQHandler()
{
	while (1);
}

// 0x060, 8: EXTI Line2
__attribute__ ((weak)) void EXTI2_IRQHandler()
{
	while (1);
}

// 0x064, 9: EXTI Line3
__attribute__ ((weak)) void EXTI3_IRQHandler()
{
	while (1);
}

// 0x068, 10: EXTI Line4
__attribute__ ((weak)) void EXTI4_IRQHandler()
{
	while (1);
}

// 0x06c, 11: DMA1 Channel1
__attribute__ ((weak)) void DMA1_Channel1_IRQHandler()
{
	while (1);
}

// 0x070, 12: DMA1 Channel2
__attribute__ ((weak)) void DMA1_Channel2_IRQHandler()
{
	while (1);
}

// 0x074, 13: DMA1 Channel3
__attribute__ ((weak)) void DMA1_Channel3_IRQHandler()
{
	while (1);
}

// 0x078, 14: DMA1 Channel4
__attribute__ ((weak)) void DMA1_Channel4_IRQHandler()
{
	while (1);
}

// 0x07c, 15: DMA1 Channel5
__attribute__ ((weak)) void DMA1_Channel5_IRQHandler()
{
	while (1);
}

// 0x080, 16: DMA1 Channel6
__attribute__ ((weak)) void DMA1_Channel6_IRQHandler()
{
	while (1);
}

// 0x084, 17: DMA1 Channel7
__attribute__ ((weak)) void DMA1_Channel7_IRQHandler()
{
	while (1);
}

#if defined(STM32F100_VECTORS)

// 0x088, 18: ADC1
__attribute__ ((weak)) void ADC1_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F100_VECTORS)

// 0x088, 18: ADC1 and ADC2
__attribute__ ((weak)) void ADC1_2_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F100_VECTORS)

#if defined(STM32F100_VECTORS)

// 0x08c, 19: Reserved
__attribute__ ((weak)) void Reserved_0x8c_Handler()
{
	while (1);
}

// 0x090, 20: Reserved
__attribute__ ((weak)) void Reserved_0x90_Handler()
{
	while (1);
}

// 0x094, 21: Reserved
__attribute__ ((weak)) void Reserved_0x94_Handler()
{
	while (1);
}

// 0x098, 22: Reserved
__attribute__ ((weak)) void Reserved_0x98_Handler()
{
	while (1);
}

#elif defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

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

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x08c, 19: USB High Priority or CAN1 TX
__attribute__ ((weak)) void USB_HP_CAN1_TX_IRQHandler()
{
	while (1);
}

// 0x090, 20: USB Low Priority or CAN1 RX0
__attribute__ ((weak)) void USB_LP_CAN1_RX0_IRQHandler()
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

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x09c, 23: EXTI Line[9:5]
__attribute__ ((weak)) void EXTI9_5_IRQHandler()
{
	while (1);
}

#if defined(STM32F100_VECTORS)

// 0x0a0, 24: TIM1 Break and TIM15
__attribute__ ((weak)) void TIM1_BRK_TIM15_IRQHandler()
{
	while (1);
}

// 0x0a4, 25: TIM1 Update and TIM16
__attribute__ ((weak)) void TIM1_UP_TIM16_IRQHandler()
{
	while (1);
}

// 0x0a8, 26: TIM1 Trigger and Commutation and TIM17
__attribute__ ((weak)) void TIM1_TRG_COM_TIM17_IRQHandler()
{
	while (1);
}

#elif defined(STM32F1_XL_DENSITY_VECTORS)

// 0x0a0, 24: TIM1 Break and TIM9
__attribute__ ((weak)) void TIM1_BRK_TIM9_IRQHandler()
{
	while (1);
}

// 0x0a4, 25: TIM1 Update and TIM10
__attribute__ ((weak)) void TIM1_UP_TIM10_IRQHandler()
{
	while (1);
}

// 0x0a8, 26: TIM1 Trigger and Commutation and TIM11
__attribute__ ((weak)) void TIM1_TRG_COM_TIM11_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_XL_DENSITY_VECTORS)

// 0x0a0, 24: TIM1 Break
__attribute__ ((weak)) void TIM1_BRK_IRQHandler()
{
	while (1);
}

// 0x0a4, 25: TIM1 Update
__attribute__ ((weak)) void TIM1_UP_IRQHandler()
{
	while (1);
}

// 0x0a8, 26: TIM1 Trigger and Commutation
__attribute__ ((weak)) void TIM1_TRG_COM_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_XL_DENSITY_VECTORS)

// 0x0ac, 27: TIM1 Capture Compare
__attribute__ ((weak)) void TIM1_CC_IRQHandler()
{
	while (1);
}

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

// 0x0bc, 31: I2C1 event
__attribute__ ((weak)) void I2C1_EV_IRQHandler()
{
	while (1);
}

// 0x0c0, 32: I2C1 error
__attribute__ ((weak)) void I2C1_ER_IRQHandler()
{
	while (1);
}

// 0x0c4, 33: I2C2 event
__attribute__ ((weak)) void I2C2_EV_IRQHandler()
{
	while (1);
}

// 0x0c8, 34: I2C2 error
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

// 0x0dc, 39: USART3
__attribute__ ((weak)) void USART3_IRQHandler()
{
	while (1);
}

// 0x0e0, 40: EXTI Line[15:10]
__attribute__ ((weak)) void EXTI15_10_IRQHandler()
{
	while (1);
}

// 0x0e4, 41: RTC Alarm (A and B) through EXTI line
__attribute__ ((weak)) void RTC_Alarm_IRQHandler()
{
	while (1);
}

#if defined(STM32F100_VECTORS)

// 0x0e8, 42: CEC
__attribute__ ((weak)) void CEC_IRQHandler()
{
	while (1);
}

#elif defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x0e8, 42: USB On-The-Go FS Wakeup through EXTI line
__attribute__ ((weak)) void OTG_FS_WKUP_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x0e8, 42: USB wakeup from suspend through EXTI line
__attribute__ ((weak)) void USB_WKUP_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F100_VECTORS)

// 0x0ec, 43: TIM12
__attribute__ ((weak)) void TIM12_IRQHandler()
{
	while (1);
}

// 0x0f0, 44: TIM13
__attribute__ ((weak)) void TIM13_IRQHandler()
{
	while (1);
}

// 0x0f4, 45: TIM14
__attribute__ ((weak)) void TIM14_IRQHandler()
{
	while (1);
}

#elif defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x0ec, 43: Reserved
__attribute__ ((weak)) void Reserved_0xec_Handler()
{
	while (1);
}

// 0x0f0, 44: Reserved
__attribute__ ((weak)) void Reserved_0xf0_Handler()
{
	while (1);
}

// 0x0f4, 45: Reserved
__attribute__ ((weak)) void Reserved_0xf4_Handler()
{
	while (1);
}

#elif defined(STM32F1_XL_DENSITY_VECTORS)

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

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS) &&
// !defined(STM32F1_XL_DENSITY_VECTORS)

// 0x0ec, 43: TIM8 Break
__attribute__ ((weak)) void TIM8_BRK_IRQHandler()
{
	while (1);
}

// 0x0f0, 44: TIM8 Update
__attribute__ ((weak)) void TIM8_UP_IRQHandler()
{
	while (1);
}

// 0x0f4, 45: TIM8 Trigger and Commutation
__attribute__ ((weak)) void TIM8_TRG_COM_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS) &&
// !defined(STM32F1_XL_DENSITY_VECTORS)

#if defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x0f8, 46: Reserved
__attribute__ ((weak)) void Reserved_0xf8_Handler()
{
	while (1);
}

// 0x0fc, 47: Reserved
__attribute__ ((weak)) void Reserved_0xfc_Handler()
{
	while (1);
}

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x0f8, 46: TIM8 Capture Compare
__attribute__ ((weak)) void TIM8_CC_IRQHandler()
{
	while (1);
}

// 0x0fc, 47: ADC3
__attribute__ ((weak)) void ADC3_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x100, 48: Reserved
__attribute__ ((weak)) void Reserved_0x100_Handler()
{
	while (1);
}

#else	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x100, 48: FSMC
__attribute__ ((weak)) void FSMC_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x104, 49: Reserved
__attribute__ ((weak)) void Reserved_0x104_Handler()
{
	while (1);
}

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x104, 49: SDIO
__attribute__ ((weak)) void SDIO_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x108, 50: TIM5
__attribute__ ((weak)) void TIM5_IRQHandler()
{
	while (1);
}

// 0x10c, 51: SPI3
__attribute__ ((weak)) void SPI3_IRQHandler()
{
	while (1);
}

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

#if defined(STM32F100_VECTORS)

// 0x118, 54: TIM6 and DAC underrun
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F100_VECTORS)

// 0x118, 54: TIM6
__attribute__ ((weak)) void TIM6_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F100_VECTORS)

// 0x11c, 55: TIM7
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

// 0x120, 56: DMA2 Channel1
__attribute__ ((weak)) void DMA2_Channel1_IRQHandler()
{
	while (1);
}

// 0x124, 57: DMA2 Channel2
__attribute__ ((weak)) void DMA2_Channel2_IRQHandler()
{
	while (1);
}

// 0x128, 58: DMA2 Channel3
__attribute__ ((weak)) void DMA2_Channel3_IRQHandler()
{
	while (1);
}

#if defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x12c, 59: DMA2 Channel4
__attribute__ ((weak)) void DMA2_Channel4_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x12c, 59: DMA2 Channel4 and DMA2 Channel5
__attribute__ ((weak)) void DMA2_Channel4_5_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x130, 60: DMA2 Channel5
__attribute__ ((weak)) void DMA2_Channel5_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

// 0x134, 61: Ethernet
__attribute__ ((weak)) void ETH_IRQHandler()
{
	while (1);
}

// 0x138, 62: Ethernet Wakeup through EXTI line
__attribute__ ((weak)) void ETH_WKUP_IRQHandler()
{
	while (1);
}

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

// 0x14c, 67: USB On The Go FS
__attribute__ ((weak)) void OTG_FS_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

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
		WWDG_IRQHandler,					// 0x040, 0: Window watchdog
		PVD_IRQHandler,						// 0x044, 1: PVD through EXTI Line detection
		TAMP_STAMP_IRQHandler,				// 0x048, 2: Tamper and TimeStamp through the EXTI line
		RTC_WKUP_IRQHandler,				// 0x04c, 3: RTC Wakeup through the EXTI line
		FLASH_IRQHandler,					// 0x050, 4: Flash
		RCC_IRQHandler,						// 0x054, 5: RCC
		EXTI0_IRQHandler,					// 0x058, 6: EXTI Line0
		EXTI1_IRQHandler,					// 0x05c, 7: EXTI Line1
		EXTI2_IRQHandler,					// 0x060, 8: EXTI Line2
		EXTI3_IRQHandler,					// 0x064, 9: EXTI Line3
		EXTI4_IRQHandler,					// 0x068, 10: EXTI Line4
		DMA1_Channel1_IRQHandler,			// 0x06c, 11: DMA1 Channel1
		DMA1_Channel2_IRQHandler,			// 0x070, 12: DMA1 Channel2
		DMA1_Channel3_IRQHandler,			// 0x074, 13: DMA1 Channel3
		DMA1_Channel4_IRQHandler,			// 0x078, 14: DMA1 Channel4
		DMA1_Channel5_IRQHandler,			// 0x07c, 15: DMA1 Channel5
		DMA1_Channel6_IRQHandler,			// 0x080, 16: DMA1 Channel6
		DMA1_Channel7_IRQHandler,			// 0x084, 17: DMA1 Channel7

#if defined(STM32F100_VECTORS)

		ADC1_IRQHandler,					// 0x088, 18: ADC1

#else	// !defined(STM32F100_VECTORS)

		ADC1_2_IRQHandler,					// 0x088, 18: ADC1 and ADC2

#endif	// !defined(STM32F100_VECTORS)

#if defined(STM32F100_VECTORS)

		Reserved_0x8c_Handler,				// 0x08c, 19: Reserved
		Reserved_0x90_Handler,				// 0x090, 20: Reserved
		Reserved_0x94_Handler,				// 0x094, 21: Reserved
		Reserved_0x98_Handler,				// 0x098, 22: Reserved

#elif defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		CAN1_TX_IRQHandler,					// 0x08c, 19: CAN1 TX
		CAN1_RX0_IRQHandler,				// 0x090, 20: CAN1 RX0
		CAN1_RX1_IRQHandler,				// 0x094, 21: CAN1 RX1
		CAN1_SCE_IRQHandler,				// 0x098, 22: CAN1 SCE

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		USB_HP_CAN1_TX_IRQHandler,			// 0x08c, 19: USB High Priority or CAN1 TX
		USB_LP_CAN1_RX0_IRQHandler,			// 0x090, 20: USB Low Priority or CAN1 RX0
		CAN1_RX1_IRQHandler,				// 0x094, 21: CAN1 RX1
		CAN1_SCE_IRQHandler,				// 0x098, 22: CAN1 SCE

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		EXTI9_5_IRQHandler,					// 0x09c, 23: EXTI Line[9:5]

#if defined(STM32F100_VECTORS)

		TIM1_BRK_TIM15_IRQHandler,			// 0x0a0, 24: TIM1 Break and TIM15
		TIM1_UP_TIM16_IRQHandler,			// 0x0a4, 25: TIM1 Update and TIM16
		TIM1_TRG_COM_TIM17_IRQHandler,		// 0x0a8, 26: TIM1 Trigger and Commutation and TIM17

#elif defined(STM32F1_XL_DENSITY_VECTORS)

		TIM1_BRK_TIM9_IRQHandler,			// 0x0a0, 24: TIM1 Break and TIM9
		TIM1_UP_TIM10_IRQHandler,			// 0x0a4, 25: TIM1 Update and TIM10
		TIM1_TRG_COM_TIM11_IRQHandler,		// 0x0a8, 26: TIM1 Trigger and Commutation and TIM11

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_XL_DENSITY_VECTORS)

		TIM1_BRK_IRQHandler,				// 0x0a0, 24: TIM1 Break
		TIM1_UP_IRQHandler,					// 0x0a4, 25: TIM1 Update
		TIM1_TRG_COM_IRQHandler,			// 0x0a8, 26: TIM1 Trigger and Commutation

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_XL_DENSITY_VECTORS)

		TIM1_CC_IRQHandler,					// 0x0ac, 27: TIM1 Capture Compare
		TIM2_IRQHandler,					// 0x0b0, 28: TIM2
		TIM3_IRQHandler,					// 0x0b4, 29: TIM3
		TIM4_IRQHandler,					// 0x0b8, 30: TIM4
		I2C1_EV_IRQHandler,					// 0x0bc, 31: I2C1 event
		I2C1_ER_IRQHandler,					// 0x0c0, 32: I2C1 error
		I2C2_EV_IRQHandler,					// 0x0c4, 33: I2C2 event
		I2C2_ER_IRQHandler,					// 0x0c8, 34: I2C2 error
		SPI1_IRQHandler,					// 0x0cc, 35: SPI1
		SPI2_IRQHandler,					// 0x0d0, 36: SPI2
		USART1_IRQHandler,					// 0x0d4, 37: USART1
		USART2_IRQHandler,					// 0x0d8, 38: USART2
		USART3_IRQHandler,					// 0x0dc, 39: USART3
		EXTI15_10_IRQHandler,				// 0x0e0, 40: EXTI Line[15:10]
		RTC_Alarm_IRQHandler,				// 0x0e4, 41: RTC Alarm (A and B) through EXTI line

#if defined(STM32F100_VECTORS)

		CEC_IRQHandler,						// 0x0e8, 42: CEC

#elif defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		OTG_FS_WKUP_IRQHandler,				// 0x0e8, 42: USB On-The-Go FS Wakeup through EXTI line

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		USB_WKUP_IRQHandler,				// 0x0e8, 42: USB wakeup from suspend through EXTI line

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F100_VECTORS)

		TIM12_IRQHandler,					// 0x0ec, 43: TIM12
		TIM13_IRQHandler,					// 0x0f0, 44: TIM13
		TIM14_IRQHandler,					// 0x0f4, 45: TIM14

#elif defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		Reserved_0xec_Handler,				// 0x0ec, 43: Reserved
		Reserved_0xf0_Handler,				// 0x0f0, 44: Reserved
		Reserved_0xf4_Handler,				// 0x0f4, 45: Reserved

#elif defined(STM32F1_XL_DENSITY_VECTORS)

		TIM8_BRK_TIM12_IRQHandler,			// 0x0ec, 43: TIM8 Break and TIM12
		TIM8_UP_TIM13_IRQHandler,			// 0x0f0, 44: TIM8 Update and TIM13
		TIM8_TRG_COM_TIM14_IRQHandler,		// 0x0f4, 45: TIM8 Trigger and Commutation and TIM14

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS) &&
		// !defined(STM32F1_XL_DENSITY_VECTORS)

		TIM8_BRK_IRQHandler,				// 0x0ec, 43: TIM8 Break
		TIM8_UP_IRQHandler,					// 0x0f0, 44: TIM8 Update
		TIM8_TRG_COM_IRQHandler,			// 0x0f4, 45: TIM8 Trigger and Commutation

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS) &&
		// !defined(STM32F1_XL_DENSITY_VECTORS)

#if defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		Reserved_0xf8_Handler,				// 0x0f8, 46: Reserved
		Reserved_0xfc_Handler,				// 0x0fc, 47: Reserved

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		TIM8_CC_IRQHandler,					// 0x0f8, 46: TIM8 Capture Compare
		ADC3_IRQHandler,					// 0x0fc, 47: ADC3

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		Reserved_0x100_Handler,				// 0x100, 48: Reserved

#else	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		FSMC_IRQHandler,					// 0x100, 48: FSMC

#endif	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		Reserved_0x104_Handler,				// 0x104, 49: Reserved

#else	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		SDIO_IRQHandler,					// 0x104, 49: SDIO

#endif	// !defined(STM32F100_VECTORS) && !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		TIM5_IRQHandler,					// 0x108, 50: TIM5
		SPI3_IRQHandler,					// 0x10c, 51: SPI3
		UART4_IRQHandler,					// 0x110, 52: UART4
		UART5_IRQHandler,					// 0x114, 53: UART5

#if defined(STM32F100_VECTORS)

		TIM6_DAC_IRQHandler,				// 0x118, 54: TIM6 and DAC underrun

#else	// !defined(STM32F100_VECTORS)

		TIM6_IRQHandler,					// 0x118, 54: TIM6

#endif	// !defined(STM32F100_VECTORS)

		TIM7_IRQHandler,					// 0x11c, 55: TIM7
		DMA2_Channel1_IRQHandler,			// 0x120, 56: DMA2 Channel1
		DMA2_Channel2_IRQHandler,			// 0x124, 57: DMA2 Channel2
		DMA2_Channel3_IRQHandler,			// 0x128, 58: DMA2 Channel3

#if defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		DMA2_Channel4_IRQHandler,			// 0x12c, 59: DMA2 Channel4

#else	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		DMA2_Channel4_5_IRQHandler,			// 0x12c, 59: DMA2 Channel4 and DMA2 Channel5

#endif	// !defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		DMA2_Channel5_IRQHandler,			// 0x130, 60: DMA2 Channel5

#endif	// defined(STM32F100_VECTORS) || defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

#if defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

		ETH_IRQHandler,						// 0x134, 61: Ethernet
		ETH_WKUP_IRQHandler,				// 0x138, 62: Ethernet Wakeup through EXTI line
		CAN2_TX_IRQHandler,					// 0x13c, 63: CAN2 TX
		CAN2_RX0_IRQHandler,				// 0x140, 64: CAN2 RX0
		CAN2_RX1_IRQHandler,				// 0x144, 65: CAN2 RX1
		CAN2_SCE_IRQHandler,				// 0x148, 66: CAN2 SCE
		OTG_FS_IRQHandler,					// 0x14c, 67: USB On The Go FS

#endif	// defined(STM32F1_CONNECTIVITY_LINE_VECTORS)

};

namespace
{

/// expected number of chip vectors
constexpr size_t expectedChipVectorsSize
{
#if defined(STM32F100_VECTORS)
		61
#elif defined(STM32F1_CONNECTIVITY_LINE_VECTORS)
		68
#elif defined(STM32F1_XL_DENSITY_VECTORS)
		60
#else
		60
#endif
};

static_assert(sizeof(chipVectors) / sizeof(*chipVectors) == expectedChipVectorsSize, "Invalid size of chipVectors[]!");

}	// namespace
