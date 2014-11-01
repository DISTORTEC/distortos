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
 * \date 2014-10-30
 */

#include "distortos/scheduler/ThreadControlBlock.hpp"

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

ThreadControlBlock::ThreadControlBlock(void* const buffer, const size_t size, const uint8_t priority) :
		stack_{buffer, size, threadRunner, *this},
		list_{},
		iterator_{},
		priority_{priority},
		boostedPriority_{},
		roundRobinQuantum_{},
		state_{State::New}
{

}

void ThreadControlBlock::setPriority(const uint8_t priority, const bool alwaysBehind)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (priority_ == priority)
		return;

	// special case of new priority UINT8_MAX does need to be handled, as it will never be "lowering" of priority anyway
	const auto loweringBefore = alwaysBehind == false && priority_ > priority;

	priority_ = priority;

	if (list_ == nullptr)
		return;

	if (loweringBefore == true)
		++priority_;	// effectively causes new position to be "before" threads with the same priority

	list_->sortedSplice(*list_, iterator_);

	if (loweringBefore == true)
		--priority_;

	getScheduler().maybeRequestContextSwitch();
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(architecture::Stack&& stack, const uint8_t priority) :
		stack_{stack},
		list_{},
		iterator_{},
		priority_{priority},
		boostedPriority_{},
		roundRobinQuantum_{},
		state_{State::New}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadControlBlock::threadRunner(ThreadControlBlock& threadControlBlock)
{
	threadControlBlock.run();
	getScheduler().remove();

	while (1);
}

}	// namespace scheduler

}	// namespace distortos
