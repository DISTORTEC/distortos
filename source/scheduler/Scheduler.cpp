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
 * \date 2014-09-07
 */

#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/scheduler/Thread.hpp"
#include "distortos/scheduler/SoftwareTimer.hpp"
#include "distortos/scheduler/MainThreadControlBlock.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"
#include "distortos/architecture/InterruptUnmaskingLock.hpp"

#include "distortos/distortosConfiguration.h"

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
		runnableList_{ThreadControlBlock::State::Runnable},
		suspendedList_{ThreadControlBlock::State::Suspended},
		softwareTimerControlBlockSupervisor_{},
		tickCount_{0}
{
	mainThreadControlBlock.getRoundRobinQuantum().reset();
	add(mainThreadControlBlock);
	currentThreadControlBlock_ = runnableList_.begin();

	idleThread.start();

	architecture::startScheduling();
}

void Scheduler::add(ThreadControlBlock& thread_control_block)
{
	architecture::InterruptMaskingLock interrupt_masking_lock;
	runnableList_.sortedEmplace(thread_control_block);
	yield();
}

void Scheduler::block(ThreadControlBlockList& container)
{
	block(container, currentThreadControlBlock_);
}

int Scheduler::block(ThreadControlBlockList& container, const Iterator iterator)
{
	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		const auto ret = blockInternal_(container, iterator);
		if (ret != 0)
			return ret;

		if (iterator != currentThreadControlBlock_)	// blocked thread is not current thread - no forced switch required
			return 0;
	}

	forceContextSwitch_();

	return 0;
}

uint64_t Scheduler::getTickCount() const
{
	architecture::InterruptMaskingLock interrupt_masking_lock;
	return tickCount_;
}

int Scheduler::remove()
{
	{
		architecture::InterruptMaskingLock interruptMaskingLock;
		ThreadControlBlockList terminatedList {ThreadControlBlock::State::Terminated};

		const auto ret = blockInternal_(terminatedList, currentThreadControlBlock_);
		if (ret != 0)
			return ret;

		terminatedList.begin()->get().terminationHook();
	}

	forceContextSwitch_();

	return 0;
}

int Scheduler::resume(const Iterator iterator)
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	if (iterator->get().getState() != ThreadControlBlock::State::Suspended)
		return EINVAL;

	unblock(suspendedList_, iterator);
	return 0;
}

void Scheduler::sleepUntil(const TickClock::time_point time_point)
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	ThreadControlBlockList sleeping_list {ThreadControlBlock::State::Sleeping};
	auto software_timer =
			makeSoftwareTimer(&Scheduler::unblockInternal_, this, std::ref(sleeping_list), currentThreadControlBlock_);
	software_timer.start(time_point);

	block(sleeping_list);
}

void Scheduler::suspend()
{
	suspend(currentThreadControlBlock_);
}

int Scheduler::suspend(const Iterator iterator)
{
	return block(suspendedList_, iterator);
}

void* Scheduler::switchContext(void* stack_pointer)
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	getCurrentThreadControlBlock().getStack().setStackPointer(stack_pointer);

	// if the object is on the "runnable" list and it used its round-robin quantum - do the rotation
	// move current thread to the end of same-priority group to implement round-robin scheduling
	if (getCurrentThreadControlBlock().getState() == ThreadControlBlock::State::Runnable &&
			getCurrentThreadControlBlock().getRoundRobinQuantum().get() == 0)
		runnableList_.sortedSplice(runnableList_, currentThreadControlBlock_);

	currentThreadControlBlock_ = runnableList_.begin();

	getCurrentThreadControlBlock().getRoundRobinQuantum().reset();

	return getCurrentThreadControlBlock().getStack().getStackPointer();
}

bool Scheduler::tickInterruptHandler()
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	++tickCount_;

	getCurrentThreadControlBlock().getRoundRobinQuantum().decrement();

	softwareTimerControlBlockSupervisor_.tickInterruptHandler(TickClock::time_point{TickClock::duration{tickCount_}});

	return isContextSwitchRequired_();
}

void Scheduler::unblock(ThreadControlBlockList& container, const Iterator iterator)
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	unblockInternal_(container, iterator);
	yield();
}

void Scheduler::yield() const
{
	architecture::InterruptMaskingLock interrupt_masking_lock;

	if (isContextSwitchRequired_() == true)
		requestContextSwitch_();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int Scheduler::blockInternal_(ThreadControlBlockList& container, const Iterator iterator)
{
	if (iterator->get().getState() != ThreadControlBlock::State::Runnable)
		return EINVAL;

	container.sortedSplice(runnableList_, iterator);

	return 0;
}

void Scheduler::forceContextSwitch_() const
{
	architecture::InterruptUnmaskingLock interruptUnmaskingLock;
	requestContextSwitch_();
}

bool Scheduler::isContextSwitchRequired_() const
{
	// this check must be first, because during early startup currentThreadControlBlock_ is not yet initialized (so
	// futher conditions would dereference nullptr) and no threads are available
	if (runnableList_.size() <= 1)	// no threads or single thread available?
		return false;				// no context switch possible

	if (getCurrentThreadControlBlock().getState() != ThreadControlBlock::State::Runnable)
		return true;

	if (runnableList_.begin() != currentThreadControlBlock_)	// is there a higher-priority thread available?
		return true;

	if (getCurrentThreadControlBlock().getRoundRobinQuantum().get() == 0)
	{
		const auto next_thread = ++runnableList_.begin();
		const auto next_thread_priority = next_thread->get().getPriority();
		// thread with same priority available?
		if (getCurrentThreadControlBlock().getPriority() == next_thread_priority)
			return true;	// switch context to do round-robin scheduling
	}

	return false;
}

void Scheduler::requestContextSwitch_() const
{
	architecture::requestContextSwitch();
}

void Scheduler::unblockInternal_(ThreadControlBlockList& container, const Iterator iterator)
{
	runnableList_.sortedSplice(container, iterator);
}

}	// namespace scheduler

}	// namespace distortos
