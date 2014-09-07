/**
 * \file
 * \brief SoftwareTimerControlBlockSupervisor class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-07
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKSUPERVISOR_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKSUPERVISOR_HPP_

#include "distortos/scheduler/SoftwareTimerControlBlockList.hpp"

namespace distortos
{

namespace scheduler
{

/// SoftwareTimerControlBlockSupervisor class is a supervisor of SoftwareTimerControlBlock objects
class SoftwareTimerControlBlockSupervisor
{
public:

	/**
	 * \brief SoftwareTimerControlBlock's constructor
	 */

	SoftwareTimerControlBlockSupervisor();

	/**
	 * \brief Adds SoftwareTimerControlBlock to supervisor, effectively starting the software timer.
	 *
	 * \param [in] software_timer_control_block is the SoftwareTimerControlBlock being added/started
	 *
	 * \return iterator to added SoftwareTimerControlBlock object on the list
	 */

	SoftwareTimerControlBlockListIterator add(SoftwareTimerControlBlock& software_timer_control_block);

	/**
	 * \brief Handler of "tick" interrupt.
	 *
	 * \note this must not be called by user code
	 *
	 * \param [in] time_point is the current time point
	 */

	void tickInterruptHandler(TickClock::time_point time_point);

private:

	/// pool instance used by allocator_
	SoftwareTimerControlBlockListAllocator::Pool allocatorPool_;

	/// PoolAllocator<> of SoftwareTimerControlBlockList
	SoftwareTimerControlBlockListAllocator allocator_;

	/// list of active software timers (waiting for execution)
	SoftwareTimerControlBlockList activeList_;

	/// list of dormant software timers (already executed)
	SoftwareTimerControlBlockList dormantList_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_SOFTWARETIMERCONTROLBLOCKSUPERVISOR_HPP_
