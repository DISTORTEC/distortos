/**
 * \file
 * \brief SoftwareTimerSupervisor class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-05
 */

#include "distortos/internal/scheduler/SoftwareTimerSupervisor.hpp"

#include "distortos/internal/scheduler/SoftwareTimerControlBlock.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimerSupervisor::add(SoftwareTimerControlBlock& softwareTimerControlBlock)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	activeList_.insert(softwareTimerControlBlock);
}

void SoftwareTimerSupervisor::tickInterruptHandler(const TickClock::time_point timePoint)
{
	// execute all software timers that reached their time point
	for (auto iterator = activeList_.begin(); iterator != activeList_.end() && iterator->getTimePoint() <= timePoint;
			iterator = activeList_.begin())
	{
		iterator->run();
		SoftwareTimerList::erase(iterator);
	}
}

}	// namespace internal

}	// namespace distortos
