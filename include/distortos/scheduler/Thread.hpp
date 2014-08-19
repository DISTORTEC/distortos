/**
 * \file
 * \brief Thread class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-19
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREAD_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREAD_HPP_

#include "distortos/scheduler/ThreadBase.hpp"

#include <type_traits>
#include <functional>

namespace distortos
{

namespace scheduler
{

/**
 * \brief Thread class is a templated interface for thread
 *
 * \param Function is the function that will be executed in separate thread
 * \param Args are the arguments for Function
 */

template<typename Function, typename... Args>
class Thread : private ThreadBase
{
public:

	/// base of Thread
	using Base = ThreadBase;

	/**
	 * \brief Thread's constructor
	 *
	 * \param [in] function is a function that will be executed in separate thread
	 * \param [in] args are arguments for function
	 */

	Thread(void* const buffer, const size_t size, const uint8_t priority, Function&& function, Args&&... args) :
			ThreadBase{buffer, size, priority},
			boundFunction_{std::bind(std::forward<Function>(function), std::forward<Args>(args)...)}
	{

	}

	using ThreadBase::join;

	using ThreadBase::start;

private:

	/**
	 * \brief Thread's internal function.
	 *
	 * Executes bound function object.
	 */

	virtual void run_() override { boundFunction_(); }

	/// bound function object
	decltype(std::bind(std::declval<Function>(), std::declval<Args>()...)) boundFunction_;
};

/**
 * \brief Helper factory function to make Thread object with deduced template arguments
 *
 * \param Function is the function that will be executed
 * \param Args are the arguments for Function
 *
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

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREAD_HPP_
