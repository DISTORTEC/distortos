/**
 * \file
 * \brief STM32L47[56] and STM32L4[89A] chip vector table and default weak handlers
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if defined(CONFIG_CHIP_STM32L475) || defined(CONFIG_CHIP_STM32L476) || defined(CONFIG_CHIP_STM32L48) || \
		defined(CONFIG_CHIP_STM32L49) || defined(CONFIG_CHIP_STM32L4A)

#include <cstddef>

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| default weak handlers
+---------------------------------------------------------------------------------------------------------------------*/

// 0x040, 0: Window Watchdog
__attribute__ ((weak)) void WWDG_IRQHandler()
{
	while (1);
}

// 0x044, 1: PVD / PVM[1-4] through EXTI lines 16 / 35 / 36 / 37 / 38
__attribute__ ((weak)) void PVD_PVM_IRQHandler()
{
	while (1);
}

// 0x048, 2: RTC Tamper or TimeStamp /CSS on LSE through EXTI line 19
__attribute__ ((weak)) void RTC_TAMP_STAMP_IRQHandler()
{
	while (1);
}

// 0x04c, 3: RTC Wakeup Timer through EXTI line 20
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

// 0x06c, 11: DMA1 Channel 1
__attribute__ ((weak)) void DMA1_Channel1_IRQHandler()
{
	while (1);
}

// 0x070, 12: DMA1 Channel 2
__attribute__ ((weak)) void DMA1_Channel2_IRQHandler()
{
	while (1);
}

// 0x074, 13: DMA1 Channel 3
__attribute__ ((weak)) void DMA1_Channel3_IRQHandler()
{
	while (1);
}

// 0x078, 14: DMA1 Channel 4
__attribute__ ((weak)) void DMA1_Channel4_IRQHandler()
{
	while (1);
}

// 0x07c, 15: DMA1 Channel 5
__attribute__ ((weak)) void DMA1_Channel5_IRQHandler()
{
	while (1);
}

// 0x080, 16: DMA1 Channel 6
__attribute__ ((weak)) void DMA1_Channel6_IRQHandler()
{
	while (1);
}

// 0x084, 17: DMA1 Channel 7
__attribute__ ((weak)) void DMA1_Channel7_IRQHandler()
{
	while (1);
}

// 0x088, 18: ADC1 and ADC2
__attribute__ ((weak)) void ADC1_2_IRQHandler()
{
	while (1);
}

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

// 0x09c, 23: EXTI Line[9:5]
__attribute__ ((weak)) void EXTI9_5_IRQHandler()
{
	while (1);
}

// 0x0a0, 24: TIM1 Break / TIM15
__attribute__ ((weak)) void TIM1_BRK_TIM15_IRQHandler()
{
	while (1);
}

// 0x0a4, 25: TIM1 Update / TIM16
__attribute__ ((weak)) void TIM1_UP_TIM16_IRQHandler()
{
	while (1);
}

// 0x0a8, 26: TIM1 Trigger and Commutation / TIM17
__attribute__ ((weak)) void TIM1_TRG_COM_TIM17_IRQHandler()
{
	while (1);
}

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

// 0x0e4, 41: RTC Alarms through EXTI line 18
__attribute__ ((weak)) void RTC_Alarm_IRQHandler()
{
	while (1);
}

// 0x0e8, 42: DFSDM1 FLT3
__attribute__ ((weak)) void DFSDM1_FLT3_IRQHandler()
{
	while (1);
}

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

// 0x100, 48: FMC
__attribute__ ((weak)) void FMC_IRQHandler()
{
	while (1);
}

// 0x104, 49: SDMMC1
__attribute__ ((weak)) void SDMMC1_IRQHandler()
{
	while (1);
}

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

// 0x118, 54: TIM6 and DAC1 Underrun
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

// 0x11c, 55: TIM7
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

// 0x120, 56: DMA2 Channel 1
__attribute__ ((weak)) void DMA2_Channel1_IRQHandler()
{
	while (1);
}

// 0x124, 57: DMA2 Channel 2
__attribute__ ((weak)) void DMA2_Channel2_IRQHandler()
{
	while (1);
}

// 0x128, 58: DMA2 Channel 3
__attribute__ ((weak)) void DMA2_Channel3_IRQHandler()
{
	while (1);
}

// 0x12c, 59: DMA2 Channel 4
__attribute__ ((weak)) void DMA2_Channel4_IRQHandler()
{
	while (1);
}

// 0x130, 60: DMA2 Channel 5
__attribute__ ((weak)) void DMA2_Channel5_IRQHandler()
{
	while (1);
}

