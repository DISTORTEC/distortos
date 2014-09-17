/**
 * \file
 * \brief Scheduler class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-17
 */

#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/scheduler/Thread.hpp"
#include "distortos/scheduler/SoftwareTimer.hpp"
#include "distortos/scheduler/MainThreadControlBlock.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"
#include "distortos/architecture/InterruptUnmaskingLock.hpp"

#include <cerrno>

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Scheduler::Scheduler(MainThreadControlBlock& mainThreadControlBlock, Thread<void (&)()>& idleThread) :
		currentThreadControlBlock_{},
		threadControlBlockListAllocatorPool_{},
		threadControlBlockListAllocator_{threadControlBlockListAllocatorPool_},
		runnableList_{threadControlBlockListAllocator_, ThreadControlBlock::State::Runnable},
		suspendedList_{threadControlBlockListAllocator_, ThreadControlBlock::State::Suspended},
		softwareTimerControlBlockSupervisor_{},
		tickCount_{0}
{
	add(mainThreadControlBlock);
	currentThreadControlBlock_ = runnableList_.begin();

	idleThread.start();

	architecture::startScheduling();
}

void Scheduler::add(ThreadControlBlock& threadControlBlock)
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	threadControlBlockListAllocatorPool_.feed(threadControlBlock.getLink());
	runnableList_.sortedEmplace(threadControlBlock);
	maybeRequestContextSwitch();
}

void Scheduler::block(ThreadControlBlockList& container)
{
	block(container, currentThreadControlBlock_);
}

int Scheduler::block(ThreadControlBlockList& container, const ThreadControlBlockListIterator iterator)
{
	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		const auto ret = blockInternal(container, iterator);
		if (ret != 0)
			return ret;

		if (iterator != currentThreadControlBlock_)	// blocked thread is not current thread - no forced switch required
			return 0;
	}

	forceContextSwitch();

	return 0;
}

int Scheduler::blockUntil(ThreadControlBlockList& container, const TickClock::time_point timePoint)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto iterator = currentThreadControlBlock_;
	bool timedOut {};
	// This lambda unblocks the thread only if it wasn't already unblocked - this is necessary because double unblock
	// should be avoided (it could mess the order of threads of the same priority). In that case it also marks the
	// "timed out" reason of unblocking.
	auto softwareTimer = makeSoftwareTimer(
			[this, iterator, &timedOut]()
			{
				if (iterator->get().getList() != &runnableList_)
				{
					unblockInternal(iterator);
					timedOut = true;
				}
			});
	softwareTimer.start(timePoint);

	block(container);

	return timedOut == false ? 0 : ETIMEDOUT;
}

uint64_t Scheduler::getTickCount() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	return tickCount_;
}

void Scheduler::maybeRequestContextSwitch() const
{
	if (isContextSwitchRequired() == true)
		architecture::requestContextSwitch();
}

int Scheduler::remove()
{
	{
		architecture::InterruptMaskingLock interruptMaskingLock;
		ThreadControlBlockList terminatedList {threadControlBlockListAllocator_, ThreadControlBlock::State::Terminated};

		const auto ret = blockInternal(terminatedList, currentThreadControlBlock_);
		if (ret != 0)
			return ret;

		terminatedList.begin()->get().terminationHook();
	}

	forceContextSwitch();

	return 0;
}

int Scheduler::resume(const ThreadControlBlockListIterator iterator)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	if (iterator->get().getList() != &suspendedList_)
		return EINVAL;

	unblock(iterator);
	return 0;
}

void Scheduler::suspend()
{
	suspend(currentThreadControlBlock_);
}

int Scheduler::suspend(const ThreadControlBlockListIterator iterator)
{
	return block(suspendedList_, iterator);
}

void* Scheduler::switchContext(void* const stackPointer)
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	getCurrentThreadControlBlock().getStack().setStackPointer(stackPointer);
	currentThreadControlBlock_ = runnableList_.begin();
	return getCurrentThreadControlBlock().getStack().getStackPointer();
}

bool Scheduler::tickInterruptHandler()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	++tickCount_;

	getCurrentThreadControlBlock().getRoundRobinQuantum().decrement();

	// if the object is on the "runnable" list and it used its round-robin quantum, then do the "rotation": move current
	// thread to the end of same-priority group to implement round-robin scheduling
	if (getCurrentThreadControlBlock().getList() == &runnableList_ &&
			getCurrentThreadControlBlock().getRoundRobinQuantum().isZero() == true)
	{
		getCurrentThreadControlBlock().getRoundRobinQuantum().reset();
		runnableList_.sortedSplice(runnableList_, currentThreadControlBlock_);
	}

	softwareTimerControlBlockSupervisor_.tickInterruptHandler(TickClock::time_point{TickClock::duration{tickCount_}});

	return isContextSwitchRequired();
}

void Scheduler::unblock(const ThreadControlBlockListIterator iterator)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	unblockInternal(iterator);
	maybeRequestContextSwitch();
}

void Scheduler::yield()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	runnableList_.sortedSplice(runnableList_, currentThreadControlBlock_);
	maybeRequestContextSwitch();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int Scheduler::blockInternal(ThreadControlBlockList& container, const ThreadControlBlockListIterator iterator)
{
	if (iterator->get().getList() != &runnableList_)
		return EINVAL;

	container.sortedSplice(runnableList_, iterator);

	return 0;
}

void Scheduler::forceContextSwitch() const
{
	architecture::InterruptUnmaskingLock interruptUnmaskingLock;
	architecture::requestContextSwitch();
}

bool Scheduler::isContextSwitchRequired() const
{
	// this check must be first, because during early startup currentThreadControlBlock_ is not yet initialized (so
	// futher conditions would dereference nullptr) and no threads are available
	if (runnableList_.size() <= 1)	// no threads or single thread available?
		return false;				// no context switch possible

	if (getCurrentThreadControlBlock().getList() != &runnableList_)
		return true;

	if (runnableList_.begin() != currentThreadControlBlock_)	// is there a higher-priority thread available?
		return true;

	if (getCurrentThreadControlBlock().getRoundRobinQuantum().isZero() == true)
	{
		const auto nextThread = ++runnableList_.begin();
		const auto nextThreadPriority = nextThread->get().getPriority();
		// thread with same priority available?
		if (getCurrentThreadControlBlock().getPriority() == nextThreadPriority)
			return true;	// switch context to do round-robin scheduling
	}

	return false;
}

void Scheduler::unblockInternal(const ThreadControlBlockListIterator iterator)
{
	runnableList_.sortedSplice(*iterator->get().getList(), iterator);
	iterator->get().getRoundRobinQuantum().reset();
}

}	// namespace scheduler

}	// namespace distortos
