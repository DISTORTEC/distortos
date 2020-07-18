/**
 * \file
 * \brief SemaphoreOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SemaphoreOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/StaticSoftwareTimer.hpp"
#include "distortos/statistics.hpp"
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

/// long duration used in tests
constexpr auto longDuration = singleDuration * 10;

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/// expected number of context switches in phase1 block involving tryWaitFor() or tryWaitUntil() (excluding
/// waitForNextTick()): 1 - main thread blocks on semaphore (main -> idle), 2 - main thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TryWaitForUntilContextSwitchCount {2};

/// expected number of context switches in phase3 block involving test thread (excluding waitForNextTick()): 1 - test
/// thread starts (main -> test), 2 - test thread goes to sleep (test -> main), 3 - main thread blocks on semaphore
/// (main -> idle), 4 - test thread wakes (idle -> test), 5 - test thread terminates (test -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase3ThreadContextSwitchCount {5};

/// expected number of context switches in phase4 block involving software timer (excluding waitForNextTick()): 1 - main
/// thread blocks on semaphore (main -> idle), 2 - main thread is unblocked by interrupt (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase4SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests Semaphore::post() - it must succeed immediately
 *
 * \param [in] semaphore is a reference to semaphore that will be posted
 *
 * \return true if test succeeded, false otherwise
 */

bool testPost(Semaphore& semaphore)
{
	waitForNextTick();
	const auto start = TickClock::now();
	const auto ret = semaphore.post();
	return ret == 0 && start == TickClock::now() && semaphore.getValue() > 0;
}

