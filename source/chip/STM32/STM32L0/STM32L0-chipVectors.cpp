/**
 * \file
 * \brief STM32F0 chip vector table and default weak handlers
 *
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
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

#if defined(CONFIG_CHIP_STM32L053) || defined(CONFIG_CHIP_STM32L063) || defined(CONFIG_CHIP_STM32L073) || \
		defined(CONFIG_CHIP_STM32L083)
	#define STM32L0X3_VECTORS
#elif defined(CONFIG_CHIP_STM32L052) || defined(CONFIG_CHIP_STM32L062) || defined(CONFIG_CHIP_STM32L072) || \
		defined(CONFIG_CHIP_STM32L082)
	#define STM32L0X2_VECTORS
#else
	#define SMT32L0_STANDARD_VECTORS
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

// 0x44, 1: PVD through EXTI Line detect
__attribute__ ((weak)) void PVD_IRQHandler()
{
	while (1);
}

// 0x48, 2: RTC through EXTI Line
__attribute__ ((weak)) void RTC_IRQHandler()
{
	while (1);
}

// 0x4c, 3: Flash
__attribute__ ((weak)) void FLASH_IRQHandler()
{
	while (1);
}

// 0x50, 4: RCC and CRS
__attribute__ ((weak)) void RCC_CRS_IRQHandler()
{
	while (1);
}

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

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

// 0x60, 8: TSC
__attribute__ ((weak)) void TSC_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

// 0x60, 8: Reserved
__attribute__ ((weak)) void Reserved_0x60_Handler()
{
	while (1);
}

#endif	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

// 0x64, 9: DMA channel 1
__attribute__ ((weak)) void DMA_Ch1_IRQHandler()
{
	while (1);
}

// 0x68, 10: DMA channel 2 and 3
__attribute__ ((weak)) void DMA_Ch2_3_IRQHandler()
{
	while (1);
}

// 0x6c, 11: DMA channel 4, 5, 6 and 7
__attribute__ ((weak)) void DMA_Ch4_5_6_7_IRQHandler()
{
	while (1);
}

// 0x70, 12: ADC1, COMP1 and COMP2
__attribute__ ((weak)) void ADC1_COMP_IRQHandler()
{
	while (1);
}

// 0x74, 13: LPTIM1
__attribute__ ((weak)) void LPTIM1_IRQHandler()
{
	while (1);
}

// 0x78, 14: USART4 and USART5
__attribute__ ((weak)) void USART4_5_IRQHandler()
{
	while (1);
}

// 0x7c, 15: TIM2
__attribute__ ((weak)) void TIM2_IRQHandler()
{
	while (1);
}

// 0x80, 16: TIM3
__attribute__ ((weak)) void TIM3_IRQHandler()
{
	while (1);
}

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

// 0x84, 17: TIM6 and DAC
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

// 0x84, 17: TIM6
__attribute__ ((weak)) void TIM6_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

// 0x88, 18: TIM7
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

// 0x8c, 19: Reserved
__attribute__ ((weak)) void Reserved_0x8c_Handler()
{
	while (1);
}

// 0x90, 20: TIM21
__attribute__ ((weak)) void TIM21_IRQHandler()
{
	while (1);
}

// 0x94, 21: I2C3
__attribute__ ((weak)) void I2C3_IRQHandler()
{
	while (1);
}

// 0x98, 22: TIM22
__attribute__ ((weak)) void TIM22_IRQHandler()
{
	while (1);
}

// 0x9c, 23: I2C1
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

// 0xac, 27: USART1
__attribute__ ((weak)) void USART1_IRQHandler()
{
	while (1);
}

// 0xb0, 28: USART2
__attribute__ ((weak)) void USART2_IRQHandler()
{
	while (1);
}

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

// 0xb4, 29: AES and RNG and LPUART1
__attribute__ ((weak)) void AES_RNG_LPUART1_IRQHandler()
{
	while (1);
}

#else	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

// 0xb4, 29: AES and LPUART1
__attribute__ ((weak)) void AES_LPUART1_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

#if defined(STM32L0X3_VECTORS)

// 0xb8, 30: LCD
__attribute__ ((weak)) void LCD_IRQHandler()
{
	while (1);
}

#elif defined(STM32L0X2_VECTORS)

// 0xb8, 30: Reserved
__attribute__ ((weak)) void Reserved_0xb8_Handler()
{
	while (1);
}

#endif	// !defined(STM32L0X2_VECTORS)

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

// 0xbc, 31: USB
__attribute__ ((weak)) void USB_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

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
extern "C" const InterruptVector chipVectors[] __attribute__ ((section(".chipVectors"), used))
{
		WWDG_IRQHandler,					// 0x40, 0: Window watchdog
		PVD_IRQHandler,						// 0x44, 1: PVD through EXTI Line detect
		RTC_IRQHandler,						// 0x48, 2: RTC through EXTI Line
		FLASH_IRQHandler,					// 0x4c, 3: Flash
		RCC_CRS_IRQHandler,					// 0x50, 4: RCC
		EXTI0_1_IRQHandler,					// 0x54, 5: EXTI Line[1:0]
		EXTI2_3_IRQHandler,					// 0x58, 6: EXTI Line[3:2]
		EXTI4_15_IRQHandler,					// 0x5c, 7: EXTI Line[15:4]

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

		TSC_IRQHandler,						// 0x60, 8: Reserved

#else	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

		Reserved_0x60_Handler,					// 0x60, 8: Reserved

#endif	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

		DMA_Ch1_IRQHandler,					// 0x64, 9: DMA channel 1
		DMA_Ch2_3_IRQHandler,					// 0x68, 10: DMA channel 2 and 3
		DMA_Ch4_5_6_7_IRQHandler,				// 0x6c, 11: DMA channel 4, 5, 6 and 7
		ADC1_COMP_IRQHandler,					// 0x70, 12: ADC1, COMP1 and COMP2
		LPTIM1_IRQHandler,					// 0x74, 13: LPTIM1
		USART4_5_IRQHandler,					// 0x78, 14: USART4 and USART5
		TIM2_IRQHandler,					// 0x7c, 15: TIM2
		TIM3_IRQHandler,					// 0x80, 16: TIM3

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

		TIM6_DAC_IRQHandler,					// 0x84, 17: TIM6 and DAC

#else	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

		TIM6_IRQHandler,					// 0x84, 17: TIM6

#endif	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

		TIM7_IRQHandler,					// 0x88, 18: TIM7
		Reserved_0x8c_Handler,					// 0x8c, 19: Reserved
		TIM21_IRQHandler,					// 0x90, 20: TIM21
		I2C3_IRQHandler,					// 0x94, 21: Reserved
		TIM22_IRQHandler,					// 0x98, 22: TIM22
		I2C1_IRQHandler,					// 0x9c, 23: I2C1
		I2C2_IRQHandler,					// 0xa0, 24: I2C2
		SPI1_IRQHandler,					// 0xa4, 25: SPI1
		SPI2_IRQHandler,					// 0xa8, 26: SPI2
		USART1_IRQHandler,					// 0xb0, 27: USART1
		USART2_IRQHandler,					// 0xb0, 28: USART2

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

		AES_RNG_LPUART1_IRQHandler,				// 0xb4, 29: AES and RNG and LPUART1

#else	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

		AES_LPUART1_IRQHandler,					// 0xb4, 29: AES and LPUART1

#endif	// !defined(STM32L0X2_VECTORS) && !defined(STM32L0X3_VECTORS)

#if defined(STM32L0X3_VECTORS)

		LCD_IRQHandler,						// 0xb8, 30: LCD

#elif defined(STM32L0X2_VECTORS)

		Reserved_0xb8_Handler,					// 0xb8, 30: Reserved

#endif	// !defined(STM32L0X2_VECTORS)

#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

		USB_IRQHandler,						// 0xbc, 31: USB

#endif	// defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)

};

namespace
{

/// expected number of chip vectors
constexpr size_t expectedChipVectorsSize
{
#if defined(STM32L0X2_VECTORS) || defined(STM32L0X3_VECTORS)
		32
#else
		30
#endif
};

static_assert(sizeof(chipVectors) / sizeof(*chipVectors) == expectedChipVectorsSize, "Invalid size of chipVectors[]!");

}	// namespace
