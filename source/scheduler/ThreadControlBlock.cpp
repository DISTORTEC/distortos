/**
 * \file
 * \brief ThreadControlBlock class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/ThreadControlBlock.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"
#include "distortos/internal/scheduler/ThreadGroupControlBlock.hpp"

#include "distortos/internal/synchronization/MutexControlBlock.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/SignalsReceiver.hpp"

#include <cerrno>
#include <cstring>

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(architecture::Stack&& stack, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, ThreadGroupControlBlock* const threadGroupControlBlock,
		SignalsReceiver* const signalsReceiver, Thread& owner) :
		ThreadListNode{priority},
		stack_{std::move(stack)},
		owner_{owner},
		ownedProtocolMutexList_{},
		priorityInheritanceMutexControlBlock_{},
		list_{},
		threadGroupControlBlock_{threadGroupControlBlock},
		unblockFunctor_{},
		signalsReceiverControlBlock_
		{
				signalsReceiver != nullptr ? &signalsReceiver->signalsReceiverControlBlock_ : nullptr
		},
		roundRobinQuantum_{},
		schedulingPolicy_{schedulingPolicy},
		state_{ThreadState::Created}
{
	_REENT_INIT_PTR(&reent_);
}

ThreadControlBlock::~ThreadControlBlock()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	_reclaim_reent(&reent_);
}

int ThreadControlBlock::addHook()
{
	if (threadGroupControlBlock_ == nullptr)
	{
		threadGroupControlBlock_ = getScheduler().getCurrentThreadControlBlock().threadGroupControlBlock_;
		if (threadGroupControlBlock_ == nullptr)
			return EINVAL;
	}

	threadGroupControlBlock_->add(*this);

	return 0;
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

	if (previousEffectivePriority == getEffectivePriority() || threadListNode.isLinked() == false)
		return;

	reposition(loweringBefore);

	if (priorityInheritanceMutexControlBlock_ != nullptr)
		priorityInheritanceMutexControlBlock_->getOwner()->updateBoostedPriority();
}

void ThreadControlBlock::setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	schedulingPolicy_ = schedulingPolicy;
	roundRobinQuantum_.reset();
}

void ThreadControlBlock::unblockHook(const UnblockReason unblockReason)
{
	roundRobinQuantum_.reset();
	const auto unblockFunctor = unblockFunctor_;
	unblockFunctor_ = {};
	if (unblockFunctor != nullptr)
		(*unblockFunctor)(*this, unblockReason);
}

void ThreadControlBlock::updateBoostedPriority(const uint8_t boostedPriority)
{
	decltype(boostedPriority_) newBoostedPriority {boostedPriority};

	for (const auto& mutexControlBlock : ownedProtocolMutexList_)
	{
		const auto mutexBoostedPriority = mutexControlBlock.getBoostedPriority();
		newBoostedPriority = std::max(newBoostedPriority, mutexBoostedPriority);
	}

	if (boostedPriority_ == newBoostedPriority)
		return;

	const auto oldEffectivePriority = getEffectivePriority();
	boostedPriority_ = newBoostedPriority;
	const auto newEffectivePriority = getEffectivePriority();

	if (oldEffectivePriority == newEffectivePriority || threadListNode.isLinked() == false)
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
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadControlBlock::reposition(const bool loweringBefore)
{
	const auto oldPriority = priority_;

	if (loweringBefore == true)
		priority_ = getEffectivePriority() + 1;

	list_->splice(ThreadList::iterator{*this});

	if (loweringBefore == true)
		priority_ = oldPriority;

	getScheduler().maybeRequestContextSwitch();
}

}	// namespace internal

}	// namespace distortos
