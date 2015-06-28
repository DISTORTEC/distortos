/**
 * \file
 * \brief Scheduler class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-06-28
 */

#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/SoftwareTimer.hpp"
#include "distortos/scheduler/MainThread.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"
#include "distortos/architecture/InterruptUnmaskingLock.hpp"
#include "distortos/architecture/requestContextSwitch.hpp"

#include <cerrno>

namespace distortos
{

namespace scheduler
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// UnblockReasonUnblockFunctorWrapper is a wrapper for ThreadControlBlock::UnblockFunctor that saves reason of thread
/// unblocking
class UnblockReasonUnblockFunctorWrapper : public ThreadControlBlock::UnblockFunctor
{
public:

	/**
	 * \brief UnblockReasonUnblockFunctorWrapper's constructor
	 *
	 * \param [in] unblockFunctor is a pointer to wrapped unblock functor
	 * \param [out] unblockReason is a reference to variable in which the reason of thread unblocking will be stored
	 */

	constexpr UnblockReasonUnblockFunctorWrapper(const ThreadControlBlock::UnblockFunctor* const unblockFunctor,
			ThreadControlBlock::UnblockReason& unblockReason) :
			unblockFunctor_{unblockFunctor},
			unblockReason_{unblockReason}
	{

	}

	/**
	 * \brief UnblockReasonUnblockFunctorWrapper's function call operator
	 *
	 * Saves reason of thread unblocking and executes wrapped functor (if any was provided).
	 *
	 * \param [in] threadControlBlock is a reference to ThreadControlBlock that is being unblocked
	 * \param [in] unblockReason is the reason of thread unblocking
	 */

	virtual void operator()(ThreadControlBlock& threadControlBlock,
			const ThreadControlBlock::UnblockReason unblockReason) const override
	{
		unblockReason_ = unblockReason;
		if (unblockFunctor_ != nullptr)
			(*unblockFunctor_)(threadControlBlock, unblockReason);
	}

private:

	/// pointer to wrapped unblock functor
	const ThreadControlBlock::UnblockFunctor* unblockFunctor_;

	/// reference to variable in which the reason of thread unblocking will be stored
	ThreadControlBlock::UnblockReason& unblockReason_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Forces unconditional context switch.
 *
 * Temporarily disables any interrupt masking and requests unconditional context switch.
 */

void forceContextSwitch()
{
	architecture::requestContextSwitch();
	architecture::InterruptUnmaskingLock interruptUnmaskingLock;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Scheduler::Scheduler() :
		currentThreadControlBlock_{},
		mutexControlBlockListAllocatorPool_{},
		threadControlBlockListAllocatorPool_{},
		threadControlBlockListAllocator_{threadControlBlockListAllocatorPool_},
		runnableList_{threadControlBlockListAllocator_, ThreadControlBlock::State::Runnable},
		suspendedList_{threadControlBlockListAllocator_, ThreadControlBlock::State::Suspended},
		softwareTimerControlBlockSupervisor_{},
		contextSwitchCount_{},
		tickCount_{}
{

}

int Scheduler::add(ThreadControlBlock& threadControlBlock)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = addInternal(threadControlBlock);
	if (ret != 0)
		return ret;

	maybeRequestContextSwitch();

	return 0;
}

int Scheduler::block(ThreadControlBlockList& container, const ThreadControlBlock::UnblockFunctor* const unblockFunctor)
{
	return block(container, currentThreadControlBlock_, unblockFunctor);
}

int Scheduler::block(ThreadControlBlockList& container, const ThreadControlBlockListIterator iterator,
		const ThreadControlBlock::UnblockFunctor* const unblockFunctor)
{
	ThreadControlBlock::UnblockReason unblockReason {};
	const UnblockReasonUnblockFunctorWrapper unblockReasonUnblockFunctorWrapper {unblockFunctor, unblockReason};
	const auto blockingCurrentThread = iterator == currentThreadControlBlock_;

	{
		architecture::InterruptMaskingLock interruptMaskingLock;

		// if blocking current thread, use unblockReasonUnblockFunctorWrapper, otherwise use provided unblockFunctor
		const auto ret = blockInternal(container, iterator, blockingCurrentThread == true ?
				&unblockReasonUnblockFunctorWrapper : unblockFunctor);
		if (ret != 0)
			return ret;

		if (blockingCurrentThread == false)	// blocked thread is not current thread - no forced switch required
			return 0;
	}

	forceContextSwitch();

	return unblockReason == ThreadControlBlock::UnblockReason::UnblockRequest ? 0 :
			unblockReason == ThreadControlBlock::UnblockReason::Timeout ? ETIMEDOUT : EINTR;
}

int Scheduler::blockUntil(ThreadControlBlockList& container, const TickClock::time_point timePoint,
		const ThreadControlBlock::UnblockFunctor* const unblockFunctor)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto iterator = currentThreadControlBlock_;

