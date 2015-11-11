/**
 * \file
 * \brief SoftwareTimerOrderingTestCase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-11
 */

#include "SoftwareTimerOrderingTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"
#include "waitForNextTick.hpp"

#include "distortos/StaticSoftwareTimer.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SoftwareTimerOrderingTestCase::run_() const
{
	constexpr auto totalSoftwareTimers = totalThreads;

	for (const auto& phase : priorityTestPhases)
	{
		SequenceAsserter sequenceAsserter;

		using TestSoftwareTimer = decltype(makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint,
				std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>()));
		std::array<TestSoftwareTimer, totalSoftwareTimers> softwareTimers
		{{
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[0]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[1]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[2]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[3]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[4]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[5]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[6]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[7]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[8]].second)),
				makeStaticSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[9]].second)),
		}};

		waitForNextTick();

		for (size_t i = 0; i < softwareTimers.size(); ++i)
		{
			constexpr auto inverter = std::numeric_limits<decltype(phase.first[phase.second[i]].first)>::max() + 1;
			softwareTimers[i].start(TickClock::duration{inverter - phase.first[phase.second[i]].first});
		}

		if (sequenceAsserter.assertSequence(0) == false)
			return false;

		for (const auto& softwareTimer : softwareTimers)
			while(softwareTimer.isRunning())
			{

			}

		if (sequenceAsserter.assertSequence(totalSoftwareTimers) == false)
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
