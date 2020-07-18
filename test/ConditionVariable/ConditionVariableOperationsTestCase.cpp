/**
 * \file
 * \brief ConditionVariableOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "ConditionVariableOperationsTestCase.hpp"

#include "waitForNextTick.hpp"
#include "Mutex/mutexTestTryLockWhenLocked.hpp"

#include "distortos/ConditionVariable.hpp"
#include "distortos/DynamicThread.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/StaticSoftwareTimer.hpp"
#include "distortos/statistics.hpp"

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

/// long duration used in tests
constexpr auto longDuration = singleDuration * 10;

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/// expected number of context switches in testMutexAndUnlock(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) testMutexAndUnlockContextSwitchCount
{
		waitForNextTickContextSwitchCount + 2
};

/// expected number of context switches in phase1 block involving waitFor() or waitUntil() (excluding
/// waitForNextTick() and testMutexAndUnlock()): 1 - main thread blocks on condition variable (main -> idle), 2 - main
/// thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1WaitForUntilContextSwitchCount {2};

/// expected number of context switches in phase2 block involving test thread (excluding waitForNextTick() and
/// testMutexAndUnlock()): 1 - test thread starts (main -> test), 2 - test thread goes to sleep (test -> main), 3 - main
/// thread blocks on condition variable (main -> idle), 4 - test thread wakes (idle -> test), 5 - test thread terminates
/// (test -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase2ThreadContextSwitchCount {5};

/// expected number of context switches in phase3 block involving software timer (excluding waitForNextTick() and
/// testMutexAndUnlock()): 1 - main thread blocks on condition variable (main -> idle), 2 - main thread is unblocked by
/// interrupt (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase3SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests whether supplied mutex is properly locked and unlocks it.
 *
 * \param [in] mutex is a reference to mutex that will be tested and unlocked
 *
 * \return true if test succeeded and mutex was successfully unlocked, false otherwise
 */

