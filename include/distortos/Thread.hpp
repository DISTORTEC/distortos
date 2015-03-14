/**
 * \file
 * \brief Thread class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-14
 */

#ifndef INCLUDE_DISTORTOS_THREAD_HPP_
#define INCLUDE_DISTORTOS_THREAD_HPP_

#include "distortos/ThreadBase.hpp"

namespace distortos
{

/**
 * \brief Thread class is a templated interface for thread
 *
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for Function
 */

template<typename Function, typename... Args>
class Thread : public ThreadBase
{
public:

	/// base of Thread
	using Base = ThreadBase;

	/**
	 * \brief Thread's constructor
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	Thread(void* const buffer, const size_t size, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
			SignalsReceiver* const signalsReceiver, Function&& function, Args&&... args) :
			ThreadBase{buffer, size, priority, schedulingPolicy, nullptr, signalsReceiver},
			boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
	{

	}

	/**
	 * \brief Thread's constructor
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	Thread(void* const buffer, const size_t size, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
			Function&& function, Args&&... args) :
			Thread{buffer, size, priority, schedulingPolicy, nullptr, std::forward<Function>(function),
					std::forward<Args>(args)...}
	{

	}

	/**
	 * \brief Thread's constructor
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	Thread(void* const buffer, const size_t size, const uint8_t priority, Function&& function, Args&&... args) :
			Thread{buffer, size, priority, SchedulingPolicy::RoundRobin, nullptr, std::forward<Function>(function),
					std::forward<Args>(args)...}
	{

	}

	Thread(const Thread&) = delete;
	Thread(Thread&&) = default;
	const Thread& operator=(const Thread&) = delete;
	Thread& operator=(Thread&&) = delete;

private:

	/**
	 * \brief Thread's internal function.
	 *
	 * Executes bound function object.
	 */

	virtual void run() override { boundFunction_(); }

	/// bound function object
	decltype(std::bind(std::declval<Function>(), std::declval<Args>()...)) boundFunction_;
};

/**
 * \brief Helper factory function to make Thread object with deduced template arguments
 *
 * \param Function is the function that will be executed
 * \param Args are the arguments for Function
 *
 * \param [in] buffer is a pointer to stack's buffer
 * \param [in] size is the size of stack's buffer, bytes
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] schedulingPolicy is the scheduling policy of the thread
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return Thread object with deduced template arguments
 */

template<typename Function, typename... Args>
Thread<Function, Args...> makeThread(void* const buffer, const size_t size, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, Function&& function, Args&&... args)
{
	return {buffer, size, priority, schedulingPolicy, std::forward<Function>(function), std::forward<Args>(args)...};
}

/**
 * \brief Helper factory function to make Thread object with deduced template arguments
 *
 * \param Function is the function that will be executed
 * \param Args are the arguments for Function
 *
 * \param [in] buffer is a pointer to stack's buffer
 * \param [in] size is the size of stack's buffer, bytes
 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
 * \param [in] function is a function that will be executed in separate thread
 * \param [in] args are arguments for function
 *
 * \return Thread object with deduced template arguments
 */

template<typename Function, typename... Args>
Thread<Function, Args...> makeThread(void* const buffer, const size_t size, const uint8_t priority, Function&& function,
		Args&&... args)
{
	return {buffer, size, priority, std::forward<Function>(function), std::forward<Args>(args)...};
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREAD_HPP_
