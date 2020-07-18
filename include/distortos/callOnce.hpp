/**
 * \file
 * \brief callOnce() header
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_CALLONCE_HPP_
#define INCLUDE_DISTORTOS_CALLONCE_HPP_

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/OnceFlag.hpp"

#include "estd/invoke.hpp"

#include <mutex>

namespace distortos
{

/**
 * \brief Executes the callable object exactly once, even if called from multiple threads.
 *
 * Similar to std::call_once() - https://en.cppreference.com/w/cpp/thread/call_once
 * Similar to POSIX pthread_once() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_once.html#
 *
 * \warning This function must not be called from interrupt context!
 *
 * \tparam Function is the function object that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] onceFlag is a reference to shared OnceFlag object
 * \param [in] function is the function object that will be executed
 * \param [in] args are arguments for \a function
 *
 * \ingroup synchronization
 */

template<typename Function, typename... Args>
void callOnce(OnceFlag& onceFlag, Function&& function, Args&&... args)
{
	CHECK_FUNCTION_CONTEXT();

	if (onceFlag.done_ == true)
		return;

	const std::lock_guard<Mutex> lockGuard {onceFlag.mutex_};

	if (onceFlag.done_ == true)
		return;

	estd::invoke(std::forward<Function>(function), std::forward<Args>(args)...);
	onceFlag.done_ = true;
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_CALLONCE_HPP_
