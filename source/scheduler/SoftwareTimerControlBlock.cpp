/**
 * \file
 * \brief SoftwareTimerControlBlock class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-05
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

SoftwareTimerControlBlock::SoftwareTimerControlBlock(FunctionRunner& functionRunner, SoftwareTimer& owner) :
		SoftwareTimerListNode{},
		functionRunner_{functionRunner},
		owner_{owner}
{

}

SoftwareTimerControlBlock::~SoftwareTimerControlBlock()
{
	stop();
}

void SoftwareTimerControlBlock::run() const
{
	functionRunner_(owner_);
}

void SoftwareTimerControlBlock::start(const TickClock::time_point timePoint)
{
	setTimePoint(timePoint);

	getScheduler().getSoftwareTimerSupervisor().add(*this);
}

void SoftwareTimerControlBlock::stop()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (isRunning() == true)
		node.unlink();
}

}	// namespace internal

}	// namespace distortos