	if (timePoint <= TickClock::now())
	{
		if (unblockFunctor != nullptr)
			(*unblockFunctor)(*iterator, ThreadControlBlock::UnblockReason::Timeout);
		return ETIMEDOUT;
	}

	// This lambda unblocks the thread only if it wasn't already unblocked - this is necessary because double unblock
	// should be avoided (it could mess the order of threads of the same priority). In that case it also sets
	// UnblockReason::Timeout.
	auto softwareTimer = makeSoftwareTimer([this, iterator]()
			{
				if (iterator->get().getList() != &runnableList_)
					unblockInternal(iterator, ThreadControlBlock::UnblockReason::Timeout);
			});
	softwareTimer.start(timePoint);

	return block(container, unblockFunctor);
}

uint64_t Scheduler::getContextSwitchCount() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	return contextSwitchCount_;
}

uint64_t Scheduler::getTickCount() const
{
	architecture::InterruptMaskingLock interruptMaskingLock;
	return tickCount_;
}

int Scheduler::initialize(MainThread& mainThread)
{
	const auto ret = addInternal(mainThread.getThreadControlBlock());
	if (ret != 0)
		return ret;

	currentThreadControlBlock_ = runnableList_.begin();

	return 0;
}

void Scheduler::maybeRequestContextSwitch() const
{
	if (isContextSwitchRequired() == true)
		architecture::requestContextSwitch();
}

int Scheduler::remove(void (ThreadBase::*terminationHook)())
{
	{
		architecture::InterruptMaskingLock interruptMaskingLock;
		ThreadControlBlockList terminatedList {threadControlBlockListAllocator_, ThreadControlBlock::State::Terminated};

		const auto ret = blockInternal(terminatedList, currentThreadControlBlock_, {});
		if (ret != 0)
			return ret;

		(terminatedList.begin()->get().getOwner().*terminationHook)();
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

int Scheduler::suspend()
{
	return suspend(currentThreadControlBlock_);
}

int Scheduler::suspend(const ThreadControlBlockListIterator iterator)
{
	return block(suspendedList_, iterator);
}

void* Scheduler::switchContext(void* const stackPointer)
{
	++contextSwitchCount_;
	getCurrentThreadControlBlock().getStack().setStackPointer(stackPointer);
	currentThreadControlBlock_ = runnableList_.begin();
	getCurrentThreadControlBlock().switchedToHook();
	return getCurrentThreadControlBlock().getStack().getStackPointer();
}

bool Scheduler::tickInterruptHandler()
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	++tickCount_;

	getCurrentThreadControlBlock().getRoundRobinQuantum().decrement();

	// if the object is on the "runnable" list, it uses SchedulingPolicy::RoundRobin and it used its round-robin
	// quantum, then do the "rotation": move current thread to the end of same-priority group to implement round-robin
	// scheduling
	if (getCurrentThreadControlBlock().getList() == &runnableList_ &&
			getCurrentThreadControlBlock().getSchedulingPolicy() == SchedulingPolicy::RoundRobin &&
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

int Scheduler::addInternal(ThreadControlBlock& threadControlBlock)
{
	const auto ret = threadControlBlock.addHook();
	if (ret != 0)
		return ret;

	threadControlBlockListAllocatorPool_.feed(threadControlBlock.getLink());
	runnableList_.sortedEmplace(threadControlBlock);

	return 0;
}

int Scheduler::blockInternal(ThreadControlBlockList& container, const ThreadControlBlockListIterator iterator,
		const ThreadControlBlock::UnblockFunctor* const unblockFunctor)
{
	if (iterator->get().getList() != &runnableList_)
		return EINVAL;

	container.sortedSplice(runnableList_, iterator);
	iterator->get().blockHook(unblockFunctor);

	return 0;
}

bool Scheduler::isContextSwitchRequired() const
{
	if (getCurrentThreadControlBlock().getList() != &runnableList_)
		return true;

	if (runnableList_.begin() != currentThreadControlBlock_)	// is there a higher-priority thread available?
		return true;

	return false;
}

void Scheduler::unblockInternal(const ThreadControlBlockListIterator iterator,
		const ThreadControlBlock::UnblockReason unblockReason)
{
	runnableList_.sortedSplice(*iterator->get().getList(), iterator);
	iterator->get().unblockHook(unblockReason);
}

}	// namespace scheduler

}	// namespace distortos
