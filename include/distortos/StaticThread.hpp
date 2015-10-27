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
 * \date 2015-10-27
 */

#ifndef INCLUDE_DISTORTOS_STATICTHREAD_HPP_
#define INCLUDE_DISTORTOS_STATICTHREAD_HPP_

#include "distortos/Thread.hpp"
#include "distortos/StaticSignalsReceiver.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief StaticThread class is a templated wrapper for thread that also provides automatic storage for stack.
 *
 * \note Objects of this class can be safely casted to (const) reference to Thread
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
class StaticThread
{
public:

	/// type of contained Thread object
	using ThreadType = Thread<Function, Args...>;

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
			thread_{{&stack_, memory::dummyDeleter<decltype(stack_)>}, sizeof(stack_), priority, schedulingPolicy,
					std::forward<Function>(function), std::forward<Args>(args)...}
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

	/**
	 * \brief conversion to ThreadType&
	 *
	 * \return reference to internal ThreadType object
	 */

	operator ThreadType&()
	{
		return thread_;
	}

	/**
	 * \brief conversion to const ThreadType&
	 *
	 * \return const reference to internal ThreadType object
	 */

	operator const ThreadType&() const
	{
		return thread_;
	}

	StaticThread(const StaticThread&) = delete;
	StaticThread(StaticThread&&) = default;
	const StaticThread& operator=(const StaticThread&) = delete;
	StaticThread& operator=(StaticThread&&) = delete;

private:

	/// stack buffer for \a thread_
	typename std::aligned_storage<StackSize>::type stack_;

	/// internal ThreadType object
	ThreadType thread_;
};

/**
 * \brief StaticThread class is a templated wrapper for thread that also provides automatic storage for stack and
 * internal StaticSignalsReceiver object.
 *
 * \note Objects of this class can be safely casted to (const) reference to Thread
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
class StaticThread<StackSize, true, QueuedSignals, SignalActions, Function, Args...>
{
public:

	/// type of contained Thread object
	using ThreadType = Thread<Function, Args...>;

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
			staticSignalsReceiver_{},
			thread_{{&stack_, memory::dummyDeleter<decltype(stack_)>}, sizeof(stack_), priority, schedulingPolicy,
					&static_cast<SignalsReceiver&>(staticSignalsReceiver_), std::forward<Function>(function),
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

	/**
	 * \brief conversion to ThreadType&
	 *
	 * \return reference to internal ThreadType object
	 */

	operator ThreadType&()
	{
		return thread_;
	}

	/**
	 * \brief conversion to const ThreadType&
	 *
	 * \return const reference to internal ThreadType object
	 */

	operator const ThreadType&() const
	{
		return thread_;
	}

	StaticThread(const StaticThread&) = delete;
	StaticThread(StaticThread&&) = default;
	const StaticThread& operator=(const StaticThread&) = delete;
	StaticThread& operator=(StaticThread&&) = delete;

private:

	/// stack buffer for \a thread_
	typename std::aligned_storage<StackSize>::type stack_;

	/// internal StaticSignalsReceiver object for \a thread_
	StaticSignalsReceiver<QueuedSignals, SignalActions> staticSignalsReceiver_;

	/// internal ThreadType object
	ThreadType thread_;
};

}	// namespace internal

/**
 * \brief StaticThread class is a templated wrapper for thread that also provides automatic storage for stack and
 * internal StaticSignalsReceiver object.
 *
 * \note Objects of this class can be safely casted to (const) reference to Thread
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
class StaticThread
{
public:

	/// type of contained internal::StaticThread object
	using StaticThreadType =
			internal::StaticThread<StackSize, CanReceiveSignals, QueuedSignals, SignalActions, Function, Args...>;

	/// type of contained internal::StaticThread::ThreadType object
	using ThreadType = typename StaticThreadType::ThreadType;

	/**
	 * \brief StaticThread's constructor
	 *
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	StaticThread(const uint8_t priority, const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
			staticThread_{priority, schedulingPolicy, std::forward<Function>(function), std::forward<Args>(args)...}
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
			staticThread_{priority, std::forward<Function>(function), std::forward<Args>(args)...}
	{

	}

	/**
	 * \brief conversion to ThreadType&
	 *
	 * \return reference to internal ThreadType object
	 */

	operator ThreadType&()
	{
		return staticThread_;
	}

	/**
	 * \brief conversion to const ThreadType&
	 *
	 * \return const reference to internal ThreadType object
	 */

	operator const ThreadType&() const
	{
		return staticThread_;
	}

	/**
	 * \brief Wrapper for Thread::generateSignal()
	 */

	int generateSignal(const uint8_t signalNumber)
	{
		return static_cast<ThreadType&>(*this).generateSignal(signalNumber);
	}

	/**
	 * \brief Wrapper for Thread::getEffectivePriority()
	 */

	uint8_t getEffectivePriority() const
	{
		return static_cast<const ThreadType&>(*this).getEffectivePriority();
	}

	/**
	 * \brief Wrapper for Thread::getPendingSignalSet()
	 */

	SignalSet getPendingSignalSet() const
	{
		return static_cast<const ThreadType&>(*this).getPendingSignalSet();
	}

	/**
	 * \brief Wrapper for Thread::getPriority()
	 */

	uint8_t getPriority() const
	{
		return static_cast<const ThreadType&>(*this).getPriority();
	}

	/**
	 * \brief Wrapper for Thread::getSchedulingPolicy()
	 */

	SchedulingPolicy getSchedulingPolicy() const
	{
		return static_cast<const ThreadType&>(*this).getSchedulingPolicy();
	}

	/**
	 * \brief Wrapper for Thread::getState()
	 */

	scheduler::ThreadControlBlock::State getState() const
	{
		return static_cast<const ThreadType&>(*this).getState();
	}

	/**
	 * \brief Wrapper for Thread::join()
	 */

	int join()
	{
		return static_cast<ThreadType&>(*this).join();
	}

	/**
	 * \brief Wrapper for Thread::queueSignal()
	 */

	int queueSignal(const uint8_t signalNumber, const sigval value)
	{
		return static_cast<ThreadType&>(*this).queueSignal(signalNumber, value);
	}

	/**
	 * \brief Wrapper for Thread::setPriority()
	 */

	void setPriority(const uint8_t priority, const bool alwaysBehind = {})
	{
		static_cast<ThreadType&>(*this).setPriority(priority, alwaysBehind);
	}

	/**
	 * \brief Wrapper for Thread::setSchedulingPolicy()
	 */

	void setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
	{
		static_cast<ThreadType&>(*this).setSchedulingPolicy(schedulingPolicy);
	}

	/**
	 * \brief Wrapper for Thread::start()
	 */

	int start()
	{
		return static_cast<ThreadType&>(*this).start();
	}

	StaticThread(const StaticThread&) = delete;
	StaticThread(StaticThread&&) = default;
	const StaticThread& operator=(const StaticThread&) = delete;
	StaticThread& operator=(StaticThread&&) = delete;

private:

	/// internal StaticThreadType object
	StaticThreadType staticThread_;
};

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
