/**
 * \file
 * \brief MutexControlBlock class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-02
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_MUTEXCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_MUTEXCONTROLBLOCK_HPP_

#include "distortos/scheduler/ThreadControlBlockList.hpp"

namespace distortos
{

namespace scheduler
{

/// MutexControlBlock class is a control block for Mutex
class MutexControlBlock
{
public:

	/**
	 * \brief MutexControlBlock constructor
	 */

	explicit MutexControlBlock();

	/**
	 * \return reference to ThreadControlBlock objects blocked on mutex
	 */

	ThreadControlBlockList& getBlockedList()
	{
		return blockedList_;
	}

	/**
	 * \return reference to pointer to owner of the mutex
	 */

	ThreadControlBlock*& getOwner()
	{
		return owner_;
	}

	/**
	 * \brief Performs actual locking of previously unlocked mutex.
	 *
	 * \attention mutex must be unlocked
	 */

	void lock();

private:

	/// ThreadControlBlock objects blocked on mutex
	ThreadControlBlockList blockedList_;

	/// owner of the mutex
	ThreadControlBlock* owner_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_MUTEXCONTROLBLOCK_HPP_
