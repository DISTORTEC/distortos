/**
 * \file
 * \brief MutexRecursiveOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "MutexRecursiveOperationsTestCase.hpp"

#include "mutexTestUnlockFromWrongThread.hpp"
#include "mutexTestTryLockWhenLocked.hpp"
#include "waitForNextTick.hpp"

#include "distortos/Mutex.hpp"

#include <cerrno>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// single duration used in tests
constexpr auto singleDuration = TickClock::duration{1};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MutexRecursiveOperationsTestCase::run_() const
{
	using Parameters = std::pair<Mutex::Protocol, uint8_t>;
	static const Parameters parametersArray[]
	{
			Parameters{Mutex::Protocol::none, {}},
			Parameters{Mutex::Protocol::priorityProtect, UINT8_MAX},
			Parameters{Mutex::Protocol::priorityProtect, testCasePriority_},
			Parameters{Mutex::Protocol::priorityInheritance, {}},
	};

	for (const auto& parameters : parametersArray)
	{
		Mutex mutex {Mutex::Type::recursive, parameters.first, parameters.second};

		size_t lockCount {};

		{
			// simple lock - must succeed immediately
			++lockCount;
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.lock();
			if (ret != 0 || start != TickClock::now())
				return false;
		}

		const auto maxRecursiveLocks = mutex.getMaxRecursiveLocks();
		if (maxRecursiveLocks < 4)
			return false;

		{
			// recursive lock - must succeed immediately
			++lockCount;
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.lock();
			if (ret != 0 || start != TickClock::now())
				return false;
		}

		{
			// recursive lock - must succeed immediately
			++lockCount;
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLock();
			if (ret != 0 || start != TickClock::now())
				return false;
		}

		{
			// recursive lock - must succeed immediately
			++lockCount;
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockFor(singleDuration);
			if (ret != 0 || start != TickClock::now())
				return false;
		}

		{
			// recursive lock - must succeed immediately
			++lockCount;
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockUntil(start + singleDuration);
			if (ret != 0 || start != TickClock::now())
				return false;
		}

		while (lockCount < maxRecursiveLocks + 1)
		{
			// recursive lock - must succeed
			++lockCount;
			const auto ret = mutex.tryLock();
			if (ret != 0)
				return false;
		}

		{
			// excessive recursive lock - must fail with EAGAIN immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.lock();
			if (ret != EAGAIN || start != TickClock::now())
				return false;
		}

		{
			// excessive recursive lock - must fail with EAGAIN immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLock();
			if (ret != EAGAIN || start != TickClock::now())
				return false;
		}

		{
			// excessive recursive lock - must fail with EAGAIN immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockFor(singleDuration);
			if (ret != EAGAIN || start != TickClock::now())
				return false;
		}

		{
			// excessive recursive lock - must fail with EAGAIN immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockUntil(start + singleDuration);
			if (ret != EAGAIN || start != TickClock::now())
				return false;
		}

		{
			const auto ret = mutexTestTryLockWhenLocked(mutex, testCasePriority_);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = mutexTestUnlockFromWrongThread(mutex);
			if (ret != true)
				return ret;
		}

		{
			// simple unlock - must succeed immediately
			--lockCount;
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.unlock();
			if (ret != 0 || start != TickClock::now())
				return false;
		}

		{
			const auto ret = mutexTestTryLockWhenLocked(mutex, testCasePriority_);
			if (ret != true)
				return ret;
		}

		while (lockCount > 0)
		{
			// simple unlock - must succeed
			--lockCount;
			const auto ret = mutex.unlock();
			if (ret != 0)
				return false;
		}

		{
			// excessive unlock - must fail with EPERM immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.unlock();
			if (ret != EPERM || start != TickClock::now())
				return false;
		}
	}

	return true;
}

}	// namespace test

}	// namespace distortos
