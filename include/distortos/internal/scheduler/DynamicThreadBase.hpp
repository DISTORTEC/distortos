/**
 * \file
 * \brief DynamicThreadBase class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_DYNAMICTHREADBASE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_DYNAMICTHREADBASE_HPP_

#include "distortos/DynamicSignalsReceiver.hpp"
#include "distortos/DynamicThreadParameters.hpp"

#include "distortos/internal/memory/storageDeleter.hpp"

#include "distortos/internal/scheduler/ThreadCommon.hpp"

#include <functional>

namespace distortos
{

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

class DynamicThread;

#endif	// DISTORTOS_THREAD_DETACH_ENABLE == 1

namespace internal
{

/**
 * \brief DynamicThreadBase class is a type-erased interface for thread that has dynamic storage for bound function,
 * stack and - if signals are enabled - internal DynamicSignalsReceiver object.
 *
 * If thread detachment is enabled (DISTORTOS_THREAD_DETACH_ENABLE is defined) then this class is dynamically allocated
 * by DynamicThread - which allows it to be "detached". Otherwise - if thread detachment is disabled
 * (DISTORTOS_THREAD_DETACH_ENABLE is not defined) - DynamicThread just inherits from this class.
 */

class DynamicThreadBase : public ThreadCommon
{
public:

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

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

#else	// DISTORTOS_THREAD_DETACH_ENABLE != 1

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

#endif	// DISTORTOS_THREAD_DETACH_ENABLE != 1

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

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

#endif	// DISTORTOS_THREAD_DETACH_ENABLE == 1

	/**
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "created" state only.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by ThreadCommon::startInternal();
	 */

	int start()
	{
		return ThreadCommon::startInternal();
	}

	DynamicThreadBase(const DynamicThreadBase&) = delete;
	DynamicThreadBase(DynamicThreadBase&&) = default;
	const DynamicThreadBase& operator=(const DynamicThreadBase&) = delete;
	DynamicThreadBase& operator=(DynamicThreadBase&&) = delete;

protected:

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

	/**
	 * \brief Thread's "exit 0" hook function
	 *
	 * This hook will be called early during thread's exit - while the thread is still runnable.
	 *
	 * Calls "exit 0" hook of base class and - if thread is detached - locks object used for deferred deletion.
	 */

	void exit0Hook() override;

	/**
	 * \brief Thread's "exit 1" hook function
	 *
	 * This hook will be called late during thread's exit - after the thread is removed from the scheduler.
	 *
	 * Calls "exit 1" hook of base class and - if thread is detached - schedules itself for deferred deletion.
	 */

	void exit1Hook() override;

#endif	// DISTORTOS_THREAD_DETACH_ENABLE == 1

	/**
	 * \brief Thread's "run" function
	 *
	 * Executes bound function object.
	 */

	void run() override;

private:

	/**
	 * \brief Helper function to make stack with size adjusted to alignment requirements
	 *
	 * Size of "stack guard" is added to function argument.
	 *
	 * \param [in] stackSize is the size of stack, bytes
	 *
	 * \return Stack object with size adjusted to alignment requirements
	 */

	static Stack makeStack(const size_t stackSize)
	{
		static_assert(alignof(max_align_t) >= DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT,
				"Alignment of dynamically allocated memory is too low!");

		const auto adjustedStackSize = (stackSize + DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT - 1) /
				DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT * DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT;
		return {{new uint8_t[adjustedStackSize + stackGuardSize], storageDeleter<uint8_t>},
				adjustedStackSize + stackGuardSize};
	}

#if DISTORTOS_SIGNALS_ENABLE == 1

	/// internal DynamicSignalsReceiver object
	DynamicSignalsReceiver dynamicSignalsReceiver_;

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	/// bound function object
	std::function<void()> boundFunction_;

#if DISTORTOS_THREAD_DETACH_ENABLE == 1

	/// pointer to owner DynamicThread object, nullptr if thread is detached
	DynamicThread* owner_;

#endif	// DISTORTOS_THREAD_DETACH_ENABLE == 1
};

#if DISTORTOS_SIGNALS_ENABLE == 1 && DISTORTOS_THREAD_DETACH_ENABLE == 1

template<typename Function, typename... Args>
DynamicThreadBase::DynamicThreadBase(const size_t stackSize, const bool canReceiveSignals, const size_t queuedSignals,
		const size_t signalActions, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		DynamicThread& owner, Function&& function, Args&&... args) :
				ThreadCommon{makeStack(stackSize), priority, schedulingPolicy, nullptr,
						canReceiveSignals == true ? &dynamicSignalsReceiver_ : nullptr},
				dynamicSignalsReceiver_{canReceiveSignals == true ? queuedSignals : 0,
						canReceiveSignals == true ? signalActions : 0},
				boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)},
				owner_{&owner}
{

}

#elif DISTORTOS_SIGNALS_ENABLE == 1 && DISTORTOS_THREAD_DETACH_ENABLE != 1

template<typename Function, typename... Args>
DynamicThreadBase::DynamicThreadBase(const size_t stackSize, const bool canReceiveSignals, const size_t queuedSignals,
		const size_t signalActions, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		Function&& function, Args&&... args) :
				ThreadCommon{makeStack(stackSize), priority, schedulingPolicy, nullptr,
						canReceiveSignals == true ? &dynamicSignalsReceiver_ : nullptr},
				dynamicSignalsReceiver_{canReceiveSignals == true ? queuedSignals : 0,
						canReceiveSignals == true ? signalActions : 0},
				boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
{

}

#elif DISTORTOS_SIGNALS_ENABLE != 1 && DISTORTOS_THREAD_DETACH_ENABLE == 1

template<typename Function, typename... Args>
DynamicThreadBase::DynamicThreadBase(const size_t stackSize, bool, size_t, size_t, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, DynamicThread& owner, Function&& function, Args&&... args) :
				ThreadCommon{makeStack(stackSize), priority, schedulingPolicy, nullptr, nullptr},
				boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)},
				owner_{&owner}
{

}

#else	// DISTORTOS_SIGNALS_ENABLE != 1 && DISTORTOS_THREAD_DETACH_ENABLE != 1

template<typename Function, typename... Args>
DynamicThreadBase::DynamicThreadBase(const size_t stackSize, bool, size_t, size_t, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
				ThreadCommon{makeStack(stackSize), priority, schedulingPolicy, nullptr, nullptr},
				boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
{

}

#endif	// DISTORTOS_SIGNALS_ENABLE != 1 && DISTORTOS_THREAD_DETACH_ENABLE != 1

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_DYNAMICTHREADBASE_HPP_