/**
 * \brief Tests Semaphore::tryWait() when semaphore is locked - it must fail immediately and return EAGAIN
 *
 * \param [in] semaphore is a reference to semaphore that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryWaitWhenLocked(Semaphore& semaphore)
{
	// semaphore is locked, so tryWait() should fail immediately
	waitForNextTick();
	const auto start = TickClock::now();
	const auto ret = semaphore.tryWait();
	return ret == EAGAIN && TickClock::now() == start && semaphore.getValue() == 0;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all tryWait*() functions properly return some error when dealing with locked semaphore.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	Semaphore semaphore {0};

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// semaphore is locked, so tryWaitFor() should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = semaphore.tryWaitFor(singleDuration);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryWaitForUntilContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// semaphore is locked, so tryWaitUntil() should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = semaphore.tryWaitUntil(requestedTimePoint);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() || semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryWaitForUntilContextSwitchCount)
			return false;
	}

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests whether all tryWait*() functions properly lock unlocked semaphore.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	Semaphore semaphore {1};

	{
		// semaphore is unlocked, so tryWait() must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = semaphore.tryWait();
		if (ret != 0 || start != TickClock::now() || semaphore.getValue() != 0)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testPost(semaphore);
		if (ret != true)
			return ret;
	}

	{
		// semaphore is unlocked, so tryWaitFor() should succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = semaphore.tryWaitFor(singleDuration);
		if (ret != 0 || start != TickClock::now() || semaphore.getValue() != 0)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testPost(semaphore);
		if (ret != true)
			return ret;
	}

	{
		// semaphore is unlocked, so tryWaitUntil() should succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = semaphore.tryWaitUntil(start + singleDuration);
		if (ret != 0 || start != TickClock::now() || semaphore.getValue() != 0)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testPost(semaphore);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests thread-thread signaling scenario. Main (current) thread waits for a locked semaphore to become available. Test
 * thread posts the semaphore at specified time point, main thread is expected to acquire ownership of this semaphore
 * (with wait(), tryWaitFor() and tryWaitUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	constexpr size_t testThreadStackSize {512};

	Semaphore semaphore {0};

	const auto sleepUntilFunctor = [&semaphore](const TickClock::time_point timePoint)
			{
				ThisThread::sleepUntil(timePoint);
				semaphore.post();
			};

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		auto thread = makeAndStartDynamicThread({testThreadStackSize, UINT8_MAX}, sleepUntilFunctor, wakeUpTimePoint);

		ThisThread::yield();

		// semaphore is currently locked, but wait() should succeed at expected time
		const auto ret = semaphore.wait();
		const auto wokenUpTimePoint = TickClock::now();
		thread.join();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase3ThreadContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		auto thread = makeAndStartDynamicThread({testThreadStackSize, UINT8_MAX}, sleepUntilFunctor, wakeUpTimePoint);

		ThisThread::yield();

		// semaphore is currently locked, but tryWaitFor() should succeed at expected time
		const auto ret = semaphore.tryWaitFor(wakeUpTimePoint - TickClock::now() + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		thread.join();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase3ThreadContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		auto thread = makeAndStartDynamicThread({testThreadStackSize, UINT8_MAX}, sleepUntilFunctor, wakeUpTimePoint);

		ThisThread::yield();

		// semaphore is locked, but tryWaitUntil() should succeed at expected time
		const auto ret = semaphore.tryWaitUntil(wakeUpTimePoint + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		thread.join();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase3ThreadContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests interrupt-thread signaling scenario. Main (current) thread waits for a locked semaphore to become available.
 * Software timer is used to posts the semaphore at specified time point from interrupt context, main thread is expected
 * to acquire ownership of this semaphore (with wait(), tryWaitFor() and tryWaitUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4()
{
	Semaphore semaphore {0};
	auto softwareTimer = makeStaticSoftwareTimer(&Semaphore::post, std::ref(semaphore));

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;

		softwareTimer.start(wakeUpTimePoint);

		// semaphore is currently locked, but wait() should succeed at expected time
		const auto ret = semaphore.wait();
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase4SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;

		softwareTimer.start(wakeUpTimePoint);

		// semaphore is currently locked, but tryWaitFor() should succeed at expected time
		const auto ret = semaphore.tryWaitFor(wakeUpTimePoint - TickClock::now() + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase4SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;

		softwareTimer.start(wakeUpTimePoint);

		// semaphore is locked, but tryWaitUntil() should succeed at expected time
		const auto ret = semaphore.tryWaitUntil(wakeUpTimePoint + longDuration);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || semaphore.getValue() != 0 ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase4SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryWaitWhenLocked(semaphore);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 5 of test case.
 *
 * Tests correction of invalid initialization and overflow detection of semaphore with explicit max value. Semaphore's
 * initial value cannot be higher than given max value - value must be truncated in the constructor. Semaphore's value
 * must never be higher than the max value - any call to Semaphore::post() after the value reached max value must result
 * in EOVERFLOW error.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase5()
{
	constexpr auto initialValue = std::numeric_limits<Semaphore::Value>::max();
	constexpr auto maxValue = initialValue / 2;

	Semaphore semaphore {initialValue, maxValue};

	if (semaphore.getValue() != maxValue)
		return false;

	{
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = semaphore.post();
		if (ret != EOVERFLOW || start != TickClock::now() || semaphore.getValue() != maxValue)
			return false;
	}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SemaphoreOperationsTestCase::run_() const
{
	constexpr auto phase1ExpectedContextSwitchCount = 3 * waitForNextTickContextSwitchCount +
			2 * phase1TryWaitForUntilContextSwitchCount;
	constexpr auto phase2ExpectedContextSwitchCount = 9 * waitForNextTickContextSwitchCount;
	constexpr auto phase3ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			3 * phase3ThreadContextSwitchCount;
	constexpr auto phase4ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			3 * phase4SoftwareTimerContextSwitchCount;
	constexpr auto phase5ExpectedContextSwitchCount = 1 * waitForNextTickContextSwitchCount;
	constexpr auto expectedContextSwitchCount = phase1ExpectedContextSwitchCount + phase2ExpectedContextSwitchCount +
			phase3ExpectedContextSwitchCount + phase4ExpectedContextSwitchCount + phase5ExpectedContextSwitchCount;

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2, phase3, phase4, phase5})
	{
		const auto ret = function();
		if (ret != true)
			return ret;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
