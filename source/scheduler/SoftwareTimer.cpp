/**
 * \file
 * \brief SoftwareTimer class implementation
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/SoftwareTimer.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SoftwareTimer::start(const TickClock::duration duration, const TickClock::duration period)
{
	const auto now = TickClock::now();
	return start(now + duration + decltype(duration){1}, period);
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected static functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimer::softwareTimerRunner(SoftwareTimer& softwareTimer)
{
	softwareTimer.run();
}

}	// namespace distortos
