/**
 * \file
 * \brief SoftwareTimerPeriodicTestCase class implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SoftwareTimerPeriodicTestCase.hpp"

#include "SequenceAsserter.hpp"
#include "waitForNextTick.hpp"

#include "distortos/StaticSoftwareTimer.hpp"
#include "distortos/ThisThread.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// duration of whole test case
constexpr TickClock::duration testCaseDuration {100};

/// total number of software timers used in test case
constexpr size_t totalSoftwareTimers {10};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Function executed by software timers during the test case.
 *
 * Calculates next sequence point using its own period and cycle counter. The calculation of sequence point assumes that
 * timers with longer period have priority over timers with shorter period (they were queued earlier). Own period and
 * own counter are used to calculate global iteration count. This value can be used to calculate how many times this
 * function was already executed:
 * - for timers with longer period - global iteration count divided by period of analyzed timer
 * - for timers with shorter  or equal period - global iteration count minus one, divided by period of analyzed timer
 *
 * Next sequence point is the sum of times this function was executed for all analyzed timers.
 *
 * Expected sequence looks like this:
 * [global iteration]: [periods of timers that will be executed]
 * 1: 1
 * 2: 2, 1
 * 3: 3, 1
 * 4: 4, 2, 1
 * 5: 5, 1
 * 6: 6, 3, 2, 1
 * 7: 7, 1
 * 8: 8, 4, 2, 1
 * ...
 *
 * \param [in] sequenceAsserter is a reference to SequenceAsserter shared object
 * \param [in] counter is a reference to counter for this instance
 * \param [in] period is the period of this instance, system ticks
 */

void softwareTimerFunction(SequenceAsserter& sequenceAsserter, uint32_t& counter, const uint8_t period)
{
	++counter;
	const uint32_t iteration {counter * period};
	unsigned int sequencePoint {};
	for (size_t i {}; i < totalSoftwareTimers; ++i)
		sequencePoint += (period <= i ? iteration : iteration - 1) / (i + 1);

	sequenceAsserter.sequencePoint(sequencePoint);
}

}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SoftwareTimerPeriodicTestCase::run_() const
{
	SequenceAsserter sequenceAsserter;
	uint32_t counters[totalSoftwareTimers] {};
	using TestSoftwareTimer = decltype(makeStaticSoftwareTimer(softwareTimerFunction,
			std::ref(std::declval<SequenceAsserter&>()), std::ref(std::declval<uint32_t&>()), std::declval<uint8_t>()));
	std::array<TestSoftwareTimer, totalSoftwareTimers> softwareTimers
	{{
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[0]),
					uint8_t{1}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[1]),
					uint8_t{2}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[2]),
					uint8_t{3}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[3]),
					uint8_t{4}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[4]),
					uint8_t{5}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[5]),
					uint8_t{6}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[6]),
					uint8_t{7}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[7]),
					uint8_t{8}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[8]),
					uint8_t{9}),
			makeStaticSoftwareTimer(softwareTimerFunction, std::ref(sequenceAsserter), std::ref(counters[9]),
					uint8_t{10}),
	}};

	waitForNextTick();

	TickClock::duration period {1};
	unsigned int expectedSequencePoint {};
	for (auto& softwareTimer : softwareTimers)
	{
		softwareTimer.start(period, period);
		expectedSequencePoint += testCaseDuration / period;
		++period;
	}

	ThisThread::sleepFor(testCaseDuration);

	for (auto& softwareTimer : softwareTimers)
		softwareTimer.stop();

	return sequenceAsserter.assertSequence(expectedSequencePoint);
}

}	// namespace test

}	// namespace distortos
