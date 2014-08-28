/**
 * \file
 * \brief ThisThread namespace implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-28
 */

#include "distortos/scheduler/ThisThread.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

namespace distortos
{

namespace scheduler
{

namespace ThisThread
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void sleepFor(const TickClock::duration duration)
{
	sleepUntil(TickClock::now() + duration + TickClock::duration{1});
}

void sleepUntil(const TickClock::time_point time_point)
{
	schedulerInstance.sleepUntil(time_point);
}

void yield()
{
	schedulerInstance.yield();
}

}	// namespace ThisThread

}	// namespace scheduler

}	// namespace distortos
