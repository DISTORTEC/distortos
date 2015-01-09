/**
 * \file
 * \brief RawFifoQueueOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-09
 */

#include "RawFifoQueueOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/StaticRawFifoQueue.hpp"
#include "distortos/SoftwareTimer.hpp"
#include "distortos/statistics.hpp"

#include <cerrno>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of elements in tested raw FIFO queue
using TestType = unsigned int;

/// StaticRawFifoQueue with \a TestType, with storage for \a QueueSize elements
template<size_t QueueSize>
using TestStaticRawFifoQueue = StaticRawFifoQueue<TestType, QueueSize>;

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// single duration used in tests
constexpr auto singleDuration = TickClock::duration{1};

/// long duration used in tests
constexpr auto longDuration = singleDuration * 10;

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/// expected number of context switches in phase1 block involving tryPopFor(), tryPopUntil(), tryPushFor() or
/// tryPushUntil() (excluding waitForNextTick()): 1 - main thread blocks on raw FIFO queue (main -> idle), 2 - main
/// thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TryPopPushForUntilContextSwitchCount {2};

/// expected number of context switches in phase3 and phase4 block involving software timer (excluding
/// waitForNextTick()): 1 - main thread blocks on raw FIFO queue (main -> idle), 2 - main thread is unblocked by
/// interrupt (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase34SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests RawFifoQueue::tryPop() when raw FIFO queue is empty - it must fail immediately and return EAGAIN
 *
 * \param [in] rawFifoQueue is a reference to RawFifoQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenEmpty(RawFifoQueue& rawFifoQueue)
{
	// raw FIFO queue is empty, so tryPop(T&) should fail immediately
	waitForNextTick();
	const auto start = TickClock::now();
	TestType testValue {};
	const auto ret = rawFifoQueue.tryPop(testValue);
	return ret == EAGAIN && TickClock::now() == start;
}

