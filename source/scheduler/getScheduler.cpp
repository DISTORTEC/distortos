/**
 * \file
 * \brief getScheduler() definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#include "distortos/internal/scheduler/getScheduler.hpp"

#include "distortos/internal/scheduler/Scheduler.hpp"

#include <type_traits>

namespace distortos
{

namespace internal
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

}	// namespace internal

}	// namespace distortos
