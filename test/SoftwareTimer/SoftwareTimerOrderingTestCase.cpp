/**
 * \file
 * \brief SoftwareTimerOrderingTestCase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-20
 */

#include "SoftwareTimerOrderingTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"
#include "waitForNextTick.hpp"

#include "distortos/SoftwareTimer.hpp"

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

		using TestSoftwareTimer = decltype(makeSoftwareTimer(&SequenceAsserter::sequencePoint,
				std::ref(std::declval<SequenceAsserter&>()), std::declval<unsigned int>()));
		std::array<TestSoftwareTimer, totalSoftwareTimers> softwareTimers
		{{
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[0]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[1]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[2]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[3]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[4]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[5]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[6]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[7]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
						static_cast<unsigned int>(phase.first[phase.second[8]].second)),
				makeSoftwareTimer(&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter),
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