// 0x134, 61: DFSDM1 FLT0
__attribute__ ((weak)) void DFSDM1_FLT0_IRQHandler()
{
	while (1);
}

// 0x138, 62: DFSDM1 FLT1
__attribute__ ((weak)) void DFSDM1_FLT1_IRQHandler()
{
	while (1);
}

// 0x13c, 63: DFSDM1 FLT2
__attribute__ ((weak)) void DFSDM1_FLT2_IRQHandler()
{
	while (1);
}

// 0x140, 64: COMP1 / COMP2 through EXTI lines 21 / 22
__attribute__ ((weak)) void COMP_IRQHandler()
{
	while (1);
}

// 0x144, 65: LPTIM1
__attribute__ ((weak)) void LPTIM1_IRQHandler()
{
	while (1);
}

// 0x148, 66: LPTIM2
__attribute__ ((weak)) void LPTIM2_IRQHandler()
{
	while (1);
}

// 0x14c, 67: OTG_FS through EXTI line 17
__attribute__ ((weak)) void OTG_FS_IRQHandler()
{
	while (1);
}

// 0x150, 68: DMA2 Channel 6
__attribute__ ((weak)) void DMA2_Channel6_IRQHandler()
{
	while (1);
}

// 0x154, 69: DMA2 Channel 7
__attribute__ ((weak)) void DMA2_Channel7_IRQHandler()
{
	while (1);
}

// 0x158, 70: LPUART1
__attribute__ ((weak)) void LPUART1_IRQHandler()
{
	while (1);
}

// 0x15c, 71: QUADSPI
__attribute__ ((weak)) void QUADSPI_IRQHandler()
{
	while (1);
}

// 0x160, 72: I2C3 Event
__attribute__ ((weak)) void I2C3_EV_IRQHandler()
{
	while (1);
}

// 0x164, 73: I2C3 Error
__attribute__ ((weak)) void I2C3_ER_IRQHandler()
{
	while (1);
}

// 0x168, 74: SAI1
__attribute__ ((weak)) void SAI1_IRQHandler()
{
	while (1);
}

// 0x16c, 75: SAI2
__attribute__ ((weak)) void SAI2_IRQHandler()
{
	while (1);
}

// 0x170, 76: SWPMI1
__attribute__ ((weak)) void SWPMI1_IRQHandler()
{
	while (1);
}

// 0x174, 77: TSC
__attribute__ ((weak)) void TSC_IRQHandler()
{
	while (1);
}

// 0x178, 78: LCD
__attribute__ ((weak)) void LCD_IRQHandler()
{
	while (1);
}

// 0x17c, 79: AES
__attribute__ ((weak)) void AES_IRQHandler()
{
	while (1);
}

// 0x180, 80: RNG
__attribute__ ((weak)) void RNG_IRQHandler()
{
	while (1);
}

// 0x184, 81: FPU
__attribute__ ((weak)) void FPU_IRQHandler()
{
	while (1);
}

#if defined(CONFIG_CHIP_STM32L496) || defined(CONFIG_CHIP_STM32L4A6)

// 0x188, 82: HASH and CRS
__attribute__ ((weak)) void HASH_CRS_IRQHandler()
{
	while (1);
}

// 0x18c, 83: I2C4 Event
__attribute__ ((weak)) void I2C4_EV_IRQHandler()
{
	while (1);
}

// 0x190, 84: I2C4 Error
__attribute__ ((weak)) void I2C4_ER_IRQHandler()
{
	while (1);
}

// 0x194, 85: DCMI
__attribute__ ((weak)) void DCMI_IRQHandler()
{
	while (1);
}

// 0x198, 86: CAN2 TX
__attribute__ ((weak)) void CAN2_TX_IRQHandler()
{
	while (1);
}

// 0x19c, 87: CAN2 RX0
__attribute__ ((weak)) void CAN2_RX0_IRQHandler()
{
	while (1);
}

// 0x1a0, 88: CAN2 RX1
__attribute__ ((weak)) void CAN2_RX1_IRQHandler()
{
	while (1);
}

// 0x1a4, 89: CAN2 SCE
__attribute__ ((weak)) void CAN2_SCE_IRQHandler()
{
	while (1);
}

// 0x1a8, 90: DMA2D
__attribute__ ((weak)) void DMA2D_IRQHandler()
{
	while (1);
}

