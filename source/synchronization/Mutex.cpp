/**
 * \file
 * \brief Mutex class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	while ((ret = tryLockInternal()) == EBUSY && (ret = controlBlock_.block()) == EINTR);
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
	while ((ret = tryLockInternal()) == EBUSY && (ret = controlBlock_.blockUntil(timePoint)) == EINTR);
	return ret;
}

int Mutex::unlock()
{
	CHECK_FUNCTION_CONTEXT();

	const InterruptMaskingLock interruptMaskingLock;

	if (type_ != Type::normal)
	{
		if (controlBlock_.getOwner() != &internal::getScheduler().getCurrentThreadControlBlock())
			return EPERM;

		if (type_ == Type::recursive && recursiveLocksCount_ != 0)
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
	CHECK_FUNCTION_CONTEXT();

	if (controlBlock_.getProtocol() == Protocol::priorityProtect &&
			internal::getScheduler().getCurrentThreadControlBlock().getPriority() > controlBlock_.getPriorityCeiling())
		return EINVAL;

	if (controlBlock_.getOwner() == nullptr)
	{
		controlBlock_.lock();
		return 0;
	}

	if (type_ == Type::normal)
		return EBUSY;

	if (controlBlock_.getOwner() == &internal::getScheduler().getCurrentThreadControlBlock())
	{
		if (type_ == Type::errorChecking)
			return EDEADLK;

		if (type_ == Type::recursive)
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
