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

int Semaphore::post()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (value_ == std::numeric_limits<decltype(value_)>::max())
		return EOVERFLOW;

	if (blockedList_.empty() == false)
	{
		scheduler::getScheduler().unblock(blockedList_.begin());
		return 0;
	}

	++value_;

	return 0;
}

int Semaphore::tryWait()
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	return tryWaitInternal();
}

int Semaphore::wait()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = tryWaitInternal();
	if (ret != EAGAIN)	// lock successful?
		return ret;

	scheduler::getScheduler().block(blockedList_);
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int Semaphore::tryWaitInternal()
{
	if (value_ == 0)	// lock not possible?
		return EAGAIN;

	--value_;

	return 0;
}

}	// namespace distortos
