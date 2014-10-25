/**
 * \file
 * \brief ThreadSleepUntilTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-25
 */

#include "ThreadSleepUntilTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

#include "distortos/StaticThread.hpp"
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

/// offset added to sleepUntil time points - there is some time between configuring thread and actually starting it...
constexpr uint8_t timePointOffset {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(scheduler::TickClock::time_point sleepUntil, SequenceAsserter& sequenceAsserter,
		unsigned int sequencePoint, scheduler::TickClock::duration& timePointDeviation);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(scheduler::makeStaticThread<testThreadStackSize>({}, thread,
		std::declval<scheduler::TickClock::time_point>(), std::ref(std::declval<SequenceAsserter&>()),
		std::declval<unsigned int>(), std::ref(std::declval<scheduler::TickClock::duration&>())));

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread.
 *
 * Sleeps until requested time point and marks the sequence point in SequenceAsserter, also storing the deviation of
 * time point of waking up.
 *
 * \param [in] sleepUntil is the time point at which the thread will wake up to mark sequence point
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sequencePoint is the sequence point of this instance
 * \param [out] timePointDeviation is a reference to variable for storing deviation of time point of waking up
 */

void thread(const scheduler::TickClock::time_point sleepUntil, SequenceAsserter& sequenceAsserter,
		const unsigned int sequencePoint, scheduler::TickClock::duration& timePointDeviation)
{
	scheduler::ThisThread::sleepUntil(sleepUntil);

	timePointDeviation = scheduler::TickClock::now() - sleepUntil;
	sequenceAsserter.sequencePoint(sequencePoint);
}

/**
 * \brief Builder of TestThread objects.
 *
 * This test uses "priority" field of test parameters as a "reversed" amount of time to sleep.
 *
 * \param [in] now is the current time point
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [out] timePointDeviation is a reference to variable for storing deviation of time point of waking up
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const scheduler::TickClock::time_point now, const ThreadParameters& threadParameters,
		SequenceAsserter& sequenceAsserter, scheduler::TickClock::duration& timePointDeviation)
{
	return scheduler::makeStaticThread<testThreadStackSize>(1, thread,
			now + scheduler::TickClock::duration{UINT8_MAX - threadParameters.first + timePointOffset},
			std::ref(sequenceAsserter), static_cast<unsigned int>(threadParameters.second),
			std::ref(timePointDeviation));
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool ThreadSleepUntilTestCase::run_() const
{
	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;
		std::array<scheduler::TickClock::duration, totalThreads> timePointDeviations {{}};
		const auto now = scheduler::TickClock::now();

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(now, phase.first[phase.second[0]], sequenceAsserter, timePointDeviations[0]),
				makeTestThread(now, phase.first[phase.second[1]], sequenceAsserter, timePointDeviations[1]),
				makeTestThread(now, phase.first[phase.second[2]], sequenceAsserter, timePointDeviations[2]),
				makeTestThread(now, phase.first[phase.second[3]], sequenceAsserter, timePointDeviations[3]),
				makeTestThread(now, phase.first[phase.second[4]], sequenceAsserter, timePointDeviations[4]),
				makeTestThread(now, phase.first[phase.second[5]], sequenceAsserter, timePointDeviations[5]),
				makeTestThread(now, phase.first[phase.second[6]], sequenceAsserter, timePointDeviations[6]),
				makeTestThread(now, phase.first[phase.second[7]], sequenceAsserter, timePointDeviations[7]),
				makeTestThread(now, phase.first[phase.second[8]], sequenceAsserter, timePointDeviations[8]),
				makeTestThread(now, phase.first[phase.second[9]], sequenceAsserter, timePointDeviations[9]),
		}};

		{
			architecture::InterruptMaskingLock interruptMaskingLock;

			for (auto& thread : threads)
				thread.start();
		}

		for (auto& thread : threads)
			thread.join();

		if (sequenceAsserter.assertSequence(totalThreads) == false)
			return false;

		for (const auto& timePointDeviation : timePointDeviations)
			if (timePointDeviation != scheduler::TickClock::duration{})
				return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
