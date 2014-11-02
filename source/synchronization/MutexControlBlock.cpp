/**
 * \file
 * \brief MutexControlBlock class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-02
 */

#include "distortos/scheduler/MutexControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

MutexControlBlock::MutexControlBlock() :
		blockedList_{getScheduler().getThreadControlBlockListAllocator(), ThreadControlBlock::State::BlockedOnMutex},
		owner_{}
{

}

void MutexControlBlock::lock()
{
	owner_ = &getScheduler().getCurrentThreadControlBlock();
}

void MutexControlBlock::unlockOrTransferLock()
{
	if (blockedList_.empty() == false)
		transferLock();
	else
		unlock();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void MutexControlBlock::transferLock()
{
	owner_ = &blockedList_.begin()->get();	// pass ownership to the unblocked thread
	scheduler::getScheduler().unblock(blockedList_.begin());
}

void MutexControlBlock::unlock()
{
	owner_ = nullptr;
}

}	// namespace scheduler

}	// namespace distortos
