/**
 * \file
 * \brief main() thread definition and its low-level initializer
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/getMainStack.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"
#include "distortos/internal/scheduler/ThreadGroupControlBlock.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"
#include "distortos/FATAL_ERROR.h"
#include "distortos/StaticSignalsReceiver.hpp"
#include "distortos/UndetachableThread.hpp"

#if __GNUC_PREREQ(5, 1) != 1
// GCC 4.x doesn't fully support constexpr constructors
#error "GCC 5.1 is the minimum version supported by distortos"
#endif

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for Stack's constructor.
 *
 * Enables use of architecture::getMainStack() result to construct Stack.
 *
 * \param [in] stackBuffer is a pair with beginning of stack and its size in bytes
 *
 * \return constructed Stack object
 */

Stack stackWrapper(const std::pair<void*, size_t> stackBuffer)
{
	return {stackBuffer.first, stackBuffer.second};
}

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// MainThread class is a Thread for main()
class MainThread : public UndetachableThread
{
public:

	/**
	 * \brief MainThread's constructor.
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] threadGroupControlBlock is a reference to ThreadGroupControlBlock to which this object will be added
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for main thread, nullptr to disable reception
	 * of signals for main thread
	 */

	MainThread(const uint8_t priority, ThreadGroupControlBlock& threadGroupControlBlock,
			SignalsReceiver* const signalsReceiver) :
					UndetachableThread{stackWrapper(architecture::getMainStack()), priority,
							SchedulingPolicy::roundRobin, &threadGroupControlBlock, signalsReceiver}
	{

	}

	/**
	 * \brief Low-level initializer of main() thread and scheduler
	 *
	 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
	 */

	static void lowLevelInitializer();

protected:

	/**
	 * \brief Thread's "run" function
	 *
	 * \warning This function must not be used!
	 */

	void run() override
	{
		FATAL_ERROR("This function must not be used!");
	}
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// storage for main thread instance
std::aligned_storage<sizeof(MainThread), alignof(MainThread)>::type mainThreadStorage;

/// main thread group
ThreadGroupControlBlock mainThreadGroupControlBlock;

#ifdef DISTORTOS_MAIN_THREAD_CAN_RECEIVE_SIGNALS

/// type of StaticSignalsReceiver for main thread
using MainThreadStaticSignalsReceiver =
		StaticSignalsReceiver<DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS, DISTORTOS_MAIN_THREAD_SIGNAL_ACTIONS>;

/// storage for instance of MainThreadStaticSignalsReceiver for main thread
std::aligned_storage<sizeof(MainThreadStaticSignalsReceiver), alignof(MainThreadStaticSignalsReceiver)>::type
		mainThreadStaticSignalsReceiverStorage;

#endif	// def DISTORTOS_MAIN_THREAD_CAN_RECEIVE_SIGNALS

/*---------------------------------------------------------------------------------------------------------------------+
| public static functions
+---------------------------------------------------------------------------------------------------------------------*/

void MainThread::lowLevelInitializer()
{
#ifdef DISTORTOS_MAIN_THREAD_CAN_RECEIVE_SIGNALS

	auto& mainThreadStaticSignalsReceiver =
			*new (&mainThreadStaticSignalsReceiverStorage) MainThreadStaticSignalsReceiver;
	const auto mainThreadStaticSignalsReceiverPointer = &static_cast<SignalsReceiver&>(mainThreadStaticSignalsReceiver);

#else	// !def DISTORTOS_MAIN_THREAD_CAN_RECEIVE_SIGNALS

	// nullptr - reception of signals is disabled for main thread
	constexpr auto mainThreadStaticSignalsReceiverPointer = nullptr;

#endif	// !def DISTORTOS_MAIN_THREAD_CAN_RECEIVE_SIGNALS

	auto& mainThread = *new (&mainThreadStorage) MainThread {DISTORTOS_MAIN_THREAD_PRIORITY,
			mainThreadGroupControlBlock, mainThreadStaticSignalsReceiverPointer};
	getScheduler().initialize(mainThread.getThreadControlBlock());	/// \todo error handling?
	mainThread.getThreadControlBlock().switchedToHook();
}

BIND_LOW_LEVEL_INITIALIZER(10, &MainThread::lowLevelInitializer);

}	// namespace

}	// namespace internal

}	// namespace distortos
