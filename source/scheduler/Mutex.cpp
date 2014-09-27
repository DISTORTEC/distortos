/**
 * \file
 * \brief Mutex class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-27
 */

#include "distortos/scheduler/Mutex.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Mutex::Mutex(const Type type) :
		blockedList_{schedulerInstance.getThreadControlBlockListAllocator(), ThreadControlBlock::State::BlockedOnMutex},
		owner_{},
		recursiveLocksCount_{},
		type_{type}
{

}

int Mutex::lock()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = tryLockInternal();
	if (ret != EBUSY)	// lock successful, recursive lock not possible or deadlock detected?
		return ret;

	schedulerInstance.block(blockedList_);
	return 0;
}

int Mutex::tryLock()
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	const auto ret = tryLockInternal();
	return ret != EDEADLK ? ret : EBUSY;
}

int Mutex::tryLockFor(const TickClock::duration duration)
{
	return tryLockUntil(TickClock::now() + duration + TickClock::duration{1});
}

int Mutex::tryLockUntil(const TickClock::time_point timePoint)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = tryLockInternal();
	if (ret == 0 || ret == EAGAIN)	// lock successful or recursive lock not possible?
		return ret;

	return schedulerInstance.blockUntil(blockedList_, timePoint);
}

int Mutex::unlock()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (type_ != Type::Normal)
	{
		if (owner_ != &schedulerInstance.getCurrentThreadControlBlock())
			return EPERM;

		if (type_ == Type::Recursive && recursiveLocksCount_ != 0)
		{
			--recursiveLocksCount_;
			return 0;
		}
	}

	if (blockedList_.empty() == false)
	{
		owner_ = &blockedList_.begin()->get();	// pass ownership to the unblocked thread
		schedulerInstance.unblock(blockedList_.begin());
	}
	else
		owner_ = nullptr;

	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int Mutex::tryLockInternal()
{
	if (owner_ == nullptr)
	{
		owner_ = &schedulerInstance.getCurrentThreadControlBlock();
		return 0;
	}

	if (type_ == Type::Normal)
		return EBUSY;

	if (owner_ == &schedulerInstance.getCurrentThreadControlBlock())
	{
		if (type_ == Type::ErrorChecking)
			return EDEADLK;

		if (type_ == Type::Recursive)
		{
			if (recursiveLocksCount_ == std::numeric_limits<decltype(recursiveLocksCount_)>::max())
				return EAGAIN;

			++recursiveLocksCount_;
			return 0;
		}
	}

	return EBUSY;
}

}	// namespace scheduler

}	// namespace distortos