#endif	// defined(CONFIG_CHIP_STM32L496) || defined(CONFIG_CHIP_STM32L4A6)

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
		WWDG_IRQHandler,					// 0x040, 0: Window Watchdog
		PVD_PVM_IRQHandler,					// 0x044, 1: PVD / PVM[1-4] through EXTI lines 16 / 35 / 36 / 37 / 38
		RTC_TAMP_STAMP_IRQHandler,			// 0x048, 2: RTC Tamper or TimeStamp /CSS on LSE through EXTI line 19
		RTC_WKUP_IRQHandler,				// 0x04c, 3: RTC Wakeup Timer through EXTI line 20
		FLASH_IRQHandler,					// 0x050, 4: FLASH
		RCC_IRQHandler,						// 0x054, 5: RCC
		EXTI0_IRQHandler,					// 0x058, 6: EXTI Line0
		EXTI1_IRQHandler,					// 0x05c, 7: EXTI Line1
		EXTI2_IRQHandler,					// 0x060, 8: EXTI Line2
		EXTI3_IRQHandler,					// 0x064, 9: EXTI Line3
		EXTI4_IRQHandler,					// 0x068, 10: EXTI Line4
		DMA1_Channel1_IRQHandler,			// 0x06c, 11: DMA1 Channel 1
		DMA1_Channel2_IRQHandler,			// 0x070, 12: DMA1 Channel 2
		DMA1_Channel3_IRQHandler,			// 0x074, 13: DMA1 Channel 3
		DMA1_Channel4_IRQHandler,			// 0x078, 14: DMA1 Channel 4
		DMA1_Channel5_IRQHandler,			// 0x07c, 15: DMA1 Channel 5
		DMA1_Channel6_IRQHandler,			// 0x080, 16: DMA1 Channel 6
		DMA1_Channel7_IRQHandler,			// 0x084, 17: DMA1 Channel 7
		ADC1_2_IRQHandler,					// 0x088, 18: ADC1 and ADC2
		CAN1_TX_IRQHandler,					// 0x08c, 19: CAN1 TX
		CAN1_RX0_IRQHandler,				// 0x090, 20: CAN1 RX0
		CAN1_RX1_IRQHandler,				// 0x094, 21: CAN1 RX1
		CAN1_SCE_IRQHandler,				// 0x098, 22: CAN1 SCE
		EXTI9_5_IRQHandler,					// 0x09c, 23: EXTI Line[9:5]
		TIM1_BRK_TIM15_IRQHandler,			// 0x0a0, 24: TIM1 Break / TIM15
		TIM1_UP_TIM16_IRQHandler,			// 0x0a4, 25: TIM1 Update / TIM16
		TIM1_TRG_COM_TIM17_IRQHandler,		// 0x0a8, 26: TIM1 Trigger and Commutation / TIM17
		TIM1_CC_IRQHandler,					// 0x0ac, 27: TIM1 Capture Compare
		TIM2_IRQHandler,					// 0x0b0, 28: TIM2
		TIM3_IRQHandler,					// 0x0b4, 29: TIM3
		TIM4_IRQHandler,					// 0x0b8, 30: TIM4
		I2C1_EV_IRQHandler,					// 0x0bc, 31: I2C1 Event
		I2C1_ER_IRQHandler,					// 0x0c0, 32: I2C1 Error
		I2C2_EV_IRQHandler,					// 0x0c4, 33: I2C2 Event
		I2C2_ER_IRQHandler,					// 0x0c8, 34: I2C2 Error
		SPI1_IRQHandler,					// 0x0cc, 35: SPI1
		SPI2_IRQHandler,					// 0x0d0, 36: SPI2
		USART1_IRQHandler,					// 0x0d4, 37: USART1
		USART2_IRQHandler,					// 0x0d8, 38: USART2
		USART3_IRQHandler,					// 0x0dc, 39: USART3
		EXTI15_10_IRQHandler,				// 0x0e0, 40: EXTI Line[15:10]
		RTC_Alarm_IRQHandler,				// 0x0e4, 41: RTC Alarms through EXTI line 18
		DFSDM1_FLT3_IRQHandler,				// 0x0e8, 42: DFSDM1 FLT3
		TIM8_BRK_IRQHandler,				// 0x0ec, 43: TIM8 Break
		TIM8_UP_IRQHandler,					// 0x0f0, 44: TIM8 Update
		TIM8_TRG_COM_IRQHandler,			// 0x0f4, 45: TIM8 Trigger and Commutation
		TIM8_CC_IRQHandler,					// 0x0f8, 46: TIM8 Capture Compare
		ADC3_IRQHandler,					// 0x0fc, 47: ADC3
		FMC_IRQHandler,						// 0x100, 48: FMC
		SDMMC1_IRQHandler,					// 0x104, 49: SDMMC1
		TIM5_IRQHandler,					// 0x108, 50: TIM5
		SPI3_IRQHandler,					// 0x10c, 51: SPI3
		UART4_IRQHandler,					// 0x110, 52: UART4
		UART5_IRQHandler,					// 0x114, 53: UART5
		TIM6_DAC_IRQHandler,				// 0x118, 54: TIM6 and DAC1 Underrun
		TIM7_IRQHandler,					// 0x11c, 55: TIM7
		DMA2_Channel1_IRQHandler,			// 0x120, 56: DMA2 Channel 1
		DMA2_Channel2_IRQHandler,			// 0x124, 57: DMA2 Channel 2
		DMA2_Channel3_IRQHandler,			// 0x128, 58: DMA2 Channel 3
		DMA2_Channel4_IRQHandler,			// 0x12c, 59: DMA2 Channel 4
		DMA2_Channel5_IRQHandler,			// 0x130, 60: DMA2 Channel 5
		DFSDM1_FLT0_IRQHandler,				// 0x134, 61: DFSDM1 FLT0
		DFSDM1_FLT1_IRQHandler,				// 0x138, 62: DFSDM1 FLT1
		DFSDM1_FLT2_IRQHandler,				// 0x13c, 63: DFSDM1 FLT2
		COMP_IRQHandler,					// 0x140, 64: COMP1 / COMP2 through EXTI lines 21 / 22
		LPTIM1_IRQHandler,					// 0x144, 65: LPTIM1
		LPTIM2_IRQHandler,					// 0x148, 66: LPTIM2
		OTG_FS_IRQHandler,					// 0x14c, 67: OTG_FS through EXTI line 17
		DMA2_Channel6_IRQHandler,			// 0x150, 68: DMA2 Channel 6
		DMA2_Channel7_IRQHandler,			// 0x154, 69: DMA2 Channel 7
		LPUART1_IRQHandler,					// 0x158, 70: LPUART1
		QUADSPI_IRQHandler,					// 0x15c, 71: QUADSPI
		I2C3_EV_IRQHandler,					// 0x160, 72: I2C3 Event
		I2C3_ER_IRQHandler,					// 0x164, 73: I2C3 Error
		SAI1_IRQHandler,					// 0x168, 74: SAI1
		SAI2_IRQHandler,					// 0x16c, 75: SAI2
		SWPMI1_IRQHandler,					// 0x170, 76: SWPMI1
		TSC_IRQHandler,						// 0x174, 77: TSC
		LCD_IRQHandler,						// 0x178, 78: LCD
		AES_IRQHandler,						// 0x17c, 79: AES
		RNG_IRQHandler,						// 0x180, 80: RNG
		FPU_IRQHandler,						// 0x184, 81: FPU

