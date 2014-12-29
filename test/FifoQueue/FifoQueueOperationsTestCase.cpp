/**
 * \file
 * \brief FifoQueueOperationsTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-12-29
 */

#include "FifoQueueOperationsTestCase.hpp"

#include "waitForNextTick.hpp"
#include "OperationCountingType.hpp"

#include "distortos/StaticFifoQueue.hpp"
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

/// type of elements of \a TestFifoQueue
using TestType = OperationCountingType;

/// FifoQueue with \a TestType
using TestFifoQueue = FifoQueue<TestType>;

/// StaticFifoQueue with \a TestType, with storage for \a totalThreads elements
template<size_t QueueSize>
using TestStaticFifoQueue = StaticFifoQueue<TestType, QueueSize>;

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// single duration used in tests
constexpr auto singleDuration = TickClock::duration{1};

/// long duration used in tests
constexpr auto longDuration = singleDuration * 10;

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/// expected number of context switches in phase1 block involving tryEmplaceFor(), tryEmplaceUntil(), tryPopFor(),
/// tryPopUntil(), tryPushFor() or tryPushUntil() (excluding waitForNextTick()): 1 - main thread blocks on FIFO queue
/// (main -> idle), 2 - main thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TryForUntilContextSwitchCount {2};

/// expected number of context switches in phase3 and phase4 block involving software timer (excluding
/// waitForNextTick()): 1 - main thread blocks on FIFO queue (main -> idle), 2 - main thread is unblocked by interrupt
/// (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase34SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests FifoQueue::tryPop() when FIFO queue is empty - it must fail immediately and return EAGAIN
 *
 * \param [in] fifoQueue is a reference to TestFifoQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenEmpty(TestFifoQueue& fifoQueue)
{
	// FIFO queue is empty, so tryPop(T&) should fail immediately
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	TestType testValue {};	// 1 construction
	const auto ret = fifoQueue.tryPop(testValue);
	return ret == EAGAIN && TickClock::now() == start && TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Tests FifoQueue::tryPop() when FIFO queue is not empty - it must succeed immediately
 *
 * \param [in] fifoQueue is a reference to TestFifoQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenNotEmpty(TestFifoQueue& fifoQueue)
{
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	TestType testValue {};	// 1 construction
	const auto ret = fifoQueue.tryPop(testValue);	// 1 swap, 1 destruction
	return ret == 0 && start == TickClock::now() && TestType::checkCounters(1, 0, 0, 1, 0, 0, 1) == true;
}

/**
 * \brief Tests FifoQueue::tryPush(const T&) when FIFO queue is full - it must fail immediately and return EAGAIN
 *
 * \param [in] fifoQueue is a reference to TestFifoQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPushWhenFull(TestFifoQueue& fifoQueue)
{
	// FIFO queue is full, so tryPush(const T&) should fail immediately
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	const TestType testValue {};	// 1 construction
	const auto ret = fifoQueue.tryPush(testValue);
	return ret == EAGAIN && TickClock::now() == start && TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly return some error when dealing with full
 * or empty FIFO queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	TestStaticFifoQueue<0> fifoQueue;	// size 0, so queue is both full and empty at the same time
	const TestType constTestValue {};
	TestType nonConstTestValue {};

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is both full and empty, so tryPush(T&&) should fail immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPush(TestType{});	// 1 construction, 1 destruction
		if (ret != EAGAIN || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 1, 0, 0, 0) != true)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryPushFor(..., const T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPushFor(singleDuration, constTestValue);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryPushFor(..., T&&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPushFor(singleDuration, TestType{});	// 1 construction, 1 destruction
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				TestType::checkCounters(1, 0, 0, 1, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryPushUntil(..., const T&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = fifoQueue.tryPushUntil(requestedTimePoint, constTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryPushUntil(..., T&&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = fifoQueue.tryPushUntil(requestedTimePoint, TestType{});	// 1 construction, 1 destruction
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(1, 0, 0, 1, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryPopFor(..., T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPopFor(singleDuration, nonConstTestValue);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryPopUntil(..., T&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = fifoQueue.tryPopUntil(requestedTimePoint, nonConstTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

#if DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	{
		// FIFO queue is both full and empty, so tryEmplace(Args&&...) should fail immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryEmplace();
		if (ret != EAGAIN || start != TickClock::now() || TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryEmplaceFor(..., Args&&...) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryEmplaceFor(singleDuration);
		const auto realDuration = TickClock::now() - start;
		if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// FIFO queue is both full and empty, so tryEmplaceUntil(..., Args&&...) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = fifoQueue.tryEmplaceUntil(requestedTimePoint);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

#endif	// DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly send data via non-full or non-empty FIFO
 * queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	TestStaticFifoQueue<1> fifoQueue;
	const TestType constTestValue {};
	TestType nonConstTestValue {};

	{
		// FIFO queue is not full, so tryPush(const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPush(constTestValue);	// 1 copy construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not full, so tryPush(T&&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPush(TestType{});	// 1 construction, 1 move construction, 1 destruction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not full, so tryPushFor(..., const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPushFor(singleDuration, constTestValue);	// 1 copy construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not empty, so tryPopFor(..., T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPopFor(singleDuration, nonConstTestValue);	// 1 swap, 1 destruction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not full, so tryPushFor(..., T&&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 construction, 1 move construction, 1 destruction
		const auto ret = fifoQueue.tryPushFor(singleDuration, TestType{});
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not full, so tryPushUntil(..., const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPushUntil(start + singleDuration, constTestValue);	// 1 copy construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not empty, so tryPopUntil(..., T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPopUntil(start + singleDuration, nonConstTestValue);	// 1 swap, 1 destruction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not full, so tryPushUntil(..., T&&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 construction, 1 move construction, 1 destruction
		const auto ret = fifoQueue.tryPushUntil(start + singleDuration, TestType{});
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

#if DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	{
		// FIFO queue is not full, so tryEmplace(Args&&...) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryEmplace();	// 1 construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not full, so tryEmplaceFor(..., Args&&...) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryEmplaceFor(singleDuration);	// 1 construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		// FIFO queue is not full, so tryEmplaceUntil(..., Args&&...) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryEmplaceUntil(start + singleDuration);	// 1 construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

#endif	// DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests interrupt -> thread communication scenario. Main (current) thread waits for data to become available in FIFO
 * queue. Software timer pushes some values to the same FIFO queue at specified time point from interrupt context, main
 * thread is expected to receive these values (with pop(), tryPopFor() and tryPopUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	TestStaticFifoQueue<1> fifoQueue;
	TestType sharedMagicValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&fifoQueue, &sharedMagicValue]()
			{
				fifoQueue.tryPush(sharedMagicValue);
			});

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicValue = TestType{0x266adb2f};	// 1 construction, 1 move assignment, 1 destruction
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

		// FIFO queue is currently empty, but pop() should succeed at expected time
		TestType testValue {};	// 1 construction
		const auto ret = fifoQueue.pop(testValue);	// 1 swap, 1 destruction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || testValue != sharedMagicValue ||
				TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicValue = TestType{0x7ca977a0};	// 1 construction, 1 move assignment, 1 destruction
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

		// FIFO queue is currently empty, but tryPopFor() should succeed at expected time
		TestType testValue {};	// 1 construction
		// 1 swap, 1 destruction
		const auto ret = fifoQueue.tryPopFor(wakeUpTimePoint - TickClock::now() + longDuration, testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || testValue != sharedMagicValue ||
				TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicValue = TestType{0x532b5211};	// 1 construction, 1 move assignment, 1 destruction
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

		// FIFO queue is currently empty, but tryPopUntil() should succeed at expected time
		TestType testValue {};	// 1 construction
		const auto ret = fifoQueue.tryPopUntil(wakeUpTimePoint + longDuration, testValue);	// 1 swap, 1 destruction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || testValue != sharedMagicValue ||
				TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(fifoQueue);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests thread -> interrupt communication scenario. Main (current) thread pushes data to FIFO queue (which is initially
 * full). Software timer pops last value (which should match the one pushed previously) from the same FIFO queue at
 * specified time point from interrupt context, main thread is expected to succeed in pushing new value (with emplace(),
 * push(), tryEmplaceFor(), tryEmplaceUntil(), tryPushFor() and tryPushUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4()
{
	TestStaticFifoQueue<1> fifoQueue;
	TestType receivedTestValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&fifoQueue, &receivedTestValue]()
			{
				fifoQueue.tryPop(receivedTestValue);
			});

	TestType currentMagicValue {0xe09574e7};

	{
		// FIFO queue is not full, so push(const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = fifoQueue.tryPush(currentMagicValue);	// 1 copy construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but push(const T&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicValue = TestType{0xf375f6db};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = fifoQueue.push(currentMagicValue);	// 1 copy construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but push(T&&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicValue = TestType{0xb1dfbefe};	// 1 construction, 1 move assignment, 1 destruction
		// 1 copy construction, 1 move construction, 1 destruction
		const auto ret = fifoQueue.push(TestType{currentMagicValue});
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but tryPushFor(..., const T&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicValue = TestType{0x21cbce40};	// 1 construction, 1 move assignment, 1 destruction
		// 1 copy construction
		const auto ret = fifoQueue.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration, currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but tryPushFor(..., T&&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicValue = TestType{0x5e67aeb0};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = fifoQueue.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
				TestType{currentMagicValue});	// 1 copy construction, 1 move construction, 1 destruction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but tryPushUntil(..., const T&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicValue = TestType{0xb0f4451b};	// 1 construction, 1 move assignment, 1 destruction
		// 1 copy construction
		const auto ret = fifoQueue.tryPushUntil(wakeUpTimePoint + longDuration, currentMagicValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but tryPushUntil(..., T&&) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicValue = TestType{0x108fbbdf};	// 1 construction, 1 move assignment, 1 destruction
		// 1 copy construction, 1 move construction, 1 destruction
		const auto ret = fifoQueue.tryPushUntil(wakeUpTimePoint + longDuration, TestType{currentMagicValue});
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

#if DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but emplace(Args&&...) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		const TestType::Value value = 0x9a158581;
		currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = fifoQueue.emplace(value);	// 1 construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but tryEmplaceFor(..., Args&&...) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		const TestType::Value value = 0xb61b399c;
		currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
		// 1 construction
		const auto ret = fifoQueue.tryEmplaceFor(wakeUpTimePoint - TickClock::now() + longDuration, value);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// FIFO queue is currently full, but tryEmplaceUntil(..., Args&&...) should succeed at expected time
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		const TestType::Value value = 0x4d1f62c0;
		currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = fifoQueue.tryEmplaceUntil(wakeUpTimePoint + longDuration, value);	// 1 construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedTestValue != expectedTestValue ||
				TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

#endif	// DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FifoQueueOperationsTestCase::run_() const
{
	constexpr auto emplace = DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED == 1;

	constexpr auto phase1ExpectedContextSwitchCount = emplace == true ?
			12 * waitForNextTickContextSwitchCount + 8 * phase1TryForUntilContextSwitchCount :
			9 * waitForNextTickContextSwitchCount + 6 * phase1TryForUntilContextSwitchCount;
	constexpr auto phase2ExpectedContextSwitchCount = emplace == true ? 36 * waitForNextTickContextSwitchCount :
			24 * waitForNextTickContextSwitchCount;
	constexpr auto phase3ExpectedContextSwitchCount = 6 * waitForNextTickContextSwitchCount +
			3 * phase34SoftwareTimerContextSwitchCount;
	constexpr auto phase4ExpectedContextSwitchCount = emplace == true ?
			10 * waitForNextTickContextSwitchCount + 9 * phase34SoftwareTimerContextSwitchCount :
			7 * waitForNextTickContextSwitchCount + 6 * phase34SoftwareTimerContextSwitchCount;
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
