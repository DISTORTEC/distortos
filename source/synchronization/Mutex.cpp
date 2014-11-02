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
 * \date 2014-11-02
 */

#include "distortos/Mutex.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Mutex::Mutex(const Type type) :
		controlBlock_{},
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

	controlBlock_.block();
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
	if (ret != EBUSY)	// lock successful, recursive lock not possible or deadlock detected?
		return ret;

	return controlBlock_.blockUntil(timePoint);
}

int Mutex::unlock()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (type_ != Type::Normal)
	{
		if (controlBlock_.getOwner() != &scheduler::getScheduler().getCurrentThreadControlBlock())
			return EPERM;

		if (type_ == Type::Recursive && recursiveLocksCount_ != 0)
		{
			--recursiveLocksCount_;
			return 0;
		}
	}

	controlBlock_.unlockOrTransferLock();

	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int Mutex::tryLockInternal()
{
	if (controlBlock_.getOwner() == nullptr)
	{
		controlBlock_.lock();
		return 0;
	}

	if (type_ == Type::Normal)
		return EBUSY;

	if (controlBlock_.getOwner() == &scheduler::getScheduler().getCurrentThreadControlBlock())
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

}	// namespace distortos
