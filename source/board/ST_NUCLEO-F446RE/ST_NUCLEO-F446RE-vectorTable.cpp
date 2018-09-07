/**
 * \file
 * \brief ST,NUCLEO-F446RE (ST,STM32F446RE chip) vector table and default weak handlers
 *
 * \author Copyright (C) 2014-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| default weak handlers
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

// PVD, address 0x44, exception number 17, IRQ number 1
__attribute__ ((weak)) void PVD_IRQHandler()
{
	while (1);
}

// TAMP_STAMP, address 0x48, exception number 18, IRQ number 2
__attribute__ ((weak)) void TAMP_STAMP_IRQHandler()
{
	while (1);
}

// RTC_WKUP, address 0x4c, exception number 19, IRQ number 3
__attribute__ ((weak)) void RTC_WKUP_IRQHandler()
{
	while (1);
}

// FLASH, address 0x50, exception number 20, IRQ number 4
__attribute__ ((weak)) void FLASH_IRQHandler()
{
	while (1);
}

// RCC, address 0x54, exception number 21, IRQ number 5
__attribute__ ((weak)) void RCC_IRQHandler()
{
	while (1);
}

// EXTI0, address 0x58, exception number 22, IRQ number 6
__attribute__ ((weak)) void EXTI0_IRQHandler()
{
	while (1);
}

// EXTI1, address 0x5c, exception number 23, IRQ number 7
__attribute__ ((weak)) void EXTI1_IRQHandler()
{
	while (1);
}

// EXTI2, address 0x60, exception number 24, IRQ number 8
__attribute__ ((weak)) void EXTI2_IRQHandler()
{
	while (1);
}

// EXTI3, address 0x64, exception number 25, IRQ number 9
__attribute__ ((weak)) void EXTI3_IRQHandler()
{
	while (1);
}

// EXTI4, address 0x68, exception number 26, IRQ number 10
__attribute__ ((weak)) void EXTI4_IRQHandler()
{
	while (1);
}

// DMA1_Stream0, address 0x6c, exception number 27, IRQ number 11
__attribute__ ((weak)) void DMA1_Stream0_IRQHandler()
{
	while (1);
}

// DMA1_Stream1, address 0x70, exception number 28, IRQ number 12
__attribute__ ((weak)) void DMA1_Stream1_IRQHandler()
{
	while (1);
}

// DMA1_Stream2, address 0x74, exception number 29, IRQ number 13
__attribute__ ((weak)) void DMA1_Stream2_IRQHandler()
{
	while (1);
}

// DMA1_Stream3, address 0x78, exception number 30, IRQ number 14
__attribute__ ((weak)) void DMA1_Stream3_IRQHandler()
{
	while (1);
}

// DMA1_Stream4, address 0x7c, exception number 31, IRQ number 15
__attribute__ ((weak)) void DMA1_Stream4_IRQHandler()
{
	while (1);
}

// DMA1_Stream5, address 0x80, exception number 32, IRQ number 16
__attribute__ ((weak)) void DMA1_Stream5_IRQHandler()
{
	while (1);
}

// DMA1_Stream6, address 0x84, exception number 33, IRQ number 17
__attribute__ ((weak)) void DMA1_Stream6_IRQHandler()
{
	while (1);
}

// ADC, address 0x88, exception number 34, IRQ number 18
__attribute__ ((weak)) void ADC_IRQHandler()
{
	while (1);
}

// CAN1_TX, address 0x8c, exception number 35, IRQ number 19
__attribute__ ((weak)) void CAN1_TX_IRQHandler()
{
	while (1);
}

// CAN1_RX0, address 0x90, exception number 36, IRQ number 20
__attribute__ ((weak)) void CAN1_RX0_IRQHandler()
{
	while (1);
}

// CAN1_RX1, address 0x94, exception number 37, IRQ number 21
__attribute__ ((weak)) void CAN1_RX1_IRQHandler()
{
	while (1);
}

// CAN1_SCE, address 0x98, exception number 38, IRQ number 22
__attribute__ ((weak)) void CAN1_SCE_IRQHandler()
{
	while (1);
}

// EXTI9_5, address 0x9c, exception number 39, IRQ number 23
__attribute__ ((weak)) void EXTI9_5_IRQHandler()
{
	while (1);
}

// TIM1_BRK_TIM9, address 0xa0, exception number 40, IRQ number 24
__attribute__ ((weak)) void TIM1_BRK_TIM9_IRQHandler()
{
	while (1);
}

// TIM1_UP_TIM10, address 0xa4, exception number 41, IRQ number 25
__attribute__ ((weak)) void TIM1_UP_TIM10_IRQHandler()
{
	while (1);
}

// TIM1_TRG_COM_TIM11, address 0xa8, exception number 42, IRQ number 26
__attribute__ ((weak)) void TIM1_TRG_COM_TIM11_IRQHandler()
{
	while (1);
}

// TIM1_CC, address 0xac, exception number 43, IRQ number 27
__attribute__ ((weak)) void TIM1_CC_IRQHandler()
{
	while (1);
}

// TIM2, address 0xb0, exception number 44, IRQ number 28
__attribute__ ((weak)) void TIM2_IRQHandler()
{
	while (1);
}

// TIM3, address 0xb4, exception number 45, IRQ number 29
__attribute__ ((weak)) void TIM3_IRQHandler()
{
	while (1);
}

// TIM4, address 0xb8, exception number 46, IRQ number 30
__attribute__ ((weak)) void TIM4_IRQHandler()
{
	while (1);
}

// I2C1_EV, address 0xbc, exception number 47, IRQ number 31
__attribute__ ((weak)) void I2C1_EV_IRQHandler()
{
	while (1);
}

// I2C1_ER, address 0xc0, exception number 48, IRQ number 32
__attribute__ ((weak)) void I2C1_ER_IRQHandler()
{
	while (1);
}

// I2C2_EV, address 0xc4, exception number 49, IRQ number 33
__attribute__ ((weak)) void I2C2_EV_IRQHandler()
{
	while (1);
}

// I2C2_ER, address 0xc8, exception number 50, IRQ number 34
__attribute__ ((weak)) void I2C2_ER_IRQHandler()
{
	while (1);
}

// SPI1, address 0xcc, exception number 51, IRQ number 35
__attribute__ ((weak)) void SPI1_IRQHandler()
{
	while (1);
}

// SPI2, address 0xd0, exception number 52, IRQ number 36
__attribute__ ((weak)) void SPI2_IRQHandler()
{
	while (1);
}

// USART1, address 0xd4, exception number 53, IRQ number 37
__attribute__ ((weak)) void USART1_IRQHandler()
{
	while (1);
}

// USART2, address 0xd8, exception number 54, IRQ number 38
__attribute__ ((weak)) void USART2_IRQHandler()
{
	while (1);
}

// USART3, address 0xdc, exception number 55, IRQ number 39
__attribute__ ((weak)) void USART3_IRQHandler()
{
	while (1);
}

// EXTI15_10, address 0xe0, exception number 56, IRQ number 40
__attribute__ ((weak)) void EXTI15_10_IRQHandler()
{
	while (1);
}

// RTC_Alarm, address 0xe4, exception number 57, IRQ number 41
__attribute__ ((weak)) void RTC_Alarm_IRQHandler()
{
	while (1);
}

// OTG_FS_WKUP, address 0xe8, exception number 58, IRQ number 42
__attribute__ ((weak)) void OTG_FS_WKUP_IRQHandler()
{
	while (1);
}

// TIM8_BRK_TIM12, address 0xec, exception number 59, IRQ number 43
__attribute__ ((weak)) void TIM8_BRK_TIM12_IRQHandler()
{
	while (1);
}

// TIM8_UP_TIM13, address 0xf0, exception number 60, IRQ number 44
__attribute__ ((weak)) void TIM8_UP_TIM13_IRQHandler()
{
	while (1);
}

// TIM8_TRG_COM_TIM14, address 0xf4, exception number 61, IRQ number 45
__attribute__ ((weak)) void TIM8_TRG_COM_TIM14_IRQHandler()
{
	while (1);
}

// TIM8_CC, address 0xf8, exception number 62, IRQ number 46
__attribute__ ((weak)) void TIM8_CC_IRQHandler()
{
	while (1);
}

// DMA1_Stream7, address 0xfc, exception number 63, IRQ number 47
__attribute__ ((weak)) void DMA1_Stream7_IRQHandler()
{
	while (1);
}

// FMC, address 0x100, exception number 64, IRQ number 48
__attribute__ ((weak)) void FMC_IRQHandler()
{
	while (1);
}

// SDIO, address 0x104, exception number 65, IRQ number 49
__attribute__ ((weak)) void SDIO_IRQHandler()
{
	while (1);
}

// TIM5, address 0x108, exception number 66, IRQ number 50
__attribute__ ((weak)) void TIM5_IRQHandler()
{
	while (1);
}

// SPI3, address 0x10c, exception number 67, IRQ number 51
__attribute__ ((weak)) void SPI3_IRQHandler()
{
	while (1);
}

// UART4, address 0x110, exception number 68, IRQ number 52
__attribute__ ((weak)) void UART4_IRQHandler()
{
	while (1);
}

// UART5, address 0x114, exception number 69, IRQ number 53
__attribute__ ((weak)) void UART5_IRQHandler()
{
	while (1);
}

// TIM6_DAC, address 0x118, exception number 70, IRQ number 54
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

// TIM7, address 0x11c, exception number 71, IRQ number 55
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

// DMA2_Stream0, address 0x120, exception number 72, IRQ number 56
__attribute__ ((weak)) void DMA2_Stream0_IRQHandler()
{
	while (1);
}

// DMA2_Stream1, address 0x124, exception number 73, IRQ number 57
__attribute__ ((weak)) void DMA2_Stream1_IRQHandler()
{
	while (1);
}

// DMA2_Stream2, address 0x128, exception number 74, IRQ number 58
__attribute__ ((weak)) void DMA2_Stream2_IRQHandler()
{
	while (1);
}

// DMA2_Stream3, address 0x12c, exception number 75, IRQ number 59
__attribute__ ((weak)) void DMA2_Stream3_IRQHandler()
{
	while (1);
}

// DMA2_Stream4, address 0x130, exception number 76, IRQ number 60
__attribute__ ((weak)) void DMA2_Stream4_IRQHandler()
{
	while (1);
}

// CAN2_TX, address 0x13c, exception number 79, IRQ number 63
__attribute__ ((weak)) void CAN2_TX_IRQHandler()
{
	while (1);
}

// CAN2_RX0, address 0x140, exception number 80, IRQ number 64
__attribute__ ((weak)) void CAN2_RX0_IRQHandler()
{
	while (1);
}

// CAN2_RX1, address 0x144, exception number 81, IRQ number 65
__attribute__ ((weak)) void CAN2_RX1_IRQHandler()
{
	while (1);
}

// CAN2_SCE, address 0x148, exception number 82, IRQ number 66
__attribute__ ((weak)) void CAN2_SCE_IRQHandler()
{
	while (1);
}

// OTG_FS, address 0x14c, exception number 83, IRQ number 67
__attribute__ ((weak)) void OTG_FS_IRQHandler()
{
	while (1);
}

// DMA2_Stream5, address 0x150, exception number 84, IRQ number 68
__attribute__ ((weak)) void DMA2_Stream5_IRQHandler()
{
	while (1);
}

// DMA2_Stream6, address 0x154, exception number 85, IRQ number 69
__attribute__ ((weak)) void DMA2_Stream6_IRQHandler()
{
	while (1);
}

// DMA2_Stream7, address 0x158, exception number 86, IRQ number 70
__attribute__ ((weak)) void DMA2_Stream7_IRQHandler()
{
	while (1);
}

// USART6, address 0x15c, exception number 87, IRQ number 71
__attribute__ ((weak)) void USART6_IRQHandler()
{
	while (1);
}

// I2C3_EV, address 0x160, exception number 88, IRQ number 72
__attribute__ ((weak)) void I2C3_EV_IRQHandler()
{
	while (1);
}

// I2C3_ER, address 0x164, exception number 89, IRQ number 73
__attribute__ ((weak)) void I2C3_ER_IRQHandler()
{
	while (1);
}

// OTG_HS_EP1_OUT, address 0x168, exception number 90, IRQ number 74
__attribute__ ((weak)) void OTG_HS_EP1_OUT_IRQHandler()
{
	while (1);
}

// OTG_HS_EP1_IN, address 0x16c, exception number 91, IRQ number 75
__attribute__ ((weak)) void OTG_HS_EP1_IN_IRQHandler()
{
	while (1);
}

// OTG_HS_WKUP, address 0x170, exception number 92, IRQ number 76
__attribute__ ((weak)) void OTG_HS_WKUP_IRQHandler()
{
	while (1);
}

// OTG_HS, address 0x174, exception number 93, IRQ number 77
__attribute__ ((weak)) void OTG_HS_IRQHandler()
{
	while (1);
}

// DCMI, address 0x178, exception number 94, IRQ number 78
__attribute__ ((weak)) void DCMI_IRQHandler()
{
	while (1);
}

// FPU, address 0x184, exception number 97, IRQ number 81
__attribute__ ((weak)) void FPU_IRQHandler()
{
	while (1);
}

// SPI4, address 0x190, exception number 100, IRQ number 84
__attribute__ ((weak)) void SPI4_IRQHandler()
{
	while (1);
}

// SAI1, address 0x19c, exception number 103, IRQ number 87
__attribute__ ((weak)) void SAI1_IRQHandler()
{
	while (1);
}

// SAI2, address 0x1ac, exception number 107, IRQ number 91
__attribute__ ((weak)) void SAI2_IRQHandler()
{
	while (1);
}

// QUADSPI, address 0x1b0, exception number 108, IRQ number 92
__attribute__ ((weak)) void QUADSPI_IRQHandler()
{
	while (1);
}

// CEC, address 0x1b4, exception number 109, IRQ number 93
__attribute__ ((weak)) void CEC_IRQHandler()
{
	while (1);
}

// SPDIF_RX, address 0x1b8, exception number 110, IRQ number 94
__attribute__ ((weak)) void SPDIF_RX_IRQHandler()
{
	while (1);
}

// FMPI2C1_EV, address 0x1bc, exception number 111, IRQ number 95
__attribute__ ((weak)) void FMPI2C1_EV_IRQHandler()
{
	while (1);
}

// FMPI2C1_ER, address 0x1c0, exception number 112, IRQ number 96
__attribute__ ((weak)) void FMPI2C1_ER_IRQHandler()
{
	while (1);
}

/*---------------------------------------------------------------------------------------------------------------------+
| global symbols' declarations
+---------------------------------------------------------------------------------------------------------------------*/

