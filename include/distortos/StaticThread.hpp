/**
 * \file
 * \brief StaticThread class header
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICTHREAD_HPP_
#define INCLUDE_DISTORTOS_STATICTHREAD_HPP_

#include "distortos/StaticSignalsReceiver.hpp"
#include "distortos/UndetachableThread.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief StaticThreadBase class is a templated common base for StaticThread
 *
 * \tparam Function is the function that will be executed in separate thread
 * \tparam Args are the arguments for \a Function
 */

template<typename Function, typename... Args>
class StaticThreadBase : public UndetachableThread
{
public:

	/// base of StaticThreadBase
	using Base = UndetachableThread;

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
					Base{{std::move(stackStorageUniquePointer), size, *this, run, nullptr, terminationHook},
							priority, schedulingPolicy, nullptr, signalsReceiver},
					boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
	{

	}

	StaticThreadBase(const StaticThreadBase&) = delete;
	StaticThreadBase(StaticThreadBase&&) = default;
	const StaticThreadBase& operator=(const StaticThreadBase&) = delete;
	StaticThreadBase& operator=(StaticThreadBase&&) = delete;

private:

	/**
	 * \brief Thread's "run" function.
	 *
	 * Executes bound function object.
	 *
	 * \param [in] thread is a reference to Thread object, this must be StaticThreadBase!
	 */

	static void run(Thread& thread)
	{
		static_cast<StaticThreadBase&>(thread).boundFunction_();
	}

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
			StaticThread{priority, SchedulingPolicy::roundRobin, std::forward<Function>(function),
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
			StaticThread{priority, SchedulingPolicy::roundRobin, std::forward<Function>(function),
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
	thread.start();	/// \todo make sure this never fails
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
	thread.start();	/// \todo make sure this never fails
	return thread;
}

/// \}

template<size_t StackSize, bool CanReceiveSignals, size_t QueuedSignals, size_t SignalActions, typename Function,
		typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>::
StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		Base{{&stack_, internal::dummyDeleter<decltype(stack_)>}, sizeof(stack_), priority, schedulingPolicy, nullptr,
				std::forward<Function>(function), std::forward<Args>(args)...}
{

}

template<size_t StackSize, size_t QueuedSignals, size_t SignalActions, typename Function, typename... Args>
StaticThread<StackSize, true, QueuedSignals, SignalActions, Function, Args...>::StaticThread(const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		Base{{&stack_, internal::dummyDeleter<decltype(stack_)>}, sizeof(stack_), priority, schedulingPolicy,
				&static_cast<SignalsReceiver&>(staticSignalsReceiver_), std::forward<Function>(function),
				std::forward<Args>(args)...},
		staticSignalsReceiver_{}
{

}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICTHREAD_HPP_
