/**
 * \file
 * \brief ThreadBase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-14
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"

namespace distortos
{

namespace scheduler
{

/// ThreadBase class is a base for threads
class ThreadBase : public ThreadControlBlock
{
public:

	/**
	 * \brief ThreadBase's constructor.
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	ThreadBase(void* buffer, size_t size, uint8_t priority);

	/**
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "New" state only.
	 *
	 * \param [in] scheduler is a reference to Scheduler with which the thread will be associated, schedulerInstance if
	 * not provided
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - thread is already started
	 */

	int start(Scheduler& scheduler = schedulerInstance);
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
