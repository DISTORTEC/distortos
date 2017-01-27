/**
 * \file
 * \brief CallOnceOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "CallOnceOperationsTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "waitForNextTick.hpp"

#include "distortos/callOnce.hpp"
#include "distortos/DynamicThread.hpp"
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

/// sleepFor() duration used in function() passed to callOnce()
constexpr auto sleepForDuration = TickClock::duration{10};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/// number of test threads
constexpr size_t totalThreads {8};

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// pair of sequence points
using SequencePoints = std::pair<unsigned int, unsigned int>;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test function passed to callOnce().
 *
 * This function marks first sequence point, executes ThisThread::sleepFor() and marks second sequence point.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 */

void function(SequenceAsserter& sequenceAsserter)
{
	sequenceAsserter.sequencePoint(1);
	ThisThread::sleepFor(sleepForDuration);
	sequenceAsserter.sequencePoint(totalThreads + 1);
}

/**
 * \brief Test thread function
 *
 * This function marks first sequence point, passes function() to callOnce() and marks second sequence point.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] onceFlag is a reference to OnceFlag shared object
 */

void thread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, OnceFlag& onceFlag)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	callOnce(onceFlag, function, sequenceAsserter);
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Builder of test threads
 *
 * \param [in] priority is the thread's priority
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] onceFlag is a reference to OnceFlag shared object
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const uint8_t priority, SequenceAsserter& sequenceAsserter,
		const SequencePoints sequencePoints, OnceFlag& onceFlag)
{
	return makeDynamicThread({testThreadStackSize, priority}, thread, std::ref(sequenceAsserter), sequencePoints,
			std::ref(onceFlag));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool CallOnceOperationsTestCase::run_() const
{
	const auto contextSwitchCount = statistics::getContextSwitchCount();

	SequenceAsserter sequenceAsserter;
	OnceFlag onceFlag;

	std::array<DynamicThread, totalThreads> threads
	{{
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{0, 10}, onceFlag),
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{2, 11}, onceFlag),
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{3, 12}, onceFlag),
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{4, 13}, onceFlag),
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{5, 14}, onceFlag),
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{6, 15}, onceFlag),
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{7, 16}, onceFlag),
			makeTestThread(testCasePriority_ - 1, sequenceAsserter, SequencePoints{8, 17}, onceFlag),
	}};

	waitForNextTick();
	const auto start = TickClock::now();

	for (auto& thread : threads)
		thread.start();

	ThisThread::setPriority(testCasePriority_ - 2);

	bool invalidState {};
	for (size_t i = 1; i < threads.size(); ++i)
		if (threads[i].getState() != ThreadState::blockedOnMutex)
			invalidState = true;

	for (auto& thread : threads)
		thread.join();

	if (invalidState != false)
		return false;

	if (TickClock::now() - start != sleepForDuration + decltype(sleepForDuration){1})
		return false;

	if (sequenceAsserter.assertSequence(totalThreads * 2 + 2) == false)
		return false;

	constexpr auto totalContextSwitches = 2 * totalThreads + 3 + waitForNextTickContextSwitchCount;
	if (statistics::getContextSwitchCount() - contextSwitchCount != totalContextSwitches)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