#if defined(CONFIG_CHIP_STM32L496) || defined(CONFIG_CHIP_STM32L4A6)

		HASH_CRS_IRQHandler,				// 0x188, 82: HASH and CRS
		I2C4_EV_IRQHandler,					// 0x18c, 83: I2C4 Event
		I2C4_ER_IRQHandler,					// 0x190, 84: I2C4 Error
		DCMI_IRQHandler,					// 0x194, 85: DCMI
		CAN2_TX_IRQHandler,					// 0x198, 86: CAN2 TX
		CAN2_RX0_IRQHandler,				// 0x19c, 87: CAN2 RX0
		CAN2_RX1_IRQHandler,				// 0x1a0, 88: CAN2 RX1
		CAN2_SCE_IRQHandler,				// 0x1a4, 89: CAN2 SCE
		DMA2D_IRQHandler,					// 0x1a8, 90: DMA2D

#endif	// defined(CONFIG_CHIP_STM32L496) || defined(CONFIG_CHIP_STM32L4A6)

};

namespace
{

/// expected number of chip vectors
constexpr size_t expectedChipVectorsSize
{
#if defined(CONFIG_CHIP_STM32L496) || defined(CONFIG_CHIP_STM32L4A6)
		91
#else
		82
#endif
};

static_assert(sizeof(chipVectors) / sizeof(*chipVectors) == expectedChipVectorsSize, "Invalid size of chipVectors[]!");

}	// namespace

#endif	// defined(CONFIG_CHIP_STM32L475) || defined(CONFIG_CHIP_STM32L476) || defined(CONFIG_CHIP_STM32L48) ||
		// defined(CONFIG_CHIP_STM32L49) || defined(CONFIG_CHIP_STM32L4A)
