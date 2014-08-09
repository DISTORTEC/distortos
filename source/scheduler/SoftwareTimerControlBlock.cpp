/**
 * \file
 * \brief SoftwareTimerControlBlock class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-09
 */

#include "distortos/scheduler/SoftwareTimerControlBlock.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SoftwareTimerControlBlock::SoftwareTimerControlBlock() :
		timePoint_{},
		list_{},
		iterator_{}
{

}

SoftwareTimerControlBlock::~SoftwareTimerControlBlock()
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	if (list_ != nullptr)
		list_->erase(iterator_);
}

void SoftwareTimerControlBlock::start(const TickClock::duration duration)
{
	const auto now = TickClock::now();
	start(now + duration + decltype(duration){1});
}

void SoftwareTimerControlBlock::start(const TickClock::time_point time_point)
{
	timePoint_ = time_point;

	iterator_ = schedulerInstance.getSoftwareTimerSupervisor().add(*this);
}

}	// namespace scheduler

}	// namespace distortos
