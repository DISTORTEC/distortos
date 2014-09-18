/**
 * \file
 * \brief ThreadControlBlock class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-18
 */

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/threadReturnTrap.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(void* const buffer, const size_t size, const uint8_t priority) :
		stack_{buffer, size, threadRunner, *this},
		list_{},
		iterator_{},
		priority_{priority},
		roundRobinQuantum_{},
		state_{State::New}
{

}

void ThreadControlBlock::setPriority(const uint8_t priority)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (priority_ == priority)
		return;

	priority_ = priority;

	if (list_ == nullptr)
		return;

	list_->sortedSplice(*list_, iterator_);
	/// \todo implement position change according to POSIX spec -
	/// http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_08_04_02

	schedulerInstance.maybeRequestContextSwitch();
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(architecture::Stack&& stack, const uint8_t priority) :
		stack_{stack},
		list_{},
		iterator_{},
		priority_{priority},
		roundRobinQuantum_{},
		state_{State::New}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadControlBlock::threadRunner(ThreadControlBlock& threadControlBlock)
{
	threadControlBlock.run_();
	threadReturnTrap();
}

}	// namespace scheduler

}	// namespace distortos
