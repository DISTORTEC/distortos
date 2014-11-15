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
 * \date 2014-11-15
 */

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/MutexControlBlock.hpp"

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
		ownedProtocolMutexControlBlocksList_
		{
				MutexControlBlockListAllocator{getScheduler().getMutexControlBlockListAllocatorPool()}
		},
		priorityInheritanceMutexControlBlock_{},
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

	const auto previousEffectivePriority = getEffectivePriority();
	priority_ = priority;

	if (previousEffectivePriority == getEffectivePriority() || list_ == nullptr)
		return;

	reposition(loweringBefore);

	if (priorityInheritanceMutexControlBlock_ != nullptr)
		priorityInheritanceMutexControlBlock_->getOwner()->updateBoostedPriority();
}

void ThreadControlBlock::updateBoostedPriority(const uint8_t boostedPriority)
{
	decltype(boostedPriority_) newBoostedPriority {boostedPriority};

	for (const auto &mutexControlBlock : ownedProtocolMutexControlBlocksList_)
	{
		const auto mutexBoostedPriority = mutexControlBlock.get().getBoostedPriority();
		newBoostedPriority = std::max(newBoostedPriority, mutexBoostedPriority);
	}

	if (boostedPriority_ == newBoostedPriority)
		return;

	const auto oldEffectivePriority = getEffectivePriority();
	boostedPriority_ = newBoostedPriority;
	const auto newEffectivePriority = getEffectivePriority();

	if (oldEffectivePriority == newEffectivePriority || list_ == nullptr)
		return;

	const auto loweringBefore = newEffectivePriority < oldEffectivePriority;

	reposition(loweringBefore);

	// this code is placed here, even though it could be moved to ThreadControlBlock::reposition(), simplifying
	// ThreadControlBlock::setPriority(). This way optimizer can remove recursive calls to this function, reducing
	// memory usage of threads.
	if (priorityInheritanceMutexControlBlock_ != nullptr)
		priorityInheritanceMutexControlBlock_->getOwner()->updateBoostedPriority();
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(architecture::Stack&& stack, const uint8_t priority) :
		stack_{stack},
		ownedProtocolMutexControlBlocksList_
		{
				MutexControlBlockListAllocator{getScheduler().getMutexControlBlockListAllocatorPool()}
		},
		priorityInheritanceMutexControlBlock_{},
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

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadControlBlock::reposition(const bool loweringBefore)
{
	const auto oldPriority = priority_;

	if (loweringBefore == true)
		priority_ = getEffectivePriority() + 1;

	list_->sortedSplice(*list_, iterator_);

	if (loweringBefore == true)
		priority_ = oldPriority;

	getScheduler().maybeRequestContextSwitch();
}

}	// namespace scheduler

}	// namespace distortos
