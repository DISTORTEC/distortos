/**
 * \file
 * \brief DynamicThread class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-17
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_
#define INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_

#include "distortos/DynamicSignalsReceiver.hpp"
#include "distortos/Thread.hpp"

namespace distortos
{

/**
 * \brief DynamicThreadParameters struct is a helper with parameters for DynamicThread's constructor
 *
 * This struct is a replacement for overloads of DynamicThread's constructor and makeDynamicThread() which -
 * unfortunately - cannot be used, as they would lead to compilation errors due to ambiguity.
 */

struct DynamicThreadParameters
{
	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] canReceiveSignalss selects whether reception of signals is enabled (true) or disabled (false) for
	 * this thread
	 * \param [in] queuedSignalss is the max number of queued signals for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
	 * \param [in] signalActionss is the max number of different SignalAction objects for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicyy is the scheduling policy of the thread
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const bool canReceiveSignalss,
			const size_t queuedSignalss, const size_t signalActionss, const uint8_t priorityy,
			const SchedulingPolicy schedulingPolicyy) :
			stackSize{stackSizee},
			canReceiveSignals{canReceiveSignalss},
			queuedSignals{queuedSignalss},
			signalActions{signalActionss},
			priority{priorityy},
			schedulingPolicy{schedulingPolicyy}
	{

	}

	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] canReceiveSignalss selects whether reception of signals is enabled (true) or disabled (false) for
	 * this thread
	 * \param [in] queuedSignalss is the max number of queued signals for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable queuing of signals for this thread
	 * \param [in] signalActionss is the max number of different SignalAction objects for this thread, relevant only if
	 * \a canReceiveSignals == true, 0 to disable catching of signals for this thread
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const bool canReceiveSignalss,
			const size_t queuedSignalss, const size_t signalActionss, const uint8_t priorityy) :
			stackSize{stackSizee},
			canReceiveSignals{canReceiveSignalss},
			queuedSignals{queuedSignalss},
			signalActions{signalActionss},
			priority{priorityy},
			schedulingPolicy{SchedulingPolicy::RoundRobin}
	{

	}

	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicyy is the scheduling policy of the thread
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const uint8_t priorityy,
			const SchedulingPolicy schedulingPolicyy) :
			stackSize{stackSizee},
			canReceiveSignals{},
			queuedSignals{},
			signalActions{},
			priority{priorityy},
			schedulingPolicy{schedulingPolicyy}
	{

	}

	/**
	 * \brief DynamicThreadParameters's constructor
	 *
	 * \param [in] stackSizee is the size of stack, bytes
	 * \param [in] priorityy is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	constexpr DynamicThreadParameters(const size_t stackSizee, const uint8_t priorityy) :
			stackSize{stackSizee},
			canReceiveSignals{},
			queuedSignals{},
			signalActions{},
			priority{priorityy},
			schedulingPolicy{SchedulingPolicy::RoundRobin}
	{

	}

	/// size of stack, bytes
	const size_t stackSize;

	/// selects whether reception of signals is enabled (true) or disabled (false) for this thread
	const bool canReceiveSignals;

	/// max number of queued signals for this thread, relevant only if \a canReceiveSignals == true, 0 to disable
	/// queuing of signals for this thread
	const size_t queuedSignals;

	/// max number of different SignalAction objects for this thread, relevant only if \a canReceiveSignals == true, 0
	/// to disable catching of signals for this thread
	const size_t signalActions;

	/// thread's priority, 0 - lowest, UINT8_MAX - highest
	const uint8_t priority;

	/// scheduling policy of the thread
	SchedulingPolicy schedulingPolicy;
};

/**
 * \brief StaticThread class is a templated wrapper for thread that also provides dynamic storage for stack and internal
 * DynamicSignalsReceiver object.
 *
 * \note Objects of this class can be safely casted to (const) reference to Thread
 *
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for \a Function
 */

template<typename Function, typename... Args>
class DynamicThread
{
public:

	/// type of contained Thread object
	using ThreadType = Thread<Function, Args...>;

	/// unique_ptr to stack buffer
	using StackUniquePointer = std::unique_ptr<uint8_t>;

	/**
	 * \brief DynamicThread's constructor
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

	DynamicThread(size_t stackSize, bool canReceiveSignals, size_t queuedSignals, size_t signalActions,
			uint8_t priority, SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args);

	/**
	 * \brief DynamicThread's constructor
	 *
	 * \param [in] parameters is a DynamicThreadParameters struct with thread parameters
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for \a function
	 */

	DynamicThread(const DynamicThreadParameters parameters, Function&& function, Args&&... args) :
			DynamicThread{parameters.stackSize, parameters.canReceiveSignals, parameters.queuedSignals,
					parameters.signalActions, parameters.priority, parameters.schedulingPolicy,
					std::forward<Function>(function), std::forward<Args>(args)...}
	{

	}

