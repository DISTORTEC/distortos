/**
 * \file
 * \brief Declaration of buttons for 32F746GDISCOVERY
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \warning
 * Automatically generated file - do not edit!
 *
 * \date 2017-03-15 17:37:30
 */

#ifndef SOURCE_BOARD_STM32_STM32F7_32F746GDISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
#define SOURCE_BOARD_STM32_STM32F7_32F746GDISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstddef>

/// indicates whether B1 button is enabled (1) or not (0)
#ifdef CONFIG_CHIP_STM32_GPIOV2_GPIOI_ENABLE
#define DISTORTOS_BOARD_B1_BUTTON_ENABLE	1
#else	// !def CONFIG_CHIP_STM32_GPIOV2_GPIOI_ENABLE
#define DISTORTOS_BOARD_B1_BUTTON_ENABLE	0
#endif	// !def CONFIG_CHIP_STM32_GPIOV2_GPIOI_ENABLE

/// total number of buttons on the board
#define DISTORTOS_BOARD_TOTAL_BUTTONS		( \
		DISTORTOS_BOARD_B1_BUTTON_ENABLE + \
		0)

namespace distortos
{

#ifdef CONFIG_BOARD_BUTTONS_ENABLE

namespace chip
{

class ChipInputPin;

}	// namespace chip

#endif	// def CONFIG_BOARD_BUTTONS_ENABLE

namespace board
{

/// total number of buttons on the board
constexpr size_t totalButtons {DISTORTOS_BOARD_TOTAL_BUTTONS};

/*---------------------------------------------------------------------------------------------------------------------+
| button indexes
+---------------------------------------------------------------------------------------------------------------------*/

enum
{
#if DISTORTOS_BOARD_B1_BUTTON_ENABLE == 1
		/// index of B1 button
		b1ButtonIndex,
#endif	// DISTORTOS_BOARD_B1_BUTTON_ENABLE == 1
};

#ifdef CONFIG_BOARD_BUTTONS_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to button objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with all button objects
extern const chip::ChipInputPin buttons[totalButtons];

#endif	// def CONFIG_BOARD_BUTTONS_ENABLE

}	// namespace board

}	// namespace distortos

#endif	// SOURCE_BOARD_STM32_STM32F7_32F746GDISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
