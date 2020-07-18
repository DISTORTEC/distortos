/**
 * \file
 * \brief Semaphore class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/Semaphore.hpp"

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

int Semaphore::post()
{
	const InterruptMaskingLock interruptMaskingLock;

	if (value_ == maxValue_)
		return EOVERFLOW;

	if (blockedList_.empty() == false)
	{
		internal::getScheduler().unblock(blockedList_.begin());
		return 0;
	}

	++value_;

	return 0;
}

int Semaphore::tryWait()
{
	const InterruptMaskingLock interruptMaskingLock;
	return tryWaitInternal();
}

int Semaphore::tryWaitFor(const TickClock::duration duration)
{
	return tryWaitUntil(TickClock::now() + duration + TickClock::duration{1});
}

int Semaphore::tryWaitUntil(const TickClock::time_point timePoint)
{
	CHECK_FUNCTION_CONTEXT();

	const InterruptMaskingLock interruptMaskingLock;

	const auto ret = tryWaitInternal();
	if (ret != EAGAIN)	// lock successful?
		return ret;

	return internal::getScheduler().blockUntil(blockedList_, ThreadState::blockedOnSemaphore, timePoint);
}

int Semaphore::wait()
{
	CHECK_FUNCTION_CONTEXT();

	const InterruptMaskingLock interruptMaskingLock;

	const auto ret = tryWaitInternal();
	if (ret != EAGAIN)	// lock successful?
		return ret;

	return internal::getScheduler().block(blockedList_, ThreadState::blockedOnSemaphore);
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
