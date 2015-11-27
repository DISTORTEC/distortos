/**
 * \file
 * \brief CallOnceControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#include "distortos/synchronization/CallOnceControlBlock.hpp"

#if DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void CallOnceControlBlock::callOnceImplementation(const Functor& functor)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (done_ == true)	// function already executed?
		return;

	if (blockedList_ != nullptr)	// function is currently being executed, but not yet done?
	{
		while (done_ == false)
			scheduler::getScheduler().block(*blockedList_);
		return;
	}

	scheduler::ThreadControlBlockList blockedList {scheduler::getScheduler().getThreadControlBlockListAllocator(),
			ThreadState::BlockedOnOnceFlag};
	blockedList_ = &blockedList;

	functor();

	done_ = true;
	blockedList_ = nullptr;

	while (blockedList.empty() == false)
		scheduler::getScheduler().unblock(blockedList.begin());
}

}	// namespace synchronization

}	// namespace distortos

#endif	// DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1
