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
 * \date 2015-12-24
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_
#define INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_

#include "distortos/internal/scheduler/DynamicThreadBase.hpp"

namespace distortos
{

/// \addtogroup threads
/// \{

/**
 * \brief DynamicThread class is a type-erased interface for thread that has dynamic storage for bounded function, stack
 * and internal DynamicSignalsReceiver object.
 */

class DynamicThread : public internal::DynamicThreadBase
{
public:

	using internal::DynamicThreadBase::DynamicThreadBase;
};

/**
 * \brief Helper factory function to make DynamicThread object
 *
 * \tparam Function is the function that will be executed
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
 *
 * \return DynamicThread object
 */

template<typename Function, typename... Args>
DynamicThread makeDynamicThread(const size_t stackSize, const bool canReceiveSignals, const size_t queuedSignals,
		const size_t signalActions, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		Function&& function, Args&&... args)
{
	return {stackSize, canReceiveSignals, queuedSignals, signalActions, priority, schedulingPolicy,
			std::forward<Function>(function), std::forward<Args>(args)...};
}

/**
 * \brief Helper factory function to make DynamicThread object
 *
 * \tparam Function is the function that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] parameters is a DynamicThreadParameters struct with thread parameters
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for \a function
 *
 * \return DynamicThread object
 */

template<typename Function, typename... Args>
DynamicThread makeDynamicThread(const DynamicThreadParameters parameters, Function&& function, Args&&... args)
{
	return {parameters, std::forward<Function>(function), std::forward<Args>(args)...};
}

/// \}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICTHREAD_HPP_
