/**
 * \file
 * \brief Implementation of C-API for distortos::ConditionVariable
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/C-API/ConditionVariable.h"

#include "distortos/ConditionVariable.hpp"
#include "distortos/fromCApi.hpp"

#ifndef DISTORTOS_UNIT_TEST

static_assert(sizeof(distortos_ConditionVariable) == sizeof(distortos::ConditionVariable),
		"Size of distortos_ConditionVariable does not match size of distortos::ConditionVariable!");
static_assert(alignof(distortos_ConditionVariable) == alignof(distortos::ConditionVariable),
		"Alignment of distortos_ConditionVariable does not match alignment of distortos::ConditionVariable!");

#endif	// !def DISTORTOS_UNIT_TEST

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int distortos_ConditionVariable_construct(distortos_ConditionVariable* const conditionVariable)
{
	if (conditionVariable == nullptr)
		return EINVAL;

	new (conditionVariable) distortos::ConditionVariable;
	return 0;
}

int distortos_ConditionVariable_destruct(distortos_ConditionVariable* const conditionVariable)
{
	if (conditionVariable == nullptr)
		return EINVAL;

	auto& realConditionVariable = distortos::fromCApi(*conditionVariable);
	realConditionVariable.~ConditionVariable();
	return 0;
}

int distortos_ConditionVariable_notifyAll(distortos_ConditionVariable* const conditionVariable)
{
	if (conditionVariable == nullptr)
		return EINVAL;

	auto& realConditionVariable = distortos::fromCApi(*conditionVariable);
	realConditionVariable.notifyAll();
	return 0;
}

int distortos_ConditionVariable_notifyOne(distortos_ConditionVariable* const conditionVariable)
{
	if (conditionVariable == nullptr)
		return EINVAL;

	auto& realConditionVariable = distortos::fromCApi(*conditionVariable);
	realConditionVariable.notifyOne();
	return 0;
}

int distortos_ConditionVariable_wait(distortos_ConditionVariable* const conditionVariable, distortos_Mutex* const mutex)
{
	if (conditionVariable == nullptr || mutex == nullptr)
		return EINVAL;

	auto& realConditionVariable = distortos::fromCApi(*conditionVariable);
	auto& realMutex = distortos::fromCApi(*mutex);
	return realConditionVariable.wait(realMutex);
}

int distortos_ConditionVariable_waitFor(distortos_ConditionVariable* const conditionVariable,
		distortos_Mutex* const mutex, const int64_t duration)
{
	if (conditionVariable == nullptr || mutex == nullptr)
		return EINVAL;

	auto& realConditionVariable = distortos::fromCApi(*conditionVariable);
	auto& realMutex = distortos::fromCApi(*mutex);
	return realConditionVariable.waitFor(realMutex, distortos::TickClock::duration{duration});
}

int distortos_ConditionVariable_waitUntil(distortos_ConditionVariable* const conditionVariable,
		distortos_Mutex* const mutex, const int64_t timePoint)
{
	if (conditionVariable == nullptr || mutex == nullptr)
		return EINVAL;

	auto& realConditionVariable = distortos::fromCApi(*conditionVariable);
	auto& realMutex = distortos::fromCApi(*mutex);
	return realConditionVariable.waitUntil(realMutex,
			distortos::TickClock::time_point{distortos::TickClock::duration{timePoint}});
}
