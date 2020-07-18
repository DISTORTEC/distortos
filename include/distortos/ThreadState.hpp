/**
 * \file
 * \brief ThreadState enum class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_THREADSTATE_HPP_
#define INCLUDE_DISTORTOS_THREADSTATE_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

/**
 * \brief state of the thread
 *
 * \ingroup threads
 */

enum class ThreadState : uint8_t
{
	/// state in which thread is created, before being added to Scheduler
	created,
	/// thread is runnable
	runnable,
	/// thread is terminated
	terminated,
	/// thread is sleeping
	sleeping,
	/// thread is blocked on Semaphore
	blockedOnSemaphore,
	/// thread is suspended
	suspended,
	/// thread is blocked on Mutex
	blockedOnMutex,
	/// thread is blocked on ConditionVariable
	blockedOnConditionVariable,

#if DISTORTOS_SIGNALS_ENABLE == 1

	/// thread is waiting for signal
	waitingForSignal,

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	/// internal thread object was detached
	detached,
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREADSTATE_HPP_
