/**
 * \file
 * \brief Implementation of C-API for distortos::Mutex
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/C-API/Mutex.h"

#include "distortos/fromCApi.hpp"
#include "distortos/Mutex.hpp"

#ifndef DISTORTOS_UNIT_TEST

static_assert(sizeof(distortos_Mutex) == sizeof(distortos::Mutex),
		"Size of distortos_Mutex does not match size of distortos::Mutex!");
static_assert(alignof(distortos_Mutex) == alignof(distortos::Mutex),
		"Alignment of distortos_Mutex does not match alignment of distortos::Mutex!");

#endif	// !def DISTORTOS_UNIT_TEST

static_assert(distortos_Mutex_Protocol_none == static_cast<uint8_t>(distortos::Mutex::Protocol::none),
		"Value of distortos_Mutex_Protocol_none does not match value of distortos::Mutex::Protocol::none!");
static_assert(distortos_Mutex_Protocol_priorityInheritance ==
		static_cast<uint8_t>(distortos::Mutex::Protocol::priorityInheritance),
		"Value of distortos_Mutex_Protocol_priorityInheritance does not match value of "
		"distortos::Mutex::Protocol::priorityInheritance!");
static_assert(distortos_Mutex_Protocol_priorityProtect ==
		static_cast<uint8_t>(distortos::Mutex::Protocol::priorityProtect),
		"Value of distortos_Mutex_Protocol_priorityProtect does not match value of "
		"distortos::Mutex::Protocol::priorityProtect!");

static_assert(distortos_Mutex_Type_normal == static_cast<uint8_t>(distortos::Mutex::Type::normal),
		"Value of distortos_Mutex_Type_normal does not match value of distortos::Mutex::Type::normal!");
static_assert(distortos_Mutex_Type_errorChecking == static_cast<uint8_t>(distortos::Mutex::Type::errorChecking),
		"Value of distortos_Mutex_Type_errorChecking does not match value of distortos::Mutex::Type::errorChecking!");
static_assert(distortos_Mutex_Type_recursive == static_cast<uint8_t>(distortos::Mutex::Type::recursive),
		"Value of distortos_Mutex_Type_recursive does not match value of distortos::Mutex::Type::recursive!");

static_assert(distortos_Mutex_typeShift == distortos::internal::MutexControlBlock::typeShift,
		"Value of distortos_Mutex_typeShift does not match value of "
		"distortos::internal::MutexControlBlock::typeShift!");
static_assert(distortos_Mutex_protocolShift == distortos::internal::MutexControlBlock::protocolShift,
		"Value of distortos_Mutex_protocolShift does not match value of "
		"distortos::internal::MutexControlBlock::protocolShift!");

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int distortos_Mutex_construct_3(distortos_Mutex* const mutex, const uint8_t type, const uint8_t protocol,
		const uint8_t priorityCeiling)
{
	if (mutex == nullptr)
		return EINVAL;
	if (type != distortos_Mutex_Type_normal && type != distortos_Mutex_Type_errorChecking &&
			type != distortos_Mutex_Type_recursive)
		return EINVAL;
	if (protocol != distortos_Mutex_Protocol_none && protocol != distortos_Mutex_Protocol_priorityInheritance &&
			protocol != distortos_Mutex_Protocol_priorityProtect)
		return EINVAL;

	new (mutex) distortos::Mutex {static_cast<distortos::Mutex::Type>(type),
			static_cast<distortos::Mutex::Protocol>(protocol), priorityCeiling};
	return 0;
}

int distortos_Mutex_destruct(distortos_Mutex* const mutex)
{
	if (mutex == nullptr)
		return EINVAL;

	auto& realMutex = distortos::fromCApi(*mutex);
	realMutex.~Mutex();
	return 0;
}

int distortos_Mutex_lock(distortos_Mutex* const mutex)
{
	if (mutex == nullptr)
		return EINVAL;

	auto& realMutex = distortos::fromCApi(*mutex);
	return realMutex.lock();
}

int distortos_Mutex_tryLock(distortos_Mutex* const mutex)
{
	if (mutex == nullptr)
		return EINVAL;

	auto& realMutex = distortos::fromCApi(*mutex);
	return realMutex.tryLock();
}

int distortos_Mutex_tryLockFor(distortos_Mutex* const mutex, const int64_t duration)
{
	if (mutex == nullptr)
		return EINVAL;

	auto& realMutex = distortos::fromCApi(*mutex);
	return realMutex.tryLockFor(distortos::TickClock::duration{duration});
}

int distortos_Mutex_tryLockUntil(distortos_Mutex* const mutex, const int64_t timePoint)
{
	if (mutex == nullptr)
		return EINVAL;

	auto& realMutex = distortos::fromCApi(*mutex);
	return realMutex.tryLockUntil(distortos::TickClock::time_point{distortos::TickClock::duration{timePoint}});
}

int distortos_Mutex_unlock(distortos_Mutex* const mutex)
{
	if (mutex == nullptr)
		return EINVAL;

	auto& realMutex = distortos::fromCApi(*mutex);
	return realMutex.unlock();
}
