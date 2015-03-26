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
 * \date 2015-03-26
 */

#ifndef INCLUDE_DISTORTOS_STATICTHREAD_HPP_
#define INCLUDE_DISTORTOS_STATICTHREAD_HPP_

#include "distortos/Thread.hpp"
#include "distortos/StaticSignalsReceiver.hpp"

namespace distortos
{

/**
 * \brief StaticThread class is a templated interface for thread that has automatic storage for stack.
 *
 * \param StackSize is the size of stack, bytes
 * \param CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \param QueuedSignals is the max number of queued signals for this thread, relevant only if CanReceiveSignals == true,
 * 0 to disable queuing of signals for this thread
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for Function
 */

template<size_t StackSize, bool CanReceiveSignals, size_t QueuedSignals, typename Function, typename... Args>
class StaticThread : public Thread<Function, Args...>
{
public:

	/// base of StaticThread
	using Base = Thread<Function, Args...>;

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
			Base{&stack_, sizeof(stack_), priority, schedulingPolicy, std::forward<Function>(function),
					std::forward<Args>(args)...}
	{

	}

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
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for Function
 */

template<size_t StackSize, size_t QueuedSignals, typename Function, typename... Args>
class StaticThread<StackSize, true, QueuedSignals, Function, Args...> : public Thread<Function, Args...>
{
public:

	/// base of StaticThread
	using Base = Thread<Function, Args...>;

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
			Base{&stack_, sizeof(stack_), priority, schedulingPolicy, &staticSignalsReceiver_,
					std::forward<Function>(function), std::forward<Args>(args)...},
			staticSignalsReceiver_{}
	{

	}

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
	StaticSignalsReceiver<QueuedSignals> staticSignalsReceiver_;
};

/**
 * \brief Helper factory function to make StaticThread object with partially deduced template arguments
 *
 * \param StackSize is the size of stack, bytes
 * \param CanReceiveSignals selects whether reception of signals is enabled (true) or disabled (false) for this thread
 * \param QueuedSignals is the max number of queued signals for this thread, relevant only if CanReceiveSignals == true,
 * 0 to disable queuing of signals for this thread
 * \param Function is the function that will be executed
 * \param Args are the arguments for Function
 *
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] schedulingPolicy is the scheduling policy of the thread
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return StaticThread object with partially deduced template arguments
 */

template<size_t StackSize, bool CanReceiveSignals = {}, size_t QueuedSignals = {}, typename Function, typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, Function, Args...> makeStaticThread(const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args)
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
 * \param Function is the function that will be executed
 * \param Args are the arguments for Function
 *
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return StaticThread object with partially deduced template arguments
 */

template<size_t StackSize, bool CanReceiveSignals = {}, size_t QueuedSignals = {}, typename Function, typename... Args>
StaticThread<StackSize, CanReceiveSignals, QueuedSignals, Function, Args...> makeStaticThread(const uint8_t priority,
		Function&& function, Args&&... args)
{
	return {priority, std::forward<Function>(function), std::forward<Args>(args)...};
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICTHREAD_HPP_
