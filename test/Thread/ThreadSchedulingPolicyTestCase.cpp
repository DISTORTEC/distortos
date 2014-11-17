/**
 * \file
 * \brief ThreadSchedulingPolicyTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-17
 */

#include "ThreadSchedulingPolicyTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "wasteTime.hpp"

#include "distortos/StaticThread.hpp"
#include "distortos/ThisThread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

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

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {192};

/// priority of test thread
constexpr uint8_t testThreadPriority {1};

/// number of test threads
constexpr size_t totalThreads {10};

/// duration of single test thread - significantly longer than single round-robin quantum
constexpr auto testThreadDuration = scheduler::RoundRobinQuantum::getInitial() * 2;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(SequenceAsserter& sequenceAsserter, SequencePoints sequencePoints);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, thread,
		std::ref(std::declval<SequenceAsserter&>()), std::declval<SequencePoints>()));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Marks the first sequence point in SequenceAsserter, wastes some time and marks the second sequence point in
 * SequenceAsserter.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 */

void thread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints)
{
	sequenceAsserter.sequencePoint(sequencePoints.first);
	wasteTime(testThreadDuration);
	sequenceAsserter.sequencePoint(sequencePoints.second);
}

/**
 * \brief Builder of TestThread objects.
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(SequenceAsserter& sequenceAsserter, const SequencePoints sequencePoints)
{
	return makeStaticThread<testThreadStackSize>(testThreadPriority, thread, std::ref(sequenceAsserter),
			static_cast<SequencePoints>(sequencePoints));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadSchedulingPolicyTestCase::run_() const
{
	SequenceAsserter sequenceAsserter;

	std::array<TestThread, totalThreads> threads
	{{
			makeTestThread(sequenceAsserter, {0, 0 + totalThreads}),
			makeTestThread(sequenceAsserter, {1, 1 + totalThreads}),
			makeTestThread(sequenceAsserter, {2, 2 + totalThreads}),
			makeTestThread(sequenceAsserter, {3, 3 + totalThreads}),
			makeTestThread(sequenceAsserter, {4, 4 + totalThreads}),
			makeTestThread(sequenceAsserter, {5, 5 + totalThreads}),
			makeTestThread(sequenceAsserter, {6, 6 + totalThreads}),
			makeTestThread(sequenceAsserter, {7, 7 + totalThreads}),
			makeTestThread(sequenceAsserter, {8, 8 + totalThreads}),
			makeTestThread(sequenceAsserter, {9, 9 + totalThreads}),
	}};

	decltype(TickClock::now()) testStart;

	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		// wait for beginning of next tick - test threads should be started in the same tick
		ThisThread::sleepFor({});

		for (auto& thread : threads)
			thread.start();

		testStart = TickClock::now();
	}

	for (auto& thread : threads)
		thread.join();

	const auto testDuration = TickClock::now() - testStart;

	if (sequenceAsserter.assertSequence(totalThreads * 2) == false)
		return false;

	// exact time cannot be calculated, because wasteTime() is not precise when context switches occur during it
	if (testDuration < testThreadDuration * totalThreads)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
