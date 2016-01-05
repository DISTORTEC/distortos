/**
 * \file
 * \brief callOnce() header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_CALLONCE_HPP_
#define INCLUDE_DISTORTOS_CALLONCE_HPP_

#include "distortos/OnceFlag.hpp"

#if DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1

namespace distortos
{

/**
 * \brief Executes the callable object exactly once, even if called from multiple threads.
 *
 * Similar to std::call_once() - http://en.cppreference.com/w/cpp/thread/call_once
 * Similar to POSIX pthread_once() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_once.html#
 *
 * \note This function requires GCC 4.9.
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
	onceFlag.callOnceControlBlock_(std::forward<Function>(function), std::forward<Args>(args)...);
}

}	// namespace distortos

#endif	// DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1

#endif	// INCLUDE_DISTORTOS_CALLONCE_HPP_
