/**
 * \file
 * \brief SoftwareTimerControlBlock class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/SoftwareTimerControlBlock.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/InterruptMaskingLock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimerControlBlock::run(SoftwareTimerSupervisor& supervisor)
{
	functionRunner_(owner_);

	// was timer restarted in timer's function or is this a one-shot timer?
	if (node.isLinked() == true || period_ == decltype(period_){})
		return;

	startInternal(supervisor, getTimePoint() + period_);	// this is a periodic timer, so restart it
}

void SoftwareTimerControlBlock::start(SoftwareTimerSupervisor& supervisor, const TickClock::time_point timePoint,
		const TickClock::duration period)
{
	const InterruptMaskingLock interruptMaskingLock;

	stopInternal();
	period_ = period;
	startInternal(supervisor, timePoint);
}

void SoftwareTimerControlBlock::stop()
{
	const InterruptMaskingLock interruptMaskingLock;

	stopInternal();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SoftwareTimerControlBlock::startInternal(SoftwareTimerSupervisor& supervisor,
		const TickClock::time_point timePoint)
{
	setTimePoint(timePoint);
	supervisor.add(*this);
}

void SoftwareTimerControlBlock::stopInternal()
{
	if (isRunning() == false)	// timer is already stopped?
		return;

	node.unlink();
	period_ = {};
}

}	// namespace internal

}	// namespace distortos
