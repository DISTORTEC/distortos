/**
 * \file
 * \brief Implementation of C-API for distortos::Semaphore
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/C-API/Semaphore.h"

#include "distortos/fromCApi.hpp"
#include "distortos/Semaphore.hpp"

#ifndef DISTORTOS_UNIT_TEST

static_assert(sizeof(distortos_Semaphore) == sizeof(distortos::Semaphore),
		"Size of distortos_Semaphore does not match size of distortos::Semaphore!");
static_assert(alignof(distortos_Semaphore) == alignof(distortos::Semaphore),
		"Alignment of distortos_Semaphore does not match alignment of distortos::Semaphore!");

#endif	// !def DISTORTOS_UNIT_TEST

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int distortos_Semaphore_construct_1(distortos_Semaphore* const semaphore, const unsigned int value,
		const unsigned int maxValue)
{
	if (semaphore == nullptr)
		return EINVAL;

	new (semaphore) distortos::Semaphore {value, maxValue};
	return 0;
}

int distortos_Semaphore_destruct(distortos_Semaphore* const semaphore)
{
	if (semaphore == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	realSemaphore.~Semaphore();
	return 0;
}

int distortos_Semaphore_getMaxValue(const struct distortos_Semaphore* const semaphore, unsigned int* const maxValue)
{
	if (semaphore == nullptr || maxValue == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	*maxValue = realSemaphore.getMaxValue();
	return 0;
}

int distortos_Semaphore_getValue(const struct distortos_Semaphore* const semaphore, unsigned int* const value)
{
	if (semaphore == nullptr || value == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	*value = realSemaphore.getValue();
	return 0;
}

int distortos_Semaphore_post(distortos_Semaphore* const semaphore)
{
	if (semaphore == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	return realSemaphore.post();
}

int distortos_Semaphore_tryWait(distortos_Semaphore* const semaphore)
{
	if (semaphore == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	return realSemaphore.tryWait();
}

int distortos_Semaphore_tryWaitFor(distortos_Semaphore* const semaphore, const int64_t duration)
{
	if (semaphore == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	return realSemaphore.tryWaitFor(distortos::TickClock::duration{duration});
}

int distortos_Semaphore_tryWaitUntil(distortos_Semaphore* const semaphore, const int64_t timePoint)
{
	if (semaphore == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	return realSemaphore.tryWaitUntil(distortos::TickClock::time_point{distortos::TickClock::duration{timePoint}});
}

int distortos_Semaphore_wait(distortos_Semaphore* const semaphore)
{
	if (semaphore == nullptr)
		return EINVAL;

	auto& realSemaphore = distortos::fromCApi(*semaphore);
	return realSemaphore.wait();
}
