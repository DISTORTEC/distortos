/**
 * \file
 * \brief MutexControlBlock class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-03
 */

#include "distortos/scheduler/MutexControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

MutexControlBlock::MutexControlBlock(const Protocol protocol, const uint8_t priorityCeiling) :
		blockedList_{getScheduler().getThreadControlBlockListAllocator(), ThreadControlBlock::State::BlockedOnMutex},
		list_{},
		iterator_{},
		owner_{},
		protocol_{protocol},
		priorityCeiling_{priorityCeiling}
{

}

void MutexControlBlock::block()
{
	getScheduler().block(blockedList_);
}

int MutexControlBlock::blockUntil(const TickClock::time_point timePoint)
{
	return getScheduler().blockUntil(blockedList_, timePoint);
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
	auto& scheduler = scheduler::getScheduler();
	owner_ = &scheduler.getCurrentThreadControlBlock();

	if (protocol_ == Protocol::None)
		return;

	scheduler.getMutexControlBlockListAllocatorPool().feed(link_);
	list_ = &owner_->getOwnedProtocolMutexControlBlocksList();
	list_->emplace_front(*this);
	iterator_ = list_->begin();

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

void MutexControlBlock::transferLock()
{
	owner_ = &blockedList_.begin()->get();	// pass ownership to the unblocked thread
	scheduler::getScheduler().unblock(blockedList_.begin());

	if (list_ == nullptr)
		return;

	auto& oldList = *list_;
	list_ = &owner_->getOwnedProtocolMutexControlBlocksList();
	list_->splice(list_->begin(), oldList, iterator_);
}

void MutexControlBlock::unlock()
{
	owner_ = nullptr;

	if (list_ == nullptr)
		return;

	list_->erase(iterator_);
	list_ = nullptr;
}

}	// namespace scheduler

}	// namespace distortos
