/**
 * \file
 * \brief SemaphorePriorityTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-19
 */

#include "SemaphorePriorityTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/statistics.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// pair of sequence points
using SequencePoints = std::pair<unsigned int, unsigned int>;

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// priority of current test thread - just above idle, lower than any value in priorityTestPhases array
constexpr uint8_t testThreadPriority {1};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {256};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(SequenceAsserter& sequenceAsserter, SequencePoints sequencePoints, Semaphore& semaphore);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, thread,
		std::ref(std::declval<SequenceAsserter&>()), std::declval<SequencePoints>(),
		std::ref(std::declval<Semaphore&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Marks the first sequence point in SequenceAsserter, waits for the semaphore and marks the last sequence point in
 * SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] semaphore is a reference to shared semaphore
 */

void thread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, Semaphore& semaphore)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	semaphore.wait();
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] firstSequencePoint is the first sequence point for this instance - equal to the order in which this
 * thread will be started
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] semaphore is a reference to shared semaphore
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const unsigned int firstSequencePoint, const ThreadParameters& threadParameters,
		SequenceAsserter& sequenceAsserter, Semaphore& semaphore)
{
	return makeStaticThread<testThreadStackSize>(threadParameters.first, thread, std::ref(sequenceAsserter),
			SequencePoints{firstSequencePoint, threadParameters.second + totalThreads}, std::ref(semaphore));
}

/**
 * \brief Runs the test case.
 *
 * \attention this function expects the priority of test thread to be testThreadPriority
 *
 * \return true if the test case succeeded, false otherwise
 */

bool testRunner()
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();
	std::remove_const<decltype(contextSwitchCount)>::type expectedContextSwitchCount {};

	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;
		Semaphore semaphore {0};

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(0, phase.first[phase.second[0]], sequenceAsserter, semaphore),
				makeTestThread(1, phase.first[phase.second[1]], sequenceAsserter, semaphore),
				makeTestThread(2, phase.first[phase.second[2]], sequenceAsserter, semaphore),
				makeTestThread(3, phase.first[phase.second[3]], sequenceAsserter, semaphore),
				makeTestThread(4, phase.first[phase.second[4]], sequenceAsserter, semaphore),
				makeTestThread(5, phase.first[phase.second[5]], sequenceAsserter, semaphore),
				makeTestThread(6, phase.first[phase.second[6]], sequenceAsserter, semaphore),
				makeTestThread(7, phase.first[phase.second[7]], sequenceAsserter, semaphore),
				makeTestThread(8, phase.first[phase.second[8]], sequenceAsserter, semaphore),
				makeTestThread(9, phase.first[phase.second[9]], sequenceAsserter, semaphore),
		}};

		bool result {true};

		for (auto& thread : threads)
		{
			thread.start();
			// 2 context switches: "into" the thread and "back" to main thread when test thread blocks on semaphore
			expectedContextSwitchCount += 2;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
				result = false;
		}

		if (sequenceAsserter.assertSequence(totalThreads) == false || semaphore.getValue() != 0)
			result = false;

		for (size_t i = 0; i < threads.size(); ++i)
		{
			semaphore.post();
			// 2 context switches: into" the unblocked thread and "back" to main thread when test thread terminates
			expectedContextSwitchCount += 2;
			if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount ||
					semaphore.getValue() != 0)
				result = false;
		}

		for (auto& thread : threads)
			thread.join();

		if (result == false || sequenceAsserter.assertSequence(totalThreads * 2) == false || semaphore.getValue() != 0)
			return false;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != 4 * totalThreads * priorityTestPhases.size())
		return false;

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SemaphorePriorityTestCase::Implementation::run_() const
{
	const auto thisThreadPriority = ThisThread::getPriority();
	ThisThread::setPriority(testThreadPriority);
	const auto ret = testRunner();
	ThisThread::setPriority(thisThreadPriority);
	return ret;
}

}	// namespace test

}	// namespace distortos
