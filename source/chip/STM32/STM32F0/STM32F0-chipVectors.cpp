/**
 * \file
 * \brief STM32F0 chip vector table and default weak handlers
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

#if defined(CONFIG_CHIP_STM32F030) || defined(CONFIG_CHIP_STM32F070)
	#define STM32F030_STM32F070_VECTORS
#else
	#define STM32F0_STANDARD_VECTORS
#endif

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| default weak handlers
+---------------------------------------------------------------------------------------------------------------------*/

// 0x40, 0: Window watchdog
__attribute__ ((weak)) void WWDG_IRQHandler()
{
	while (1);
}

#if defined(STM32F030_STM32F070_VECTORS)

// 0x44, 1: Reserved
__attribute__ ((weak)) void Reserved_0x44_Handler()
{
	while (1);
}

#else	// !defined(STM32F030_STM32F070_VECTORS)

// 0x44, 1: PVD and VDDIO2 supply comparator (combined EXTI lines 16 and 31)
__attribute__ ((weak)) void PVD_VDDIO2_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F030_STM32F070_VECTORS)

// 0x48, 2: RTC (combined EXTI lines 17, 19 and 20)
__attribute__ ((weak)) void RTC_IRQHandler()
{
	while (1);
}

// 0x4c, 3: Flash
__attribute__ ((weak)) void FLASH_IRQHandler()
{
	while (1);
}

#if defined(STM32F030_STM32F070_VECTORS)

// 0x50, 4: RCC
__attribute__ ((weak)) void RCC_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F030_STM32F070_VECTORS)

// 0x50, 4: RCC and CRS
__attribute__ ((weak)) void RCC_CRS_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F030_STM32F070_VECTORS)

// 0x54, 5: EXTI Line[1:0]
__attribute__ ((weak)) void EXTI0_1_IRQHandler()
{
	while (1);
}

// 0x58, 6: EXTI Line[3:2]
__attribute__ ((weak)) void EXTI2_3_IRQHandler()
{
	while (1);
}

// 0x5c, 7: EXTI Line[15:4]
__attribute__ ((weak)) void EXTI4_15_IRQHandler()
{
	while (1);
}

#if defined(STM32F030_STM32F070_VECTORS)

// 0x60, 8: Reserved
__attribute__ ((weak)) void Reserved_0x60_Handler()
{
	while (1);
}

#else	// !defined(STM32F030_STM32F070_VECTORS)

// 0x60, 8: Touch sensing
__attribute__ ((weak)) void TSC_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F030_STM32F070_VECTORS)

// 0x64, 9: DMA channel 1
__attribute__ ((weak)) void DMA_Ch1_IRQHandler()
{
	while (1);
}

#if defined(STM32F030_STM32F070_VECTORS)

// 0x68, 10: DMA channel 2 and 3
__attribute__ ((weak)) void DMA_Ch2_3_IRQHandler()
{
	while (1);
}

// 0x6c, 11: DMA channel 4 and 5
__attribute__ ((weak)) void DMA_Ch4_5_IRQHandler()
{
	while (1);
}

// 0x70, 12: ADC
__attribute__ ((weak)) void ADC_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32F030_STM32F070_VECTORS)

// 0x68, 10: DMA channel 2 and 3, DMA2 channel 1 and 2
__attribute__ ((weak)) void DMA_Ch2_3_DMA2_Ch1_2_IRQHandler()
{
	while (1);
}

// 0x6c, 11: DMA channel 4, 5, 6 and 7, DMA2 channel 3, 4 and 5
__attribute__ ((weak)) void DMA_Ch4_7_DMA2_Ch3_5_IRQHandler()
{
	while (1);
}

// 0x70, 12: ADC and COMP (ADC combined with EXTI lines 21 and 22)
__attribute__ ((weak)) void ADC_COMP_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F030_STM32F070_VECTORS)

// 0x74, 13: TIM1 break, update, trigger and commutation
__attribute__ ((weak)) void TIM1_BRK_UP_TRG_COM_IRQHandler()
{
	while (1);
}

// 0x78, 14: TIM1 capture compare
__attribute__ ((weak)) void TIM1_CC_IRQHandler()
{
	while (1);
}

#if defined(STM32F030_STM32F070_VECTORS)

// 0x7c, 15: Reserved
__attribute__ ((weak)) void Reserved_0x7c_Handler()
{
	while (1);
}

