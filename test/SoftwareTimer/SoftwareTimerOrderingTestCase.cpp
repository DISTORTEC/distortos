/**
 * \file
 * \brief SoftwareTimerOrderingTestCase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SoftwareTimerOrderingTestCase.hpp"

#include "priorityTestPhases.hpp"
#include "SequenceAsserter.hpp"
#include "waitForNextTick.hpp"

#include "distortos/DynamicSoftwareTimer.hpp"

#include <malloc.h>

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

	const auto allocatedMemory = mallinfo().uordblks;

	for (const auto& phase : priorityTestPhases)
	{
		{
			SequenceAsserter sequenceAsserter;
			std::array<DynamicSoftwareTimer, totalSoftwareTimers> softwareTimers
			{{
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[0]].second},
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[1]].second},
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[2]].second},
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[3]].second},
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[4]].second},
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[5]].second},
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[6]].second},
					{&SequenceAsserter::sequencePoint, std::ref(sequenceAsserter), phase.first[phase.second[7]].second},
			}};

			waitForNextTick();

			for (size_t i = 0; i < softwareTimers.size(); ++i)
				softwareTimers[i].start(TickClock::duration{maxPhasePriority + 1 - phase.first[phase.second[i]].first});

			if (sequenceAsserter.assertSequence(0) == false)
				return false;

			for (const auto& softwareTimer : softwareTimers)
				while(softwareTimer.isRunning())
				{

				}

			if (sequenceAsserter.assertSequence(totalSoftwareTimers) == false)
				return false;
		}

		if (mallinfo().uordblks != allocatedMemory)	// dynamic memory must be deallocated after each test phase
			return false;
	}

	return true;
}

}	// namespace test

}	// namespace distortos
