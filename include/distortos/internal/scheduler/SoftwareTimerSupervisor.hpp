/**
 * \file
 * \brief SoftwareTimerSupervisor class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERSUPERVISOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERSUPERVISOR_HPP_

#include "distortos/internal/scheduler/SoftwareTimerList.hpp"

namespace distortos
{

namespace internal
{

/// SoftwareTimerSupervisor class is a supervisor of software timers
class SoftwareTimerSupervisor
{
public:

	/**
	 * \brief SoftwareTimerControlBlock's constructor
	 */

	constexpr SoftwareTimerSupervisor() :
			activeList_{}
	{

	}

	/**
	 * \brief Adds SoftwareTimerControlBlock to supervisor, effectively starting the software timer.
	 *
	 * \param [in] softwareTimerControlBlock is the SoftwareTimerControlBlock being added/started
	 */

	void add(SoftwareTimerControlBlock& softwareTimerControlBlock);

	/**
	 * \brief Handler of "tick" interrupt.
	 *
	 * \note this must not be called by user code
	 *
	 * \param [in] timePoint is the current time point
	 */

	void tickInterruptHandler(TickClock::time_point timePoint);

private:

	/// list of active software timers (waiting for execution)
	SoftwareTimerList activeList_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_SOFTWARETIMERSUPERVISOR_HPP_
