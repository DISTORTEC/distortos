/**
 * \file
 * \brief MutexControlBlock class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/synchronization/MutexControlBlock.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

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
/// a mutex with priorityInheritance protocol
class PriorityInheritanceMutexControlBlockUnblockFunctor : public UnblockFunctor
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
	 * to MutexControlBlock with priorityInheritance protocol which caused the thread to block is reset to nullptr.
	 *
	 * \param [in] threadControlBlock is a reference to ThreadControlBlock that is being unblocked
	 * \param [in] unblockReason is the reason of thread unblocking
	 */

	void operator()(ThreadControlBlock& threadControlBlock, const UnblockReason unblockReason) const override
	{
		const auto owner = mutexControlBlock_.getOwner();

		// waiting for mutex was interrupted and some thread still holds it?
		if (unblockReason != UnblockReason::unblockRequest && owner != nullptr)
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

uint8_t MutexControlBlock::getBoostedPriority() const
{
	if (getProtocol() == Protocol::priorityInheritance)
	{
		if (blockedList_.empty() == true)
			return 0;
		return blockedList_.front().getEffectivePriority();
	}

	if (getProtocol() == Protocol::priorityProtect)
		return getPriorityCeiling();

	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

int MutexControlBlock::doBlock()
{
	beforeBlock();

	const PriorityInheritanceMutexControlBlockUnblockFunctor unblockFunctor {*this};
	return getScheduler().block(blockedList_, ThreadState::blockedOnMutex,
			getProtocol() == Protocol::priorityInheritance ? &unblockFunctor : nullptr);
}

int MutexControlBlock::doBlockUntil(const TickClock::time_point timePoint)
{
	beforeBlock();

	const PriorityInheritanceMutexControlBlockUnblockFunctor unblockFunctor {*this};
	return getScheduler().blockUntil(blockedList_, ThreadState::blockedOnMutex, timePoint,
			getProtocol() == Protocol::priorityInheritance ? &unblockFunctor : nullptr);
}

void MutexControlBlock::doLock()
{
	auto& scheduler = getScheduler();
	owner_ = &scheduler.getCurrentThreadControlBlock();

	if (getProtocol() == Protocol::none)
		return;

	getOwner()->getOwnedProtocolMutexList().push_front(*this);

	if (getProtocol() == Protocol::priorityProtect)
		getOwner()->updateBoostedPriority();
}

void MutexControlBlock::doUnlockOrTransferLock()
{
	auto& oldOwner = *getOwner();

	if (blockedList_.empty() == false)
		doTransferLock();
	else
		doUnlock();

	if (getProtocol() == Protocol::none)
		return;

	oldOwner.updateBoostedPriority();

	if (getOwner() == nullptr)
		return;

	getOwner()->updateBoostedPriority();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void MutexControlBlock::beforeBlock() const
{
	if (getProtocol() != Protocol::priorityInheritance)
		return;

	auto& currentThreadControlBlock = getScheduler().getCurrentThreadControlBlock();

	currentThreadControlBlock.setPriorityInheritanceMutexControlBlock(this);

	// calling thread is not yet on the blocked list, that's why it's effective priority is given explicitly
	getOwner()->updateBoostedPriority(currentThreadControlBlock.getEffectivePriority());
}

void MutexControlBlock::doTransferLock()
{
	owner_ = &blockedList_.front();	// pass ownership to the unblocked thread
	getScheduler().unblock(blockedList_.begin());

	if (node.isLinked() == false)
		return;

	MutexList::splice(getOwner()->getOwnedProtocolMutexList().begin(), MutexList::iterator{*this});

	if (getProtocol() == Protocol::priorityInheritance)
		getOwner()->setPriorityInheritanceMutexControlBlock(nullptr);
}

void MutexControlBlock::doUnlock()
{
	owner_ = nullptr;

	if (node.isLinked() == false)
		return;

	node.unlink();
}

}	// namespace internal

}	// namespace distortos
