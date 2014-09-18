/**
 * \file
 * \brief SysTick_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-18
 */

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture.hpp"

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief SysTick_Handler() for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * Tick interrupt of scheduler.
 */

extern "C" void SysTick_Handler()
{
	const auto contextSwitchRequired = distortos::scheduler::schedulerInstance.tickInterruptHandler();
	if (contextSwitchRequired == true)
		distortos::architecture::requestContextSwitch();
}
