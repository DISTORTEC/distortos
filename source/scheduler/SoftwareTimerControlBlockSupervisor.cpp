/**
 * \file
 * \brief SoftwareTimerControlBlockSupervisor class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-07
 */

#include "distortos/scheduler/SoftwareTimerControlBlockSupervisor.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SoftwareTimerControlBlockSupervisor::SoftwareTimerControlBlockSupervisor() :
		allocatorPool_{},
		allocator_{allocatorPool_},
		activeList_{allocator_},
		dormantList_{allocator_}
{

}

SoftwareTimerControlBlockListIterator
SoftwareTimerControlBlockSupervisor::add(SoftwareTimerControlBlock& software_timer_control_block)
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	allocatorPool_.feed(software_timer_control_block.getLink());
	software_timer_control_block.setList(&activeList_);
	return activeList_.sortedEmplace(software_timer_control_block);
}

void SoftwareTimerControlBlockSupervisor::tickInterruptHandler(const TickClock::time_point time_point)
{
	// execute all software timers that reached their time point
	for (auto iterator = activeList_.begin();
			iterator != activeList_.end() && iterator->get().getTimePoint() <= time_point;
			iterator = activeList_.begin())
	{
		iterator->get().execute();
		iterator->get().setList(&dormantList_);
		dormantList_.sortedSplice(activeList_, iterator);
	}
}

}	// namespace scheduler

}	// namespace distortos
