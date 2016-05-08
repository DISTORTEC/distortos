/**
 * \file
 * \brief SoftwareTimerControlBlock class implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/SoftwareTimerControlBlock.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimerControlBlock::run() const
{
	functionRunner_(owner_);
}

void SoftwareTimerControlBlock::start(SoftwareTimerSupervisor& supervisor, const TickClock::time_point timePoint)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	stopInternal();
	setTimePoint(timePoint);
	supervisor.add(*this);
}

void SoftwareTimerControlBlock::stop()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	stopInternal();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimerControlBlock::stopInternal()
{
	if (isRunning() == false)	// timer is already stopped?
		return;

	node.unlink();
	period_ = {};
}

}	// namespace internal

}	// namespace distortos
