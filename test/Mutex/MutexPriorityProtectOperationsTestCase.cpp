/**
 * \file
 * \brief MutexPriorityProtectOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-09
 */

#include "MutexPriorityProtectOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/Mutex.hpp"
#include "distortos/ThisThread.hpp"

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

/// priority of current test thread
constexpr uint8_t testThreadPriority {UINT8_MAX};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Runs the test case.
 *
 * \attention this function expects the priority of test thread to be testThreadPriority
 *
 * \return true if the test case succeeded, false otherwise
 */

bool testRunner_()
{
	static const Mutex::Type types[]
	{
			Mutex::Type::Normal,
			Mutex::Type::ErrorChecking,
			Mutex::Type::Recursive,
	};

	for (const auto type : types)
	{
		Mutex mutex {type, Mutex::Protocol::PriorityProtect, testThreadPriority - 1};

		{
			// invalid lock attempt - must fail with EINVAL immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.lock();
			if (ret != EINVAL || start != TickClock::now())
				return false;
		}

		{
			// invalid lock attempt - must fail with EINVAL immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLock();
			if (ret != EINVAL || start != TickClock::now())
				return false;
		}

		{
			// invalid lock attempt - must fail with EINVAL immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockFor(singleDuration);
			if (ret != EINVAL || start != TickClock::now())
				return false;
		}

		{
			// invalid lock attempt - must fail with EINVAL immediately
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = mutex.tryLockUntil(start + singleDuration);
			if (ret != EINVAL || start != TickClock::now())
				return false;
		}
	}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MutexPriorityProtectOperationsTestCase::run_() const
{
	const auto thisThreadPriority = ThisThread::getPriority();
	ThisThread::setPriority(testThreadPriority);
	const auto ret = testRunner_();
	ThisThread::setPriority(thisThreadPriority);
	return ret;
}

}	// namespace test

}	// namespace distortos
