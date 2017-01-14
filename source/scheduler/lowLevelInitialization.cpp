/**
 * \file
 * \brief internal::lowLevelInitialization() definition
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/lowLevelInitialization.hpp"

#include "distortos/Mutex.hpp"	// needed when CONFIG_THREAD_DETACH_ENABLE option is disabled
#include "distortos/StaticThread.hpp"

#include "distortos/internal/memory/DeferredThreadDeleter.hpp"
#include "distortos/internal/memory/getDeferredThreadDeleter.hpp"
#include "distortos/internal/memory/getMallocMutex.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"
#include "distortos/internal/scheduler/idleThreadFunction.hpp"
#include "distortos/internal/scheduler/MainThread.hpp"
#include "distortos/internal/scheduler/ThreadGroupControlBlock.hpp"

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of idle thread's stack, bytes
#ifdef CONFIG_THREAD_DETACH_ENABLE
constexpr size_t idleThreadStackSize {256};
#else	// !def CONFIG_THREAD_DETACH_ENABLE
constexpr size_t idleThreadStackSize {128};
#endif	// !def CONFIG_THREAD_DETACH_ENABLE

/// type of idle thread
using IdleThread = decltype(makeStaticThread<idleThreadStackSize>(0, idleThreadFunction));

/// storage for idle thread instance
std::aligned_storage<sizeof(IdleThread), alignof(IdleThread)>::type idleThreadStorage;

/// storage for main thread instance
std::aligned_storage<sizeof(MainThread), alignof(MainThread)>::type mainThreadStorage;

/// storage for main thread group instance
std::aligned_storage<sizeof(ThreadGroupControlBlock), alignof(ThreadGroupControlBlock)>::type
		mainThreadGroupControlBlockStorage;

#ifdef CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS

/// type of StaticSignalsReceiver for main thread
using MainThreadStaticSignalsReceiver =
		StaticSignalsReceiver<CONFIG_MAIN_THREAD_QUEUED_SIGNALS, CONFIG_MAIN_THREAD_SIGNAL_ACTIONS>;

/// storage for instance of MainThreadStaticSignalsReceiver for main thread
std::aligned_storage<sizeof(MainThreadStaticSignalsReceiver), alignof(MainThreadStaticSignalsReceiver)>::type
		mainThreadStaticSignalsReceiverStorage;

#endif	// def CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
	auto& schedulerInstance = getScheduler();
	new (&schedulerInstance) Scheduler;

	auto& mainThreadGroupControlBlock = *new (&mainThreadGroupControlBlockStorage) ThreadGroupControlBlock;

#ifdef CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS

	auto& mainThreadStaticSignalsReceiver =
			*new (&mainThreadStaticSignalsReceiverStorage) MainThreadStaticSignalsReceiver;
	const auto mainThreadStaticSignalsReceiverPointer = &static_cast<SignalsReceiver&>(mainThreadStaticSignalsReceiver);

#else	// !def CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS

	// nullptr - reception of signals is disabled for main thread
	constexpr auto mainThreadStaticSignalsReceiverPointer = nullptr;

#endif	// !def CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS

	auto& mainThread = *new (&mainThreadStorage) MainThread {CONFIG_MAIN_THREAD_PRIORITY, mainThreadGroupControlBlock,
			mainThreadStaticSignalsReceiverPointer};
	schedulerInstance.initialize(mainThread);	/// \todo error handling?
	mainThread.getThreadControlBlock().switchedToHook();

	auto& idleThread = *new (&idleThreadStorage) IdleThread {0, idleThreadFunction};
	idleThread.start();

	new (&getMallocMutex()) Mutex {Mutex::Type::recursive, Mutex::Protocol::priorityInheritance};

#ifdef CONFIG_THREAD_DETACH_ENABLE

	new (&getDeferredThreadDeleter()) DeferredThreadDeleter;

#endif	// def CONFIG_THREAD_DETACH_ENABLE
}

}	// namespace internal

}	// namespace distortos
