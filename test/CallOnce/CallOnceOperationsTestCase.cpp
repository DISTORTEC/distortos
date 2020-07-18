/**
 * \file
 * \brief CallOnceOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "CallOnceOperationsTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "wasteTime.hpp"

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

/// size of stack for first test thread, bytes
constexpr size_t testThreadHighStackSize {768};

/// size of stack for remaining test threads, bytes
constexpr size_t testThreadLowStackSize {512};

/// number of test threads
constexpr size_t totalThreads {8};

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
 * This function marks first sequence point, passes function() to callOnce(), saves wake time point and marks second
 * sequence point.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] onceFlag is a reference to OnceFlag shared object
 * \param [out] timePoint is a reference to variable into which wake time point will be saved
 */

void thread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints, OnceFlag& onceFlag,
		TickClock::time_point& timePoint)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	callOnce(onceFlag, function, sequenceAsserter);
	timePoint = TickClock::now();
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Builder of test threads
 *
 * \param [in] stackSize is the size of stack for test thread, bytes
 * \param [in] priority is the thread's priority
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 * \param [in] onceFlag is a reference to OnceFlag shared object
 * \param [in] timePoint is a reference to variable into which wake time point of thread will be saved
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const size_t stackSize, const uint8_t priority, SequenceAsserter& sequenceAsserter,
		const SequencePoints sequencePoints, OnceFlag& onceFlag, TickClock::time_point& timePoint)
{
	return makeDynamicThread({stackSize, priority}, thread, std::ref(sequenceAsserter), sequencePoints,
			std::ref(onceFlag), std::ref(timePoint));
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
	std::array<TickClock::time_point, totalThreads> timePoints;

	std::array<DynamicThread, totalThreads> threads
	{{
			makeTestThread(testThreadHighStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{0, 10},
					onceFlag, timePoints[0]),
			makeTestThread(testThreadLowStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{2, 11},
					onceFlag, timePoints[1]),
			makeTestThread(testThreadLowStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{3, 12},
					onceFlag, timePoints[2]),
			makeTestThread(testThreadLowStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{4, 13},
					onceFlag, timePoints[3]),
			makeTestThread(testThreadLowStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{5, 14},
					onceFlag, timePoints[4]),
			makeTestThread(testThreadLowStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{6, 15},
					onceFlag, timePoints[5]),
			makeTestThread(testThreadLowStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{7, 16},
					onceFlag, timePoints[6]),
			makeTestThread(testThreadLowStackSize, testCasePriority_ - 1, sequenceAsserter, SequencePoints{8, 17},
					onceFlag, timePoints[7]),
	}};

	for (auto& thread : threads)
		thread.start();

	wasteTime(TickClock::duration{});
	const auto start = TickClock::now();

	ThisThread::setPriority(testCasePriority_ - 2);

	bool invalidState {};
	for (size_t i = 1; i < threads.size(); ++i)
		if (threads[i].getState() != ThreadState::blockedOnMutex)
			invalidState = true;

	for (auto& thread : threads)
		thread.join();

	if (invalidState != false)
		return false;

	for (const auto& timePoint : timePoints)
		if (timePoint - start != sleepForDuration + decltype(sleepForDuration){1})
			return false;

	if (sequenceAsserter.assertSequence(totalThreads * 2 + 2) == false)
		return false;

	constexpr auto totalContextSwitches = 2 * totalThreads + 3;
	if (statistics::getContextSwitchCount() - contextSwitchCount != totalContextSwitches)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
