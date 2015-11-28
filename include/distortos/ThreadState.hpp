/**
 * \file
 * \brief ThreadState enum class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-28
 */

#ifndef INCLUDE_DISTORTOS_THREADSTATE_HPP_
#define INCLUDE_DISTORTOS_THREADSTATE_HPP_

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
	New,
	/// thread is runnable
	Runnable,
	/// thread is terminated
	Terminated,
	/// thread is sleeping
	Sleeping,
	/// thread is blocked on Semaphore
	BlockedOnSemaphore,
	/// thread is suspended
	Suspended,
	/// thread is blocked on Mutex
	BlockedOnMutex,
	/// thread is blocked on ConditionVariable
	BlockedOnConditionVariable,
	/// thread is waiting for signal
	WaitingForSignal,
	/// thread is blocked on OnceFlag
	BlockedOnOnceFlag
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREADSTATE_HPP_
