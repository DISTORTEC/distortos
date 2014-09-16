/**
 * \file
 * \brief wasteTime() header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-16
 */

#ifndef TEST_WASTETIME_HPP_
#define TEST_WASTETIME_HPP_

#include "distortos/scheduler/TickClock.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Wastes time.
 *
 * \param [in] timePoint is the time point at which the function will return
 */

void wasteTime(scheduler::TickClock::time_point timePoint);

}	// namespace test

}	// namespace distortos

#endif	// TEST_WASTETIME_HPP_