#else	// !defined(STM32F030_STM32F070_VECTORS)

// 0x7c, 15: TIM2
__attribute__ ((weak)) void TIM2_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F030_STM32F070_VECTORS)

// 0x80, 16: TIM3
__attribute__ ((weak)) void TIM3_IRQHandler()
{
	while (1);
}

#if defined(STM32F030_STM32F070_VECTORS)

// 0x84, 17: TIM6
__attribute__ ((weak)) void TIM6_IRQHandler()
{
	while (1);
}

// 0x88, 18: Reserved
__attribute__ ((weak)) void Reserved_0x88_Handler()
{
	while (1);
}

#else	// !defined(STM32F030_STM32F070_VECTORS)

// 0x84, 17: TIM6 and DAC underrun
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

// 0x88, 18: TIM7
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F030_STM32F070_VECTORS)

// 0x8c, 19: TIM14
__attribute__ ((weak)) void TIM14_IRQHandler()
{
	while (1);
}

// 0x90, 20: TIM15
__attribute__ ((weak)) void TIM15_IRQHandler()
{
	while (1);
}

// 0x94, 21: TIM16
__attribute__ ((weak)) void TIM16_IRQHandler()
{
	while (1);
}

// 0x98, 22: TIM17
__attribute__ ((weak)) void TIM17_IRQHandler()
{
	while (1);
}

// 0x9c, 23: I2C1 (combined with EXTI line 23)
__attribute__ ((weak)) void I2C1_IRQHandler()
{
	while (1);
}

// 0xa0, 24: I2C2
__attribute__ ((weak)) void I2C2_IRQHandler()
{
	while (1);
}

// 0xa4, 25: SPI1
__attribute__ ((weak)) void SPI1_IRQHandler()
{
	while (1);
}

// 0xa8, 26: SPI2
__attribute__ ((weak)) void SPI2_IRQHandler()
{
	while (1);
}

// 0xac, 27: USART1 (combined with EXTI line 25)
__attribute__ ((weak)) void USART1_IRQHandler()
{
	while (1);
}

// 0xb0, 28: USART2 (combined with EXTI line 26)
__attribute__ ((weak)) void USART2_IRQHandler()
{
	while (1);
}

#if defined(STM32F030_STM32F070_VECTORS)

// 0xb4, 29: USART3-USART6 (combined with EXTI line 28)
__attribute__ ((weak)) void USART3_6_IRQHandler()
{
	while (1);
}

// 0xb8, 30: Reserved
__attribute__ ((weak)) void Reserved_0xb8_Handler()
{
	while (1);
}

#else	// !defined(STM32F030_STM32F070_VECTORS)

// 0xb4, 29: USART3-USART8 (combined with EXTI line 28)
__attribute__ ((weak)) void USART3_8_IRQHandler()
{
	while (1);
}

// 0xb8, 30: CEC and CAN (combined with EXTI line 27)
__attribute__ ((weak)) void CEC_CAN_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F030_STM32F070_VECTORS)

// 0xbc, 31: USB (combined with EXTI line 18)
__attribute__ ((weak)) void USB_IRQHandler()
{
	while (1);
}

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
		WWDG_IRQHandler,					// 0x40, 0: Window watchdog

#if defined(STM32F030_STM32F070_VECTORS)

		Reserved_0x44_Handler,				// 0x44, 1: Reserved

#else	// !defined(STM32F030_STM32F070_VECTORS)

		PVD_VDDIO2_IRQHandler,				// 0x44, 1: PVD and VDDIO2 supply comparator (combined EXTI lines 16 and 31)

#endif	// !defined(STM32F030_STM32F070_VECTORS)

		RTC_IRQHandler,						// 0x48, 2: RTC (combined EXTI lines 17, 19 and 20)
		FLASH_IRQHandler,					// 0x4c, 3: Flash

#if defined(STM32F030_STM32F070_VECTORS)

		RCC_IRQHandler,						// 0x50, 4: RCC

#else	// !defined(STM32F030_STM32F070_VECTORS)

		RCC_CRS_IRQHandler,					// 0x50, 4: RCC and CRS

#endif	// !defined(STM32F030_STM32F070_VECTORS)

		EXTI0_1_IRQHandler,					// 0x54, 5: EXTI Line[1:0]
		EXTI2_3_IRQHandler,					// 0x58, 6: EXTI Line[3:2]
		EXTI4_15_IRQHandler,				// 0x5c, 7: EXTI Line[15:4]

