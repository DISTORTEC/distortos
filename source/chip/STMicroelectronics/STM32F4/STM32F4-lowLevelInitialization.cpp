/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F4
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-12
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/architecture/configureSysTick.hpp"

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
	constexpr uint32_t period {CONFIG_TICK_CLOCK / CONFIG_TICK_RATE_HZ};
	constexpr uint32_t periodDividedBy8 {period / 8};
	constexpr bool divideBy8 {period > architecture::maxSysTickPeriod};
	// at least one of the periods must be valid
	static_assert(period <= architecture::maxSysTickPeriod || periodDividedBy8 <= architecture::maxSysTickPeriod,
			"Invalid SysTick configuration!");
	architecture::configureSysTick(divideBy8 == false ? period : periodDividedBy8, divideBy8);
}

}	// namespace chip

}	// namespace distortos
