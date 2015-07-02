/**
 * \file
 * \brief ThreadSleepForTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-07-02
 */

#include "ThreadSleepForTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"

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
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {384};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

void thread(uint8_t sleepFor, SequenceAsserter& sequenceAsserter, unsigned int sequencePoint,
		const TickClock::time_point& sleepStart, TickClock::duration& durationDeviation, int& sharedRet);

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of test thread
using TestThread = decltype(makeStaticThread<testThreadStackSize>({}, thread, std::declval<uint8_t>(),
		std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>(),
		std::cref(std::declval<const TickClock::time_point&>()), std::ref(std::declval<TickClock::duration&>()),
		std::ref(std::declval<int&>())));

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
 * \brief Builder of TestThread objects.
 *
 * This test uses "priority" field of test parameters as a "reversed" amount of time to sleep.
 *
 * \param [in] threadParameters is a reference to ThreadParameters object
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] sleepStart is a reference to variable with time point of sleep's start - it is used for deviation of
 * sleep duration calculation
 * \param [out] durationDeviation is a reference to variable for storing deviation of sleep duration
 * \param [out] sharedRet is a reference to variable for storing return value of ThisThread::sleepFor()
 *
 * \return constructed TestThread object
 */

TestThread makeTestThread(const ThreadParameters& threadParameters, SequenceAsserter& sequenceAsserter,
		const TickClock::time_point& sleepStart, TickClock::duration& durationDeviation, int& sharedRet)
{
	return makeStaticThread<testThreadStackSize>(1, thread, static_cast<uint8_t>(UINT8_MAX - threadParameters.first),
			std::ref(sequenceAsserter), static_cast<unsigned int>(threadParameters.second), std::cref(sleepStart),
			std::ref(durationDeviation), std::ref(sharedRet));
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
		std::array<TickClock::duration, totalThreads> durationDeviations {{}};
		std::array<int, totalThreads> sharedRets {{}};
		TickClock::time_point sleepStart;

		std::array<TestThread, totalThreads> threads
		{{
				makeTestThread(phase.first[phase.second[0]], sequenceAsserter, sleepStart, durationDeviations[0],
						sharedRets[0]),
				makeTestThread(phase.first[phase.second[1]], sequenceAsserter, sleepStart, durationDeviations[1],
						sharedRets[1]),
				makeTestThread(phase.first[phase.second[2]], sequenceAsserter, sleepStart, durationDeviations[2],
						sharedRets[2]),
				makeTestThread(phase.first[phase.second[3]], sequenceAsserter, sleepStart, durationDeviations[3],
						sharedRets[3]),
				makeTestThread(phase.first[phase.second[4]], sequenceAsserter, sleepStart, durationDeviations[4],
						sharedRets[4]),
				makeTestThread(phase.first[phase.second[5]], sequenceAsserter, sleepStart, durationDeviations[5],
						sharedRets[5]),
				makeTestThread(phase.first[phase.second[6]], sequenceAsserter, sleepStart, durationDeviations[6],
						sharedRets[6]),
				makeTestThread(phase.first[phase.second[7]], sequenceAsserter, sleepStart, durationDeviations[7],
						sharedRets[7]),
				makeTestThread(phase.first[phase.second[8]], sequenceAsserter, sleepStart, durationDeviations[8],
						sharedRets[8]),
				makeTestThread(phase.first[phase.second[9]], sequenceAsserter, sleepStart, durationDeviations[9],
						sharedRets[9]),
		}};

		{
			architecture::InterruptMaskingLock interruptMaskingLock;

			// wait for beginning of next tick - test threads should be started in the same tick
			ThisThread::sleepFor({});
			sleepStart = TickClock::now();

			for (auto& thread : threads)
				thread.start();
		}

		for (auto& thread : threads)
			thread.join();

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

	return true;
}

}	// namespace test

}	// namespace distortos
