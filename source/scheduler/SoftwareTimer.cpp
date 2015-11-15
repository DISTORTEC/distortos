/**
 * \file
 * \brief SoftwareTimer class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-15
 */

#include "distortos/SoftwareTimer.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SoftwareTimer::isRunning() const
{
	return softwareTimerControlBlock_.isRunning();
}

void SoftwareTimer::start(const TickClock::duration duration)
{
	softwareTimerControlBlock_.start(duration);
}

void SoftwareTimer::start(const TickClock::time_point timePoint)
{
	softwareTimerControlBlock_.start(timePoint);
}

void SoftwareTimer::stop()
{
	softwareTimerControlBlock_.stop();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimer::softwareTimerRunner(SoftwareTimer& softwareTimer)
{
	softwareTimer.run();
}

}	// namespace distortos