bool testMutexAndUnlock(Mutex& mutex)
{
	const auto testRet = mutexTestTryLockWhenLocked(mutex, UINT8_MAX);
	const auto unlockRet = mutex.unlock();

	return testRet == true && unlockRet == 0;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether both waitFor() and waitUntil() functions properly return some error and properly handle mutex when
 * timing out without notification.
 *
 * \param [in] mutex is a reference to mutex used with condition variable, must be unlocked
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1(Mutex& mutex)
{
	ConditionVariable conditionVariable;

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		// there will be no notification, so waitFor() should time-out at expected time
		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto start = TickClock::now();
		const auto ret = conditionVariable.waitFor(mutex, singleDuration);
		const auto realDuration = TickClock::now() - start;
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} || mutexTest != true ||
				contextSwitches != phase1WaitForUntilContextSwitchCount)
			return false;
	}

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		// there will be no notification, so waitUntil() should time-out at exact expected time
		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + singleDuration;
		const auto ret = conditionVariable.waitUntil(mutex, wakeUpTimePoint);
		const auto wokenUpTimePoint = TickClock::now();
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != ETIMEDOUT || wakeUpTimePoint != wokenUpTimePoint || mutexTest != true ||
				contextSwitches != phase1WaitForUntilContextSwitchCount)
			return false;
	}

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests thread-thread signaling scenario. Main (current) thread waits for condition variable notification (using
 * wait(), waitFor() and waitUntil()). Test thread notifies one waiter at specified time point, main thread is expected
 * to wake up and acquire ownership of the mutex in the same moment.
 *
 * \param [in] mutex is a reference to mutex used with condition variable, must be unlocked
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2(Mutex& mutex)
{
	constexpr size_t testThreadStackSize {512};

	ConditionVariable conditionVariable;

	const auto sleepUntilFunctor = [&conditionVariable](const TickClock::time_point timePoint)
			{
				ThisThread::sleepUntil(timePoint);
				conditionVariable.notifyOne();
			};

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		auto thread = makeAndStartDynamicThread({testThreadStackSize, UINT8_MAX}, sleepUntilFunctor, wakeUpTimePoint);

		ThisThread::yield();

		// wait() should succeed at expected time
		const auto ret = conditionVariable.wait(mutex);
		const auto wokenUpTimePoint = TickClock::now();
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		thread.join();
		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || mutexTest != true ||
				contextSwitches != phase2ThreadContextSwitchCount)
			return false;
	}

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		auto thread = makeAndStartDynamicThread({testThreadStackSize, UINT8_MAX}, sleepUntilFunctor, wakeUpTimePoint);

		ThisThread::yield();

		// waitFor() should succeed at expected time
		const auto ret = conditionVariable.waitFor(mutex, wakeUpTimePoint - TickClock::now() + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		thread.join();
		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || mutexTest != true ||
				contextSwitches != phase2ThreadContextSwitchCount)
			return false;
	}

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		auto thread = makeAndStartDynamicThread({testThreadStackSize, UINT8_MAX}, sleepUntilFunctor, wakeUpTimePoint);

		ThisThread::yield();

		// waitUntil() should succeed at expected time
		const auto ret = conditionVariable.waitUntil(mutex, wakeUpTimePoint + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		thread.join();
		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || mutexTest != true ||
				contextSwitches != phase2ThreadContextSwitchCount)
			return false;
	}

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests interrupt-thread signaling scenario. Main (current) thread waits for condition variable notification (using
 * wait(), waitFor() and waitUntil()). Software timer is used to notify one waiter at specified time point from
 * interrupt context, main thread is expected to wake up and acquire ownership of the mutex in the same moment.
 *
 * \param [in] mutex is a reference to mutex used with condition variable, must be unlocked
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3(Mutex& mutex)
{
	ConditionVariable conditionVariable;

	auto softwareTimer = makeStaticSoftwareTimer(&ConditionVariable::notifyOne, std::ref(conditionVariable));

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;

		softwareTimer.start(wakeUpTimePoint);

		// wait() should succeed at expected time
		const auto ret = conditionVariable.wait(mutex);
		const auto wokenUpTimePoint = TickClock::now();
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || mutexTest != true ||
				contextSwitches != phase3SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;

		softwareTimer.start(wakeUpTimePoint);

		// waitFor() should succeed at expected time
		const auto ret = conditionVariable.waitFor(mutex, wakeUpTimePoint - TickClock::now() + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || mutexTest != true ||
				contextSwitches != phase3SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = mutex.lock();
		if (ret != 0)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;

		softwareTimer.start(wakeUpTimePoint);

		// waitUntil() should succeed at expected time
		const auto ret = conditionVariable.waitUntil(mutex, wakeUpTimePoint + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		const auto contextSwitches = statistics::getContextSwitchCount() - contextSwitchCount;

		const auto mutexTest = testMutexAndUnlock(mutex);
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || mutexTest != true ||
				contextSwitches != phase3SoftwareTimerContextSwitchCount)
			return false;
	}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ConditionVariableOperationsTestCase::run_() const
{
	using Parameters = std::tuple<Mutex::Type, Mutex::Protocol, uint8_t>;
	static const std::array<Parameters, 9> parametersArray
	{{
			Parameters{Mutex::Type::normal, Mutex::Protocol::none, {}},
			Parameters{Mutex::Type::normal, Mutex::Protocol::priorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::normal, Mutex::Protocol::priorityInheritance, {}},
			Parameters{Mutex::Type::errorChecking, Mutex::Protocol::none, {}},
			Parameters{Mutex::Type::errorChecking, Mutex::Protocol::priorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::errorChecking, Mutex::Protocol::priorityInheritance, {}},
			Parameters{Mutex::Type::recursive, Mutex::Protocol::none, {}},
			Parameters{Mutex::Type::recursive, Mutex::Protocol::priorityProtect, UINT8_MAX},
			Parameters{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance, {}},
	}};

	constexpr auto phase1ExpectedContextSwitchCount = 2 * (waitForNextTickContextSwitchCount +
			phase1WaitForUntilContextSwitchCount + testMutexAndUnlockContextSwitchCount);
	constexpr auto phase2ExpectedContextSwitchCount = 3 * (waitForNextTickContextSwitchCount +
			phase2ThreadContextSwitchCount + testMutexAndUnlockContextSwitchCount);
	constexpr auto phase3ExpectedContextSwitchCount = 3 * (waitForNextTickContextSwitchCount +
			phase3SoftwareTimerContextSwitchCount + testMutexAndUnlockContextSwitchCount);
	constexpr auto expectedContextSwitchCount = parametersArray.size() * (phase1ExpectedContextSwitchCount +
			phase2ExpectedContextSwitchCount + phase3ExpectedContextSwitchCount);

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& parameters : parametersArray)
		for (const auto& function : {phase1, phase2, phase3})
		{
			Mutex mutex {std::get<0>(parameters), std::get<1>(parameters), std::get<2>(parameters)};
			const auto ret = function(mutex);
			if (ret != true)
				return ret;
		}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
