/**
 * \file
 * \brief lowLevelSchedulerInitialization() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-10
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_LOWLEVELSCHEDULERINITIALIZATION_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_LOWLEVELSCHEDULERINITIALIZATION_HPP_

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Initializes main instance of system's Scheduler, starts idle thread and starts scheduling.
 *
 * This function is called before constructors for global and static objects from __libc_init_array() via address in
 * distortosPreinitArray[].
 */

void lowLevelSchedulerInitialization();

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_LOWLEVELSCHEDULERINITIALIZATION_HPP_
