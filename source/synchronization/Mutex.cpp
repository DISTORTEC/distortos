/**
 * \file
 * \brief Mutex class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/Mutex.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int Mutex::lock()
{
	const InterruptMaskingLock interruptMaskingLock;

	int ret;
	// break the loop when one of following conditions is true:
	// - lock successful, recursive lock not possible or deadlock detected;
	// - lock transferred successfully;
	while ((ret = tryLockInternal()) == EBUSY && (ret = doBlock()) == EINTR);
	return ret;
}

int Mutex::tryLock()
{
	const InterruptMaskingLock interruptMaskingLock;
	const auto ret = tryLockInternal();
	return ret != EDEADLK ? ret : EBUSY;
}

int Mutex::tryLockFor(const TickClock::duration duration)
{
	return tryLockUntil(TickClock::now() + duration + TickClock::duration{1});
}

int Mutex::tryLockUntil(const TickClock::time_point timePoint)
{
	const InterruptMaskingLock interruptMaskingLock;

	int ret;
	// break the loop when one of following conditions is true:
	// - lock successful, recursive lock not possible or deadlock detected;
	// - lock transferred successfully;
	// - timeout expired;
	while ((ret = tryLockInternal()) == EBUSY && (ret = doBlockUntil(timePoint)) == EINTR);
	return ret;
}

int Mutex::unlock()
{
	CHECK_FUNCTION_CONTEXT();

	const InterruptMaskingLock interruptMaskingLock;

	if (getType() != Type::normal)
	{
		if (getOwner() != &internal::getScheduler().getCurrentThreadControlBlock())
			return EPERM;

		if (getType() == Type::recursive && getRecursiveLocksCount() != 0)
		{
			--getRecursiveLocksCount();
			return 0;
		}
	}

	doUnlockOrTransferLock();

	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int Mutex::tryLockInternal()
{
	CHECK_FUNCTION_CONTEXT();

	if (getProtocol() == Protocol::priorityProtect &&
			internal::getScheduler().getCurrentThreadControlBlock().getPriority() > getPriorityCeiling())
		return EINVAL;

	if (getOwner() == nullptr)
	{
		doLock();
		return 0;
	}

	if (getType() == Type::normal)
		return EBUSY;

	if (getOwner() == &internal::getScheduler().getCurrentThreadControlBlock())
	{
		if (getType() == Type::errorChecking)
			return EDEADLK;

		if (getType() == Type::recursive)
		{
			if (getRecursiveLocksCount() == getMaxRecursiveLocks())
				return EAGAIN;

			++getRecursiveLocksCount();
			return 0;
		}
	}

	return EBUSY;
}

}	// namespace distortos
