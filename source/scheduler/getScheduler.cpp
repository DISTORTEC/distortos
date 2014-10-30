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

#include "distortos/Thread.hpp"
#include "distortos/scheduler/idleThreadFunction.hpp"
#include "distortos/scheduler/MainThreadControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of idle thread's stack, bytes
constexpr size_t idleThreadStackSize_ {128};

/// idle thread's stack
uint64_t idleThreadStack_[idleThreadStackSize_ / sizeof(uint64_t)];

/// idle thread instance
auto idleThread_ = makeThread(idleThreadStack_, sizeof(idleThreadStack_), 0, idleThreadFunction);

/// main thread instance
MainThreadControlBlock mainThreadControlBlock_ {UINT8_MAX};

/// main instance of system's Scheduler
Scheduler schedulerInstance {mainThreadControlBlock_, idleThread_};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

Scheduler& getScheduler()
{
	return schedulerInstance;
}

}	// namespace scheduler

}	// namespace distortos
