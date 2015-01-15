/**
 * \file
 * \brief FifoQueuePriorityTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-15
 */

#include "FifoQueuePriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/StaticFifoQueue.hpp"
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

/// type of elements of \a TestFifoQueue
using TestType = unsigned int;

/// FifoQueue with \a TestType
using TestFifoQueue = FifoQueue<TestType>;

/// StaticFifoQueue with \a TestType, with storage for \a totalThreads elements
using TestStaticFifoQueue = StaticFifoQueue<TestType, totalThreads>;

/// type of test thread function
using TestThreadFunction = void(SequenceAsserter&, SequencePoints, TestFifoQueue&);

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, std::declval<TestThreadFunction>(),
		std::ref(std::declval<SequenceAsserter&>()), std::declval<SequencePoints>(),
		std::ref(std::declval<TestFifoQueue&>())));

/// function executed to prepare queue for test
using Prepare = void(TestFifoQueue&);

/// function executed on queue to trigger unblocking of test thread
using Trigger = bool(TestFifoQueue&, size_t);

/// function with final check of queue's contents after all test threads are terminated
using FinalCheck = bool(TestFifoQueue&);

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
 * \param [in] fifoQueue is a reference to shared FIFO queue
 *
 * \return true if final check succeeded, false otherwise
 */

bool popFinalCheck(TestFifoQueue& fifoQueue)
{
	for (size_t i = 0; i < totalThreads; ++i)
	{
		TestType testValue {};
		const auto ret = fifoQueue.tryPop(testValue);
		if (ret != 0 || testValue != i + totalThreads)
			return false;
	}

	return true;
}

/**
 * \brief Prepares queue for "pop" stage - just fills it completely with increasing values.
 *
 * \param [in] fifoQueue is a reference to shared FIFO queue
 */

void popPrepare(TestFifoQueue& fifoQueue)
{
	for (size_t i = 0; i < totalThreads; ++i)
		fifoQueue.tryPush(i);
}

/**
 * \brief FifoQueue::pop() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for the last sequence point from FIFO queue and marks it in
 * SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance (second one is ignored)
 * \param [in] fifoQueue is a reference to shared FIFO queue
 */

void popThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, TestFifoQueue& fifoQueue)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	unsigned int lastSequencePoint {};
	fifoQueue.pop(lastSequencePoint);
	sequenceAsserter.sequencePoint(lastSequencePoint);
}

/**
 * \brief Trigger action with FifoQueue::pop().
 *
 * \param [in] fifoQueue is a reference to shared FIFO queue
 * \param [in] i is the iteration counter
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool popTrigger(TestFifoQueue& fifoQueue, const size_t i)
{
	TestType testValue {};
	fifoQueue.pop(testValue);
	return testValue == i;
}

/**
 * \brief FifoQueue::push() test thread
 *
 * Marks the first sequence point in SequenceAsserter, waits for free space in FIFO queue and marks last sequence point
 * in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] fifoQueue is a reference to shared FIFO queue
 */

void pushThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, TestFifoQueue& fifoQueue)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	fifoQueue.push(sequencePoints.second);
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Trigger action with FifoQueue::push().
 *
 * \param [in] fifoQueue is a reference to shared FIFO queue
 * \param [in] i is the iteration counter
 *
 * \return true if trigger check succeeded, false otherwise
 */

bool pushTrigger(TestFifoQueue& fifoQueue, const size_t i)
{
	fifoQueue.push(i + totalThreads);
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
 * \param [in] fifoQueue is a reference to shared FIFO queue
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const TestThreadFunction& testThreadFunction, const unsigned int firstSequencePoint,
		const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter, TestFifoQueue& fifoQueue)
{
	return makeStaticThread<testThreadStackSize>(threadParameters.first, testThreadFunction, std::ref(sequenceAsserter),
			SequencePoints{firstSequencePoint, threadParameters.second + totalThreads}, std::ref(fifoQueue));
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

bool FifoQueuePriorityTestCase::Implementation::run_() const
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& stage : stages)
		for (const auto& phase : priorityTestPhases)
		{
			SequenceAsserter sequenceAsserter;
			TestStaticFifoQueue fifoQueue;

			const auto& threadFunction = std::get<0>(stage);
			std::array<TestThread, totalThreads> threads
			{{
					makeTestThread(threadFunction, 0, phase.first[phase.second[0]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 1, phase.first[phase.second[1]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 2, phase.first[phase.second[2]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 3, phase.first[phase.second[3]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 4, phase.first[phase.second[4]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 5, phase.first[phase.second[5]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 6, phase.first[phase.second[6]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 7, phase.first[phase.second[7]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 8, phase.first[phase.second[8]], sequenceAsserter, fifoQueue),
					makeTestThread(threadFunction, 9, phase.first[phase.second[9]], sequenceAsserter, fifoQueue),
			}};

			// execute Prepare
			if (std::get<1>(stage) != nullptr)
				std::get<1>(stage)(fifoQueue);

			bool result {true};

			for (auto& thread : threads)
			{
				thread.start();
				// 2 context switches: "into" the thread and "back" to main thread when test thread blocks on FIFO queue
				expectedContextSwitchCount += 2;
				if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
					result = false;
			}

			if (sequenceAsserter.assertSequence(totalThreads) == false)
				result = false;

			for (size_t i = 0; i < threads.size(); ++i)
			{
				const auto triggerResult = std::get<2>(stage)(fifoQueue, i);
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
			if (std::get<3>(stage) != nullptr && std::get<3>(stage)(fifoQueue) == false)
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
