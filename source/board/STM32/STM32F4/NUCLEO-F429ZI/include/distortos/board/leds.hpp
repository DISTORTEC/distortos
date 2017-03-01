/**
 * \file
 * \brief Declaration of LEDs for NUCLEO-F429ZI
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_BOARD_STM32_STM32F4_NUCLEO_F429ZI_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
#define SOURCE_BOARD_STM32_STM32F4_NUCLEO_F429ZI_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

namespace distortos
{

#ifdef CONFIG_BOARD_LEDS_ENABLE

namespace chip
{

class ChipOutputPin;

}	// namespace chip

#endif	// def CONFIG_BOARD_LEDS_ENABLE

namespace board
{

/*---------------------------------------------------------------------------------------------------------------------+
| LED indexes
+---------------------------------------------------------------------------------------------------------------------*/

enum LedIndexes
{
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
		/// index of LD1 LED (green)
		ld1LedIndex,
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
		/// index of LD2 LED (blue)
		ld2LedIndex,
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
		/// index of LD3 LED (red)
		ld3LedIndex,
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
		/// total number of LEDs on the board
		totalLeds
};

#ifdef CONFIG_BOARD_LEDS_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| alternative (color-based) LED indexes
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
/// alternative index of LD1 LED (green)
constexpr size_t greenLedIndex {ld1LedIndex};
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE

#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
/// alternative index of LD2 LED (blue)
constexpr size_t blueLedIndex {ld2LedIndex};
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE

#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE
/// alternative index of LD3 LED (red)
constexpr size_t redLedIndex {ld3LedIndex};
#endif	// def CONFIG_CHIP_STM32_GPIOV2_GPIOB_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to LED objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all LED objects
extern chip::ChipOutputPin leds[totalLeds];

#endif	// def CONFIG_BOARD_LEDS_ENABLE

}	// namespace board

}	// namespace distortos

#endif	// SOURCE_BOARD_STM32_STM32F4_NUCLEO_F429ZI_INCLUDE_DISTORTOS_BOARD_LEDS_HPP_
