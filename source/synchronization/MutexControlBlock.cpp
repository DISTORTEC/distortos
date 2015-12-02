/**
 * \file
 * \brief MutexControlBlock class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-02
 */

#include "distortos/internal/synchronization/MutexControlBlock.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include <cerrno>

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// PriorityInheritanceMutexControlBlockUnblockFunctor is a functor executed when unblocking a thread that is blocked on
/// a mutex with PriorityInheritance protocol
class PriorityInheritanceMutexControlBlockUnblockFunctor : public ThreadControlBlock::UnblockFunctor
{
public:

	/**
	 * \brief PriorityInheritanceMutexControlBlockUnblockFunctor's constructor
	 *
	 * \param [in] mutexControlBlock is a reference to MutexControlBlock that blocked the thread
	 */

	constexpr explicit PriorityInheritanceMutexControlBlockUnblockFunctor(const MutexControlBlock& mutexControlBlock) :
			mutexControlBlock_{mutexControlBlock}
	{

	}

	/**
	 * \brief PriorityInheritanceMutexControlBlockUnblockFunctor's function call operator
	 *
	 * If the wait for mutex was interrupted, requests update of boosted priority of current owner of the mutex. Pointer
	 * to MutexControlBlock with PriorityInheritance protocol which caused the thread to block is reset to nullptr.
	 *
	 * \param [in] threadControlBlock is a reference to ThreadControlBlock that is being unblocked
	 * \param [in] unblockReason is the reason of thread unblocking
	 */

	virtual void operator()(ThreadControlBlock& threadControlBlock,
			const ThreadControlBlock::UnblockReason unblockReason) const override
	{
		const auto owner = mutexControlBlock_.getOwner();

		// waiting for mutex was interrupted and some thread still holds it?
		if (unblockReason != ThreadControlBlock::UnblockReason::UnblockRequest && owner != nullptr)
			owner->updateBoostedPriority();

		threadControlBlock.setPriorityInheritanceMutexControlBlock(nullptr);
	}

private:

	/// reference to MutexControlBlock that blocked the thread
	const MutexControlBlock& mutexControlBlock_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

MutexControlBlock::MutexControlBlock(const Protocol protocol, const uint8_t priorityCeiling) :
		blockedList_{getScheduler().getThreadControlBlockListAllocator(), ThreadState::BlockedOnMutex},
		list_{},
		iterator_{},
		owner_{},
		protocol_{protocol},
		priorityCeiling_{priorityCeiling}
{

}

int MutexControlBlock::block()
{
	if (protocol_ == Protocol::PriorityInheritance)
		priorityInheritanceBeforeBlock();

	const PriorityInheritanceMutexControlBlockUnblockFunctor unblockFunctor {*this};
	return getScheduler().block(blockedList_, ThreadState::BlockedOnMutex,
			protocol_ == Protocol::PriorityInheritance ? &unblockFunctor : nullptr);
}

int MutexControlBlock::blockUntil(const TickClock::time_point timePoint)
{
	if (protocol_ == Protocol::PriorityInheritance)
		priorityInheritanceBeforeBlock();

	const PriorityInheritanceMutexControlBlockUnblockFunctor unblockFunctor {*this};
	return getScheduler().blockUntil(blockedList_, timePoint, protocol_ == Protocol::PriorityInheritance ?
			&unblockFunctor : nullptr);
}

uint8_t MutexControlBlock::getBoostedPriority() const
{
	if (protocol_ == Protocol::PriorityInheritance)
	{
		if (blockedList_.empty() == true)
			return 0;
		return blockedList_.begin()->get().getEffectivePriority();
	}

	if (protocol_ == Protocol::PriorityProtect)
		return priorityCeiling_;

	return 0;
}

void MutexControlBlock::lock()
{
	auto& scheduler = getScheduler();
	owner_ = &scheduler.getCurrentThreadControlBlock();

	if (protocol_ == Protocol::None)
		return;

	scheduler.getMutexControlBlockListAllocatorPool().feed(link_);
	list_ = &owner_->getOwnedProtocolMutexControlBlocksList();
	list_->emplace_front(*this);
	iterator_ = list_->begin();

	if (protocol_ == Protocol::PriorityProtect)
		owner_->updateBoostedPriority();
}

void MutexControlBlock::unlockOrTransferLock()
{
	auto& oldOwner = *owner_;

	if (blockedList_.empty() == false)
		transferLock();
	else
		unlock();

	if (protocol_ == Protocol::None)
		return;

	oldOwner.updateBoostedPriority();

	if (owner_ == nullptr)
		return;

	owner_->updateBoostedPriority();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void MutexControlBlock::priorityInheritanceBeforeBlock() const
{
	auto& currentThreadControlBlock = getScheduler().getCurrentThreadControlBlock();

	currentThreadControlBlock.setPriorityInheritanceMutexControlBlock(this);

	// calling thread is not yet on the blocked list, that's why it's effective priority is given explicitly
	owner_->updateBoostedPriority(currentThreadControlBlock.getEffectivePriority());
}

void MutexControlBlock::transferLock()
{
	owner_ = &blockedList_.begin()->get();	// pass ownership to the unblocked thread
	getScheduler().unblock(blockedList_.begin());

	if (list_ == nullptr)
		return;

	auto& oldList = *list_;
	list_ = &owner_->getOwnedProtocolMutexControlBlocksList();
	list_->splice(list_->begin(), oldList, iterator_);

	if (protocol_ == Protocol::PriorityInheritance)
		owner_->setPriorityInheritanceMutexControlBlock(nullptr);
}

void MutexControlBlock::unlock()
{
	owner_ = nullptr;

	if (list_ == nullptr)
		return;

	list_->erase(iterator_);
	list_ = nullptr;
}

}	// namespace internal

}	// namespace distortos
