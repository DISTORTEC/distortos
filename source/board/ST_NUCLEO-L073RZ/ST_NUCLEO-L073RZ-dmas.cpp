/**
 * \file
 * \brief Definitions of low-level DMA channel drivers for DMAv1 in ST,NUCLEO-L073RZ (ST,STM32L073RZ chip)
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 */

#include "distortos/chip/dmas.hpp"

#include "distortos/chip/DmaChannel.hpp"
#include "distortos/chip/STM32-DMAv1-DmaChannelPeripheral.hpp"
#include "distortos/chip/STM32-DMAv1-DmaPeripheral.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

namespace distortos
{

namespace chip
{

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

/**
 * \brief Low-level chip initializer for DMA1
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void dma1LowLevelInitializer()
{
#if defined(RCC_AHB1ENR_DMA1EN)
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
#elif defined(RCC_AHBENR_DMA1EN)
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
#else
	#error "Unsupported bus for DMA1!"
#endif
}

BIND_LOW_LEVEL_INITIALIZER(50, dma1LowLevelInitializer);

/// raw DMA1 peripheral
const DmaPeripheral dma1Peripheral {DMA1_BASE};

}	// namespace

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL1_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1 channel 1
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

#ifndef DMA1_Channel1_BASE
#error "DMA1 channel 1 not present!"
#endif	// !def DMA1_Channel1_BASE

/// raw DMA1 channel 1 peripheral
const DmaChannelPeripheral dma1Channel1Peripheral {DMA1_BASE, 1};

}	// namespace

DmaChannel dma1Channel1 {dma1Peripheral, dma1Channel1Peripheral};

/**
 * \brief DMA1_Channel1 interrupt handler
 */

extern "C" void DMA1_Channel1_IRQHandler()
{
	dma1Channel1.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL1_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL2_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1 channel 2
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

#ifndef DMA1_Channel2_BASE
#error "DMA1 channel 2 not present!"
#endif	// !def DMA1_Channel2_BASE

/// raw DMA1 channel 2 peripheral
const DmaChannelPeripheral dma1Channel2Peripheral {DMA1_BASE, 2};

}	// namespace

DmaChannel dma1Channel2 {dma1Peripheral, dma1Channel2Peripheral};

/**
 * \brief DMA1_Channel2 interrupt handler
 */

extern "C" void DMA1_Channel2_IRQHandler()
{
	dma1Channel2.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL2_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL3_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1 channel 3
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

#ifndef DMA1_Channel3_BASE
#error "DMA1 channel 3 not present!"
#endif	// !def DMA1_Channel3_BASE

/// raw DMA1 channel 3 peripheral
const DmaChannelPeripheral dma1Channel3Peripheral {DMA1_BASE, 3};

}	// namespace

DmaChannel dma1Channel3 {dma1Peripheral, dma1Channel3Peripheral};

/**
 * \brief DMA1_Channel3 interrupt handler
 */

extern "C" void DMA1_Channel3_IRQHandler()
{
	dma1Channel3.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL3_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL4_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1 channel 4
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

#ifndef DMA1_Channel4_BASE
#error "DMA1 channel 4 not present!"
#endif	// !def DMA1_Channel4_BASE

/// raw DMA1 channel 4 peripheral
const DmaChannelPeripheral dma1Channel4Peripheral {DMA1_BASE, 4};

}	// namespace

DmaChannel dma1Channel4 {dma1Peripheral, dma1Channel4Peripheral};

/**
 * \brief DMA1_Channel4 interrupt handler
 */

extern "C" void DMA1_Channel4_IRQHandler()
{
	dma1Channel4.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL4_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL5_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1 channel 5
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

#ifndef DMA1_Channel5_BASE
#error "DMA1 channel 5 not present!"
#endif	// !def DMA1_Channel5_BASE

/// raw DMA1 channel 5 peripheral
const DmaChannelPeripheral dma1Channel5Peripheral {DMA1_BASE, 5};

}	// namespace

DmaChannel dma1Channel5 {dma1Peripheral, dma1Channel5Peripheral};

/**
 * \brief DMA1_Channel5 interrupt handler
 */

extern "C" void DMA1_Channel5_IRQHandler()
{
	dma1Channel5.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL5_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL6_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1 channel 6
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

#ifndef DMA1_Channel6_BASE
#error "DMA1 channel 6 not present!"
#endif	// !def DMA1_Channel6_BASE

/// raw DMA1 channel 6 peripheral
const DmaChannelPeripheral dma1Channel6Peripheral {DMA1_BASE, 6};

}	// namespace

DmaChannel dma1Channel6 {dma1Peripheral, dma1Channel6Peripheral};

/**
 * \brief DMA1_Channel6 interrupt handler
 */

extern "C" void DMA1_Channel6_IRQHandler()
{
	dma1Channel6.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL6_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL7_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| DMA1 channel 7
+---------------------------------------------------------------------------------------------------------------------*/

namespace
{

#ifndef DMA1_Channel7_BASE
#error "DMA1 channel 7 not present!"
#endif	// !def DMA1_Channel7_BASE

/// raw DMA1 channel 7 peripheral
const DmaChannelPeripheral dma1Channel7Peripheral {DMA1_BASE, 7};

}	// namespace

DmaChannel dma1Channel7 {dma1Peripheral, dma1Channel7Peripheral};

/**
 * \brief DMA1_Channel7 interrupt handler
 */

extern "C" void DMA1_Channel7_IRQHandler()
{
	dma1Channel7.interruptHandler();
}

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL7_ENABLE

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_ENABLE

}	// namespace chip

}	// namespace distortos
