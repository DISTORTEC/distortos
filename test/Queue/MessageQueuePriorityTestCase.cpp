/**
 * \file
 * \brief MessageQueuePriorityTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "MessageQueuePriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/StaticMessageQueue.hpp"
#include "distortos/statistics.hpp"
#include "distortos/ThisThread.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {384};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// pair of sequence points
using SequencePoints = std::pair<unsigned int, unsigned int>;

/// type of elements of \a TestMessageQueue
using TestType = unsigned int;

/// MessageQueue with \a TestType
using TestMessageQueue = MessageQueue<TestType>;

/// StaticMessageQueue with \a TestType, with storage for \a totalThreads elements
using TestStaticMessageQueue = StaticMessageQueue<TestType, totalThreads>;

/// type of test thread function
using TestThreadFunction = void(SequenceAsserter&, SequencePoints, TestMessageQueue&);

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, std::declval<TestThreadFunction>(),
		std::ref(std::declval<SequenceAsserter&>()), std::declval<SequencePoints>(),
		std::ref(std::declval<TestMessageQueue&>())));

/// function executed to prepare queue for test
using Prepare = void(TestMessageQueue&);

/// function executed on queue to trigger unblocking of test thread
using Trigger = bool(TestMessageQueue&, size_t, const ThreadParameters&);

/// function with final check of queue's contents after all test threads are terminated
using FinalCheck = bool(TestMessageQueue&);

/// tuple with functions for one stage, Prepare and FinalCheck may be nullptr
using Stage = std::tuple<const TestThreadFunction&, const Prepare* const, const Trigger* const,
		const FinalCheck* const>;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Final check for "pop" stage.
 *
 * The queue should contain "first" sequence points of test threads in decreasing order (priority is the same as value).
 *
 * \param [in] messageQueue is a reference to shared message queue
 *
 * \return true if final check succeeded, false otherwise
 */

bool popFinalCheck(TestMessageQueue& messageQueue)
{
	for (size_t i = 0; i < totalThreads; ++i)
	{
		uint8_t priority {};
		TestType testValue {};
		const auto ret = messageQueue.tryPop(priority, testValue);
		const auto expected = totalThreads - i - 1;
		if (ret != 0 || priority != expected || testValue != expected)
			return false;
	}

	return true;
}

/**
 * \brief Prepares queue for "pop" stage - just fills it completely with increasing values.
 *
 * \param [in] messageQueue is a reference to shared message queue
 */

void popPrepare(TestMessageQueue& messageQueue)
{
	for (size_t i = 0; i < totalThreads; ++i)
		messageQueue.tryPush(i, i);
}

/**
 * \brief MessageQueue::pop() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for the last sequence point from message queue and marks it
 * in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance (second one is ignored)
 * \param [in] messageQueue is a reference to shared message queue
 */

void popThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, TestMessageQueue& messageQueue)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	uint8_t priority;
	unsigned int lastSequencePoint {};
	messageQueue.pop(priority, lastSequencePoint);
	sequenceAsserter.sequencePoint(lastSequencePoint);
}

/**
 * \brief Trigger action with MessageQueue::pop().
 *
 * \param [in] messageQueue is a reference to shared message queue
 * \param [in] i is the iteration counter
 * \param [in] threadParameters is a reference to ThreadParameters object
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool popTrigger(TestMessageQueue& messageQueue, const size_t i, const ThreadParameters&)
{
	uint8_t priority {};
	TestType testValue {};
	messageQueue.pop(priority, testValue);
	const auto expected = totalThreads - i - 1;
	return priority == expected && testValue == expected;
}

/**
 * \brief MessageQueue::push() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for free space in message queue to send this first sequence
 * point (with priority equal to the value) and marks last sequence point in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] messageQueue is a reference to shared message queue
 */

void pushThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, TestMessageQueue& messageQueue)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	const uint8_t priority = sequencePoints.first;
	messageQueue.push(priority, sequencePoints.first);
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Trigger action with MessageQueue::push().
 *
 * \param [in] messageQueue is a reference to shared message queue
 * \param [in] i is the iteration counter
 * \param [in] threadParameters is a reference to ThreadParameters object
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool pushTrigger(TestMessageQueue& messageQueue, size_t, const ThreadParameters& threadParameters)
{
	messageQueue.push(threadParameters.first, totalThreads + threadParameters.second);
	return true;
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] testThreadFunction is a reference to test thread function that will be used in TestThread
 * \param [in] firstSequencePoint is the first sequence point for this instance - equal to the order in which this
 * thread will be started
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] messageQueue is a reference to shared message queue
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const TestThreadFunction& testThreadFunction, const unsigned int firstSequencePoint,
		const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter, TestMessageQueue& messageQueue)
{
	return makeStaticThread<testThreadStackSize>(threadParameters.first, testThreadFunction, std::ref(sequenceAsserter),
			SequencePoints{firstSequencePoint, threadParameters.second + totalThreads}, std::ref(messageQueue));
}

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// test stages
const std::array<Stage, 2> stages
{{
		Stage{popThread, nullptr, pushTrigger, nullptr},
		Stage{pushThread, popPrepare, popTrigger, popFinalCheck},
}};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MessageQueuePriorityTestCase::run_() const
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& stage : stages)
		for (const auto& phase : priorityTestPhases)
		{
			SequenceAsserter sequenceAsserter;
			TestStaticMessageQueue messageQueue;

			const auto& threadFunction = std::get<0>(stage);
			std::array<TestThread, totalThreads> threads
			{{
					makeTestThread(threadFunction, 0, phase.first[phase.second[0]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 1, phase.first[phase.second[1]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 2, phase.first[phase.second[2]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 3, phase.first[phase.second[3]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 4, phase.first[phase.second[4]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 5, phase.first[phase.second[5]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 6, phase.first[phase.second[6]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 7, phase.first[phase.second[7]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 8, phase.first[phase.second[8]], sequenceAsserter, messageQueue),
					makeTestThread(threadFunction, 9, phase.first[phase.second[9]], sequenceAsserter, messageQueue),
			}};

			// execute Prepare
			if (std::get<1>(stage) != nullptr)
				std::get<1>(stage)(messageQueue);

			bool result {true};

			for (auto& thread : threads)
			{
				thread.start();
				// 2 context switches: "into" the thread and "back" to main thread when test thread blocks on queue
				expectedContextSwitchCount += 2;
				if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
					result = false;
			}

			if (sequenceAsserter.assertSequence(totalThreads) == false)
				result = false;

			const auto priority = ThisThread::getPriority();
			ThisThread::setPriority(UINT8_MAX);	// make sure all test threads are "released" in the same moment

			for (size_t i = 0; i < threads.size(); ++i)
			{
				const auto triggerResult = std::get<2>(stage)(messageQueue, i, phase.first[phase.second[i]]);
				if (triggerResult == false)
					result = false;
			}

			ThisThread::setPriority(priority);	// restore previous priority

			// "totalThreads" context switches between unblocked test threads and 1 "back" to main thread
			expectedContextSwitchCount += totalThreads + 1;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				result = false;

			for (auto& thread : threads)
				thread.join();

			// execute FinalCheck
			if (std::get<3>(stage) != nullptr && std::get<3>(stage)(messageQueue) == false)
				result = false;

			if (result == false || sequenceAsserter.assertSequence(totalThreads * 2) == false)
				return false;
		}

	if (statistics::getContextSwitchCount() - contextSwitchCount !=
			(3 * totalThreads + 1) * priorityTestPhases.size() * stages.size())
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
