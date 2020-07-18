/**
 * \file
 * \brief SoftwareTimerSupervisor class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/SoftwareTimerSupervisor.hpp"

#include "distortos/internal/scheduler/SoftwareTimerControlBlock.hpp"

#include "distortos/InterruptMaskingLock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimerSupervisor::add(SoftwareTimerControlBlock& softwareTimerControlBlock)
{
	activeList_.insert(softwareTimerControlBlock);
}

void SoftwareTimerSupervisor::tickInterruptHandler(const TickClock::time_point timePoint)
{
	// execute all software timers that reached their time point
	decltype(activeList_.begin()) iterator;
	while (iterator = activeList_.begin(), iterator != activeList_.end() && iterator->getTimePoint() <= timePoint)
	{
		auto& softwareTimer = *iterator;
		SoftwareTimerList::erase(iterator);
		softwareTimer.run(*this);
	}
}

}	// namespace internal

}	// namespace distortos
