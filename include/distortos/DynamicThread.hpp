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
 * \date 2015-10-27
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_
#define INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_

#include "distortos/DynamicSignalsReceiver.hpp"
#include "distortos/Thread.hpp"

#include "distortos/memory/storageDeleter.hpp"

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
 * \brief DynamicThread class is a templated interface for thread that has dynamic storage for stack and internal
 * DynamicSignalsReceiver object.
 *
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for \a Function
 */

template<typename Function, typename... Args>
class DynamicThread : public Thread<Function, Args...>
{
public:

	/// base of DynamicThread
	using Base = Thread<Function, Args...>;

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

	DynamicThread(const DynamicThread&) = delete;
	DynamicThread(DynamicThread&&) = default;
	const DynamicThread& operator=(const DynamicThread&) = delete;
	DynamicThread& operator=(DynamicThread&&) = delete;

private:

	/// internal DynamicSignalsReceiver object
	DynamicSignalsReceiver dynamicSignalsReceiver_;
};

template<typename Function, typename... Args>
DynamicThread<Function, Args...>::DynamicThread(const size_t stackSize, const bool canReceiveSignals,
		const size_t queuedSignals, const size_t signalActions, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args) :
		Base{{new uint8_t[stackSize], memory::storageDeleter<uint8_t>}, stackSize, priority, schedulingPolicy,
				canReceiveSignals == true ? &dynamicSignalsReceiver_ : nullptr, std::forward<Function>(function),
				std::forward<Args>(args)...},
		dynamicSignalsReceiver_{canReceiveSignals == true ? queuedSignals : 0,
				canReceiveSignals == true ? signalActions : 0}
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
