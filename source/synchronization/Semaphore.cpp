/**
 * \file
 * \brief Semaphore class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-18
 */

#include "distortos/Semaphore.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Semaphore::Semaphore(int value) :
		blockedList_{scheduler::getScheduler().getThreadControlBlockListAllocator(),
				scheduler::ThreadControlBlock::State::BlockedOnSemaphore},
		value_{value}
{

}

Semaphore::~Semaphore()
{

}

void Semaphore::post()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	++value_;

	if (blockedList_.empty() == false)
		scheduler::getScheduler().unblock(blockedList_.begin());
}

int Semaphore::tryWait()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (value_ <= 0)	// lock not possible?
		return EAGAIN;

	--value_;

	return 0;
}

int Semaphore::wait()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	--value_;

	if (value_ < 0)
		scheduler::getScheduler().block(blockedList_);

	return 0;
}

}	// namespace distortos
