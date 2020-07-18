/**
 * \file
 * \brief getScheduler() definition
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_

namespace distortos
{

namespace internal
{

class Scheduler;

/**
 * \return reference to main instance of system's Scheduler
 */

constexpr Scheduler& getScheduler()
{
	extern Scheduler schedulerInstance;
	return schedulerInstance;
}

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_GETSCHEDULER_HPP_
