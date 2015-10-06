/**
 * \file
 * \brief RawFifoQueuePriorityTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "RawFifoQueuePriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/StaticRawFifoQueue.hpp"
#include "distortos/statistics.hpp"

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

/// type of elements in tested raw FIFO queue
using TestType = unsigned int;

/// StaticRawFifoQueue with \a TestType, with storage for \a totalThreads elements
using TestStaticRawFifoQueue = StaticRawFifoQueue<TestType, totalThreads>;

/// type of test thread function
using TestThreadFunction = void(SequenceAsserter&, SequencePoints, RawFifoQueue&);

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, std::declval<TestThreadFunction>(),
		std::ref(std::declval<SequenceAsserter&>()), std::declval<SequencePoints>(),
		std::ref(std::declval<RawFifoQueue&>())));

/// function executed to prepare queue for test
using Prepare = void(RawFifoQueue&);

/// function executed on queue to trigger unblocking of test thread
using Trigger = bool(RawFifoQueue&, size_t);

/// function with final check of queue's contents after all test threads are terminated
using FinalCheck = bool(RawFifoQueue&);

/// tuple with functions for one stage, Prepare and FinalCheck may be nullptr
using Stage = std::tuple<const TestThreadFunction&, const Prepare* const, const Trigger* const,
		const FinalCheck* const>;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Final check for "pop" stage.
 *
 * The queue should contain "second" sequence points of test threads in the same order as expected by SequenceAsserter.
 *
 * \param [in] rawFifoQueue is a reference to shared raw FIFO queue
 *
 * \return true if final check succeeded, false otherwise
 */

bool popFinalCheck(RawFifoQueue& rawFifoQueue)
{
	for (size_t i = 0; i < totalThreads; ++i)
	{
		TestType testValue {};
		const auto ret = rawFifoQueue.tryPop(testValue);
		if (ret != 0 || testValue != i + totalThreads)
			return false;
	}

	return true;
}

/**
 * \brief Prepares queue for "pop" stage - just fills it completely with increasing values.
 *
 * \param [in] rawFifoQueue is a reference to shared raw FIFO queue
 */

void popPrepare(RawFifoQueue& rawFifoQueue)
{
	for (size_t i = 0; i < totalThreads; ++i)
		rawFifoQueue.tryPush(i);
}

/**
 * \brief RawFifoQueue::pop() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for the last sequence point from raw FIFO queue and marks
 * it in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance (second one is ignored)
 * \param [in] rawFifoQueue is a reference to shared raw FIFO queue
 */

void popThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, RawFifoQueue& rawFifoQueue)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	unsigned int lastSequencePoint {};
	rawFifoQueue.pop(lastSequencePoint);
	sequenceAsserter.sequencePoint(lastSequencePoint);
}

/**
 * \brief Trigger action with RawFifoQueue::pop().
 *
 * \param [in] rawFifoQueue is a reference to shared raw FIFO queue
 * \param [in] i is the iteration counter
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool popTrigger(RawFifoQueue& rawFifoQueue, const size_t i)
{
	TestType testValue {};
	rawFifoQueue.pop(testValue);
	return testValue == i;
}

/**
 * \brief RawFifoQueue::push() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for free space in raw FIFO queue and marks last sequence
 * point in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] rawFifoQueue is a reference to shared raw FIFO queue
 */

void pushThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, RawFifoQueue& rawFifoQueue)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	rawFifoQueue.push(sequencePoints.second);
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Trigger action with RawFifoQueue::push().
 *
 * \param [in] rawFifoQueue is a reference to shared raw FIFO queue
 * \param [in] i is the iteration counter
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool pushTrigger(RawFifoQueue& rawFifoQueue, const size_t i)
{
	rawFifoQueue.push(i + totalThreads);
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
 * \param [in] rawFifoQueue is a reference to shared raw FIFO queue
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const TestThreadFunction& testThreadFunction, const unsigned int firstSequencePoint,
		const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter, RawFifoQueue& rawFifoQueue)
{
	return makeStaticThread<testThreadStackSize>(threadParameters.first, testThreadFunction, std::ref(sequenceAsserter),
			SequencePoints{firstSequencePoint, threadParameters.second + totalThreads}, std::ref(rawFifoQueue));
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

bool RawFifoQueuePriorityTestCase::run_() const
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& stage : stages)
		for (const auto& phase : priorityTestPhases)
		{
			SequenceAsserter sequenceAsserter;
			TestStaticRawFifoQueue rawFifoQueue;

			const auto& threadFunction = std::get<0>(stage);
			std::array<TestThread, totalThreads> threads
			{{
					makeTestThread(threadFunction, 0, phase.first[phase.second[0]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 1, phase.first[phase.second[1]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 2, phase.first[phase.second[2]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 3, phase.first[phase.second[3]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 4, phase.first[phase.second[4]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 5, phase.first[phase.second[5]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 6, phase.first[phase.second[6]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 7, phase.first[phase.second[7]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 8, phase.first[phase.second[8]], sequenceAsserter, rawFifoQueue),
					makeTestThread(threadFunction, 9, phase.first[phase.second[9]], sequenceAsserter, rawFifoQueue),
			}};

			// execute Prepare
			if (std::get<1>(stage) != nullptr)
				std::get<1>(stage)(rawFifoQueue);

			bool result {true};

			for (auto& thread : threads)
			{
				thread.start();
				// 2 context switches: "into" the thread and "back" to main thread when test thread blocks on raw FIFO
				// queue
				expectedContextSwitchCount += 2;
				if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
					result = false;
			}

			if (sequenceAsserter.assertSequence(totalThreads) == false)
				result = false;

			for (size_t i = 0; i < threads.size(); ++i)
			{
				const auto triggerResult = std::get<2>(stage)(rawFifoQueue, i);
				if (triggerResult == false)
					result = false;
				// 2 context switches: into" the unblocked thread and "back" to main thread when test thread terminates
				expectedContextSwitchCount += 2;
				if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
					result = false;
			}

			for (auto& thread : threads)
				thread.join();

			// execute FinalCheck
			if (std::get<3>(stage) != nullptr && std::get<3>(stage)(rawFifoQueue) == false)
				result = false;

			if (result == false || sequenceAsserter.assertSequence(totalThreads * 2) == false)
				return false;
		}

	if (statistics::getContextSwitchCount() - contextSwitchCount != 2 * 4 * totalThreads * priorityTestPhases.size())
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
