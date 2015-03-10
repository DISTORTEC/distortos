/**
 * \file
 * \brief lowLevelSchedulerInitialization() definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-10
 */

#include "distortos/scheduler/lowLevelSchedulerInitialization.hpp"

#include "distortos/StaticThread.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/idleThreadFunction.hpp"
#include "distortos/scheduler/MainThread.hpp"
#include "distortos/scheduler/ThreadGroupControlBlock.hpp"

#include "distortos/architecture/architecture.hpp"

namespace distortos
{

namespace scheduler
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// pointer to function taking no arguments and returning no value
using FunctionPointer = void(*)();

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

/// storage for main thread group instance
std::aligned_storage<sizeof(ThreadGroupControlBlock), alignof(ThreadGroupControlBlock)>::type
		mainThreadGroupControlBlockStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelSchedulerInitialization()
{
	auto& schedulerInstance = getScheduler();
	new (&schedulerInstance) Scheduler;

	auto& mainThreadGroupControlBlock = *new (&mainThreadGroupControlBlockStorage) ThreadGroupControlBlock;

	auto& mainThread = *new (&mainThreadStorage) MainThread {UINT8_MAX, mainThreadGroupControlBlock};
	schedulerInstance.initialize(mainThread);	/// \todo error handling?
	mainThread.getThreadControlBlock().switchedToHook();

	auto& idleThread = *new (&idleThreadStorage) IdleThread {0, idleThreadFunction};
	idleThread.start();
	
	architecture::startScheduling();
}

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array of pointers to functions executed before constructors for global and static objects
const FunctionPointer distortosPreinitArray[] __attribute__ ((section(".preinit_array"), used))
{
		lowLevelSchedulerInitialization,
};

}	// namespace scheduler

}	// namespace distortos