extern const char __main_stack_end[];		// main stack end - imported from linker script

void Reset_Handler();		// import the address of Reset_Handler()

}	// extern "C"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// single exception vector - pointer to function with no arguments and no return value
using ExceptionVector = void(*)();

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global variables
+---------------------------------------------------------------------------------------------------------------------*/

/// core vector table
extern "C" const ExceptionVector coreVectors[] __attribute__ ((section(".coreVectors"), used))
{
		reinterpret_cast<ExceptionVector>(__main_stack_end),	// 0x0: Main stack end address
		Reset_Handler,		// Reset, address 0x4, exception number 1, IRQ number -15
		NMI_Handler,		// NMI, address 0x8, exception number 2, IRQ number -14
		HardFault_Handler,		// HardFault, address 0xc, exception number 3, IRQ number -13
		MemManage_Handler,		// MemManage, address 0x10, exception number 4, IRQ number -12
		BusFault_Handler,		// BusFault, address 0x14, exception number 5, IRQ number -11
		UsageFault_Handler,		// UsageFault, address 0x18, exception number 6, IRQ number -10
		nullptr,		// Reserved, address 0x1c, exception number 7, IRQ number -9
		nullptr,		// Reserved, address 0x20, exception number 8, IRQ number -8
		nullptr,		// Reserved, address 0x24, exception number 9, IRQ number -7
		nullptr,		// Reserved, address 0x28, exception number 10, IRQ number -6
		SVC_Handler,		// SVC, address 0x2c, exception number 11, IRQ number -5
		DebugMon_Handler,		// DebugMon, address 0x30, exception number 12, IRQ number -4
		nullptr,		// Reserved, address 0x34, exception number 13, IRQ number -3
		PendSV_Handler,		// PendSV, address 0x38, exception number 14, IRQ number -2
		SysTick_Handler,		// SysTick, address 0x3c, exception number 15, IRQ number -1
};

