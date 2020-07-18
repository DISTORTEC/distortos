/**
 * \file
 * \brief SoftwareTimerCommon class implementation
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/SoftwareTimerCommon.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SoftwareTimerCommon::~SoftwareTimerCommon()
{

}

bool SoftwareTimerCommon::isRunning() const
{
	return softwareTimerControlBlock_.isRunning();
}

int SoftwareTimerCommon::start(const TickClock::time_point timePoint, const TickClock::duration period)
{
	softwareTimerControlBlock_.start(internal::getScheduler().getSoftwareTimerSupervisor(), timePoint, period);
	return 0;
}

int SoftwareTimerCommon::stop()
{
	softwareTimerControlBlock_.stop();
	return 0;
}

}	// namespace distortos
