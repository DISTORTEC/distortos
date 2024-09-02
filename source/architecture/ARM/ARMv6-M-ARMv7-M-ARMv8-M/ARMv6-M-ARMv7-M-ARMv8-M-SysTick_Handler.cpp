/**
 * \file
 * \brief SysTick_Handler() for ARMv6-M, ARMv7-M and ARMv8-M
 *
 * \author Copyright (C) 2014-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/architecture/requestContextSwitch.hpp"

#ifdef DISTORTOS_CHECK_STACK_POINTER_RANGE_SYSTEM_TICK_ENABLE

#include "distortos/chip/CMSIS-proxy.h"

#include "distortos/FATAL_ERROR.h"

#endif	// def DISTORTOS_CHECK_STACK_POINTER_RANGE_SYSTEM_TICK_ENABLE

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief SysTick_Handler() for ARMv6-M, ARMv7-M and ARMv8-M
 *
 * Tick interrupt of scheduler. This function also checks stack pointer range when this functionality is enabled - if
 * the check fails, FATAL_ERROR() is called.
 */

extern "C" void SysTick_Handler()
{
	auto& scheduler = distortos::internal::getScheduler();

#ifdef DISTORTOS_CHECK_STACK_POINTER_RANGE_SYSTEM_TICK_ENABLE

	const auto stackPointer = reinterpret_cast<const void*>(__get_PSP());
	if (scheduler.getCurrentThreadControlBlock().getStack().checkStackPointer(stackPointer) == false)
		FATAL_ERROR("Stack overflow detected!");

#endif	// def DISTORTOS_CHECK_STACK_POINTER_RANGE_SYSTEM_TICK_ENABLE

	const auto contextSwitchRequired = scheduler.tickInterruptHandler();
	if (contextSwitchRequired == true)
		distortos::architecture::requestContextSwitch();
}
