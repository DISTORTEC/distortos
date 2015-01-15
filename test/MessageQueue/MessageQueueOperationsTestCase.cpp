/**
 * \file
 * \brief MessageQueueOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-15
 */

#include "MessageQueueOperationsTestCase.hpp"

#include "waitForNextTick.hpp"
#include "OperationCountingType.hpp"

#include "distortos/StaticMessageQueue.hpp"
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

/// type of elements of \a TestMessageQueue
using TestType = OperationCountingType;

/// MessageQueue with \a TestType
using TestMessageQueue = MessageQueue<TestType>;

/// StaticMessageQueue with \a TestType, with storage for \a QueueSize elements
template<size_t QueueSize>
using TestStaticMessageQueue = StaticMessageQueue<TestType, QueueSize>;

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
/// tryPopUntil(), tryPushFor() or tryPushUntil() (excluding waitForNextTick()): 1 - main thread blocks on message queue
/// (main -> idle), 2 - main thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TryForUntilContextSwitchCount {2};

/// expected number of context switches in phase3 and phase4 block involving software timer (excluding
/// waitForNextTick()): 1 - main thread blocks on message queue (main -> idle), 2 - main thread is unblocked by
/// interrupt (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase34SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests MessageQueue::tryPop() when message queue is empty - it must fail immediately and return EAGAIN
 *
 * \param [in] messageQueue is a reference to TestMessageQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenEmpty(TestMessageQueue& messageQueue)
{
	// message queue is empty, so tryPop(T&) should fail immediately
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	TestType testValue {};	// 1 construction
	const auto ret = messageQueue.tryPop(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start && TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Tests MessageQueue::tryPop() when message queue is not empty - it must succeed immediately
 *
 * \param [in] messageQueue is a reference to TestMessageQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenNotEmpty(TestMessageQueue& messageQueue)
{
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	TestType testValue {};	// 1 construction
	const auto ret = messageQueue.tryPop(priority, testValue);	// 1 swap, 1 destruction
	return ret == 0 && start == TickClock::now() && TestType::checkCounters(1, 0, 0, 1, 0, 0, 1) == true;
}

/**
 * \brief Tests MessageQueue::tryPush(..., const T&) when message queue is full - it must fail immediately and return
 * EAGAIN
 *
 * \param [in] messageQueue is a reference to TestMessageQueue that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPushWhenFull(TestMessageQueue& messageQueue)
{
	// message queue is full, so tryPush(..., const T&) should fail immediately
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	const uint8_t priority {};
	const TestType testValue {};	// 1 construction
	const auto ret = messageQueue.tryPush(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start && TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly return some error when dealing with full
 * or empty message queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	TestStaticMessageQueue<0> messageQueue;	// size 0, so queue is both full and empty at the same time
	const uint8_t constPriority {};
	const TestType constTestValue {};
	uint8_t nonConstPriority {};
	TestType nonConstTestValue {};

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is both full and empty, so tryPush(..., T&&) should fail immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryPush(constPriority, TestType{});	// 1 construction, 1 destruction
		if (ret != EAGAIN || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 1, 0, 0, 0) != true)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// message queue is both full and empty, so tryPushFor(..., const T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryPushFor(singleDuration, constPriority, constTestValue);
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

		// message queue is both full and empty, so tryPushFor(..., T&&) should time-out at expected time
		const auto start = TickClock::now();
		// 1 construction, 1 destruction
		const auto ret = messageQueue.tryPushFor(singleDuration, constPriority, TestType{});
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

		// message queue is both full and empty, so tryPushUntil(..., const T&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = messageQueue.tryPushUntil(requestedTimePoint, constPriority, constTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// message queue is both full and empty, so tryPushUntil(..., T&&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		// 1 construction, 1 destruction
		const auto ret = messageQueue.tryPushUntil(requestedTimePoint, constPriority, TestType{});
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(1, 0, 0, 1, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// message queue is both full and empty, so tryPopFor(..., T&) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
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

		// message queue is both full and empty, so tryPopUntil(..., T&) should time-out at exact expected time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = messageQueue.tryPopUntil(requestedTimePoint, nonConstPriority, nonConstTestValue);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	{
		// message queue is both full and empty, so tryEmplace(..., Args&&...) should fail immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryEmplace(constPriority);
		if (ret != EAGAIN || start != TickClock::now() || TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();

		// message queue is both full and empty, so tryEmplaceFor(..., Args&&...) should time-out at expected time
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryEmplaceFor(singleDuration, constPriority);
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

		// message queue is both full and empty, so tryEmplaceUntil(..., Args&&...) should time-out at exact expected
		// time
		const auto requestedTimePoint = TickClock::now() + singleDuration;
		const auto ret = messageQueue.tryEmplaceUntil(requestedTimePoint, constPriority);
		if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
				TestType::checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
			return false;
	}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly send data via non-full or non-empty
 * message queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	TestStaticMessageQueue<1> messageQueue;
	const uint8_t constPriority {};
	const TestType constTestValue {};
	uint8_t nonConstPriority {};
	TestType nonConstTestValue {};

	{
		// message queue is not full, so tryPush(..., const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryPush(constPriority, constTestValue);	// 1 copy construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not full, so tryPush(..., T&&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 construction, 1 move construction, 1 destruction
		const auto ret = messageQueue.tryPush(constPriority, TestType{});
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not full, so tryPushFor(..., const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 copy construction
		const auto ret = messageQueue.tryPushFor(singleDuration, constPriority, constTestValue);
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not empty, so tryPopFor(..., T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 swap, 1 destruction
		const auto ret = messageQueue.tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not full, so tryPushFor(..., T&&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 construction, 1 move construction, 1 destruction
		const auto ret = messageQueue.tryPushFor(singleDuration, constPriority, TestType{});
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not full, so tryPushUntil(..., const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 copy construction
		const auto ret = messageQueue.tryPushUntil(start + singleDuration, constPriority, constTestValue);
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not empty, so tryPopUntil(..., T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 swap, 1 destruction
		const auto ret = messageQueue.tryPopUntil(start + singleDuration, nonConstPriority, nonConstTestValue);
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not full, so tryPushUntil(..., T&&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		// 1 construction, 1 move construction, 1 destruction
		const auto ret = messageQueue.tryPushUntil(start + singleDuration, constPriority, TestType{});
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	{
		// message queue is not full, so tryEmplace(..., Args&&...) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryEmplace(constPriority);	// 1 construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not full, so tryEmplaceFor(..., Args&&...) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryEmplaceFor(singleDuration, constPriority);	// 1 construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		// message queue is not full, so tryEmplaceUntil(..., Args&&...) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryEmplaceUntil(start + singleDuration, constPriority);	// 1 construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		const auto ret = testTryPushWhenFull(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenNotEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests interrupt -> thread communication scenario. Main (current) thread waits for data to become available in message
 * queue. Software timer pushes some values to the same message queue at specified time point from interrupt context,
 * main thread is expected to receive these values (with pop(), tryPopFor() and tryPopUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	TestStaticMessageQueue<1> messageQueue;
	uint8_t sharedMagicPriority {};
	TestType sharedMagicValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&messageQueue, &sharedMagicPriority, &sharedMagicValue]()
			{
				messageQueue.tryPush(sharedMagicPriority, sharedMagicValue);
			});

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicPriority = 0x93;
		sharedMagicValue = TestType{0x2f5be1a4};	// 1 construction, 1 move assignment, 1 destruction
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

		// message queue is currently empty, but pop() should succeed at expected time
		uint8_t priority {};
		TestType testValue {};	// 1 construction
		const auto ret = messageQueue.pop(priority, testValue);	// 1 swap, 1 destruction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || priority != sharedMagicPriority ||
				testValue != sharedMagicValue || TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicPriority = 0x01;
		sharedMagicValue = TestType{0xc1fe105a};	// 1 construction, 1 move assignment, 1 destruction
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

		// message queue is currently empty, but tryPopFor() should succeed at expected time
		uint8_t priority {};
		TestType testValue {};	// 1 construction
		// 1 swap, 1 destruction
		const auto ret = messageQueue.tryPopFor(wakeUpTimePoint - TickClock::now() + longDuration, priority, testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || priority != sharedMagicPriority ||
				testValue != sharedMagicValue || TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		sharedMagicPriority = 0x48;
		sharedMagicValue = TestType{0xda0e4e30};	// 1 construction, 1 move assignment, 1 destruction
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

		// message queue is currently empty, but tryPopUntil() should succeed at expected time
		uint8_t priority {};
		TestType testValue {};	// 1 construction
		// 1 swap, 1 destruction
		const auto ret = messageQueue.tryPopUntil(wakeUpTimePoint + longDuration, priority, testValue);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || priority != sharedMagicPriority ||
				testValue != sharedMagicValue || TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		const auto ret = testTryPopWhenEmpty(messageQueue);
		if (ret != true)
			return ret;
	}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests thread -> interrupt communication scenario. Main (current) thread pushes data to message queue (which is
 * initially full). Software timer pops first value (which should match the one pushed previously) from the same message
 * queue at specified time point from interrupt context, main thread is expected to succeed in pushing new value (with\
 * emplace(), push(), tryEmplaceFor(), tryEmplaceUntil(), tryPushFor() and tryPushUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4()
{
	TestStaticMessageQueue<1> messageQueue;
	uint8_t receivedPriority {};
	TestType receivedTestValue {};
	auto softwareTimer = makeSoftwareTimer(
			[&messageQueue, &receivedPriority, &receivedTestValue]()
			{
				messageQueue.tryPop(receivedPriority, receivedTestValue);
			});

	uint8_t currentMagicPriority {0xc9};
	TestType currentMagicValue {0xa810b166};

	{
		// message queue is not full, so push(..., const T&) must succeed immediately
		TestType::resetCounters();
		waitForNextTick();
		const auto start = TickClock::now();
		const auto ret = messageQueue.tryPush(currentMagicPriority, currentMagicValue);	// 1 copy construction
		if (ret != 0 || start != TickClock::now() || TestType::checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but push(..., const T&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0x96;
		currentMagicValue = TestType{0xc9e7e479};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = messageQueue.push(currentMagicPriority, currentMagicValue);	// 1 copy construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but push(..., T&&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0x06;
		currentMagicValue = TestType{0x51607941};	// 1 construction, 1 move assignment, 1 destruction
		// 1 copy construction, 1 move construction, 1 destruction
		const auto ret = messageQueue.push(currentMagicPriority, TestType{currentMagicValue});
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but tryPushFor(..., const T&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0xcc;
		currentMagicValue = TestType{0xb9f4b42e};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = messageQueue.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
				currentMagicPriority, currentMagicValue);	// 1 copy construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but tryPushFor(..., T&&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0xf6;
		currentMagicValue = TestType{0xbb0bfe00};	// 1 construction, 1 move assignment, 1 destruction
		// 1 copy construction, 1 move construction, 1 destruction
		const auto ret = messageQueue.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
				currentMagicPriority, TestType{currentMagicValue});
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but tryPushUntil(..., const T&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0x2e;
		currentMagicValue = TestType{0x25eb4357};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = messageQueue.tryPushUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
				currentMagicValue);	// 1 copy construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but tryPushUntil(..., T&&) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0xb6;
		currentMagicValue = TestType{0x625652d7};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = messageQueue.tryPushUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
				TestType{currentMagicValue});	// 1 copy construction, 1 move construction, 1 destruction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but emplace(..., Args&&...) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0xe7;
		const TestType::Value value = 0x8de61877;
		currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = messageQueue.emplace(currentMagicPriority, value);	// 1 construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but tryEmplaceFor(..., Args&&...) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0x98;
		const TestType::Value value = 0x2b2cd349;
		currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
		const auto ret = messageQueue.tryEmplaceFor(wakeUpTimePoint - TickClock::now() + longDuration,
				currentMagicPriority, value);	// 1 construction
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

	{
		TestType::resetCounters();
		waitForNextTick();

		const auto contextSwitchCount = statistics::getContextSwitchCount();
		const auto wakeUpTimePoint = TickClock::now() + longDuration;
		softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

		// message queue is currently full, but tryEmplaceUntil(..., Args&&...) should succeed at expected time
		const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
		const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
		currentMagicPriority = 0xa5;
		const TestType::Value value = 0x7df8502a;
		currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
		// 1 construction
		const auto ret = messageQueue.tryEmplaceUntil(wakeUpTimePoint + longDuration, currentMagicPriority, value);
		const auto wokenUpTimePoint = TickClock::now();
		if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint || receivedPriority != expectedPriority ||
				receivedTestValue != expectedTestValue || TestType::checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
				statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
			return false;
	}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MessageQueueOperationsTestCase::run_() const
{
	constexpr auto emplace = DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1;

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
