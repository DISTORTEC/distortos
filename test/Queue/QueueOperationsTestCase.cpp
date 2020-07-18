/**
 * \file
 * \brief QueueOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "QueueOperationsTestCase.hpp"

#include "QueueWrappers.hpp"

#include "waitForNextTick.hpp"

#include "distortos/StaticSoftwareTimer.hpp"
#include "distortos/statistics.hpp"

#include <malloc.h>

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

/// array with "false" and "true" bool values
constexpr bool falseTrue[] {false, true};

/// single duration used in tests
constexpr auto singleDuration = TickClock::duration{1};

/// long duration used in tests
constexpr auto longDuration = singleDuration * 10;

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/// expected number of context switches in phase1 block involving tryEmplaceFor(), tryEmplaceUntil(), tryPopFor(),
/// tryPopUntil(), tryPushFor() or tryPushUntil() (excluding waitForNextTick()): 1 - main thread blocks on queue
/// (main -> idle), 2 - main thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TryForUntilContextSwitchCount {2};

/// expected number of context switches in phase3 and phase4 block involving software timer (excluding
/// waitForNextTick()): 1 - main thread blocks on queue (main -> idle), 2 - main thread is unblocked by interrupt
/// (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase34SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests QueueWrapper::tryPop() when queue is empty - it must fail immediately and return EAGAIN
 *
 * \param [in] queueWrapper is a reference to QueueWrapper that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenEmpty(const QueueWrapper& queueWrapper)
{
	// queue is empty, so tryPop(T&) should fail immediately
	OperationCountingType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	OperationCountingType testValue {};	// 1 construction
	const auto ret = queueWrapper.tryPop(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start && queueWrapper.checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Tests QueueWrapper::tryPop() when queue is not empty - it must succeed immediately
 *
 * \param [in] queueWrapper is a reference to QueueWrapper that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenNotEmpty(const QueueWrapper& queueWrapper)
{
	OperationCountingType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	OperationCountingType testValue {};	// 1 construction
	const auto ret = queueWrapper.tryPop(priority, testValue);	// 1 swap, 1 destruction
	return ret == 0 && start == TickClock::now() && queueWrapper.checkCounters(1, 0, 0, 1, 0, 0, 1) == true;
}

/**
 * \brief Tests QueueWrapper::tryPush(..., const T&) when queue is full - it must fail immediately and return EAGAIN
 *
 * \param [in] queueWrapper is a reference to QueueWrapper that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPushWhenFull(const QueueWrapper& queueWrapper)
{
	// queue is full, so tryPush(..., const T&) should fail immediately
	OperationCountingType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	const uint8_t priority {};
	const OperationCountingType testValue {};	// 1 construction
	const auto ret = queueWrapper.tryPush(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start && queueWrapper.checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly return some error when dealing with full
 * or empty queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	for (const auto dynamic : falseTrue)
		for (const auto raw : falseTrue)
			for (const auto fifo : falseTrue)
			{
				// size 0, so queue is both full and empty at the same time
				const auto queueWrapper = makeQueueWrapper<0>(dynamic, raw, fifo);
				const uint8_t constPriority {};
				const OperationCountingType constTestValue {};
				uint8_t nonConstPriority {};
				OperationCountingType nonConstTestValue {};

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is both full and empty, so tryPush(..., T&&) should fail immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 construction, 1 destruction
					const auto ret = queueWrapper->tryPush(constPriority, OperationCountingType{});
					if (ret != EAGAIN || start != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 0, 1, 0, 0, 0) != true)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryPushFor(..., const T&) should time-out at expected time
					const auto start = TickClock::now();
					const auto ret = queueWrapper->tryPushFor(singleDuration, constPriority, constTestValue);
					const auto realDuration = TickClock::now() - start;
					if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
							queueWrapper->checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryPushFor(..., T&&) should time-out at expected time
					const auto start = TickClock::now();
					// 1 construction, 1 destruction
					const auto ret = queueWrapper->tryPushFor(singleDuration, constPriority, OperationCountingType{});
					const auto realDuration = TickClock::now() - start;
					if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
							queueWrapper->checkCounters(1, 0, 0, 1, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryPushUntil(..., const T&) should time-out at exact expected
					// time
					const auto requestedTimePoint = TickClock::now() + singleDuration;
					const auto ret = queueWrapper->tryPushUntil(requestedTimePoint, constPriority, constTestValue);
					if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
							queueWrapper->checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryPushUntil(..., T&&) should time-out at exact expected time
					const auto requestedTimePoint = TickClock::now() + singleDuration;
					// 1 construction, 1 destruction
					const auto ret = queueWrapper->tryPushUntil(requestedTimePoint, constPriority,
							OperationCountingType{});
					if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 0, 1, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryPopFor(..., T&) should time-out at expected time
					const auto start = TickClock::now();
					const auto ret = queueWrapper->tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
					const auto realDuration = TickClock::now() - start;
					if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
							queueWrapper->checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryPopUntil(..., T&) should time-out at exact expected time
					const auto requestedTimePoint = TickClock::now() + singleDuration;
					const auto ret = queueWrapper->tryPopUntil(requestedTimePoint, nonConstPriority, nonConstTestValue);
					if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
							queueWrapper->checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}

				{
					// queue is both full and empty, so tryEmplace(..., Args&&...) should fail immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					const auto ret = queueWrapper->tryEmplace(constPriority);
					if (ret != EAGAIN || start != TickClock::now() ||
							queueWrapper->checkCounters(0, 0, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryEmplaceFor(..., Args&&...) should time-out at expected time
					const auto start = TickClock::now();
					const auto ret = queueWrapper->tryEmplaceFor(singleDuration, constPriority);
					const auto realDuration = TickClock::now() - start;
					if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
							queueWrapper->checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();

					// queue is both full and empty, so tryEmplaceUntil(..., Args&&...) should time-out at exact
					// expected time
					const auto requestedTimePoint = TickClock::now() + singleDuration;
					const auto ret = queueWrapper->tryEmplaceUntil(requestedTimePoint, constPriority);
					if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
							queueWrapper->checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase1TryForUntilContextSwitchCount)
						return false;
				}
			}

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly send data via non-full or non-empty
 * queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	for (const auto dynamic : falseTrue)
		for (const auto raw : falseTrue)
			for (const auto fifo : falseTrue)
			{
				const auto queueWrapper = makeQueueWrapper<1>(dynamic, raw, fifo);
				const uint8_t constPriority {};
				const OperationCountingType constTestValue {};
				uint8_t nonConstPriority {};
				OperationCountingType nonConstTestValue {};

				{
					// queue is not full, so tryPush(..., const T&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					const auto ret = queueWrapper->tryPush(constPriority, constTestValue);	// 1 copy construction
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenNotEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryPush(..., T&&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 construction, 1 move construction, 1 destruction
					const auto ret = queueWrapper->tryPush(constPriority, OperationCountingType{});
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenNotEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryPushFor(..., const T&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 copy construction
					const auto ret = queueWrapper->tryPushFor(singleDuration, constPriority, constTestValue);
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not empty, so tryPopFor(..., T&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 swap, 1 destruction
					const auto ret = queueWrapper->tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
						return false;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryPushFor(..., T&&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 construction, 1 move construction, 1 destruction
					const auto ret = queueWrapper->tryPushFor(singleDuration, constPriority, OperationCountingType{});
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenNotEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryPushUntil(..., const T&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 copy construction
					const auto ret = queueWrapper->tryPushUntil(start + singleDuration, constPriority, constTestValue);
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not empty, so tryPopUntil(..., T&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 swap, 1 destruction
					const auto ret = queueWrapper->tryPopUntil(start + singleDuration, nonConstPriority,
							nonConstTestValue);
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
						return false;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryPushUntil(..., T&&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 construction, 1 move construction, 1 destruction
					const auto ret = queueWrapper->tryPushUntil(start + singleDuration, constPriority,
							OperationCountingType{});
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenNotEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryEmplace(..., Args&&...) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					const auto ret = queueWrapper->tryEmplace(constPriority);	// 1 construction
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenNotEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryEmplaceFor(..., Args&&...) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					const auto ret = queueWrapper->tryEmplaceFor(singleDuration, constPriority);	// 1 construction
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenNotEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					// queue is not full, so tryEmplaceUntil(..., Args&&...) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 construction
					const auto ret = queueWrapper->tryEmplaceUntil(start + singleDuration, constPriority);
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					const auto ret = testTryPushWhenFull(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenNotEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}
			}

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests interrupt -> thread communication scenario. Main (current) thread waits for data to become available in queue.
 * Software timer pushes some values to the same queue at specified time point from interrupt context, main thread is
 * expected to receive these values (with pop(), tryPopFor() and tryPopUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	for (const auto dynamic : falseTrue)
		for (const auto raw : falseTrue)
			for (const auto fifo : falseTrue)
			{
				const auto queueWrapper = makeQueueWrapper<1>(dynamic, raw, fifo);
				uint8_t sharedMagicPriority {};
				OperationCountingType sharedMagicValue {};
				auto softwareTimer = makeStaticSoftwareTimer(
						[&queueWrapper, &sharedMagicPriority, &sharedMagicValue]()
						{
							queueWrapper->tryPush(sharedMagicPriority, sharedMagicValue);
						});

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					sharedMagicPriority = 0x93;
					// 1 construction, 1 move assignment, 1 destruction
					sharedMagicValue = OperationCountingType{0x2f5be1a4};
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

					// queue is currently empty, but pop() should succeed at expected time
					uint8_t priority {};
					OperationCountingType testValue {};	// 1 construction
					const auto ret = queueWrapper->pop(priority, testValue);	// 1 swap, 1 destruction
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(sharedMagicPriority, sharedMagicValue, priority, testValue) == false ||
							queueWrapper->checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					sharedMagicPriority = 0x01;
					// 1 construction, 1 move assignment, 1 destruction
					sharedMagicValue = OperationCountingType{0xc1fe105a};
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

					// queue is currently empty, but tryPopFor() should succeed at expected time
					uint8_t priority {};
					OperationCountingType testValue {};	// 1 construction
					// 1 swap, 1 destruction
					const auto ret = queueWrapper->tryPopFor(wakeUpTimePoint - TickClock::now() + longDuration,
							priority, testValue);
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(sharedMagicPriority, sharedMagicValue, priority, testValue) == false ||
							queueWrapper->checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					sharedMagicPriority = 0x48;
					// 1 construction, 1 move assignment, 1 destruction
					sharedMagicValue = OperationCountingType{0xda0e4e30};
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

					// queue is currently empty, but tryPopUntil() should succeed at expected time
					uint8_t priority {};
					OperationCountingType testValue {};	// 1 construction
					// 1 swap, 1 destruction
					const auto ret = queueWrapper->tryPopUntil(wakeUpTimePoint + longDuration, priority, testValue);
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(sharedMagicPriority, sharedMagicValue, priority, testValue) == false ||
							queueWrapper->checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					const auto ret = testTryPopWhenEmpty(*queueWrapper);
					if (ret != true)
						return ret;
				}

			}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests thread -> interrupt communication scenario. Main (current) thread pushes data to queue (which is initially
 * full). Software timer pops first value (which should match the one pushed previously) from the same queue at
 * specified time point from interrupt context, main thread is expected to succeed in pushing new value (with emplace(),
 * push(), tryEmplaceFor(), tryEmplaceUntil(), tryPushFor() and tryPushUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4()
{
	for (const auto dynamic : falseTrue)
		for (const auto raw : falseTrue)
			for (const auto fifo : falseTrue)
			{
				const auto queueWrapper = makeQueueWrapper<1>(dynamic, raw, fifo);
				uint8_t receivedPriority {};
				OperationCountingType receivedTestValue {};
				auto softwareTimer = makeStaticSoftwareTimer(
						[&queueWrapper, &receivedPriority, &receivedTestValue]()
						{
							queueWrapper->tryPop(receivedPriority, receivedTestValue);
						});

				uint8_t currentMagicPriority {0xc9};
				OperationCountingType currentMagicValue {0xa810b166};

				{
					// queue is not full, so push(..., const T&) must succeed immediately
					OperationCountingType::resetCounters();
					waitForNextTick();
					const auto start = TickClock::now();
					// 1 copy construction
					const auto ret = queueWrapper->tryPush(currentMagicPriority, currentMagicValue);
					if (ret != 0 || start != TickClock::now() ||
							queueWrapper->checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but push(..., const T&) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0x96;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{0xc9e7e479};
					// 1 copy construction
					const auto ret = queueWrapper->push(currentMagicPriority, currentMagicValue);
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but push(..., T&&) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0x06;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{0x51607941};
					// 1 copy construction, 1 move construction, 1 destruction
					const auto ret = queueWrapper->push(currentMagicPriority, OperationCountingType{currentMagicValue});
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but tryPushFor(..., const T&) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0xcc;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{0xb9f4b42e};
					const auto ret = queueWrapper->tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
							currentMagicPriority, currentMagicValue);	// 1 copy construction
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but tryPushFor(..., T&&) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0xf6;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{0xbb0bfe00};
					// 1 copy construction, 1 move construction, 1 destruction
					const auto ret = queueWrapper->tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
							currentMagicPriority, OperationCountingType{currentMagicValue});
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but tryPushUntil(..., const T&) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0x2e;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{0x25eb4357};
					const auto ret = queueWrapper->tryPushUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
							currentMagicValue);	// 1 copy construction
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but tryPushUntil(..., T&&) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0xb6;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{0x625652d7};
					// 1 copy construction, 1 move construction, 1 destruction
					const auto ret = queueWrapper->tryPushUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
							OperationCountingType{currentMagicValue});
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but emplace(..., Args&&...) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0xe7;
					const OperationCountingType::Value value = 0x8de61877;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{value};
					const auto ret = queueWrapper->emplace(currentMagicPriority, value);	// 1 construction
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but tryEmplaceFor(..., Args&&...) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0x98;
					const OperationCountingType::Value value = 0x2b2cd349;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{value};
					const auto ret = queueWrapper->tryEmplaceFor(wakeUpTimePoint - TickClock::now() + longDuration,
							currentMagicPriority, value);	// 1 construction
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}

				{
					OperationCountingType::resetCounters();
					waitForNextTick();

					const auto contextSwitchCount = statistics::getContextSwitchCount();
					const auto wakeUpTimePoint = TickClock::now() + longDuration;
					softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

					// queue is currently full, but tryEmplaceUntil(..., Args&&...) should succeed at expected time
					const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
					const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
					currentMagicPriority = 0xa5;
					const OperationCountingType::Value value = 0x7df8502a;
					// 1 construction, 1 move assignment, 1 destruction
					currentMagicValue = OperationCountingType{value};
					// 1 construction
					const auto ret = queueWrapper->tryEmplaceUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
							value);
					const auto wokenUpTimePoint = TickClock::now();
					if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
							queueWrapper->check(expectedPriority, expectedTestValue, receivedPriority,
									receivedTestValue) == false ||
							queueWrapper->checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
							statistics::getContextSwitchCount() - contextSwitchCount !=
									phase34SoftwareTimerContextSwitchCount)
						return false;
				}
			}

	return true;
}

/**
 * \brief Phase 5 of test case.
 *
 * Tests whether all \*push\*() and \*pop\*() functions of "raw" queue properly return some error when given invalid
 * size of buffer.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase5()
{
	for (const auto dynamic : falseTrue)
		for (const auto fifo : falseTrue)
		{
			// size 0, so queue is both full and empty at the same time
			const auto rawQueueWrapper = makeRawQueueWrapper<0>(dynamic, fifo);
			const uint8_t constPriority {};
			const OperationCountingType constTestValue {};
			uint8_t nonConstPriority {};
			OperationCountingType nonConstTestValue {};

			{
				// invalid size is given, so push(..., const void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->push(constPriority, &constTestValue, sizeof(constTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}

			{
				// invalid size is given, so tryPush(..., const void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->tryPush(constPriority, &constTestValue, sizeof(constTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}

			{
				// invalid size is given, so tryPushFor(..., const void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->tryPushFor(singleDuration, constPriority, &constTestValue,
						sizeof(constTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}

			{
				// invalid size is given, so tryPushUntil(..., const void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->tryPushUntil(TickClock::now() + singleDuration, constPriority,
						&constTestValue, sizeof(constTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}

			{
				// invalid size is given, so pop(..., void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->pop(nonConstPriority, &nonConstTestValue,
						sizeof(nonConstTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}

			{
				// invalid size is given, so tryPop(..., void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->tryPop(nonConstPriority, &nonConstTestValue,
						sizeof(nonConstTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}

			{
				// invalid size is given, so tryPopFor(..., void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->tryPopFor(singleDuration, nonConstPriority, &nonConstTestValue,
						sizeof(nonConstTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}

			{
				// invalid size is given, so tryPopUntil(..., void*, size_t) should fail immediately
				waitForNextTick();
				const auto start = TickClock::now();
				const auto ret = rawQueueWrapper->tryPopUntil(TickClock::now() + singleDuration, nonConstPriority,
						&nonConstTestValue, sizeof(nonConstTestValue) - 1);
				if (ret != EMSGSIZE || TickClock::now() != start)
					return false;
			}
		}

	return true;
}

/**
 * \brief Phase 6 of test case.
 *
 * Tests whether destructor of "non-raw" queue properly destructs objects that remain in the queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase6()
{
	for (const auto dynamic : falseTrue)
		for (const auto fifo : falseTrue)
		{
			const OperationCountingType constTestValue {};

			{
				const auto queueWrapper = makeQueueWrapper<1>(dynamic, false, fifo);
				queueWrapper->push({}, constTestValue);
				OperationCountingType::resetCounters();
				// in destructor - 1 construction, 2 destructions and 1 swap
			}

			if (OperationCountingType::checkCounters(1, 0, 0, 2, 0, 0, 1) == false)
				return false;
		}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool QueueOperationsTestCase::run_() const
{
	constexpr size_t nonRawQueueTypes {4};
	constexpr size_t rawQueueTypes {4};
	constexpr size_t queueTypes {nonRawQueueTypes + rawQueueTypes};
	constexpr auto phase1ExpectedContextSwitchCount = queueTypes *
			(12 * waitForNextTickContextSwitchCount + 8 * phase1TryForUntilContextSwitchCount);
	constexpr auto phase2ExpectedContextSwitchCount = queueTypes * 36 * waitForNextTickContextSwitchCount;
	constexpr auto phase3ExpectedContextSwitchCount = queueTypes * (6 * waitForNextTickContextSwitchCount +
			3 * phase34SoftwareTimerContextSwitchCount);
	constexpr auto phase4ExpectedContextSwitchCount = queueTypes *
			(10 * waitForNextTickContextSwitchCount + 9 * phase34SoftwareTimerContextSwitchCount);
	constexpr auto phase5ExpectedContextSwitchCount = rawQueueTypes * 8 * waitForNextTickContextSwitchCount;
	constexpr auto expectedContextSwitchCount = phase1ExpectedContextSwitchCount + phase2ExpectedContextSwitchCount +
			phase3ExpectedContextSwitchCount + phase4ExpectedContextSwitchCount + phase5ExpectedContextSwitchCount;

	const auto allocatedMemory = mallinfo().uordblks;
	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2, phase3, phase4, phase5, phase6})
	{
		const auto ret = function();
		if (ret != true)
			return ret;

		if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
			return false;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
