/**
 * \file
 * \brief wasteTime() header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_WASTETIME_HPP_
#define TEST_WASTETIME_HPP_

#include "distortos/TickClock.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Wastes time.
 *
 * \note Wasted duration must not be shorter, so one additional tick is always added to the duration.
 *
 * \param [in] duration is the duration of time to waste
 */

void wasteTime(TickClock::duration duration);

/**
 * \brief Wastes time.
 *
 * \note Wasted duration must not be shorter, so one additional tick is always added to the duration.
 *
 * \tparam Rep is type of tick counter
 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
 *
 * \param [in] duration is the duration of time to waste
 */

template<typename Rep, typename Period>
void wasteTime(const std::chrono::duration<Rep, Period> duration)
{
	wasteTime(std::chrono::duration_cast<TickClock::duration>(duration));
}

/**
 * \brief Wastes time.
 *
 * \param [in] timePoint is the time point at which the function will return
 */

void wasteTime(TickClock::time_point timePoint);

/**
 * \brief Wastes time.
 *
 * \tparam Duration is a std::chrono::duration type used to measure duration
 *
 * \param [in] timePoint is the time point at which the function will return
 */

template<typename Duration>
void wasteTime(const std::chrono::time_point<TickClock, Duration> timePoint)
{
	wasteTime(std::chrono::time_point_cast<TickClock::duration>(timePoint));
}

}	// namespace test

}	// namespace distortos

#endif	// TEST_WASTETIME_HPP_
