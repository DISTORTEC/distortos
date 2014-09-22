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
 * \date 2014-09-18
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
		activeList_{allocator_}
{

}

SoftwareTimerControlBlockListIterator
SoftwareTimerControlBlockSupervisor::add(SoftwareTimerControlBlock& softwareTimerControlBlock)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	allocatorPool_.feed(softwareTimerControlBlock.getLink());
	softwareTimerControlBlock.setList(&activeList_);
	return activeList_.sortedEmplace(softwareTimerControlBlock);
}

void SoftwareTimerControlBlockSupervisor::tickInterruptHandler(const TickClock::time_point timePoint)
{
	// execute all software timers that reached their time point
	for (auto iterator = activeList_.begin();
			iterator != activeList_.end() && iterator->get().getTimePoint() <= timePoint;
			iterator = activeList_.begin())
	{
		iterator->get().execute();
		iterator->get().setList(nullptr);
		activeList_.erase(iterator);
	}
}

}	// namespace scheduler

}	// namespace distortos
