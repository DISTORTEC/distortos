/**
 * \file
 * \brief MutexErrorCheckingOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-27
 */

#include "MutexErrorCheckingOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/scheduler/Mutex.hpp"
#include "distortos/scheduler/StaticThread.hpp"

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
constexpr auto singleDuration = distortos::scheduler::TickClock::duration{1};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {256};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MutexErrorCheckingOperationsTestCase::run_() const
{
	using distortos::scheduler::TickClock;
	using distortos::scheduler::makeStaticThread;

	distortos::scheduler::Mutex mutex {distortos::scheduler::Mutex::Type::ErrorChecking};

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
		// unlock from a wrong thread - must fail with EPERM immediately
		bool sharedRet {};
		auto unlockThreadObject = makeStaticThread<testThreadStackSize>(UINT8_MAX,
				[&mutex, &sharedRet]()
				{
					using distortos::scheduler::TickClock;

					const auto start = TickClock::now();
					const auto ret = mutex.unlock();
					sharedRet = ret == EPERM && start == TickClock::now();
				});
		waitForNextTick();
		unlockThreadObject.start();
		unlockThreadObject.join();
		if (sharedRet != true)
			return sharedRet;
	}

	{
		// simple unlock - must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = mutex.unlock();
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
