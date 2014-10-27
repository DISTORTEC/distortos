/**
 * \file
 * \brief ConditionVariable class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-27
 */

#include "distortos/ConditionVariable.hpp"

#include "distortos/Mutex.hpp"
#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ConditionVariable::ConditionVariable() :
		blockedList_{scheduler::schedulerInstance.getThreadControlBlockListAllocator(),
				scheduler::ThreadControlBlock::State::BlockedOnConditionVariable}
{

}

void ConditionVariable::notifyAll()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	while (blockedList_.empty() == false)
		scheduler::schedulerInstance.unblock(blockedList_.begin());
}

void ConditionVariable::notifyOne()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (blockedList_.empty() == false)
		scheduler::schedulerInstance.unblock(blockedList_.begin());
}

void ConditionVariable::wait(Mutex& mutex)
{
	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		mutex.unlock();
		scheduler::schedulerInstance.block(blockedList_);
	}

	// --- blocked ---

	mutex.lock();
}

}	// namespace distortos
