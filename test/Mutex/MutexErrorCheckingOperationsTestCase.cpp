/**
 * \file
 * \brief MutexErrorCheckingOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "MutexErrorCheckingOperationsTestCase.hpp"

#include "waitForNextTick.hpp"
#include "mutexTestUnlockFromWrongThread.hpp"

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

bool MutexErrorCheckingOperationsTestCase::run_() const
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
		Mutex mutex {Mutex::Type::errorChecking, parameters.first, parameters.second};

		{
			// simple lock - must succeed immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.lock();
			if (ret != 0 || start != TickClock::now())
				return false;
		}

		{
			// re-lock attempt - must fail with EDEADLK immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.lock();
			if (ret != EDEADLK || start != TickClock::now())
				return false;
		}

		{
			// re-lock attempt - must fail with EDEADLK immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockFor(singleDuration);
			if (ret != EDEADLK || start != TickClock::now())
				return false;
		}

		{
			// re-lock attempt - must fail with EDEADLK immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockUntil(start + singleDuration);
			if (ret != EDEADLK || start != TickClock::now())
				return false;
		}

		{
			const auto ret = mutexTestUnlockFromWrongThread(mutex);
			if (ret != true)
				return ret;
		}

		{
			// simple unlock - must succeed immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.unlock();
			if (ret != 0 || start != TickClock::now())
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
