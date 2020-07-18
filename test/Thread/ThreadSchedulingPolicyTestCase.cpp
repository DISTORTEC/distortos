/**
 * \file
 * \brief ThreadSchedulingPolicyTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreadSchedulingPolicyTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "wasteTime.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/InterruptMaskingLock.hpp"
#include "distortos/ThisThread.hpp"

#include <malloc.h>

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
constexpr size_t testThreadStackSize {512};

/// priority of test thread
constexpr uint8_t testThreadPriority {1};

/// number of test threads
constexpr size_t totalThreads {8};

/// duration of single test thread - significantly longer than single round-robin quantum
constexpr auto testThreadDuration = internal::RoundRobinQuantum::getInitial() * 2;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread
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
 * \brief Builder of test threads
 *
 * \param [in] schedulingPolicy is the scheduling policy of the test thread
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoints is a pair of sequence points for this instance
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const SchedulingPolicy schedulingPolicy, SequenceAsserter& sequenceAsserter,
		const SequencePoints sequencePoints)
{
	return makeDynamicThread({testThreadStackSize, testThreadPriority, schedulingPolicy}, thread,
			std::ref(sequenceAsserter), sequencePoints);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadSchedulingPolicyTestCase::run_() const
{
	const auto allocatedMemory = mallinfo().uordblks;

	// scheduling policy, sequence point multiplier, sequence point step
	using Parameters = std::tuple<SchedulingPolicy, unsigned int, unsigned int>;
	static const Parameters parametersArray[]
	{
			Parameters{SchedulingPolicy::fifo, 2, 1},
			Parameters{SchedulingPolicy::roundRobin, 1, totalThreads},
	};

	for (const auto& parameters : parametersArray)
	{
		{
			const auto schedulingPolicy = std::get<0>(parameters);
			const auto multiplier = std::get<1>(parameters);
			const auto step = std::get<2>(parameters);

			SequenceAsserter sequenceAsserter;

			std::array<DynamicThread, totalThreads> threads
			{{
					makeTestThread(schedulingPolicy, sequenceAsserter, {0 * multiplier, 0 * multiplier + step}),
					makeTestThread(schedulingPolicy, sequenceAsserter, {1 * multiplier, 1 * multiplier + step}),
					makeTestThread(schedulingPolicy, sequenceAsserter, {2 * multiplier, 2 * multiplier + step}),
					makeTestThread(schedulingPolicy, sequenceAsserter, {3 * multiplier, 3 * multiplier + step}),
					makeTestThread(schedulingPolicy, sequenceAsserter, {4 * multiplier, 4 * multiplier + step}),
					makeTestThread(schedulingPolicy, sequenceAsserter, {5 * multiplier, 5 * multiplier + step}),
					makeTestThread(schedulingPolicy, sequenceAsserter, {6 * multiplier, 6 * multiplier + step}),
					makeTestThread(schedulingPolicy, sequenceAsserter, {7 * multiplier, 7 * multiplier + step}),
			}};

			decltype(TickClock::now()) testStart;

			{
				const InterruptMaskingLock interruptMaskingLock;

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
		}

		if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
