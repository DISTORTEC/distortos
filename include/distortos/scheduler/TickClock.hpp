/**
 * \file
 * \brief TickClock class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-06
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_TICKCLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_TICKCLOCK_HPP_

#include "distortos/distortosConfiguration.h"

#include <chrono>

namespace distortos
{

namespace scheduler
{

/// TickClock is a std::chrono clock, equivalent of std::chrono::steady_clock
class TickClock
{
public:

	/// type of tick counter
	using rep = uint64_t;

	/// std::ratio type representing the tick period of the clock, in seconds
	using period = std::ratio<1, CONFIG_TICK_RATE_HZ>;

	/// basic duration type of clock
	using duration = std::chrono::duration<rep, period>;

	/// basic time_point type of clock
	using time_point = std::chrono::time_point<TickClock>;

	/// \return time_point representing the current value of the clock
	static time_point now();

	/// this is a steady clock - it cannot be adjusted
	static constexpr bool is_steady = true;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_TICKCLOCK_HPP_
