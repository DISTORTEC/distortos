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
 * \date 2015-11-14
 */

#include "distortos/scheduler/SoftwareTimerControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SoftwareTimerControlBlock::SoftwareTimerControlBlock(FunctionRunner& functionRunner, SoftwareTimer& owner) :
		timePoint_{},
		functionRunner_{functionRunner},
		owner_{owner},
		list_{},
		iterator_{}
{

}

void SoftwareTimerControlBlock::start(const TickClock::duration duration)
{
	const auto now = TickClock::now();
	start(now + duration + decltype(duration){1});
}

void SoftwareTimerControlBlock::start(const TickClock::time_point timePoint)
{
	timePoint_ = timePoint;

	iterator_ = getScheduler().getSoftwareTimerSupervisor().add(*this);
}

void SoftwareTimerControlBlock::stop()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (list_ != nullptr)
	{
		list_->erase(iterator_);
		list_ = nullptr;
	}
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

SoftwareTimerControlBlock::~SoftwareTimerControlBlock()
{
	stop();
}

}	// namespace scheduler

}	// namespace distortos
