/**
 * \file
 * \brief STM32F7 chip vector table and default weak handlers
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

#if defined(CONFIG_CHIP_STM32F72) || defined(CONFIG_CHIP_STM32F73)
	#define STM32F72_STM32F73_VECTORS
#elif defined(CONFIG_CHIP_STM32F74) || defined(CONFIG_CHIP_STM32F75)
	#define STM32F74_STM32F75_VECTORS
#elif defined(CONFIG_CHIP_STM32F76) || defined(CONFIG_CHIP_STM32F77)
	#define STM32F76_STM32F77_VECTORS
#else
	#error "Wrong configuration or unsupported STM32F7 chip!"
#endif

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| default weak handlers
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

// 0x09c, 23: External Line[9:5]
__attribute__ ((weak)) void EXTI9_5_IRQHandler()
{
	while (1);
}

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

// 0x0e0, 40: External Line[15:10]
__attribute__ ((weak)) void EXTI15_10_IRQHandler()
{
	while (1);
}

// 0x0e4, 41: RTC Alarm (A and B) through EXTI Line
__attribute__ ((weak)) void RTC_Alarm_IRQHandler()
{
	while (1);
}

// 0x0e8, 42: USB OTG FS Wakeup through EXTI line
__attribute__ ((weak)) void OTG_FS_WKUP_IRQHandler()
{
	while (1);
}

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

// 0x0fc, 47: DMA1 Stream7
__attribute__ ((weak)) void DMA1_Stream7_IRQHandler()
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

// 0x118, 54: TIM6 and DAC1&2 underrun error
__attribute__ ((weak)) void TIM6_DAC_IRQHandler()
{
	while (1);
}

// 0x11c, 55: TIM7
__attribute__ ((weak)) void TIM7_IRQHandler()
{
	while (1);
}

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

#if defined(STM32F72_STM32F73_VECTORS)

// 0x134, 61: Reserved
__attribute__ ((weak)) void Reserved_0x134_Handler()
{
	while (1);
}

// 0x138, 62: Reserved
__attribute__ ((weak)) void Reserved_0x138_Handler()
{
	while (1);
}

// 0x13c, 63: Reserved
__attribute__ ((weak)) void Reserved_0x13c_Handler()
{
	while (1);
}

// 0x140, 64: Reserved
__attribute__ ((weak)) void Reserved_0x140_Handler()
{
	while (1);
}

// 0x144, 65: Reserved
__attribute__ ((weak)) void Reserved_0x144_Handler()
{
	while (1);
}

// 0x148, 66: Reserved
__attribute__ ((weak)) void Reserved_0x148_Handler()
{
	while (1);
}

#else	// !defined(STM32F72_STM32F73_VECTORS)

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

#endif	// !defined(STM32F72_STM32F73_VECTORS)

// 0x14c, 67: USB OTG FS
__attribute__ ((weak)) void OTG_FS_IRQHandler()
{
	while (1);
}

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

// 0x170, 76: USB OTG HS Wakeup through EXTI
__attribute__ ((weak)) void OTG_HS_WKUP_IRQHandler()
{
	while (1);
}

// 0x174, 77: USB OTG HS
__attribute__ ((weak)) void OTG_HS_IRQHandler()
{
	while (1);
}

#if defined(STM32F72_STM32F73_VECTORS)

// 0x178, 78: Reserved
__attribute__ ((weak)) void Reserved_0x178_Handler()
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

#else	// !defined(STM32F72_STM32F73_VECTORS)

// 0x178, 78: DCMI
__attribute__ ((weak)) void DCMI_IRQHandler()
{
	while (1);
}

// 0x17c, 79: Crypto
__attribute__ ((weak)) void CRYP_IRQHandler()
{
	while (1);
}

// 0x180, 80: Hash and Rng
__attribute__ ((weak)) void HASH_RNG_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F72_STM32F73_VECTORS)

// 0x184, 81: FPU
__attribute__ ((weak)) void FPU_IRQHandler()
{
	while (1);
}

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

// 0x190, 84: SPI4
__attribute__ ((weak)) void SPI4_IRQHandler()
{
	while (1);
}

// 0x194, 85: SPI5
__attribute__ ((weak)) void SPI5_IRQHandler()
{
	while (1);
}

#if defined(STM32F72_STM32F73_VECTORS)

// 0x198, 86: Reserved
__attribute__ ((weak)) void Reserved_0x198_Handler()
{
	while (1);
}

#else	// !defined(STM32F72_STM32F73_VECTORS)

// 0x198, 86: SPI6
__attribute__ ((weak)) void SPI6_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F72_STM32F73_VECTORS)

// 0x19c, 87: SAI1
__attribute__ ((weak)) void SAI1_IRQHandler()
{
	while (1);
}

#if defined(STM32F72_STM32F73_VECTORS)

// 0x1a0, 88: Reserved
__attribute__ ((weak)) void Reserved_0x1a0_Handler()
{
	while (1);
}

// 0x1a4, 89: Reserved
__attribute__ ((weak)) void Reserved_0x1a4_Handler()
{
	while (1);
}

// 0x1a8, 90: Reserved
__attribute__ ((weak)) void Reserved_0x1a8_Handler()
{
	while (1);
}

#else	// !defined(STM32F72_STM32F73_VECTORS)

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

#endif	// !defined(STM32F72_STM32F73_VECTORS)

// 0x1ac, 91: SAI2
__attribute__ ((weak)) void SAI2_IRQHandler()
{
	while (1);
}

// 0x1b0, 92: QUADSPI
__attribute__ ((weak)) void QUADSPI_IRQHandler()
{
	while (1);
}

// 0x1b4, 93: LPTIM1
__attribute__ ((weak)) void LPTIM1_IRQHandler()
{
	while (1);
}

#if defined(STM32F72_STM32F73_VECTORS)

// 0x1b8, 94: Reserved
__attribute__ ((weak)) void Reserved_0x1b8_Handler()
{
	while (1);
}

// 0x1bc, 95: Reserved
__attribute__ ((weak)) void Reserved_0x1bc_Handler()
{
	while (1);
}

// 0x1c0, 96: Reserved
__attribute__ ((weak)) void Reserved_0x1c0_Handler()
{
	while (1);
}

// 0x1c4, 97: Reserved
__attribute__ ((weak)) void Reserved_0x1c4_Handler()
{
	while (1);
}

#else	// !defined(STM32F72_STM32F73_VECTORS)

// 0x1b8, 94: HDMI_CEC
__attribute__ ((weak)) void CEC_IRQHandler()
{
	while (1);
}

// 0x1bc, 95: I2C4 Event
__attribute__ ((weak)) void I2C4_EV_IRQHandler()
{
	while (1);
}

// 0x1c0, 96: I2C4 Error
__attribute__ ((weak)) void I2C4_ER_IRQHandler()
{
	while (1);
}

// 0x1c4, 97: SPDIF_RX
__attribute__ ((weak)) void SPDIF_RX_IRQHandler()
{
	while (1);
}

#endif	// !defined(STM32F72_STM32F73_VECTORS)

#if defined(STM32F72_STM32F73_VECTORS)

// 0x1c8, 98: Reserved
__attribute__ ((weak)) void Reserved_0x1c8_Handler()
{
	while (1);
}

// 0x1cc, 99: Reserved
__attribute__ ((weak)) void Reserved_0x1cc_Handler()
{
	while (1);
}

// 0x1d0, 100: Reserved
__attribute__ ((weak)) void Reserved_0x1d0_Handler()
{
	while (1);
}

// 0x1d4, 101: Reserved
__attribute__ ((weak)) void Reserved_0x1d4_Handler()
{
	while (1);
}

// 0x1d8, 102: Reserved
__attribute__ ((weak)) void Reserved_0x1d8_Handler()
{
	while (1);
}

#elif defined(STM32F76_STM32F77_VECTORS)

// 0x1c8, 98: DSI
__attribute__ ((weak)) void DSI_IRQHandler()
{
	while (1);
}

// 0x1cc, 99: DFSDM1 Filter 0
__attribute__ ((weak)) void DFSDM1_FLT0_IRQHandler()
{
	while (1);
}

// 0x1d0, 100: DFSDM1 Filter 1
__attribute__ ((weak)) void DFSDM1_FLT1_IRQHandler()
{
	while (1);
}

// 0x1d4, 101: DFSDM1 Filter 2
__attribute__ ((weak)) void DFSDM1_FLT2_IRQHandler()
{
	while (1);
}

// 0x1d8, 102: DFSDM1 Filter 3
__attribute__ ((weak)) void DFSDM1_FLT3_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F76_STM32F77_VECTORS)

#if defined(STM32F72_STM32F73_VECTORS) || defined(STM32F76_STM32F77_VECTORS)

// 0x1dc, 103: SDMMC2
__attribute__ ((weak)) void SDMMC2_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F72_STM32F73_VECTORS) || defined(STM32F76_STM32F77_VECTORS)

#if defined(STM32F76_STM32F77_VECTORS)

// 0x1e0, 104: CAN3 TX
__attribute__ ((weak)) void CAN3_TX_IRQHandler()
{
	while (1);
}

// 0x1e4, 105: CAN3 RX0
__attribute__ ((weak)) void CAN3_RX0_IRQHandler()
{
	while (1);
}

// 0x1e8, 106: CAN3 RX1
__attribute__ ((weak)) void CAN3_RX1_IRQHandler()
{
	while (1);
}

// 0x1ec, 107: CAN3 SCE
__attribute__ ((weak)) void CAN3_SCE_IRQHandler()
{
	while (1);
}

// 0x1f0, 108: JPEG
__attribute__ ((weak)) void JPEG_IRQHandler()
{
	while (1);
}

// 0x1f4, 109: MDIOS
__attribute__ ((weak)) void MDIOS_IRQHandler()
{
	while (1);
}

#endif	// defined(STM32F76_STM32F77_VECTORS)

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
		EXTI0_IRQHandler,					// 0x058, 6: EXTI Line0
		EXTI1_IRQHandler,					// 0x05c, 7: EXTI Line1
		EXTI2_IRQHandler,					// 0x060, 8: EXTI Line2
		EXTI3_IRQHandler,					// 0x064, 9: EXTI Line3
		EXTI4_IRQHandler,					// 0x068, 10: EXTI Line4
		DMA1_Stream0_IRQHandler,			// 0x06c, 11: DMA1 Stream 0
		DMA1_Stream1_IRQHandler,			// 0x070, 12: DMA1 Stream 1
		DMA1_Stream2_IRQHandler,			// 0x074, 13: DMA1 Stream 2
		DMA1_Stream3_IRQHandler,			// 0x078, 14: DMA1 Stream 3
		DMA1_Stream4_IRQHandler,			// 0x07c, 15: DMA1 Stream 4
		DMA1_Stream5_IRQHandler,			// 0x080, 16: DMA1 Stream 5
		DMA1_Stream6_IRQHandler,			// 0x084, 17: DMA1 Stream 6
		ADC_IRQHandler,						// 0x088, 18: ADC1, ADC2 and ADC3
		CAN1_TX_IRQHandler,					// 0x08c, 19: CAN1 TX
		CAN1_RX0_IRQHandler,				// 0x090, 20: CAN1 RX0
		CAN1_RX1_IRQHandler,				// 0x094, 21: CAN1 RX1
		CAN1_SCE_IRQHandler,				// 0x098, 22: CAN1 SCE
		EXTI9_5_IRQHandler,					// 0x09c, 23: External Line[9:5]
		TIM1_BRK_TIM9_IRQHandler,			// 0x0a0, 24: TIM1 Break and TIM9
		TIM1_UP_TIM10_IRQHandler,			// 0x0a4, 25: TIM1 Update and TIM10
		TIM1_TRG_COM_TIM11_IRQHandler,		// 0x0a8, 26: TIM1 Trigger and Commutation and TIM11
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
		EXTI15_10_IRQHandler,				// 0x0e0, 40: External Line[15:10]
		RTC_Alarm_IRQHandler,				// 0x0e4, 41: RTC Alarm (A and B) through EXTI Line
		OTG_FS_WKUP_IRQHandler,				// 0x0e8, 42: USB OTG FS Wakeup through EXTI line
		TIM8_BRK_TIM12_IRQHandler,			// 0x0ec, 43: TIM8 Break and TIM12
		TIM8_UP_TIM13_IRQHandler,			// 0x0f0, 44: TIM8 Update and TIM13
		TIM8_TRG_COM_TIM14_IRQHandler,		// 0x0f4, 45: TIM8 Trigger and Commutation and TIM14
		TIM8_CC_IRQHandler,					// 0x0f8, 46: TIM8 Capture Compare
		DMA1_Stream7_IRQHandler,			// 0x0fc, 47: DMA1 Stream7
		FMC_IRQHandler,						// 0x100, 48: FMC
		SDMMC1_IRQHandler,					// 0x104, 49: SDMMC1
		TIM5_IRQHandler,					// 0x108, 50: TIM5
		SPI3_IRQHandler,					// 0x10c, 51: SPI3
		UART4_IRQHandler,					// 0x110, 52: UART4
		UART5_IRQHandler,					// 0x114, 53: UART5
		TIM6_DAC_IRQHandler,				// 0x118, 54: TIM6 and DAC1&2 underrun error
		TIM7_IRQHandler,					// 0x11c, 55: TIM7
		DMA2_Stream0_IRQHandler,			// 0x120, 56: DMA2 Stream 0
		DMA2_Stream1_IRQHandler,			// 0x124, 57: DMA2 Stream 1
		DMA2_Stream2_IRQHandler,			// 0x128, 58: DMA2 Stream 2
		DMA2_Stream3_IRQHandler,			// 0x12c, 59: DMA2 Stream 3
		DMA2_Stream4_IRQHandler,			// 0x130, 60: DMA2 Stream 4

#if defined(STM32F72_STM32F73_VECTORS)

		Reserved_0x134_Handler,				// 0x134, 61: Reserved
		Reserved_0x138_Handler,				// 0x138, 62: Reserved
		Reserved_0x13c_Handler,				// 0x13c, 63: Reserved
		Reserved_0x140_Handler,				// 0x140, 64: Reserved
		Reserved_0x144_Handler,				// 0x144, 65: Reserved
		Reserved_0x148_Handler,				// 0x148, 66: Reserved

#else	// !defined(STM32F72_STM32F73_VECTORS)

		ETH_IRQHandler,						// 0x134, 61: Ethernet
		ETH_WKUP_IRQHandler,				// 0x138, 62: Ethernet Wakeup through EXTI line
		CAN2_TX_IRQHandler,					// 0x13c, 63: CAN2 TX
		CAN2_RX0_IRQHandler,				// 0x140, 64: CAN2 RX0
		CAN2_RX1_IRQHandler,				// 0x144, 65: CAN2 RX1
		CAN2_SCE_IRQHandler,				// 0x148, 66: CAN2 SCE

#endif	// !defined(STM32F72_STM32F73_VECTORS)

		OTG_FS_IRQHandler,					// 0x14c, 67: USB OTG FS
		DMA2_Stream5_IRQHandler,			// 0x150, 68: DMA2 Stream 5
		DMA2_Stream6_IRQHandler,			// 0x154, 69: DMA2 Stream 6
		DMA2_Stream7_IRQHandler,			// 0x158, 70: DMA2 Stream 7
		USART6_IRQHandler,					// 0x15c, 71: USART6
		I2C3_EV_IRQHandler,					// 0x160, 72: I2C3 event
		I2C3_ER_IRQHandler,					// 0x164, 73: I2C3 error
		OTG_HS_EP1_OUT_IRQHandler,			// 0x168, 74: USB OTG HS End Point 1 Out
		OTG_HS_EP1_IN_IRQHandler,			// 0x16c, 75: USB OTG HS End Point 1 In
		OTG_HS_WKUP_IRQHandler,				// 0x170, 76: USB OTG HS Wakeup through EXTI
		OTG_HS_IRQHandler,					// 0x174, 77: USB OTG HS

#if defined(STM32F72_STM32F73_VECTORS)

		Reserved_0x178_Handler,				// 0x178, 78: Reserved
		AES_IRQHandler,						// 0x17c, 79: AES
		RNG_IRQHandler,						// 0x180, 80: RNG

#else	// !defined(STM32F72_STM32F73_VECTORS)

		DCMI_IRQHandler,					// 0x178, 78: DCMI
		CRYP_IRQHandler,					// 0x17c, 79: Crypto
		HASH_RNG_IRQHandler,				// 0x180, 80: Hash and Rng

#endif	// !defined(STM32F72_STM32F73_VECTORS)

		FPU_IRQHandler,						// 0x184, 81: FPU
		UART7_IRQHandler,					// 0x188, 82: UART7
		UART8_IRQHandler,					// 0x18c, 83: UART8
		SPI4_IRQHandler,					// 0x190, 84: SPI4
		SPI5_IRQHandler,					// 0x194, 85: SPI5

#if defined(STM32F72_STM32F73_VECTORS)

		Reserved_0x198_Handler,				// 0x198, 86: Reserved

#else	// !defined(STM32F72_STM32F73_VECTORS)

		SPI6_IRQHandler,					// 0x198, 86: SPI6

#endif	// !defined(STM32F72_STM32F73_VECTORS)

		SAI1_IRQHandler,					// 0x19c, 87: SAI1

#if defined(STM32F72_STM32F73_VECTORS)

		Reserved_0x1a0_Handler,				// 0x1a0, 88: Reserved
		Reserved_0x1a4_Handler,				// 0x1a4, 89: Reserved
		Reserved_0x1a8_Handler,				// 0x1a8, 90: Reserved

#else	// !defined(STM32F72_STM32F73_VECTORS)

		LTDC_IRQHandler,					// 0x1a0, 88: LTDC
		LTDC_ER_IRQHandler,					// 0x1a4, 89: LTDC error
		DMA2D_IRQHandler,					// 0x1a8, 90: DMA2D

#endif	// !defined(STM32F72_STM32F73_VECTORS)

		SAI2_IRQHandler,					// 0x1ac, 91: SAI2
		QUADSPI_IRQHandler,					// 0x1b0, 92: QUADSPI
		LPTIM1_IRQHandler,					// 0x1b4, 93: LPTIM1

#if defined(STM32F72_STM32F73_VECTORS)

		Reserved_0x1b8_Handler,				// 0x1b8, 94: Reserved
		Reserved_0x1bc_Handler,				// 0x1bc, 95: Reserved
		Reserved_0x1c0_Handler,				// 0x1c0, 96: Reserved
		Reserved_0x1c4_Handler,				// 0x1c4, 97: Reserved

#else	// !defined(STM32F72_STM32F73_VECTORS)

		CEC_IRQHandler,						// 0x1b8, 94: HDMI_CEC
		I2C4_EV_IRQHandler,					// 0x1bc, 95: I2C4 Event
		I2C4_ER_IRQHandler,					// 0x1c0, 96: I2C4 Error
		SPDIF_RX_IRQHandler,				// 0x1c4, 97: SPDIF_RX

#endif	// !defined(STM32F72_STM32F73_VECTORS)

#if defined(STM32F72_STM32F73_VECTORS)

		Reserved_0x1c8_Handler,				// 0x1c8, 98: Reserved
		Reserved_0x1cc_Handler,				// 0x1cc, 99: Reserved
		Reserved_0x1d0_Handler,				// 0x1d0, 100: Reserved
		Reserved_0x1d4_Handler,				// 0x1d4, 101: Reserved
		Reserved_0x1d8_Handler,				// 0x1d8, 102: Reserved

#elif defined(STM32F76_STM32F77_VECTORS)

		DSI_IRQHandler,						// 0x1c8, 98: DSI
		DFSDM1_FLT0_IRQHandler,				// 0x1cc, 99: DFSDM1 Filter 0
		DFSDM1_FLT1_IRQHandler,				// 0x1d0, 100: DFSDM1 Filter 1
		DFSDM1_FLT2_IRQHandler,				// 0x1d4, 101: DFSDM1 Filter 2
		DFSDM1_FLT3_IRQHandler,				// 0x1d8, 102: DFSDM1 Filter 3

#endif	// defined(STM32F76_STM32F77_VECTORS)

#if defined(STM32F72_STM32F73_VECTORS) || defined(STM32F76_STM32F77_VECTORS)

		SDMMC2_IRQHandler,					// 0x1dc, 103: SDMMC2

#endif	// defined(STM32F72_STM32F73_VECTORS) || defined(STM32F76_STM32F77_VECTORS)

#if defined(STM32F76_STM32F77_VECTORS)

		CAN3_TX_IRQHandler,					// 0x1e0, 104: CAN3 TX
		CAN3_RX0_IRQHandler,				// 0x1e4, 105: CAN3 RX0
		CAN3_RX1_IRQHandler,				// 0x1e8, 106: CAN3 RX1
		CAN3_SCE_IRQHandler,				// 0x1ec, 107: CAN3 SCE
		JPEG_IRQHandler,					// 0x1f0, 108: JPEG
		MDIOS_IRQHandler,					// 0x1f4, 109: MDIOS

#endif	// defined(STM32F76_STM32F77_VECTORS)

};

namespace
{

/// expected number of chip vectors
constexpr size_t expectedChipVectorsSize
{
#if defined(STM32F72_STM32F73_VECTORS)
		104
#elif defined(STM32F74_STM32F75_VECTORS)
		98
#elif defined(STM32F76_STM32F77_VECTORS)
		110
#else
#	error "Wrong configuration or unsupported STM32F7 chip!"
#endif
};

static_assert(sizeof(chipVectors) / sizeof(*chipVectors) == expectedChipVectorsSize, "Invalid size of chipVectors[]!");

}	// namespace
