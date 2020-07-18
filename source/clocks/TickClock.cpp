/**
 * \file
 * \brief TickClock class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/TickClock.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

namespace distortos
{

TickClock::time_point TickClock::now()
{
	const auto tickCount = internal::getScheduler().getTickCount();
	return time_point{duration{tickCount}};
}

}	// namespace distortos
