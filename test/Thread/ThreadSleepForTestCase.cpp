/**
 * \file
 * \brief ThreadSleepForTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreadSleepForTestCase.hpp"

#include "priorityTestPhases.hpp"
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
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread
 *
 * Sleeps for requested amount of time and marks the sequence point in SequenceAsserter, also storing the deviation of
 * duration actually slept.
 *
 * \param [in] sleepFor is the number of ticks the thread will sleep before marking sequence point
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 * \param [in] sleepStart is a reference to variable with time point of sleep's start - it is used for deviation of
 * sleep duration calculation
 * \param [out] durationDeviation is a reference to variable for storing deviation of sleep duration
 * \param [out] sharedRet is a reference to variable for storing return value of ThisThread::sleepFor()
 */

void thread(const uint8_t sleepFor, SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint,
		const TickClock::time_point& sleepStart, TickClock::duration& durationDeviation, int& sharedRet)
{
	const auto sleepForDuration = TickClock::duration{sleepFor};

	sharedRet = ThisThread::sleepFor(sleepForDuration);

	const auto durationSlept = TickClock::now() - sleepStart;
	sequenceAsserter.sequencePoint(sequencePoint);
	durationDeviation = durationSlept - sleepForDuration;
}

/**
 * \brief Builder of test threads
 *
 * This test uses "priority" field of test parameters as a "reversed" amount of time to sleep.
 *
 * \param [in] priority is the thread's priority
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sleepStart is a reference to variable with time point of sleep's start - it is used for deviation of
 * sleep duration calculation
 * \param [out] durationDeviation is a reference to variable for storing deviation of sleep duration
 * \param [out] sharedRet is a reference to variable for storing return value of ThisThread::sleepFor()
 *
 * \return constructed DynamicThread object
 */

DynamicThread makeTestThread(const uint8_t priority, const ThreadParameters& threadParameters,
		SequenceAsserter& sequenceAsserter, const TickClock::time_point& sleepStart,
		TickClock::duration& durationDeviation, int& sharedRet)
{
	return makeDynamicThread({testThreadStackSize, priority}, thread, maxPhasePriority - threadParameters.first,
			std::ref(sequenceAsserter), threadParameters.second, std::cref(sleepStart), std::ref(durationDeviation),
			std::ref(sharedRet));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadSleepForTestCase::run_() const
{
	const auto allocatedMemory = mallinfo().uordblks;

	for (const auto& phase : priorityTestPhases)
	{
		{
			SequenceAsserter sequenceAsserter;
			std::array<TickClock::duration, totalThreads> durationDeviations {{}};
			std::array<int, totalThreads> sharedRets {{}};
			TickClock::time_point sleepStart;

			std::array<DynamicThread, totalThreads> threads
			{{
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[0]], sequenceAsserter, sleepStart,
							durationDeviations[0], sharedRets[0]),
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[1]], sequenceAsserter, sleepStart,
							durationDeviations[1], sharedRets[1]),
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[2]], sequenceAsserter, sleepStart,
							durationDeviations[2], sharedRets[2]),
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[3]], sequenceAsserter, sleepStart,
							durationDeviations[3], sharedRets[3]),
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[4]], sequenceAsserter, sleepStart,
							durationDeviations[4], sharedRets[4]),
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[5]], sequenceAsserter, sleepStart,
							durationDeviations[5], sharedRets[5]),
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[6]], sequenceAsserter, sleepStart,
							durationDeviations[6], sharedRets[6]),
					makeTestThread(testCasePriority_ - 1, phase.first[phase.second[7]], sequenceAsserter, sleepStart,
							durationDeviations[7], sharedRets[7]),
			}};

			ThisThread::setPriority(testCasePriority_);

			for (auto& thread : threads)
				thread.start();

			// wait for beginning of next tick - test threads should be started in the same tick
			wasteTime(TickClock::duration{});
			sleepStart = TickClock::now();

			ThisThread::setPriority(testCasePriority_ - 2);

			bool invalidState {};
			for (const auto& thread : threads)
				if (thread.getState() != ThreadState::sleeping)
					invalidState = true;

			for (auto& thread : threads)
				thread.join();

			if (invalidState != false)
				return false;

			if (sequenceAsserter.assertSequence(totalThreads) == false)
				return false;

			// sleepFor() always sleeps one tick longer
			for (const auto& durationDeviation : durationDeviations)
				if (durationDeviation != TickClock::duration{1})
					return false;

			for (const auto& sharedRet : sharedRets)
				if (sharedRet != 0)
					return false;
		}

		if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