#if defined(STM32F030_STM32F070_VECTORS)

		Reserved_0x60_Handler,				// 0x60, 8: Reserved

#else	// !defined(STM32F030_STM32F070_VECTORS)

		TSC_IRQHandler,						// 0x60, 8: Touch sensing

#endif	// !defined(STM32F030_STM32F070_VECTORS)

		DMA_Ch1_IRQHandler,					// 0x64, 9: DMA channel 1

#if defined(STM32F030_STM32F070_VECTORS)

		DMA_Ch2_3_IRQHandler,				// 0x68, 10: DMA channel 2 and 3
		DMA_Ch4_5_IRQHandler,				// 0x6c, 11: DMA channel 4 and 5
		ADC_IRQHandler,						// 0x70, 12: ADC

#else	// !defined(STM32F030_STM32F070_VECTORS)

		DMA_Ch2_3_DMA2_Ch1_2_IRQHandler,	// 0x68, 10: DMA channel 2 and 3, DMA2 channel 1 and 2
		DMA_Ch4_7_DMA2_Ch3_5_IRQHandler,	// 0x6c, 11: DMA channel 4, 5, 6 and 7, DMA2 channel 3, 4 and 5
		ADC_COMP_IRQHandler,				// 0x70, 12: ADC and COMP (ADC combined with EXTI lines 21 and 22)

#endif	// !defined(STM32F030_STM32F070_VECTORS)

		TIM1_BRK_UP_TRG_COM_IRQHandler,		// 0x74, 13: TIM1 break, update, trigger and commutation
		TIM1_CC_IRQHandler,					// 0x78, 14: TIM1 capture compare

#if defined(STM32F030_STM32F070_VECTORS)

		Reserved_0x7c_Handler,				// 0x7c, 15: Reserved

#else	// !defined(STM32F030_STM32F070_VECTORS)

		TIM2_IRQHandler,					// 0x7c, 15: TIM2

#endif	// !defined(STM32F030_STM32F070_VECTORS)

		TIM3_IRQHandler,					// 0x80, 16: TIM3

#if defined(STM32F030_STM32F070_VECTORS)

		TIM6_IRQHandler,					// 0x84, 17: TIM6
		Reserved_0x88_Handler,				// 0x88, 18: Reserved

#else	// !defined(STM32F030_STM32F070_VECTORS)

		TIM6_DAC_IRQHandler,				// 0x84, 17: TIM6 and DAC underrun
		TIM7_IRQHandler,					// 0x88, 18: TIM7

#endif	// !defined(STM32F030_STM32F070_VECTORS)

		TIM14_IRQHandler,					// 0x8c, 19: TIM14
		TIM15_IRQHandler,					// 0x90, 20: TIM15
		TIM16_IRQHandler,					// 0x94, 21: TIM16
		TIM17_IRQHandler,					// 0x98, 22: TIM17
		I2C1_IRQHandler,					// 0x9c, 23: I2C1 (combined with EXTI line 23)
		I2C2_IRQHandler,					// 0xa0, 24: I2C2
		SPI1_IRQHandler,					// 0xa4, 25: SPI1
		SPI2_IRQHandler,					// 0xa8, 26: SPI2
		USART1_IRQHandler,					// 0xac, 27: USART1 (combined with EXTI line 25)
		USART2_IRQHandler,					// 0xb0, 28: USART2 (combined with EXTI line 26)

#if defined(STM32F030_STM32F070_VECTORS)

		USART3_6_IRQHandler,				// 0xb4, 29: USART3-USART6 (combined with EXTI line 28)
		Reserved_0xb8_Handler,				// 0xb8, 30: Reserved

#else	// !defined(STM32F030_STM32F070_VECTORS)

		USART3_8_IRQHandler,				// 0xb4, 29: USART3-USART8 (combined with EXTI line 28)
		CEC_CAN_IRQHandler,					// 0xb8, 30: CEC and CAN (combined with EXTI line 27)

#endif	// !defined(STM32F030_STM32F070_VECTORS)

		USB_IRQHandler,						// 0xbc, 31: USB (combined with EXTI line 18)
};

namespace
{

/// expected number of chip vectors
constexpr size_t expectedChipVectorsSize {32};

static_assert(sizeof(chipVectors) / sizeof(*chipVectors) == expectedChipVectorsSize, "Invalid size of chipVectors[]!");

}	// namespace
