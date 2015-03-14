/**
 * \file
 * \brief ThreadControlBlock class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-14
 */

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/scheduler/ThreadGroupControlBlock.hpp"

#include "distortos/synchronization/MutexControlBlock.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/SignalsReceiver.hpp"

#include <cerrno>
#include <cstring>

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadControlBlock::ThreadControlBlock(architecture::Stack&& stack, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, ThreadGroupControlBlock* const threadGroupControlBlock,
		SignalsReceiver* const signalsReceiver, ThreadBase& owner) :
		stack_{std::move(stack)},
		owner_(owner),
		ownedProtocolMutexControlBlocksList_
		{
				MutexControlBlockListAllocator{getScheduler().getMutexControlBlockListAllocatorPool()}
		},
		priorityInheritanceMutexControlBlock_{},
		list_{},
		iterator_{},
		threadGroupControlBlock_{threadGroupControlBlock},
		threadGroupList_{},
		threadGroupIterator_{},
		unblockReason_{},
		signalsReceiverControlBlock_
		{
				signalsReceiver != nullptr ? &signalsReceiver->signalsReceiverControlBlock_ : nullptr
		},
		pendingSignalSet_{SignalSet::empty},
		waitingSignalSet_{},
		priority_{priority},
		boostedPriority_{},
		roundRobinQuantum_{},
		schedulingPolicy_{schedulingPolicy},
		state_{State::New}
{
	_REENT_INIT_PTR(&reent_);
}

ThreadControlBlock::~ThreadControlBlock()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (threadGroupList_ != nullptr)
		threadGroupList_->erase(threadGroupIterator_);

	_reclaim_reent(&reent_);
}

int ThreadControlBlock::acceptPendingSignal(const uint8_t signalNumber)
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	return pendingSignalSet_.remove(signalNumber);
}

int ThreadControlBlock::addHook()
{
	if (threadGroupControlBlock_ == nullptr)
	{
		threadGroupControlBlock_ = getScheduler().getCurrentThreadControlBlock().threadGroupControlBlock_;
		if (threadGroupControlBlock_ == nullptr)
			return EINVAL;
	}

	const auto addRet = threadGroupControlBlock_->add(*this);
	threadGroupList_ = &addRet.first;
	threadGroupIterator_ = addRet.second;
	return 0;
}

SignalSet ThreadControlBlock::getPendingSignalSet() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	return pendingSignalSet_;
}

int ThreadControlBlock::generateSignal(const uint8_t signalNumber)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = pendingSignalSet_.add(signalNumber);
	if (ret != 0)
		return ret;

	if (waitingSignalSet_ == nullptr)
		return 0;

	const auto testResult = waitingSignalSet_->test(signalNumber);
	if (testResult.first != 0)
		return testResult.first;
	if (testResult.second == false)	// signalNumber is not "waited for"?
		return 0;

	getScheduler().unblock(iterator_);
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

	if (previousEffectivePriority == getEffectivePriority() || list_ == nullptr)
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
	unblockReason_ = unblockReason;
	if (unblockFunctor != nullptr)
		(*unblockFunctor)(*this);
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