	/**
	 * \brief DynamicThread's destructor
	 */

	~DynamicThread();

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

	/**
	 * \brief Wrapper for Thread::generateSignal()
	 */

	int generateSignal(const uint8_t signalNumber)
	{
		return thread_.generateSignal(signalNumber);
	}

	/**
	 * \brief Wrapper for Thread::getEffectivePriority()
	 */

	uint8_t getEffectivePriority() const
	{
		return thread_.getEffectivePriority();
	}

	/**
	 * \brief Wrapper for Thread::getPendingSignalSet()
	 */

	SignalSet getPendingSignalSet() const
	{
		return thread_.getPendingSignalSet();
	}

	/**
	 * \brief Wrapper for Thread::getPriority()
	 */

	uint8_t getPriority() const
	{
		return thread_.getPriority();
	}

	/**
	 * \brief Wrapper for Thread::getSchedulingPolicy()
	 */

	SchedulingPolicy getSchedulingPolicy() const
	{
		return thread_.getSchedulingPolicy();
	}

	/**
	 * \brief Wrapper for Thread::getState()
	 */

	scheduler::ThreadControlBlock::State getState() const
	{
		return thread_.getState();
	}

	/**
	 * \brief Wrapper for Thread::join()
	 */

	int join()
	{
		return thread_.join();
	}

	/**
	 * \brief Wrapper for Thread::queueSignal()
	 */

	int queueSignal(const uint8_t signalNumber, const sigval value)
	{
		return thread_.queueSignal(signalNumber, value);
	}

	/**
	 * \brief Wrapper for Thread::setPriority()
	 */

	void setPriority(const uint8_t priority, const bool alwaysBehind = {})
	{
		thread_.setPriority(priority, alwaysBehind);
	}

	/**
	 * \brief Wrapper for Thread::setSchedulingPolicy()
	 */

	void setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
	{
		thread_.setSchedulingPolicy(schedulingPolicy);
	}

	/**
	 * \brief Wrapper for Thread::start()
	 */

	int start()
	{
		return thread_.start();
	}

	DynamicThread(const DynamicThread&) = delete;
	DynamicThread(DynamicThread&&) = default;
	const DynamicThread& operator=(const DynamicThread&) = delete;
	DynamicThread& operator=(DynamicThread&&) = delete;

private:

	/// unique_ptr to allocated stack buffer for \a thread_
	StackUniquePointer stackUniquePointer_;

	/// internal DynamicSignalsReceiver object for \a thread_
	DynamicSignalsReceiver dynamicSignalsReceiver_;

	/// internal ThreadType object
	ThreadType thread_;
};

template<typename Function, typename... Args>
DynamicThread<Function, Args...>::DynamicThread(const size_t stackSize, const bool canReceiveSignals,
		const size_t queuedSignals, const size_t signalActions, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		stackUniquePointer_{new uint8_t[stackSize]},
		dynamicSignalsReceiver_{canReceiveSignals == true ? queuedSignals : 0,
				canReceiveSignals == true ? signalActions : 0},
		thread_{stackUniquePointer_.get(), stackSize, priority, schedulingPolicy,
				canReceiveSignals == true ? &static_cast<SignalsReceiver&>(dynamicSignalsReceiver_) : nullptr,
				std::forward<Function>(function), std::forward<Args>(args)...}
{

}

template<typename Function, typename... Args>
DynamicThread<Function, Args...>::~DynamicThread()
{

}

/**
 * \brief Helper factory function to make DynamicThread object with deduced template arguments
 *
 * \param Function is the function that will be executed
 * \param Args are the arguments for \a Function
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
 *
 * \return DynamicThread object with deduced template arguments
 */

template<typename Function, typename... Args>
DynamicThread<Function, Args...> makeDynamicThread(const size_t stackSize, const bool canReceiveSignals,
		const size_t queuedSignals, const size_t signalActions, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args)
{
	return {stackSize, canReceiveSignals, queuedSignals, signalActions, priority, schedulingPolicy,
			std::forward<Function>(function), std::forward<Args>(args)...};
}

/**
 * \brief Helper factory function to make DynamicThread object with deduced template arguments
 *
 * \param Function is the function that will be executed
 * \param Args are the arguments for \a Function
 *
 * \param [in] parameters is a DynamicThreadParameters struct with thread parameters
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for \a function
 *
 * \return DynamicThread object with deduced template arguments
 */

template<typename Function, typename... Args>
DynamicThread<Function, Args...> makeDynamicThread(const DynamicThreadParameters parameters, Function&& function,
		Args&&... args)
{
	return {parameters, std::forward<Function>(function), std::forward<Args>(args)...};
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_
