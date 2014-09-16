/**
 * \file
 * \brief RoundRobinQuantum class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-16
 */

#include "distortos/scheduler/RoundRobinQuantum.hpp"

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace scheduler
{

namespace
{

static_assert(CONFIG_TICK_RATE_HZ > 0, "CONFIG_TICK_RATE_HZ must be positive and non-zero!");
static_assert(CONFIG_ROUND_ROBIN_RATE_HZ > 0, "CONFIG_ROUND_ROBIN_RATE_HZ must be positive and non-zero!");

/// initializer value for round-robin quantum, calculated with rounding to nearest
constexpr auto quantumInitializer_ = (CONFIG_TICK_RATE_HZ + CONFIG_ROUND_ROBIN_RATE_HZ / 2) /
		CONFIG_ROUND_ROBIN_RATE_HZ;

static_assert(quantumInitializer_ > 0,
		"CONFIG_TICK_RATE_HZ and CONFIG_ROUND_ROBIN_RATE_HZ values result in negative or zero round-robin quantum!");
static_assert(quantumInitializer_ <= UINT8_MAX,
		"CONFIG_TICK_RATE_HZ and CONFIG_ROUND_ROBIN_RATE_HZ values result in too large round-robin quantum!");

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void RoundRobinQuantum::decrement()
{
	if (isZero() == false)
		--quantum_;
}

void RoundRobinQuantum::reset()
{
	quantum_ = quantumInitializer_;
}

}	// namespace scheduler

}	// namespace distortos
