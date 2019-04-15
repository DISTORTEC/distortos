/**
 * \file
 * \brief StaticThread class header
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICTHREAD_HPP_
#define INCLUDE_DISTORTOS_STATICTHREAD_HPP_

#include "distortos/internal/memory/dummyDeleter.hpp"

#include "distortos/assert.h"
#include "distortos/StaticSignalsReceiver.hpp"
#include "distortos/UndetachableThread.hpp"

#include <functional>

namespace distortos
{

namespace internal
{

/**
 * \brief StaticThreadBase class is a templated common base for StaticThread
 *
 * \tparam StackSize is the size of stack, bytes
 * \tparam Function is the function that will be executed in separate thread
 * \tparam Args are the arguments for \a Function
 */

template<size_t StackSize, typename Function, typename... Args>
class StaticThreadBase : public UndetachableThread
{
public:

	/// base of StaticThreadBase
	using Base = UndetachableThread;

	/**
	 * \brief StaticThreadBase's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThreadBase(const uint8_t priority, const SchedulingPolicy schedulingPolicy,
			SignalsReceiver* const signalsReceiver, Function&& function, Args&&... args) :
					Base{{{&stack_, internal::dummyDeleter<decltype(stack_)>}, sizeof(stack_)}, priority,
							schedulingPolicy, nullptr, signalsReceiver},
					boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
	{

	}

	StaticThreadBase(const StaticThreadBase&) = delete;
	StaticThreadBase(StaticThreadBase&&) = default;
	const StaticThreadBase& operator=(const StaticThreadBase&) = delete;
	StaticThreadBase& operator=(StaticThreadBase&&) = delete;

	/**
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "created" state only.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by UndetachableThread::startInternal();
	 */

	int start()
	{
		return UndetachableThread::startInternal();
	}

protected:

	/**
	 * \brief Thread's "run" function
	 *
	 * Executes bound function object.
	 */

	void run() override
	{
		boundFunction_();
	}

private:

	/// size of stack adjusted to alignment requirements, bytes
	constexpr static size_t adjustedStackSize {(StackSize + DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT - 1) /
			DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT * DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT};

	/// stack buffer
	alignas(DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT)
	typename std::aligned_storage<adjustedStackSize + internal::stackGuardSize>::type stack_;

	static_assert(sizeof(stack_) % DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT == 0, "Stack size is not aligned!");

	/// bound function object
	decltype(std::bind(std::declval<Function>(), std::declval<Args>()...)) boundFunction_;
};

}	// namespace internal

/// \addtogroup threads
/// \{

/**
 * \brief StaticThread class is a templated interface for thread that has automatic storage for stack.
 *
 * \tparam StackSize is the size of stack, bytes
 * \tparam CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \tparam QueuedSignals is the max number of queued signals for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable queuing of signals for this thread
 * \tparam SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \tparam Function is the function that will be executed in separate thread
 * \tparam Args are the arguments for \a Function
 */

template<size_t StackSize, bool CanReceiveSignals, size_t QueuedSignals, size_t SignalActions, typename Function,
		typename... Args>
class StaticThread : public internal::StaticThreadBase<StackSize, Function, Args...>
{
public:

	/// base of StaticThread
	using Base = internal::StaticThreadBase<StackSize, Function, Args...>;

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(uint8_t priority, SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args);

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(const uint8_t priority, Function&& function, Args&&... args) :
			StaticThread{priority, SchedulingPolicy::roundRobin, std::forward<Function>(function),
					std::forward<Args>(args)...}
	{

	}

	StaticThread(const StaticThread&) = delete;
	StaticThread(StaticThread&&) = default;
	const StaticThread& operator=(const StaticThread&) = delete;
	StaticThread& operator=(StaticThread&&) = delete;
};

#if CONFIG_SIGNALS_ENABLE == 1

/**
 * \brief StaticThread class is a templated interface for thread that has automatic storage for stack and internal
 * StaticSignalsReceiver object.
 *
 * Specialization for threads with enabled reception of signals (CanReceiveSignals == true)
 *
 * \tparam StackSize is the size of stack, bytes
 * \tparam QueuedSignals is the max number of queued signals for this thread, 0 to disable queuing of signals for this
 * thread
 * \tparam SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \tparam Function is the function that will be executed in separate thread
 * \tparam Args are the arguments for \a Function
 */

