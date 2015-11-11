/**
 * \file
 * \brief StaticThread class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-11
 */

#ifndef INCLUDE_DISTORTOS_STATICTHREAD_HPP_
#define INCLUDE_DISTORTOS_STATICTHREAD_HPP_

#include "distortos/StaticSignalsReceiver.hpp"
#include "distortos/Thread.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief StaticThreadBase class is a templated common base for StaticThread
 *
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for \a Function
 */

template<typename Function, typename... Args>
class StaticThreadBase : public Thread
{
public:

	/// base of StaticThreadBase
	using Base = Thread;

	/**
	 * \brief StaticThreadBase's constructor
	 *
	 * \param [in] stackStorageUniquePointer is a rvalue reference to StackStorageUniquePointer with storage for stack
	 * (\a size bytes long) and appropriate deleter
	 * \param [in] size is the size of stack's storage, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThreadBase(StackStorageUniquePointer&& stackStorageUniquePointer, const size_t size, const uint8_t priority,
			const SchedulingPolicy schedulingPolicy, SignalsReceiver* const signalsReceiver, Function&& function,
			Args&&... args) :
			Base{std::move(stackStorageUniquePointer), size, priority, schedulingPolicy, nullptr, signalsReceiver},
			boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
	{

	}

	StaticThreadBase(const StaticThreadBase&) = delete;
	StaticThreadBase(StaticThreadBase&&) = default;
	const StaticThreadBase& operator=(const StaticThreadBase&) = delete;
	StaticThreadBase& operator=(StaticThreadBase&&) = delete;

private:

	/**
	 * \brief StaticThreadBase's internal function.
	 *
	 * Executes bound function object.
	 */

	virtual void run() override
	{
		boundFunction_();
	}

	/// bound function object
	decltype(std::bind(std::declval<Function>(), std::declval<Args>()...)) boundFunction_;
};

}	// namespace internal

/**
 * \brief StaticThread class is a templated interface for thread that has automatic storage for stack.
 *
 * \param StackSize is the size of stack, bytes
 * \param CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \param QueuedSignals is the max number of queued signals for this thread, relevant only if CanReceiveSignals == true,
 * 0 to disable queuing of signals for this thread
 * \param SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for \a Function
 */

template<size_t StackSize, bool CanReceiveSignals, size_t QueuedSignals, size_t SignalActions, typename Function,
		typename... Args>
class StaticThread : public internal::StaticThreadBase<Function, Args...>
{
public:

	/// base of StaticThread
	using Base = internal::StaticThreadBase<Function, Args...>;

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
			StaticThread{priority, SchedulingPolicy::RoundRobin, std::forward<Function>(function),
					std::forward<Args>(args)...}
	{

	}

	StaticThread(const StaticThread&) = delete;
	StaticThread(StaticThread&&) = default;
	const StaticThread& operator=(const StaticThread&) = delete;
	StaticThread& operator=(StaticThread&&) = delete;

private:

	/// stack buffer
	typename std::aligned_storage<StackSize>::type stack_;
};

/**
 * \brief StaticThread class is a templated interface for thread that has automatic storage for stack and internal
 * StaticSignalsReceiver object.
 *
 * Specialization for threads with enabled reception of signals (CanReceiveSignals == true)
 *
 * \param StackSize is the size of stack, bytes
 * \param QueuedSignals is the max number of queued signals for this thread, 0 to disable queuing of signals for this
 * thread
 * \param SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for \a Function
 */

template<size_t StackSize, size_t QueuedSignals, size_t SignalActions, typename Function, typename... Args>
class StaticThread<StackSize, true, QueuedSignals, SignalActions, Function, Args...> :
		public internal::StaticThreadBase<Function, Args...>
{
public:

	/// base of StaticThread
	using Base = internal::StaticThreadBase<Function, Args...>;

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
			StaticThread{priority, SchedulingPolicy::RoundRobin, std::forward<Function>(function),
					std::forward<Args>(args)...}
	{

	}

	StaticThread(const StaticThread&) = delete;
	StaticThread(StaticThread&&) = default;
	const StaticThread& operator=(const StaticThread&) = delete;
	StaticThread& operator=(StaticThread&&) = delete;

private:

	/// stack buffer
	typename std::aligned_storage<StackSize>::type stack_;

	/// internal StaticSignalsReceiver object
	StaticSignalsReceiver<QueuedSignals, SignalActions> staticSignalsReceiver_;
};

template<size_t StackSize, bool CanReceiveSignals, size_t QueuedSignals, size_t SignalActions, typename Function,
		typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>::
StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		Base{{&stack_, memory::dummyDeleter<decltype(stack_)>}, sizeof(stack_), priority, schedulingPolicy, nullptr,
				std::forward<Function>(function), std::forward<Args>(args)...}
{

}

template<size_t StackSize, size_t QueuedSignals, size_t SignalActions, typename Function, typename... Args>
StaticThread<StackSize, true, QueuedSignals, SignalActions, Function, Args...>::StaticThread(const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		Base{{&stack_, memory::dummyDeleter<decltype(stack_)>}, sizeof(stack_), priority, schedulingPolicy,
				&static_cast<SignalsReceiver&>(staticSignalsReceiver_), std::forward<Function>(function),
				std::forward<Args>(args)...},
		staticSignalsReceiver_{}
{

}

/**
 * \brief Helper factory function to make StaticThread object with partially deduced template arguments
 *
 * \param StackSize is the size of stack, bytes
 * \param CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \param QueuedSignals is the max number of queued signals for this thread, relevant only if CanReceiveSignals == true,
 * 0 to disable queuing of signals for this thread
 * \param SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \param Function is the function that will be executed
 * \param Args are the arguments for \a Function
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
 * \param StackSize is the size of stack, bytes
 * \param CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \param QueuedSignals is the max number of queued signals for this thread, relevant only if CanReceiveSignals == true,
 * 0 to disable queuing of signals for this thread
 * \param SignalActions is the max number of different SignalAction objects for this thread, relevant only if
 * CanReceiveSignals == true, 0 to disable catching of signals for this thread
 * \param Function is the function that will be executed
 * \param Args are the arguments for \a Function
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

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICTHREAD_HPP_
