/**
 * \file
 * \brief TickClock class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-27
 */

#include "distortos/TickClock.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

namespace distortos
{

TickClock::time_point TickClock::now()
{
	const auto tickCount = scheduler::schedulerInstance.getTickCount();
	return time_point{duration{tickCount}};
}

}	// namespace distortos