/// chip vector table
extern "C" const ExceptionVector chipVectors[] __attribute__ ((section(".chipVectors"), used))
{
		WWDG_IRQHandler,		// WWDG, address 0x40, exception number 16, IRQ number 0
		PVD_IRQHandler,		// PVD, address 0x44, exception number 17, IRQ number 1
		TAMP_STAMP_IRQHandler,		// TAMP_STAMP, address 0x48, exception number 18, IRQ number 2
		RTC_WKUP_IRQHandler,		// RTC_WKUP, address 0x4c, exception number 19, IRQ number 3
		FLASH_IRQHandler,		// FLASH, address 0x50, exception number 20, IRQ number 4
		RCC_IRQHandler,		// RCC, address 0x54, exception number 21, IRQ number 5
		EXTI0_IRQHandler,		// EXTI0, address 0x58, exception number 22, IRQ number 6
		EXTI1_IRQHandler,		// EXTI1, address 0x5c, exception number 23, IRQ number 7
		EXTI2_IRQHandler,		// EXTI2, address 0x60, exception number 24, IRQ number 8
		EXTI3_IRQHandler,		// EXTI3, address 0x64, exception number 25, IRQ number 9
		EXTI4_IRQHandler,		// EXTI4, address 0x68, exception number 26, IRQ number 10
		DMA1_Stream0_IRQHandler,		// DMA1_Stream0, address 0x6c, exception number 27, IRQ number 11
		DMA1_Stream1_IRQHandler,		// DMA1_Stream1, address 0x70, exception number 28, IRQ number 12
		DMA1_Stream2_IRQHandler,		// DMA1_Stream2, address 0x74, exception number 29, IRQ number 13
		DMA1_Stream3_IRQHandler,		// DMA1_Stream3, address 0x78, exception number 30, IRQ number 14
		DMA1_Stream4_IRQHandler,		// DMA1_Stream4, address 0x7c, exception number 31, IRQ number 15
		DMA1_Stream5_IRQHandler,		// DMA1_Stream5, address 0x80, exception number 32, IRQ number 16
		DMA1_Stream6_IRQHandler,		// DMA1_Stream6, address 0x84, exception number 33, IRQ number 17
		ADC_IRQHandler,		// ADC, address 0x88, exception number 34, IRQ number 18
		CAN1_TX_IRQHandler,		// CAN1_TX, address 0x8c, exception number 35, IRQ number 19
		CAN1_RX0_IRQHandler,		// CAN1_RX0, address 0x90, exception number 36, IRQ number 20
		CAN1_RX1_IRQHandler,		// CAN1_RX1, address 0x94, exception number 37, IRQ number 21
		CAN1_SCE_IRQHandler,		// CAN1_SCE, address 0x98, exception number 38, IRQ number 22
		EXTI9_5_IRQHandler,		// EXTI9_5, address 0x9c, exception number 39, IRQ number 23
		TIM1_BRK_TIM9_IRQHandler,		// TIM1_BRK_TIM9, address 0xa0, exception number 40, IRQ number 24
		TIM1_UP_TIM10_IRQHandler,		// TIM1_UP_TIM10, address 0xa4, exception number 41, IRQ number 25
		TIM1_TRG_COM_TIM11_IRQHandler,		// TIM1_TRG_COM_TIM11, address 0xa8, exception number 42, IRQ number 26
		TIM1_CC_IRQHandler,		// TIM1_CC, address 0xac, exception number 43, IRQ number 27
		TIM2_IRQHandler,		// TIM2, address 0xb0, exception number 44, IRQ number 28
		TIM3_IRQHandler,		// TIM3, address 0xb4, exception number 45, IRQ number 29
		TIM4_IRQHandler,		// TIM4, address 0xb8, exception number 46, IRQ number 30
		I2C1_EV_IRQHandler,		// I2C1_EV, address 0xbc, exception number 47, IRQ number 31
		I2C1_ER_IRQHandler,		// I2C1_ER, address 0xc0, exception number 48, IRQ number 32
		I2C2_EV_IRQHandler,		// I2C2_EV, address 0xc4, exception number 49, IRQ number 33
		I2C2_ER_IRQHandler,		// I2C2_ER, address 0xc8, exception number 50, IRQ number 34
		SPI1_IRQHandler,		// SPI1, address 0xcc, exception number 51, IRQ number 35
		SPI2_IRQHandler,		// SPI2, address 0xd0, exception number 52, IRQ number 36
		USART1_IRQHandler,		// USART1, address 0xd4, exception number 53, IRQ number 37
		USART2_IRQHandler,		// USART2, address 0xd8, exception number 54, IRQ number 38
		USART3_IRQHandler,		// USART3, address 0xdc, exception number 55, IRQ number 39
		EXTI15_10_IRQHandler,		// EXTI15_10, address 0xe0, exception number 56, IRQ number 40
		RTC_Alarm_IRQHandler,		// RTC_Alarm, address 0xe4, exception number 57, IRQ number 41
		OTG_FS_WKUP_IRQHandler,		// OTG_FS_WKUP, address 0xe8, exception number 58, IRQ number 42
		TIM8_BRK_TIM12_IRQHandler,		// TIM8_BRK_TIM12, address 0xec, exception number 59, IRQ number 43
		TIM8_UP_TIM13_IRQHandler,		// TIM8_UP_TIM13, address 0xf0, exception number 60, IRQ number 44
		TIM8_TRG_COM_TIM14_IRQHandler,		// TIM8_TRG_COM_TIM14, address 0xf4, exception number 61, IRQ number 45
		TIM8_CC_IRQHandler,		// TIM8_CC, address 0xf8, exception number 62, IRQ number 46
		DMA1_Stream7_IRQHandler,		// DMA1_Stream7, address 0xfc, exception number 63, IRQ number 47
		FMC_IRQHandler,		// FMC, address 0x100, exception number 64, IRQ number 48
		SDIO_IRQHandler,		// SDIO, address 0x104, exception number 65, IRQ number 49
		TIM5_IRQHandler,		// TIM5, address 0x108, exception number 66, IRQ number 50
		SPI3_IRQHandler,		// SPI3, address 0x10c, exception number 67, IRQ number 51
		UART4_IRQHandler,		// UART4, address 0x110, exception number 68, IRQ number 52
		UART5_IRQHandler,		// UART5, address 0x114, exception number 69, IRQ number 53
		TIM6_DAC_IRQHandler,		// TIM6_DAC, address 0x118, exception number 70, IRQ number 54
		TIM7_IRQHandler,		// TIM7, address 0x11c, exception number 71, IRQ number 55
		DMA2_Stream0_IRQHandler,		// DMA2_Stream0, address 0x120, exception number 72, IRQ number 56
		DMA2_Stream1_IRQHandler,		// DMA2_Stream1, address 0x124, exception number 73, IRQ number 57
		DMA2_Stream2_IRQHandler,		// DMA2_Stream2, address 0x128, exception number 74, IRQ number 58
		DMA2_Stream3_IRQHandler,		// DMA2_Stream3, address 0x12c, exception number 75, IRQ number 59
		DMA2_Stream4_IRQHandler,		// DMA2_Stream4, address 0x130, exception number 76, IRQ number 60
		nullptr,		// Reserved, address 0x134, exception number 77, IRQ number 61
		nullptr,		// Reserved, address 0x138, exception number 78, IRQ number 62
		CAN2_TX_IRQHandler,		// CAN2_TX, address 0x13c, exception number 79, IRQ number 63
		CAN2_RX0_IRQHandler,		// CAN2_RX0, address 0x140, exception number 80, IRQ number 64
		CAN2_RX1_IRQHandler,		// CAN2_RX1, address 0x144, exception number 81, IRQ number 65
		CAN2_SCE_IRQHandler,		// CAN2_SCE, address 0x148, exception number 82, IRQ number 66
		OTG_FS_IRQHandler,		// OTG_FS, address 0x14c, exception number 83, IRQ number 67
		DMA2_Stream5_IRQHandler,		// DMA2_Stream5, address 0x150, exception number 84, IRQ number 68
		DMA2_Stream6_IRQHandler,		// DMA2_Stream6, address 0x154, exception number 85, IRQ number 69
		DMA2_Stream7_IRQHandler,		// DMA2_Stream7, address 0x158, exception number 86, IRQ number 70
		USART6_IRQHandler,		// USART6, address 0x15c, exception number 87, IRQ number 71
		I2C3_EV_IRQHandler,		// I2C3_EV, address 0x160, exception number 88, IRQ number 72
		I2C3_ER_IRQHandler,		// I2C3_ER, address 0x164, exception number 89, IRQ number 73
		OTG_HS_EP1_OUT_IRQHandler,		// OTG_HS_EP1_OUT, address 0x168, exception number 90, IRQ number 74
		OTG_HS_EP1_IN_IRQHandler,		// OTG_HS_EP1_IN, address 0x16c, exception number 91, IRQ number 75
		OTG_HS_WKUP_IRQHandler,		// OTG_HS_WKUP, address 0x170, exception number 92, IRQ number 76
		OTG_HS_IRQHandler,		// OTG_HS, address 0x174, exception number 93, IRQ number 77
		DCMI_IRQHandler,		// DCMI, address 0x178, exception number 94, IRQ number 78
		nullptr,		// Reserved, address 0x17c, exception number 95, IRQ number 79
		nullptr,		// Reserved, address 0x180, exception number 96, IRQ number 80
		FPU_IRQHandler,		// FPU, address 0x184, exception number 97, IRQ number 81
		nullptr,		// Reserved, address 0x188, exception number 98, IRQ number 82
		nullptr,		// Reserved, address 0x18c, exception number 99, IRQ number 83
		SPI4_IRQHandler,		// SPI4, address 0x190, exception number 100, IRQ number 84
		nullptr,		// Reserved, address 0x194, exception number 101, IRQ number 85
		nullptr,		// Reserved, address 0x198, exception number 102, IRQ number 86
		SAI1_IRQHandler,		// SAI1, address 0x19c, exception number 103, IRQ number 87
		nullptr,		// Reserved, address 0x1a0, exception number 104, IRQ number 88
		nullptr,		// Reserved, address 0x1a4, exception number 105, IRQ number 89
		nullptr,		// Reserved, address 0x1a8, exception number 106, IRQ number 90
		SAI2_IRQHandler,		// SAI2, address 0x1ac, exception number 107, IRQ number 91
		QUADSPI_IRQHandler,		// QUADSPI, address 0x1b0, exception number 108, IRQ number 92
		CEC_IRQHandler,		// CEC, address 0x1b4, exception number 109, IRQ number 93
		SPDIF_RX_IRQHandler,		// SPDIF_RX, address 0x1b8, exception number 110, IRQ number 94
		FMPI2C1_EV_IRQHandler,		// FMPI2C1_EV, address 0x1bc, exception number 111, IRQ number 95
		FMPI2C1_ER_IRQHandler,		// FMPI2C1_ER, address 0x1c0, exception number 112, IRQ number 96
};