template<size_t StackSize, size_t QueuedSignals, size_t SignalActions, typename Function, typename... Args>
class StaticThread<StackSize, true, QueuedSignals, SignalActions, Function, Args...> :
		public internal::StaticThreadBase<StackSize, Function, Args...>
{
public:

	/// base of StaticThread
	using Base = internal::StaticThreadBase<StackSize, Function, Args...>;

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(uint8_t priority, SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args);

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(const uint8_t priority, Function&& function, Args&&... args) :
			StaticThread{priority, SchedulingPolicy::roundRobin, std::forward<Function>(function),
					std::forward<Args>(args)...}
	{

	}

	StaticThread(const StaticThread&) = delete;
	StaticThread(StaticThread&&) = default;
	const StaticThread& operator=(const StaticThread&) = delete;
	StaticThread& operator=(StaticThread&&) = delete;

private:

	/// internal StaticSignalsReceiver object
	StaticSignalsReceiver<QueuedSignals, SignalActions> staticSignalsReceiver_;
};

#endif	// CONFIG_SIGNALS_ENABLE == 1

/**
 * \brief Helper factory function to make StaticThread object with partially deduced template arguments
 *
 * \tparam StackSize is the size of stack, bytes
 * \tparam CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \tparam QueuedSignals is the max number of queued signals for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable queuing of signals for this thread
 * \tparam SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] schedulingPolicy is the scheduling policy of the thread
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return StaticThread object with partially deduced template arguments
 */

template<size_t StackSize, bool CanReceiveSignals = {}, size_t QueuedSignals = {}, size_t SignalActions = {},
		typename Function, typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>
makeStaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args)
{
	return {priority, schedulingPolicy, std::forward<Function>(function), std::forward<Args>(args)...};
}

/**
 * \brief Helper factory function to make StaticThread object with partially deduced template arguments
 *
 * \tparam StackSize is the size of stack, bytes
 * \tparam CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \tparam QueuedSignals is the max number of queued signals for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable queuing of signals for this thread
 * \tparam SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return StaticThread object with partially deduced template arguments
 */

template<size_t StackSize, bool CanReceiveSignals = {}, size_t QueuedSignals = {}, size_t SignalActions = {},
		typename Function, typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>
makeStaticThread(const uint8_t priority, Function&& function, Args&&... args)
{
	return {priority, std::forward<Function>(function), std::forward<Args>(args)...};
}

/**
 * \brief Helper factory function to make and start StaticThread object with partially deduced template arguments
 *
 * \tparam StackSize is the size of stack, bytes
 * \tparam CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \tparam QueuedSignals is the max number of queued signals for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable queuing of signals for this thread
 * \tparam SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] schedulingPolicy is the scheduling policy of the thread
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return StaticThread object with partially deduced template arguments
 */

template<size_t StackSize, bool CanReceiveSignals = {}, size_t QueuedSignals = {}, size_t SignalActions = {},
		typename Function, typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>
makeAndStartStaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function,
		Args&&... args)
{
	auto thread = makeStaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions>(priority,
			schedulingPolicy, std::forward<Function>(function), std::forward<Args>(args)...);
	{
		const auto ret = thread.start();
		assert(ret == 0 && "Could not start thread!");
	}
	return thread;
}

/**
 * \brief Helper factory function to make and start StaticThread object with partially deduced template arguments
 *
 * \tparam StackSize is the size of stack, bytes
 * \tparam CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \tparam QueuedSignals is the max number of queued signals for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable queuing of signals for this thread
 * \tparam SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return StaticThread object with partially deduced template arguments
 */

template<size_t StackSize, bool CanReceiveSignals = {}, size_t QueuedSignals = {}, size_t SignalActions = {},
		typename Function, typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>
makeAndStartStaticThread(const uint8_t priority, Function&& function, Args&&... args)
{
	auto thread = makeStaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions>(priority,
			std::forward<Function>(function), std::forward<Args>(args)...);
	{
		const auto ret = thread.start();
		assert(ret == 0 && "Could not start thread!");
	}
	return thread;
}

/// \}

template<size_t StackSize, bool CanReceiveSignals, size_t QueuedSignals, size_t SignalActions, typename Function,
		typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>::
StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		Base{priority, schedulingPolicy, nullptr, std::forward<Function>(function), std::forward<Args>(args)...}
{

}

#if CONFIG_SIGNALS_ENABLE == 1

template<size_t StackSize, size_t QueuedSignals, size_t SignalActions, typename Function, typename... Args>
StaticThread<StackSize, true, QueuedSignals, SignalActions, Function, Args...>::StaticThread(const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		Base{priority, schedulingPolicy, &static_cast<SignalsReceiver&>(staticSignalsReceiver_),
				std::forward<Function>(function), std::forward<Args>(args)...},
		staticSignalsReceiver_{}
{

}

#endif	// CONFIG_SIGNALS_ENABLE == 1

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICTHREAD_HPP_
