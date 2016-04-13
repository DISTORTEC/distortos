/**
 * \file
 * \brief DynamicThreadBase class header
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_DYNAMICTHREADBASE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_DYNAMICTHREADBASE_HPP_

#include "distortos/DynamicSignalsReceiver.hpp"
#include "distortos/DynamicThreadParameters.hpp"
#include "distortos/ThreadCommon.hpp"

#include "distortos/internal/memory/storageDeleter.hpp"

namespace distortos
{

#ifdef CONFIG_THREAD_DETACH_ENABLE

class DynamicThread;

#endif	// def CONFIG_THREAD_DETACH_ENABLE

namespace internal
{

/**
 * \brief DynamicThreadBase class is a type-erased interface for thread that has dynamic storage for bounded function,
 * stack and internal DynamicSignalsReceiver object.
 *
 * If thread detachment is enabled (CONFIG_THREAD_DETACH_ENABLE is defined) then this class is dynamically allocated by
 * DynamicThread - which allows it to be "detached". Otherwise - if thread detachment is disabled
 * (CONFIG_THREAD_DETACH_ENABLE is not defined) - DynamicThread just inherits from this class.
 */

class DynamicThreadBase : public ThreadCommon
{
public:

#ifdef CONFIG_THREAD_DETACH_ENABLE

	/**
	 * \brief DynamicThreadBase's constructor
	 *
	 * \tparam Function is the function that will be executed in separate thread
	 * \tparam Args are the arguments for \a Function
	 *
	 * \param [in] stackSize is the size of stack, bytes
	 * \param [in] canReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this
	 * thread
	 * \param [in] queuedSignals is the max number of queued signals for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
	 * \param [in] signalActions is the max number of different SignalAction objects for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] owner is a reference to owner DynamicThread object
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for \a function
	 */

	template<typename Function, typename... Args>
	DynamicThreadBase(size_t stackSize, bool canReceiveSignals, size_t queuedSignals, size_t signalActions,
			uint8_t priority, SchedulingPolicy schedulingPolicy, DynamicThread& owner, Function&& function,
			Args&&... args);

	/**
	 * \brief Detaches the thread.
	 *
	 * Similar to std::thread::detach() - http://en.cppreference.com/w/cpp/thread/thread/detach
	 * Similar to POSIX pthread_detach() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_detach.html
	 *
	 * Detaches the executing thread from the Thread object, allowing execution to continue independently. All resources
	 * allocated for the thread will be deallocated when the thread terminates.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - this thread is already detached;
	 */

	int detach() override;

#else	// !def CONFIG_THREAD_DETACH_ENABLE

	/**
	 * \brief DynamicThreadBase's constructor
	 *
	 * \tparam Function is the function that will be executed in separate thread
	 * \tparam Args are the arguments for \a Function
	 *
	 * \param [in] stackSize is the size of stack, bytes
	 * \param [in] canReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this
	 * thread
	 * \param [in] queuedSignals is the max number of queued signals for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
	 * \param [in] signalActions is the max number of different SignalAction objects for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for \a function
	 */

	template<typename Function, typename... Args>
	DynamicThreadBase(size_t stackSize, bool canReceiveSignals, size_t queuedSignals, size_t signalActions,
			uint8_t priority, SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args);

	/**
	 * \brief DynamicThreadBase's constructor
	 *
	 * \tparam Function is the function that will be executed in separate thread
	 * \tparam Args are the arguments for \a Function
	 *
	 * \param [in] parameters is a DynamicThreadParameters struct with thread parameters
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for \a function
	 */

	template<typename Function, typename... Args>
	DynamicThreadBase(const DynamicThreadParameters parameters, Function&& function, Args&&... args) :
			DynamicThreadBase{parameters.stackSize, parameters.canReceiveSignals, parameters.queuedSignals,
					parameters.signalActions, parameters.priority, parameters.schedulingPolicy,
					std::forward<Function>(function), std::forward<Args>(args)...}
	{

	}

#endif	// !def CONFIG_THREAD_DETACH_ENABLE

	DynamicThreadBase(const DynamicThreadBase&) = delete;
	DynamicThreadBase(DynamicThreadBase&&) = default;
	const DynamicThreadBase& operator=(const DynamicThreadBase&) = delete;
	DynamicThreadBase& operator=(DynamicThreadBase&&) = delete;

protected:

#ifdef CONFIG_THREAD_DETACH_ENABLE

	/**
	 * \brief Pre-termination hook function of thread
	 *
	 * If thread is detached, locks object used for deferred deletion.
	 *
	 * \param [in] thread is a reference to Thread object, this must be DynamicThreadBase!
	 */

	static void preTerminationHook(Thread& thread);

	/**
	 * \brief Termination hook function of thread
	 *
	 * Calls ThreadCommon::terminationHook() and - if thread is detached - schedules itself for deferred deletion.
	 *
	 * \param [in] thread is a reference to Thread object, this must be DynamicThreadBase!
	 */

	static void terminationHook(Thread& thread);

#endif	// def CONFIG_THREAD_DETACH_ENABLE

private:

	/**
	 * \brief Thread's "run" function.
	 *
	 * Executes bound function object.
	 *
	 * \param [in] thread is a reference to Thread object, this must be DynamicThreadBase!
	 */

	static void run(Thread& thread);

	/// internal DynamicSignalsReceiver object
	DynamicSignalsReceiver dynamicSignalsReceiver_;

	/// bound function object
	std::function<void()> boundFunction_;

#ifdef CONFIG_THREAD_DETACH_ENABLE

	/// pointer to owner DynamicThread object, nullptr if thread is detached
	DynamicThread* owner_;

#endif	// def CONFIG_THREAD_DETACH_ENABLE
};

#ifdef CONFIG_THREAD_DETACH_ENABLE

template<typename Function, typename... Args>
DynamicThreadBase::DynamicThreadBase(const size_t stackSize, const bool canReceiveSignals, const size_t queuedSignals,
		const size_t signalActions, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		DynamicThread& owner, Function&& function, Args&&... args) :
				ThreadCommon{{{new uint8_t[stackSize], storageDeleter<uint8_t>}, stackSize, *this, run,
						preTerminationHook, terminationHook}, priority, schedulingPolicy, nullptr,
						canReceiveSignals == true ? &dynamicSignalsReceiver_ : nullptr},
				dynamicSignalsReceiver_{canReceiveSignals == true ? queuedSignals : 0,
						canReceiveSignals == true ? signalActions : 0},
				boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)},
				owner_{&owner}
{

}

#else	// !def CONFIG_THREAD_DETACH_ENABLE

template<typename Function, typename... Args>
DynamicThreadBase::DynamicThreadBase(const size_t stackSize, const bool canReceiveSignals, const size_t queuedSignals,
		const size_t signalActions, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		Function&& function, Args&&... args) :
				ThreadCommon{{{new uint8_t[stackSize], storageDeleter<uint8_t>}, stackSize, *this, run, nullptr,
						terminationHook}, priority, schedulingPolicy, nullptr,
						canReceiveSignals == true ? &dynamicSignalsReceiver_ : nullptr},
				dynamicSignalsReceiver_{canReceiveSignals == true ? queuedSignals : 0,
						canReceiveSignals == true ? signalActions : 0},
				boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
{

}

#endif	// !def CONFIG_THREAD_DETACH_ENABLE

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_DYNAMICTHREADBASE_HPP_
