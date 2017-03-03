/**
 * \file
 * \brief Declaration of buttons for STM32F4DISCOVERY
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_BOARD_STM32_STM32F4_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
#define SOURCE_BOARD_STM32_STM32F4_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_

#include "distortos/chip/STM32-GPIOv2.hpp"

#include <array>

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

/*---------------------------------------------------------------------------------------------------------------------+
| button indexes
+---------------------------------------------------------------------------------------------------------------------*/

enum ButtonIndexes
{
#if defined(CONFIG_CHIP_STM32_GPIOV2_GPIOA_ENABLE)
		/// index of B1 button
		b1ButtonIndex,
#endif // def CONFIG_CHIP_STM32_GPIOV2_GPIOA_ENABLE
		/// total number of buttons on the board
		totalButtons
};

/*---------------------------------------------------------------------------------------------------------------------+
| indexed access to pin identifiers
+---------------------------------------------------------------------------------------------------------------------*/

/// array with pin identifiers of all buttons
constexpr std::array<chip::Pin, totalButtons> buttonPins
{
		chip::Pin::pa0,
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

#endif	// SOURCE_BOARD_STM32_STM32F4_STM32F4DISCOVERY_INCLUDE_DISTORTOS_BOARD_BUTTONS_HPP_
