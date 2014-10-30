/**
 * \file
 * \brief getScheduler() definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-30
 */

#include "distortos/scheduler/getScheduler.hpp"

#include "distortos/scheduler/Scheduler.hpp"

#include <type_traits>

namespace distortos
{

namespace scheduler
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// storage for main instance of system's Scheduler
std::aligned_storage<sizeof(Scheduler), alignof(Scheduler)>::type schedulerInstanceStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

Scheduler& getScheduler()
{
	return reinterpret_cast<Scheduler&>(schedulerInstanceStorage);
}

}	// namespace scheduler

}	// namespace distortos
