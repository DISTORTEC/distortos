/**
 * \file
 * \brief ThreadPriorityTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-24
 */

#include "ThreadPriorityTestCase.hpp"

#include "SequenceAsserter.hpp"

#include "distortos/scheduler/StaticThread.hpp"
#include "distortos/scheduler/Scheduler.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// number of test threads
constexpr size_t totalThreads {10};

/// number of test phases
constexpr size_t totalPhases {8};

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {128};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread_(SequenceAsserter& sequenceAsserter, unsigned int sequencePoint);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// parameters of test thread - priority, sequence point
using ThreadParameters = std::pair<uint8_t, uint8_t>;

/// description of test phase - reference to array with ThreadParameters, sequence of indexes for this array
using TestPhase = std::pair<const std::array<ThreadParameters, totalThreads>&, std::array<uint8_t, totalThreads>>;

/// type of test thread
using TestThread = decltype(scheduler::makeStaticThread<testThreadStackSize>({}, thread_,
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>()));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Just marks the sequence point in SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 */

void thread_(SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint)
{
	sequenceAsserter.sequencePoint(sequencePoint);
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const ThreadParameters &threadParameters, SequenceAsserter& sequenceAsserter)
{
	return scheduler::makeStaticThread<testThreadStackSize>(threadParameters.first, thread_, std::ref(sequenceAsserter),
		static_cast<unsigned int>(threadParameters.second));
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with unique priorities and expected sequence points
const std::array<ThreadParameters, totalThreads> uniquePriorities
{{
		{10, 0},
		{9, 1},
		{8, 2},
		{7, 3},
		{6, 4},
		{5, 5},
		{4, 6},
		{3, 7},
		{2, 8},
		{1, 9},
}};

/// array with non-unique priorities and expected sequence points
const std::array<ThreadParameters, totalThreads> nonUniquePriorities
{{
		{2, 0},
		{2, 1},
		{2, 2},
		{2, 3},
		{2, 4},
		{1, 5},
		{1, 6},
		{1, 7},
		{1, 8},
		{1, 9},
}};

/// array with test phases
const std::array<TestPhase, totalPhases> testPhases
{{
	// phase 0 - threads added with decreasing unique priority
	{uniquePriorities, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	// phase 1 - threads added with increasing unique priority
	{uniquePriorities, {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}},
	// phase 2 - threads added with random unique priority
	{uniquePriorities, {7, 2, 6, 3, 0, 8, 5, 9, 1, 4}},
	// phase 3 - multiple threads with identical priority (block 1)
	{nonUniquePriorities, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	// phase 4 - multiple threads with identical priority (block 2)
	{nonUniquePriorities, {5, 6, 7, 8, 9, 0, 1, 2, 3, 4}},
	// phase 5 - multiple threads with identical priority (alternating 1)
	{nonUniquePriorities, {0, 5, 1, 6, 2, 7, 3, 8, 4, 9}},
	// phase 6 - multiple threads with identical priority (alternating 2)
	{nonUniquePriorities, {5, 0, 6, 1, 7, 2, 8, 3, 9, 4}},
	// phase 7 - multiple threads with identical priority (sort-of-random)
	{nonUniquePriorities, {5, 6, 0, 7, 1, 8, 9, 2, 3, 4}},
}};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadPriorityTestCase::run_() const
{
	for (const auto& phase : testPhases)
	{
		SequenceAsserter sequenceAsserter;

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(phase.first[phase.second[0]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[1]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[2]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[3]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[4]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[5]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[6]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[7]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[8]], sequenceAsserter),
				makeTestThread(phase.first[phase.second[9]], sequenceAsserter),
		}};

		for (auto& thread : threads)
			thread.start();

		for (auto& thread : threads)
			thread.join();

		scheduler::schedulerInstance.sleepFor(20);	/// \todo temporary fix for bug in thread join/termination/removal

		if (sequenceAsserter.assertSequence(totalThreads) == false)
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
