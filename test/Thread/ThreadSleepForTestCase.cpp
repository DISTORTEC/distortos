/**
 * \file
 * \brief ThreadSleepForTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-01
 */

#include "ThreadSleepForTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/scheduler/StaticThread.hpp"
#include "distortos/scheduler/ThisThread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

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
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread_(uint8_t sleepFor, SequenceAsserter& sequenceAsserter, unsigned int sequencePoint,
		scheduler::TickClock::duration& durationDeviation);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(scheduler::makeStaticThread<testThreadStackSize>({}, thread_, std::declval<uint8_t>(),
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>(),
		std::ref(std::declval<scheduler::TickClock::duration&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Sleeps for requested amount of time and marks the sequence point in SequenceAsserter, also storing the deviation of
 * duration actually slept.
 *
 * \param [in] sleepFor is the number of ticks the thread will sleep before marking sequence point
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 * \param [out] durationDeviation is a reference to variable for storing deviation of sleep duration
 */

void thread_(const uint8_t sleepFor, SequenceAsserter& sequenceAsserter, const unsigned int sequencePoint,
		scheduler::TickClock::duration& durationDeviation)
{
	const auto sleepForDuration = scheduler::TickClock::duration{sleepFor};
	const auto sleepStart = scheduler::TickClock::now();

	scheduler::ThisThread::sleepFor(sleepForDuration);

	const auto durationSlept = scheduler::TickClock::now() - sleepStart;
	sequenceAsserter.sequencePoint(sequencePoint);
	durationDeviation = durationSlept - sleepForDuration;
}

/**
 * \brief Builder of TestThread objects.
 *
 * This test uses "priority" field of test parameters as a "reversed" amount of time to sleep.
 *
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [out] durationDeviation is a reference to variable for storing deviation of sleep duration
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const ThreadParameters &threadParameters, SequenceAsserter& sequenceAsserter,
		scheduler::TickClock::duration& durationDeviation)
{
	return scheduler::makeStaticThread<testThreadStackSize>(1, thread_,
			static_cast<uint8_t>(UINT8_MAX - threadParameters.first), std::ref(sequenceAsserter),
			static_cast<unsigned int>(threadParameters.second), std::ref(durationDeviation));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadSleepForTestCase::run_() const
{
	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;
		std::array<scheduler::TickClock::duration, totalThreads> durationDeviations {{}};

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(phase.first[phase.second[0]], sequenceAsserter, durationDeviations[0]),
				makeTestThread(phase.first[phase.second[1]], sequenceAsserter, durationDeviations[1]),
				makeTestThread(phase.first[phase.second[2]], sequenceAsserter, durationDeviations[2]),
				makeTestThread(phase.first[phase.second[3]], sequenceAsserter, durationDeviations[3]),
				makeTestThread(phase.first[phase.second[4]], sequenceAsserter, durationDeviations[4]),
				makeTestThread(phase.first[phase.second[5]], sequenceAsserter, durationDeviations[5]),
				makeTestThread(phase.first[phase.second[6]], sequenceAsserter, durationDeviations[6]),
				makeTestThread(phase.first[phase.second[7]], sequenceAsserter, durationDeviations[7]),
				makeTestThread(phase.first[phase.second[8]], sequenceAsserter, durationDeviations[8]),
				makeTestThread(phase.first[phase.second[9]], sequenceAsserter, durationDeviations[9]),
		}};

		{
			architecture::InterruptMaskingLock interruptMaskingLock;

			// wait for beginning of next tick - test threads should be started in the same tick
			scheduler::ThisThread::sleepFor({});

			for (auto& thread : threads)
				thread.start();
		}

		for (auto& thread : threads)
			thread.join();

		if (sequenceAsserter.assertSequence(totalThreads) == false)
			return false;

		// sleepFor() always sleeps one tick longer
		for (const auto& durationDeviation : durationDeviations)
			if (durationDeviation != scheduler::TickClock::duration{1})
				return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
