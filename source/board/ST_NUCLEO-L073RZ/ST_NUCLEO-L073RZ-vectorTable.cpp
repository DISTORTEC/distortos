/**
 * \file
 * \brief ST,NUCLEO-L073RZ (ST,STM32L073RZ chip) vector table, default handlers and NVIC low-level initializer
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
constexpr uint8_t interruptPriority {};

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
#ifdef DISTORTOS_ARCHITECTURE_NVIC_PVD_ENABLE
	NVIC_SetPriority(PVD_IRQn, interruptPriority);
	NVIC_EnableIRQ(PVD_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_PVD_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RTC_ENABLE
	NVIC_SetPriority(RTC_IRQn, interruptPriority);
	NVIC_EnableIRQ(RTC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RTC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FLASH_ENABLE
	NVIC_SetPriority(FLASH_IRQn, interruptPriority);
	NVIC_EnableIRQ(FLASH_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FLASH_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RCC_CRS_ENABLE
	NVIC_SetPriority(RCC_CRS_IRQn, interruptPriority);
	NVIC_EnableIRQ(RCC_CRS_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RCC_CRS_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI0_1_ENABLE
	NVIC_SetPriority(EXTI0_1_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI0_1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI2_3_ENABLE
	NVIC_SetPriority(EXTI2_3_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI2_3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI4_15_ENABLE
	NVIC_SetPriority(EXTI4_15_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI4_15_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TSC_ENABLE
	NVIC_SetPriority(TSC_IRQn, interruptPriority);
	NVIC_EnableIRQ(TSC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TSC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL1_ENABLE
	NVIC_SetPriority(DMA1_Channel1_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL2_3_ENABLE
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL2_3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL4_5_6_7_ENABLE
	NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL4_5_6_7_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_ADC1_COMP_ENABLE
	NVIC_SetPriority(ADC1_COMP_IRQn, interruptPriority);
	NVIC_EnableIRQ(ADC1_COMP_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_ADC1_COMP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LPTIM1_ENABLE
	NVIC_SetPriority(LPTIM1_IRQn, interruptPriority);
	NVIC_EnableIRQ(LPTIM1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LPTIM1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USART4_5_ENABLE
	NVIC_SetPriority(USART4_5_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART4_5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USART4_5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM2_ENABLE
	NVIC_SetPriority(TIM2_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM3_ENABLE
	NVIC_SetPriority(TIM3_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM6_DAC_ENABLE
	NVIC_SetPriority(TIM6_DAC_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM6_DAC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM7_ENABLE
	NVIC_SetPriority(TIM7_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM7_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM7_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM21_ENABLE
	NVIC_SetPriority(TIM21_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM21_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM21_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C3_ENABLE
	NVIC_SetPriority(I2C3_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM22_ENABLE
	NVIC_SetPriority(TIM22_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM22_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM22_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C1_ENABLE
	NVIC_SetPriority(I2C1_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C2_ENABLE
	NVIC_SetPriority(I2C2_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C2_ENABLE
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
#ifdef DISTORTOS_ARCHITECTURE_NVIC_AES_RNG_LPUART1_ENABLE
	NVIC_SetPriority(AES_RNG_LPUART1_IRQn, interruptPriority);
	NVIC_EnableIRQ(AES_RNG_LPUART1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_AES_RNG_LPUART1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_LCD_ENABLE
	NVIC_SetPriority(LCD_IRQn, interruptPriority);
	NVIC_EnableIRQ(LCD_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_LCD_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USB_ENABLE
	NVIC_SetPriority(USB_IRQn, interruptPriority);
	NVIC_EnableIRQ(USB_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USB_ENABLE
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

// SVC, address 0x2c, exception number 11, IRQ number -5
__attribute__ ((weak)) void SVC_Handler()
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

// PVD, address 0x44, exception number 17, IRQ number 1
__attribute__ ((weak)) void PVD_IRQHandler()
{
	while (1);
}

// RTC, address 0x48, exception number 18, IRQ number 2
__attribute__ ((weak)) void RTC_IRQHandler()
{
	while (1);
}

// FLASH, address 0x4c, exception number 19, IRQ number 3
__attribute__ ((weak)) void FLASH_IRQHandler()
{
	while (1);
}

// RCC_CRS, address 0x50, exception number 20, IRQ number 4
__attribute__ ((weak)) void RCC_CRS_IRQHandler()
{
	while (1);
}

// EXTI0_1, address 0x54, exception number 21, IRQ number 5
__attribute__ ((weak)) void EXTI0_1_IRQHandler()
{
	while (1);
}

// EXTI2_3, address 0x58, exception number 22, IRQ number 6
__attribute__ ((weak)) void EXTI2_3_IRQHandler()
{
	while (1);
}

// EXTI4_15, address 0x5c, exception number 23, IRQ number 7
__attribute__ ((weak)) void EXTI4_15_IRQHandler()
{
	while (1);
}

// TSC, address 0x60, exception number 24, IRQ number 8
__attribute__ ((weak)) void TSC_IRQHandler()
{
	while (1);
}

// DMA1_Channel1, address 0x64, exception number 25, IRQ number 9
__attribute__ ((weak)) void DMA1_Channel1_IRQHandler()
{
	while (1);
}

// DMA1_Channel2_3, address 0x68, exception number 26, IRQ number 10
void DMA1_Channel2_3_IRQHandler()
{
	__attribute__ ((weak)) void DMA1_Channel2_IRQHandler();
	__attribute__ ((weak)) void DMA1_Channel3_IRQHandler();

	DMA1_Channel2_IRQHandler();
	DMA1_Channel3_IRQHandler();
}

// DMA1_Channel4_5_6_7, address 0x6c, exception number 27, IRQ number 11
void DMA1_Channel4_5_6_7_IRQHandler()
{
	__attribute__ ((weak)) void DMA1_Channel4_IRQHandler();
	__attribute__ ((weak)) void DMA1_Channel5_IRQHandler();
	__attribute__ ((weak)) void DMA1_Channel6_IRQHandler();
	__attribute__ ((weak)) void DMA1_Channel7_IRQHandler();

	DMA1_Channel4_IRQHandler();
	DMA1_Channel5_IRQHandler();
	DMA1_Channel6_IRQHandler();
	DMA1_Channel7_IRQHandler();
}

// ADC1_COMP, address 0x70, exception number 28, IRQ number 12
__attribute__ ((weak)) void ADC1_COMP_IRQHandler()
{
	while (1);
}

// LPTIM1, address 0x74, exception number 29, IRQ number 13
__attribute__ ((weak)) void LPTIM1_IRQHandler()
{
	while (1);
}

// USART4_5, address 0x78, exception number 30, IRQ number 14
void USART4_5_IRQHandler()
{
	__attribute__ ((weak)) void USART4_IRQHandler();
	__attribute__ ((weak)) void USART5_IRQHandler();

	USART4_IRQHandler();
	USART5_IRQHandler();
}

// TIM2, address 0x7c, exception number 31, IRQ number 15
__attribute__ ((weak)) void TIM2_IRQHandler()
{
	while (1);
}

// TIM3, address 0x80, exception number 32, IRQ number 16
__attribute__ ((weak)) void TIM3_IRQHandler()
{
	while (1);
}

// TIM6_DAC, address 0x84, exception number 33, IRQ number 17
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

// TIM7, address 0x88, exception number 34, IRQ number 18
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

// TIM21, address 0x90, exception number 36, IRQ number 20
__attribute__ ((weak)) void TIM21_IRQHandler()
{
	while (1);
}

// I2C3, address 0x94, exception number 37, IRQ number 21
__attribute__ ((weak)) void I2C3_IRQHandler()
{
	while (1);
}

// TIM22, address 0x98, exception number 38, IRQ number 22
__attribute__ ((weak)) void TIM22_IRQHandler()
{
	while (1);
}

// I2C1, address 0x9c, exception number 39, IRQ number 23
__attribute__ ((weak)) void I2C1_IRQHandler()
{
	while (1);
}

// I2C2, address 0xa0, exception number 40, IRQ number 24
__attribute__ ((weak)) void I2C2_IRQHandler()
{
	while (1);
}

// SPI1, address 0xa4, exception number 41, IRQ number 25
__attribute__ ((weak)) void SPI1_IRQHandler()
{
	while (1);
}

// SPI2, address 0xa8, exception number 42, IRQ number 26
__attribute__ ((weak)) void SPI2_IRQHandler()
{
	while (1);
}

// USART1, address 0xac, exception number 43, IRQ number 27
__attribute__ ((weak)) void USART1_IRQHandler()
{
	while (1);
}

// USART2, address 0xb0, exception number 44, IRQ number 28
__attribute__ ((weak)) void USART2_IRQHandler()
{
	while (1);
}

// AES_RNG_LPUART1, address 0xb4, exception number 45, IRQ number 29
__attribute__ ((weak)) void AES_RNG_LPUART1_IRQHandler()
{
	while (1);
}

// LCD, address 0xb8, exception number 46, IRQ number 30
__attribute__ ((weak)) void LCD_IRQHandler()
{
	while (1);
}

// USB, address 0xbc, exception number 47, IRQ number 31
__attribute__ ((weak)) void USB_IRQHandler()
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
		nullptr,		// Reserved, address 0x10, exception number 4, IRQ number -12
		nullptr,		// Reserved, address 0x14, exception number 5, IRQ number -11
		nullptr,		// Reserved, address 0x18, exception number 6, IRQ number -10
		nullptr,		// Reserved, address 0x1c, exception number 7, IRQ number -9
		nullptr,		// Reserved, address 0x20, exception number 8, IRQ number -8
		nullptr,		// Reserved, address 0x24, exception number 9, IRQ number -7
		nullptr,		// Reserved, address 0x28, exception number 10, IRQ number -6
		SVC_Handler,		// SVC, address 0x2c, exception number 11, IRQ number -5
		nullptr,		// Reserved, address 0x30, exception number 12, IRQ number -4
		nullptr,		// Reserved, address 0x34, exception number 13, IRQ number -3
		PendSV_Handler,		// PendSV, address 0x38, exception number 14, IRQ number -2
		SysTick_Handler,		// SysTick, address 0x3c, exception number 15, IRQ number -1
		WWDG_IRQHandler,		// WWDG, address 0x40, exception number 16, IRQ number 0
		PVD_IRQHandler,		// PVD, address 0x44, exception number 17, IRQ number 1
		RTC_IRQHandler,		// RTC, address 0x48, exception number 18, IRQ number 2
		FLASH_IRQHandler,		// FLASH, address 0x4c, exception number 19, IRQ number 3
		RCC_CRS_IRQHandler,		// RCC_CRS, address 0x50, exception number 20, IRQ number 4
		EXTI0_1_IRQHandler,		// EXTI0_1, address 0x54, exception number 21, IRQ number 5
		EXTI2_3_IRQHandler,		// EXTI2_3, address 0x58, exception number 22, IRQ number 6
		EXTI4_15_IRQHandler,		// EXTI4_15, address 0x5c, exception number 23, IRQ number 7
		TSC_IRQHandler,		// TSC, address 0x60, exception number 24, IRQ number 8
		DMA1_Channel1_IRQHandler,		// DMA1_Channel1, address 0x64, exception number 25, IRQ number 9
		DMA1_Channel2_3_IRQHandler,		// DMA1_Channel2_3, address 0x68, exception number 26, IRQ number 10
		DMA1_Channel4_5_6_7_IRQHandler,		// DMA1_Channel4_5_6_7, address 0x6c, exception number 27, IRQ number 11
		ADC1_COMP_IRQHandler,		// ADC1_COMP, address 0x70, exception number 28, IRQ number 12
		LPTIM1_IRQHandler,		// LPTIM1, address 0x74, exception number 29, IRQ number 13
		USART4_5_IRQHandler,		// USART4_5, address 0x78, exception number 30, IRQ number 14
		TIM2_IRQHandler,		// TIM2, address 0x7c, exception number 31, IRQ number 15
		TIM3_IRQHandler,		// TIM3, address 0x80, exception number 32, IRQ number 16
		TIM6_DAC_IRQHandler,		// TIM6_DAC, address 0x84, exception number 33, IRQ number 17
		TIM7_IRQHandler,		// TIM7, address 0x88, exception number 34, IRQ number 18
		nullptr,		// Reserved, address 0x8c, exception number 35, IRQ number 19
		TIM21_IRQHandler,		// TIM21, address 0x90, exception number 36, IRQ number 20
		I2C3_IRQHandler,		// I2C3, address 0x94, exception number 37, IRQ number 21
		TIM22_IRQHandler,		// TIM22, address 0x98, exception number 38, IRQ number 22
		I2C1_IRQHandler,		// I2C1, address 0x9c, exception number 39, IRQ number 23
		I2C2_IRQHandler,		// I2C2, address 0xa0, exception number 40, IRQ number 24
		SPI1_IRQHandler,		// SPI1, address 0xa4, exception number 41, IRQ number 25
		SPI2_IRQHandler,		// SPI2, address 0xa8, exception number 42, IRQ number 26
		USART1_IRQHandler,		// USART1, address 0xac, exception number 43, IRQ number 27
		USART2_IRQHandler,		// USART2, address 0xb0, exception number 44, IRQ number 28
		AES_RNG_LPUART1_IRQHandler,		// AES_RNG_LPUART1, address 0xb4, exception number 45, IRQ number 29
		LCD_IRQHandler,		// LCD, address 0xb8, exception number 46, IRQ number 30
		USB_IRQHandler,		// USB, address 0xbc, exception number 47, IRQ number 31
};

}	// extern "C"
