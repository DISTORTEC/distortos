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
 * \date 2014-08-13
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_

#include "distortos/scheduler/ThreadControlBlock.hpp"

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
	 * \param [in] function is a reference to thread's function
	 * \param [in] arguments is an argument for function
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	ThreadBase(void* buffer, size_t size, void* (&function)(void*), void* arguments, uint8_t priority);

private:

	/**
	 * \brief "Run" function of thread
	 *
	 * \return value returned by function_(arguments_)
	 */

	virtual void* run_() const override;

	/// reference to thread's function
	void* (&function_)(void*);

	/// argument for function
	void* const arguments_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADBASE_HPP_
