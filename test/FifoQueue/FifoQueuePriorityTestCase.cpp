/**
 * \file
 * \brief FifoQueuePriorityTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-12-14
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
constexpr size_t testThreadStackSize {256};

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

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

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

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FifoQueuePriorityTestCase::Implementation::run_() const
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;
		TestStaticFifoQueue fifoQueue;

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(popThread, 0, phase.first[phase.second[0]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 1, phase.first[phase.second[1]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 2, phase.first[phase.second[2]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 3, phase.first[phase.second[3]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 4, phase.first[phase.second[4]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 5, phase.first[phase.second[5]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 6, phase.first[phase.second[6]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 7, phase.first[phase.second[7]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 8, phase.first[phase.second[8]], sequenceAsserter, fifoQueue),
				makeTestThread(popThread, 9, phase.first[phase.second[9]], sequenceAsserter, fifoQueue),
		}};

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
			pushTrigger(fifoQueue, i);
			// 2 context switches: into" the unblocked thread and "back" to main thread when test thread terminates
			expectedContextSwitchCount += 2;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				result = false;
		}

		for (auto& thread : threads)
			thread.join();

		if (result == false || sequenceAsserter.assertSequence(totalThreads * 2) == false)
			return false;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != 4 * totalThreads * priorityTestPhases.size())
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
