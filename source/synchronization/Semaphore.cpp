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
 * \date 2014-10-27
 */

#include "distortos/Semaphore.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Semaphore::Semaphore(int value) :
		blockedList_{scheduler::schedulerInstance.getThreadControlBlockListAllocator(),
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
		scheduler::schedulerInstance.unblock(blockedList_.begin());
}

int Semaphore::tryWait()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (value_ > 0)	// lock possible?
	{
		--value_;
		return 0;
	}

	return EAGAIN;
}

int Semaphore::wait()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	--value_;

	if (value_ < 0)
		scheduler::schedulerInstance.block(blockedList_);

	return 0;
}

}	// namespace distortos