/**
 * \brief Tests RawFifoQueue::tryPop() when raw FIFO queue is not empty - it must succeed immediately
 *
 * \param [in] rawFifoQueue is a reference to RawFifoQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenNotEmpty(RawFifoQueue& rawFifoQueue)
{
	waitForNextTick();
	const auto start = TickClock::now();
	TestType testValue {};
	const auto ret = rawFifoQueue.tryPop(testValue);
	return ret == 0 && start == TickClock::now();
}

/**
 * \brief Tests RawFifoQueue::tryPush(const T&) when raw FIFO queue is full - it must fail immediately and return EAGAIN
 *
 * \param [in] rawFifoQueue is a reference to RawFifoQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPushWhenFull(RawFifoQueue& rawFifoQueue)
{
	// raw FIFO queue is full, so tryPush(const T&) should fail immediately
	waitForNextTick();
	const auto start = TickClock::now();
	const TestType testValue {};
	const auto ret = rawFifoQueue.tryPush(testValue);
	return ret == EAGAIN && TickClock::now() == start;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all tryPush*() and tryPop*() functions properly return some error when dealing with full or empty raw
 * FIFO queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	TestStaticRawFifoQueue<0> rawFifoQueue;	// size 0, so queue is both full and empty at the same time
	const TestType constTestValue {};
	TestType nonConstTestValue {};

	{
		const auto ret = testTryPushWhenFull(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw FIFO queue is both full and empty, so tryPushFor(..., const T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPushFor(singleDuration, constTestValue);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryPopPushForUntilContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw FIFO queue is both full and empty, so tryPushUntil(..., const T&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = rawFifoQueue.tryPushUntil(requestedTimePoint, constTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryPopPushForUntilContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw FIFO queue is both full and empty, so tryPopFor(..., T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPopFor(singleDuration, nonConstTestValue);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryPopPushForUntilContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw FIFO queue is both full and empty, so tryPopUntil(..., T&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = rawFifoQueue.tryPopUntil(requestedTimePoint, nonConstTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryPopPushForUntilContextSwitchCount)
			return false;
	}

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests whether all tryPush*() and tryPop*() functions properly send data via non-full or non-empty raw FIFO queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	TestStaticRawFifoQueue<1> rawFifoQueue;
	const TestType constTestValue {};
	TestType nonConstTestValue {};

	{
		// raw FIFO queue is not full, so tryPush(const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPush(constTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw FIFO queue is not full, so tryPushFor(..., const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPushFor(singleDuration, constTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw FIFO queue is not empty, so tryPopFor(..., T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPopFor(singleDuration, nonConstTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw FIFO queue is not full, so tryPushUntil(..., const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPushUntil(start + singleDuration, constTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw FIFO queue is not empty, so tryPopUntil(..., T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPopUntil(start + singleDuration, nonConstTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests interrupt -> thread communication scenario. Main (current) thread waits for data to become available in raw
 * FIFO queue. Software timer pushes some values to the same raw FIFO queue at specified time point from interrupt
 * context, main thread is expected to receive these values (with pop(), tryPopFor() and tryPopUntil()) in the same
 * moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	TestStaticRawFifoQueue<1> rawFifoQueue;
	TestType sharedMagicValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&rawFifoQueue, &sharedMagicValue]()
			{
				rawFifoQueue.tryPush(sharedMagicValue);
			});

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicValue = TestType{0x50cc9d06};
		softwareTimer.start(wakeUpTimePoint);

		// raw FIFO queue is currently empty, but pop() should succeed at expected time
		TestType testValue {};
		const auto ret = rawFifoQueue.pop(testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || testValue != sharedMagicValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicValue = TestType{0x3353b7e9};
		softwareTimer.start(wakeUpTimePoint);

		// raw FIFO queue is currently empty, but tryPopFor() should succeed at expected time
		TestType testValue {};
		const auto ret = rawFifoQueue.tryPopFor(wakeUpTimePoint - TickClock::now() + longDuration, testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || testValue != sharedMagicValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicValue = TestType{0x28d0a656};
		softwareTimer.start(wakeUpTimePoint);

		// raw FIFO queue is currently empty, but tryPopUntil() should succeed at expected time
		TestType testValue {};
		const auto ret = rawFifoQueue.tryPopUntil(wakeUpTimePoint + longDuration, testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || testValue != sharedMagicValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawFifoQueue);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests thread -> interrupt communication scenario. Main (current) thread pushes data to raw FIFO queue (which is
 * initially full). Software timer pops last value (which should match the one pushed previously) from the same raw FIFO
 * queue at specified time point from interrupt context, main thread is expected to succeed in pushing new value (with
 * push(), tryPushFor() and tryPushUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4()
{
	TestStaticRawFifoQueue<1> rawFifoQueue;
	TestType receivedTestValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&rawFifoQueue, &receivedTestValue]()
			{
				rawFifoQueue.tryPop(receivedTestValue);
			});

	TestType currentMagicValue {0xe217038c};

	{
		// raw FIFO queue is not full, so push(const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawFifoQueue.tryPush(currentMagicValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);

		// raw FIFO queue is currently full, but push(const T&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};
		currentMagicValue = TestType{0x48f405eb};
		const auto ret = rawFifoQueue.push(currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);

		// raw FIFO queue is currently full, but tryPushFor(..., const T&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};
		currentMagicValue = TestType{0xeed9591f};
		const auto ret = rawFifoQueue.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration, currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);

		// raw FIFO queue is currently full, but tryPushUntil(..., const T&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};
		currentMagicValue = TestType{0x7604284a};
		const auto ret = rawFifoQueue.tryPushUntil(wakeUpTimePoint + longDuration, currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool RawFifoQueueOperationsTestCase::run_() const
{
	constexpr auto phase1ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			4 * phase1TryPopPushForUntilContextSwitchCount;
	constexpr auto phase2ExpectedContextSwitchCount = 12 * waitForNextTickContextSwitchCount;
	constexpr auto phase3ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			3 * phase34SoftwareTimerContextSwitchCount;
	constexpr auto phase4ExpectedContextSwitchCount = 4 * waitForNextTickContextSwitchCount +
			3 * phase34SoftwareTimerContextSwitchCount;
	constexpr auto expectedContextSwitchCount = phase1ExpectedContextSwitchCount + phase2ExpectedContextSwitchCount +
			phase3ExpectedContextSwitchCount + phase4ExpectedContextSwitchCount;

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2, phase3, phase4})
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
