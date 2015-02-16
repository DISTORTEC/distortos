/**
 * \file
 * \brief lowLevelInitialization1() definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-16
 */

#include "distortos/StaticThread.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/idleThreadFunction.hpp"
#include "distortos/scheduler/MainThread.hpp"

#include "distortos/architecture/architecture.hpp"

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
constexpr size_t idleThreadStackSize {128};

/// type of idle thread
using IdleThread = decltype(makeStaticThread<idleThreadStackSize>(0, idleThreadFunction));

/// storage for idle thread instance
std::aligned_storage<sizeof(IdleThread), alignof(IdleThread)>::type idleThreadStorage;

/// storage for main thread instance
std::aligned_storage<sizeof(MainThread), alignof(MainThread)>::type mainThreadStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Initializes main instance of system's Scheduler, starts idle thread and starts scheduling.
 *
 * This function is called before constructors for global and static objects.
 */

extern "C" void lowLevelInitialization1()
{
	auto& schedulerInstance = getScheduler();
	new (&schedulerInstance) Scheduler;

	auto& mainThread = *new (&mainThreadStorage) MainThread {UINT8_MAX};
	schedulerInstance.initialize(mainThread);
	mainThread.getThreadControlBlock().switchedToHook();
	
	auto& idleThread = *new (&idleThreadStorage) IdleThread {0, idleThreadFunction};
	idleThread.start();
	
	architecture::startScheduling();
}

}	// namespace scheduler

}	// namespace distortos
