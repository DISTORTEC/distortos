/**
 * \file
 * \brief ST,NUCLEO-F103RB (ST,STM32F103RB chip) vector table, default handlers and NVIC low-level initializer
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
#if defined(DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI};
#else	// !defined(DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI)
constexpr uint8_t interruptPriority {};
#endif	// !defined(DISTORTOS_ARCHITECTURE_KERNEL_BASEPRI)

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
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TAMPER_ENABLE
	NVIC_SetPriority(TAMPER_IRQn, interruptPriority);
	NVIC_EnableIRQ(TAMPER_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TAMPER_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RTC_ENABLE
	NVIC_SetPriority(RTC_IRQn, interruptPriority);
	NVIC_EnableIRQ(RTC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RTC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FLASH_ENABLE
	NVIC_SetPriority(FLASH_IRQn, interruptPriority);
	NVIC_EnableIRQ(FLASH_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FLASH_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RCC_ENABLE
	NVIC_SetPriority(RCC_IRQn, interruptPriority);
	NVIC_EnableIRQ(RCC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RCC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI0_ENABLE
	NVIC_SetPriority(EXTI0_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI0_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI0_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI1_ENABLE
	NVIC_SetPriority(EXTI1_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI2_ENABLE
	NVIC_SetPriority(EXTI2_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI3_ENABLE
	NVIC_SetPriority(EXTI3_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI4_ENABLE
	NVIC_SetPriority(EXTI4_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL1_ENABLE
	NVIC_SetPriority(DMA1_Channel1_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL2_ENABLE
	NVIC_SetPriority(DMA1_Channel2_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL3_ENABLE
	NVIC_SetPriority(DMA1_Channel3_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL4_ENABLE
	NVIC_SetPriority(DMA1_Channel4_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL5_ENABLE
	NVIC_SetPriority(DMA1_Channel5_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL6_ENABLE
	NVIC_SetPriority(DMA1_Channel6_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL6_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL7_ENABLE
	NVIC_SetPriority(DMA1_Channel7_IRQn, interruptPriority);
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_DMA1_CHANNEL7_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_ADC1_2_ENABLE
	NVIC_SetPriority(ADC1_2_IRQn, interruptPriority);
	NVIC_EnableIRQ(ADC1_2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_ADC1_2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USB_HP_CAN1_TX_ENABLE
	NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, interruptPriority);
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USB_HP_CAN1_TX_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USB_LP_CAN1_RX0_ENABLE
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, interruptPriority);
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USB_LP_CAN1_RX0_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_CAN1_RX1_ENABLE
	NVIC_SetPriority(CAN1_RX1_IRQn, interruptPriority);
	NVIC_EnableIRQ(CAN1_RX1_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_CAN1_RX1_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_CAN1_SCE_ENABLE
	NVIC_SetPriority(CAN1_SCE_IRQn, interruptPriority);
	NVIC_EnableIRQ(CAN1_SCE_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_CAN1_SCE_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI9_5_ENABLE
	NVIC_SetPriority(EXTI9_5_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI9_5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_BRK_ENABLE
	NVIC_SetPriority(TIM1_BRK_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_BRK_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_BRK_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_UP_ENABLE
	NVIC_SetPriority(TIM1_UP_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_UP_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_UP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_TRG_COM_ENABLE
	NVIC_SetPriority(TIM1_TRG_COM_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_TRG_COM_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM1_CC_ENABLE
	NVIC_SetPriority(TIM1_CC_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM1_CC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM1_CC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM2_ENABLE
	NVIC_SetPriority(TIM2_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM2_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM2_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM3_ENABLE
	NVIC_SetPriority(TIM3_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM4_ENABLE
	NVIC_SetPriority(TIM4_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C1_EV_ENABLE
	NVIC_SetPriority(I2C1_EV_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C1_EV_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C1_EV_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C1_ER_ENABLE
	NVIC_SetPriority(I2C1_ER_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C1_ER_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C2_EV_ENABLE
	NVIC_SetPriority(I2C2_EV_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C2_EV_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C2_EV_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_I2C2_ER_ENABLE
	NVIC_SetPriority(I2C2_ER_IRQn, interruptPriority);
	NVIC_EnableIRQ(I2C2_ER_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_I2C2_ER_ENABLE
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
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USART3_ENABLE
	NVIC_SetPriority(USART3_IRQn, interruptPriority);
	NVIC_EnableIRQ(USART3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USART3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_EXTI15_10_ENABLE
	NVIC_SetPriority(EXTI15_10_IRQn, interruptPriority);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_EXTI15_10_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_RTC_ALARM_ENABLE
	NVIC_SetPriority(RTC_Alarm_IRQn, interruptPriority);
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_RTC_ALARM_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_USBWAKEUP_ENABLE
	NVIC_SetPriority(USBWakeUp_IRQn, interruptPriority);
	NVIC_EnableIRQ(USBWakeUp_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_USBWAKEUP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_BRK_ENABLE
	NVIC_SetPriority(TIM8_BRK_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_BRK_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_BRK_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_UP_ENABLE
	NVIC_SetPriority(TIM8_UP_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_UP_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_UP_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_TRG_COM_ENABLE
	NVIC_SetPriority(TIM8_TRG_COM_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_TRG_COM_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_TRG_COM_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM8_CC_ENABLE
	NVIC_SetPriority(TIM8_CC_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM8_CC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM8_CC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_ADC3_ENABLE
	NVIC_SetPriority(ADC3_IRQn, interruptPriority);
	NVIC_EnableIRQ(ADC3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_ADC3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_FSMC_ENABLE
	NVIC_SetPriority(FSMC_IRQn, interruptPriority);
	NVIC_EnableIRQ(FSMC_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_FSMC_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SDIO_ENABLE
	NVIC_SetPriority(SDIO_IRQn, interruptPriority);
	NVIC_EnableIRQ(SDIO_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SDIO_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM5_ENABLE
	NVIC_SetPriority(TIM5_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_SPI3_ENABLE
	NVIC_SetPriority(SPI3_IRQn, interruptPriority);
	NVIC_EnableIRQ(SPI3_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_SPI3_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_UART4_ENABLE
	NVIC_SetPriority(UART4_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART4_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_UART4_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_UART5_ENABLE
	NVIC_SetPriority(UART5_IRQn, interruptPriority);
	NVIC_EnableIRQ(UART5_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_UART5_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM6_ENABLE
	NVIC_SetPriority(TIM6_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM6_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM6_ENABLE
#ifdef DISTORTOS_ARCHITECTURE_NVIC_TIM7_ENABLE
	NVIC_SetPriority(TIM7_IRQn, interruptPriority);
	NVIC_EnableIRQ(TIM7_IRQn);
#endif	// def DISTORTOS_ARCHITECTURE_NVIC_TIM7_ENABLE
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

// TAMPER, address 0x48, exception number 18, IRQ number 2
__attribute__ ((weak)) void TAMPER_IRQHandler()
{
	while (1);
}

// RTC, address 0x4c, exception number 19, IRQ number 3
__attribute__ ((weak)) void RTC_IRQHandler()
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

// DMA1_Channel1, address 0x6c, exception number 27, IRQ number 11
__attribute__ ((weak)) void DMA1_Channel1_IRQHandler()
{
	while (1);
}

// DMA1_Channel2, address 0x70, exception number 28, IRQ number 12
__attribute__ ((weak)) void DMA1_Channel2_IRQHandler()
{
	while (1);
}

// DMA1_Channel3, address 0x74, exception number 29, IRQ number 13
__attribute__ ((weak)) void DMA1_Channel3_IRQHandler()
{
	while (1);
}

// DMA1_Channel4, address 0x78, exception number 30, IRQ number 14
__attribute__ ((weak)) void DMA1_Channel4_IRQHandler()
{
	while (1);
}

// DMA1_Channel5, address 0x7c, exception number 31, IRQ number 15
__attribute__ ((weak)) void DMA1_Channel5_IRQHandler()
{
	while (1);
}

// DMA1_Channel6, address 0x80, exception number 32, IRQ number 16
__attribute__ ((weak)) void DMA1_Channel6_IRQHandler()
{
	while (1);
}

// DMA1_Channel7, address 0x84, exception number 33, IRQ number 17
__attribute__ ((weak)) void DMA1_Channel7_IRQHandler()
{
	while (1);
}

// ADC1_2, address 0x88, exception number 34, IRQ number 18
__attribute__ ((weak)) void ADC1_2_IRQHandler()
{
	while (1);
}

// USB_HP_CAN1_TX, address 0x8c, exception number 35, IRQ number 19
__attribute__ ((weak)) void USB_HP_CAN1_TX_IRQHandler()
{
	while (1);
}

// USB_LP_CAN1_RX0, address 0x90, exception number 36, IRQ number 20
__attribute__ ((weak)) void USB_LP_CAN1_RX0_IRQHandler()
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

// TIM1_BRK, address 0xa0, exception number 40, IRQ number 24
__attribute__ ((weak)) void TIM1_BRK_IRQHandler()
{
	while (1);
}

// TIM1_UP, address 0xa4, exception number 41, IRQ number 25
__attribute__ ((weak)) void TIM1_UP_IRQHandler()
{
	while (1);
}

// TIM1_TRG_COM, address 0xa8, exception number 42, IRQ number 26
__attribute__ ((weak)) void TIM1_TRG_COM_IRQHandler()
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

// USBWakeUp, address 0xe8, exception number 58, IRQ number 42
__attribute__ ((weak)) void USBWakeUp_IRQHandler()
{
	while (1);
}

// TIM8_BRK, address 0xec, exception number 59, IRQ number 43
__attribute__ ((weak)) void TIM8_BRK_IRQHandler()
{
	while (1);
}

// TIM8_UP, address 0xf0, exception number 60, IRQ number 44
__attribute__ ((weak)) void TIM8_UP_IRQHandler()
{
	while (1);
}

// TIM8_TRG_COM, address 0xf4, exception number 61, IRQ number 45
__attribute__ ((weak)) void TIM8_TRG_COM_IRQHandler()
{
	while (1);
}

// TIM8_CC, address 0xf8, exception number 62, IRQ number 46
__attribute__ ((weak)) void TIM8_CC_IRQHandler()
{
	while (1);
}

// ADC3, address 0xfc, exception number 63, IRQ number 47
__attribute__ ((weak)) void ADC3_IRQHandler()
{
	while (1);
}

// FSMC, address 0x100, exception number 64, IRQ number 48
__attribute__ ((weak)) void FSMC_IRQHandler()
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

// TIM6, address 0x118, exception number 70, IRQ number 54
__attribute__ ((weak)) void TIM6_IRQHandler()
{
	while (1);
}

// TIM7, address 0x11c, exception number 71, IRQ number 55
__attribute__ ((weak)) void TIM7_IRQHandler()
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
		WWDG_IRQHandler,		// WWDG, address 0x40, exception number 16, IRQ number 0
		PVD_IRQHandler,		// PVD, address 0x44, exception number 17, IRQ number 1
		TAMPER_IRQHandler,		// TAMPER, address 0x48, exception number 18, IRQ number 2
		RTC_IRQHandler,		// RTC, address 0x4c, exception number 19, IRQ number 3
		FLASH_IRQHandler,		// FLASH, address 0x50, exception number 20, IRQ number 4
		RCC_IRQHandler,		// RCC, address 0x54, exception number 21, IRQ number 5
		EXTI0_IRQHandler,		// EXTI0, address 0x58, exception number 22, IRQ number 6
		EXTI1_IRQHandler,		// EXTI1, address 0x5c, exception number 23, IRQ number 7
		EXTI2_IRQHandler,		// EXTI2, address 0x60, exception number 24, IRQ number 8
		EXTI3_IRQHandler,		// EXTI3, address 0x64, exception number 25, IRQ number 9
		EXTI4_IRQHandler,		// EXTI4, address 0x68, exception number 26, IRQ number 10
		DMA1_Channel1_IRQHandler,		// DMA1_Channel1, address 0x6c, exception number 27, IRQ number 11
		DMA1_Channel2_IRQHandler,		// DMA1_Channel2, address 0x70, exception number 28, IRQ number 12
		DMA1_Channel3_IRQHandler,		// DMA1_Channel3, address 0x74, exception number 29, IRQ number 13
		DMA1_Channel4_IRQHandler,		// DMA1_Channel4, address 0x78, exception number 30, IRQ number 14
		DMA1_Channel5_IRQHandler,		// DMA1_Channel5, address 0x7c, exception number 31, IRQ number 15
		DMA1_Channel6_IRQHandler,		// DMA1_Channel6, address 0x80, exception number 32, IRQ number 16
		DMA1_Channel7_IRQHandler,		// DMA1_Channel7, address 0x84, exception number 33, IRQ number 17
		ADC1_2_IRQHandler,		// ADC1_2, address 0x88, exception number 34, IRQ number 18
		USB_HP_CAN1_TX_IRQHandler,		// USB_HP_CAN1_TX, address 0x8c, exception number 35, IRQ number 19
		USB_LP_CAN1_RX0_IRQHandler,		// USB_LP_CAN1_RX0, address 0x90, exception number 36, IRQ number 20
		CAN1_RX1_IRQHandler,		// CAN1_RX1, address 0x94, exception number 37, IRQ number 21
		CAN1_SCE_IRQHandler,		// CAN1_SCE, address 0x98, exception number 38, IRQ number 22
		EXTI9_5_IRQHandler,		// EXTI9_5, address 0x9c, exception number 39, IRQ number 23
		TIM1_BRK_IRQHandler,		// TIM1_BRK, address 0xa0, exception number 40, IRQ number 24
		TIM1_UP_IRQHandler,		// TIM1_UP, address 0xa4, exception number 41, IRQ number 25
		TIM1_TRG_COM_IRQHandler,		// TIM1_TRG_COM, address 0xa8, exception number 42, IRQ number 26
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
		USBWakeUp_IRQHandler,		// USBWakeUp, address 0xe8, exception number 58, IRQ number 42
		TIM8_BRK_IRQHandler,		// TIM8_BRK, address 0xec, exception number 59, IRQ number 43
		TIM8_UP_IRQHandler,		// TIM8_UP, address 0xf0, exception number 60, IRQ number 44
		TIM8_TRG_COM_IRQHandler,		// TIM8_TRG_COM, address 0xf4, exception number 61, IRQ number 45
		TIM8_CC_IRQHandler,		// TIM8_CC, address 0xf8, exception number 62, IRQ number 46
		ADC3_IRQHandler,		// ADC3, address 0xfc, exception number 63, IRQ number 47
		FSMC_IRQHandler,		// FSMC, address 0x100, exception number 64, IRQ number 48
		SDIO_IRQHandler,		// SDIO, address 0x104, exception number 65, IRQ number 49
		TIM5_IRQHandler,		// TIM5, address 0x108, exception number 66, IRQ number 50
		SPI3_IRQHandler,		// SPI3, address 0x10c, exception number 67, IRQ number 51
		UART4_IRQHandler,		// UART4, address 0x110, exception number 68, IRQ number 52
		UART5_IRQHandler,		// UART5, address 0x114, exception number 69, IRQ number 53
		TIM6_IRQHandler,		// TIM6, address 0x118, exception number 70, IRQ number 54
		TIM7_IRQHandler,		// TIM7, address 0x11c, exception number 71, IRQ number 55
};

}	// extern "C"
