/**
 * \file
 * \brief ConditionVariable class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-08
 */

#include "distortos/ConditionVariable.hpp"

#include "distortos/Mutex.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void ConditionVariable::notifyAll()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	while (blockedList_.empty() == false)
		internal::getScheduler().unblock(blockedList_.begin());
}

void ConditionVariable::notifyOne()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (blockedList_.empty() == false)
		internal::getScheduler().unblock(blockedList_.begin());
}

int ConditionVariable::wait(Mutex& mutex)
{
	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		const auto ret = mutex.unlock();
		if (ret != 0)
			return ret;

		internal::getScheduler().block(blockedList_, ThreadState::BlockedOnConditionVariable);
	}

	return mutex.lock();
}

int ConditionVariable::waitFor(Mutex& mutex, TickClock::duration duration)
{
	return waitUntil(mutex, TickClock::now() + duration + TickClock::duration{1});
}

int ConditionVariable::waitUntil(Mutex& mutex, const TickClock::time_point timePoint)
{
	int blockUntilRet {};

	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		const auto ret = mutex.unlock();
		if (ret != 0)
			return ret;

		blockUntilRet = internal::getScheduler().blockUntil(blockedList_, ThreadState::BlockedOnConditionVariable,
				timePoint);
	}

	const auto ret = mutex.lock();
	return ret != 0 ? ret : blockUntilRet != EINTR ? blockUntilRet : 0;	// don't return EINTR in case of spurious wakeup
}

}	// namespace distortos
