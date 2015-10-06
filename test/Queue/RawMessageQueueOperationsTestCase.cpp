/**
 * \file
 * \brief RawMessageQueueOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-20
 */

#include "RawMessageQueueOperationsTestCase.hpp"

#include "waitForNextTick.hpp"

#include "distortos/StaticRawMessageQueue.hpp"
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

/// type of elements of \a RawMessageQueue
using TestType = unsigned int;

/// StaticRawMessageQueue with \a TestType, with storage for \a QueueSize elements
template<size_t QueueSize>
using TestStaticRawMessageQueue = StaticRawMessageQueue<TestType, QueueSize>;

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
/// tryPushUntil() (excluding waitForNextTick()): 1 - main thread blocks on raw message queue (main -> idle), 2 - main
/// thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TryForUntilContextSwitchCount {2};

/// expected number of context switches in phase3 and phase4 block involving software timer (excluding
/// waitForNextTick()): 1 - main thread blocks on raw message queue (main -> idle), 2 - main thread is unblocked by
/// interrupt (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase34SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests RawMessageQueue::tryPop() when raw message queue is empty - it must fail immediately and return EAGAIN
 *
 * \param [in] rawMessageQueue is a reference to RawMessageQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenEmpty(RawMessageQueue& rawMessageQueue)
{
	// raw message queue is empty, so tryPop(T&) should fail immediately
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	TestType testValue {};
	const auto ret = rawMessageQueue.tryPop(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start;
}

/**
 * \brief Tests RawMessageQueue::tryPop() when raw message queue is not empty - it must succeed immediately
 *
 * \param [in] rawMessageQueue is a reference to RawMessageQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenNotEmpty(RawMessageQueue& rawMessageQueue)
{
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	TestType testValue {};
	const auto ret = rawMessageQueue.tryPop(priority, testValue);
	return ret == 0 && start == TickClock::now();
}

/**
 * \brief Tests RawMessageQueue::tryPush(..., const T&) when raw message queue is full - it must fail immediately and
 * return EAGAIN
 *
 * \param [in] rawMessageQueue is a reference to RawMessageQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPushWhenFull(RawMessageQueue& rawMessageQueue)
{
	// raw message queue is full, so tryPush(..., const T&) should fail immediately
	waitForNextTick();
	const auto start = TickClock::now();
	const uint8_t priority {};
	const TestType testValue {};
	const auto ret = rawMessageQueue.tryPush(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all tryPush*() and tryPop*() functions properly return some error when dealing with full or empty raw
 * message queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	TestStaticRawMessageQueue<0> rawMessageQueue;	// size 0, so queue is both full and empty at the same time
	const uint8_t constPriority {};
	const TestType constTestValue {};
	uint8_t nonConstPriority {};
	TestType nonConstTestValue {};

	{
		const auto ret = testTryPushWhenFull(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw message queue is both full and empty, so tryPushFor(..., const T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPushFor(singleDuration, constPriority, constTestValue);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw message queue is both full and empty, so tryPushUntil(..., const T&) should time-out at exact expected
		// time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = rawMessageQueue.tryPushUntil(requestedTimePoint, constPriority, constTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw message queue is both full and empty, so tryPopFor(..., T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// raw message queue is both full and empty, so tryPopUntil(..., T&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = rawMessageQueue.tryPopUntil(requestedTimePoint, nonConstPriority, nonConstTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests whether all tryPush*() and tryPop*() functions properly send data via non-full or non-empty raw message queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	TestStaticRawMessageQueue<1> rawMessageQueue;
	const uint8_t constPriority {};
	const TestType constTestValue {};
	uint8_t nonConstPriority {};
	TestType nonConstTestValue {};

	{
		// raw message queue is not full, so tryPush(..., const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPush(constPriority, constTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw message queue is not full, so tryPushFor(..., const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPushFor(singleDuration, constPriority, constTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw message queue is not empty, so tryPopFor(..., T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw message queue is not full, so tryPushUntil(..., const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPushUntil(start + singleDuration, constPriority, constTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		// raw message queue is not empty, so tryPopUntil(..., T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPopUntil(start + singleDuration, nonConstPriority, nonConstTestValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests interrupt -> thread communication scenario. Main (current) thread waits for data to become available in raw
 * message queue. Software timer pushes some values to the same raw message queue at specified time point from interrupt
 * context, main thread is expected to receive these values (with pop(), tryPopFor() and tryPopUntil()) in the same
 * moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	TestStaticRawMessageQueue<1> rawMessageQueue;
	uint8_t sharedMagicPriority {};
	TestType sharedMagicValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&rawMessageQueue, &sharedMagicPriority, &sharedMagicValue]()
			{
				rawMessageQueue.tryPush(sharedMagicPriority, sharedMagicValue);
			});

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicPriority = 0xde;
		sharedMagicValue = TestType{0x8e61da6a};
		softwareTimer.start(wakeUpTimePoint);

		// raw message queue is currently empty, but pop() should succeed at expected time
		uint8_t priority {};
		TestType testValue {};
		const auto ret = rawMessageQueue.pop(priority, testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || priority != sharedMagicPriority ||
				testValue != sharedMagicValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicPriority = 0x11;
		sharedMagicValue = TestType{0x3cdc2da2};
		softwareTimer.start(wakeUpTimePoint);

		// raw message queue is currently empty, but tryPopFor() should succeed at expected time
		uint8_t priority {};
		TestType testValue {};
		const auto ret = rawMessageQueue.tryPopFor(wakeUpTimePoint - TickClock::now() + longDuration, priority,
				testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || priority != sharedMagicPriority ||
				testValue != sharedMagicValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicPriority = 0x03;
		sharedMagicValue = TestType{0x820963de};
		softwareTimer.start(wakeUpTimePoint);

		// raw message queue is currently empty, but tryPopUntil() should succeed at expected time
		uint8_t priority {};
		TestType testValue {};
		const auto ret = rawMessageQueue.tryPopUntil(wakeUpTimePoint + longDuration, priority, testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || priority != sharedMagicPriority ||
				testValue != sharedMagicValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(rawMessageQueue);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests thread -> interrupt communication scenario. Main (current) thread pushes data to raw message queue (which is
 * initially full). Software timer pops first value (which should match the one pushed previously) from the same raw
 * message queue at specified time point from interrupt context, main thread is expected to succeed in pushing new value
 * (with push(), tryPushFor() and tryPushUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4()
{
	TestStaticRawMessageQueue<1> rawMessageQueue;
	uint8_t receivedPriority {};
	TestType receivedTestValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&rawMessageQueue, &receivedPriority, &receivedTestValue]()
			{
				rawMessageQueue.tryPop(receivedPriority, receivedTestValue);
			});

	uint8_t currentMagicPriority {0xfb};
	TestType currentMagicValue {0x8b45e769};

	{
		// raw message queue is not full, so push(..., const T&) must succeed immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPush(currentMagicPriority, currentMagicValue);
		if (ret != 0 || start != TickClock::now())
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);

		// raw message queue is currently full, but push(..., const T&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};
		currentMagicPriority = 0xc5;
		currentMagicValue = TestType{0x6efc7e81};
		const auto ret = rawMessageQueue.push(currentMagicPriority, currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);

		// raw message queue is currently full, but tryPushFor(..., const T&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};
		currentMagicPriority = 0x38;
		currentMagicValue = TestType{0x1de20f70};
		const auto ret = rawMessageQueue.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
				currentMagicPriority, currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);

		// raw message queue is currently full, but tryPushUntil(..., const T&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};
		currentMagicPriority = 0xcc;
		currentMagicValue = TestType{0x45616860};
		const auto ret = rawMessageQueue.tryPushUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
				currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	return true;
}

/**
 * \brief Phase 5 of test case.
 *
 * Tests whether all \*push\*() and \*pop\*() functions properly return some error when given invalid size of buffer.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase5()
{
	TestStaticRawMessageQueue<0> rawMessageQueue;	// size 0, so queue is both full and empty at the same time
	const uint8_t constPriority {};
	const TestType constTestValue {};
	uint8_t nonConstPriority {};
	TestType nonConstTestValue {};

	{
		// invalid size is given, so push(..., const void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.push(constPriority, &constTestValue, sizeof(constTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	{
		// invalid size is given, so tryPush(..., const void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPush(constPriority, &constTestValue, sizeof(constTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	{
		// invalid size is given, so tryPushFor(..., const void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPushFor(singleDuration, constPriority, &constTestValue,
				sizeof(constTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	{
		// invalid size is given, so tryPushUntil(..., const void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPushUntil(TickClock::now() + singleDuration, constPriority, &constTestValue,
				sizeof(constTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	{
		// invalid size is given, so pop(..., void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.pop(nonConstPriority, &nonConstTestValue, sizeof(nonConstTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	{
		// invalid size is given, so tryPop(..., void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPop(nonConstPriority, &nonConstTestValue, sizeof(nonConstTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	{
		// invalid size is given, so tryPopFor(..., void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPopFor(singleDuration, nonConstPriority, &nonConstTestValue,
				sizeof(nonConstTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	{
		// invalid size is given, so tryPopUntil(..., void*, size_t) should fail immediately
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = rawMessageQueue.tryPopUntil(TickClock::now() + singleDuration, nonConstPriority, &nonConstTestValue,
				sizeof(nonConstTestValue) - 1);
		if (ret != EMSGSIZE || TickClock::now() != start)
			return false;
	}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool RawMessageQueueOperationsTestCase::run_() const
{
	constexpr auto phase1ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			4 * phase1TryForUntilContextSwitchCount;
	constexpr auto phase2ExpectedContextSwitchCount = 12 * waitForNextTickContextSwitchCount;
	constexpr auto phase3ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			3 * phase34SoftwareTimerContextSwitchCount;
	constexpr auto phase4ExpectedContextSwitchCount = 4 * waitForNextTickContextSwitchCount +
			3 * phase34SoftwareTimerContextSwitchCount;
	constexpr auto phase5ExpectedContextSwitchCount = 8 * waitForNextTickContextSwitchCount;
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
