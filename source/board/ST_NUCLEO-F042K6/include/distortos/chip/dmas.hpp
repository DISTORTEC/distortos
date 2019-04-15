/**
 * \file
 * \brief Declarations of low-level DMA channel drivers for DMAv1 in ST,NUCLEO-F042K6 (ST,STM32F042K6 chip)
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

#ifndef SOURCE_BOARD_ST_NUCLEO_F042K6_INCLUDE_DISTORTOS_CHIP_DMAS_HPP_
#define SOURCE_BOARD_ST_NUCLEO_F042K6_INCLUDE_DISTORTOS_CHIP_DMAS_HPP_

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

class DmaChannel;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL1_ENABLE

/// DMA1 channel 1 low-level driver
extern DmaChannel dma1Channel1;

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL1_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL2_ENABLE

/// DMA1 channel 2 low-level driver
extern DmaChannel dma1Channel2;

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL2_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL3_ENABLE

/// DMA1 channel 3 low-level driver
extern DmaChannel dma1Channel3;

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL3_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL4_ENABLE

/// DMA1 channel 4 low-level driver
extern DmaChannel dma1Channel4;

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL4_ENABLE

#ifdef DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL5_ENABLE

/// DMA1 channel 5 low-level driver
extern DmaChannel dma1Channel5;

#endif	// def DISTORTOS_CHIP_STM32_DMAV1_DMA1_CHANNEL5_ENABLE

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_BOARD_ST_NUCLEO_F042K6_INCLUDE_DISTORTOS_CHIP_DMAS_HPP_
