/**
 * \file
 * \brief Scheduler class implementation
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/architecture/requestContextSwitch.hpp"

#include "distortos/internal/scheduler/forceContextSwitch.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/InterruptMaskingLock.hpp"
#include "distortos/StaticSoftwareTimer.hpp"

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

/// UnblockReasonUnblockFunctorWrapper is a wrapper for UnblockFunctor that saves reason of thread unblocking
class UnblockReasonUnblockFunctorWrapper : public UnblockFunctor
{
public:

	/**
	 * \brief UnblockReasonUnblockFunctorWrapper's constructor
	 *
	 * \param [in] unblockFunctor is a pointer to wrapped unblock functor
	 * \param [out] unblockReason is a reference to variable in which the reason of thread unblocking will be stored
	 */

	constexpr UnblockReasonUnblockFunctorWrapper(const UnblockFunctor* const unblockFunctor,
			UnblockReason& unblockReason) :
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

	void operator()(ThreadControlBlock& threadControlBlock, const UnblockReason unblockReason) const override
	{
		unblockReason_ = unblockReason;
		if (unblockFunctor_ != nullptr)
			(*unblockFunctor_)(threadControlBlock, unblockReason);
	}

private:

	/// pointer to wrapped unblock functor
	const UnblockFunctor* unblockFunctor_;

	/// reference to variable in which the reason of thread unblocking will be stored
	UnblockReason& unblockReason_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int Scheduler::add(ThreadControlBlock& threadControlBlock)
{
	const InterruptMaskingLock interruptMaskingLock;

	if (threadControlBlock.getState() != ThreadState::created)
		return EINVAL;

	{
		const auto ret = threadControlBlock.getStack().initialize(threadControlBlock.getOwner());
		if (ret != 0)
			return ret;
	}

	{
		const auto ret = addInternal(threadControlBlock);
		if (ret != 0)
			return ret;
	}

	maybeRequestContextSwitch();

	return 0;
}

int Scheduler::block(ThreadList& container, const ThreadState state, const UnblockFunctor* const unblockFunctor)
{
	CHECK_FUNCTION_CONTEXT();

	return block(container, currentThreadControlBlock_, state, unblockFunctor);
}

int Scheduler::block(ThreadList& container, const ThreadList::iterator iterator, const ThreadState state,
		const UnblockFunctor* const unblockFunctor)
{
	UnblockReason unblockReason {};
	const UnblockReasonUnblockFunctorWrapper unblockReasonUnblockFunctorWrapper {unblockFunctor, unblockReason};
	const auto blockingCurrentThread = iterator == currentThreadControlBlock_;

	{
		const InterruptMaskingLock interruptMaskingLock;

		// if blocking current thread, use unblockReasonUnblockFunctorWrapper, otherwise use provided unblockFunctor
		const auto ret = blockInternal(container, iterator, state, blockingCurrentThread == true ?
				&unblockReasonUnblockFunctorWrapper : unblockFunctor);
		if (ret != 0)
			return ret;

		if (blockingCurrentThread == false)	// blocked thread is not current thread - no forced switch required
			return 0;
	}

	forceContextSwitch();

	return unblockReason == UnblockReason::unblockRequest ? 0 :
			unblockReason == UnblockReason::timeout ? ETIMEDOUT : EINTR;
}

int Scheduler::blockUntil(ThreadList& container, const ThreadState state, const TickClock::time_point timePoint,
		const UnblockFunctor* const unblockFunctor)
{
	CHECK_FUNCTION_CONTEXT();

	const InterruptMaskingLock interruptMaskingLock;

	const auto iterator = currentThreadControlBlock_;

	if (timePoint <= TickClock::now())
	{
		if (unblockFunctor != nullptr)
			(*unblockFunctor)(*iterator, UnblockReason::timeout);
		return ETIMEDOUT;
	}

	// This lambda unblocks the thread only if it wasn't already unblocked - this is necessary because double unblock
	// should be avoided (it could mess the order of threads of the same priority). In that case it also sets
	// UnblockReason::timeout.
	auto softwareTimer = makeStaticSoftwareTimer([this, iterator]()
			{
				if (iterator->getList() != &runnableList_)
					unblockInternal(iterator, UnblockReason::timeout);
			});
	softwareTimer.start(timePoint);

	return block(container, state, unblockFunctor);
}

uint64_t Scheduler::getContextSwitchCount() const
{
	const InterruptMaskingLock interruptMaskingLock;
	return contextSwitchCount_;
}

uint64_t Scheduler::getTickCount() const
{
	const InterruptMaskingLock interruptMaskingLock;
	return tickCount_;
}

int Scheduler::initialize(ThreadControlBlock& mainThreadControlBlock)
{
	const auto ret = addInternal(mainThreadControlBlock);
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

int Scheduler::remove()
{
	CHECK_FUNCTION_CONTEXT();

	ThreadList terminatedList;
	const auto ret = blockInternal(terminatedList, currentThreadControlBlock_, ThreadState::terminated, {});
	if (ret != 0)
		return ret;

	currentThreadControlBlock_->setList(nullptr);

	return 0;
}

int Scheduler::resume(const ThreadList::iterator iterator)
{
	const InterruptMaskingLock interruptMaskingLock;

	if (iterator->getList() != &suspendedList_)
		return EINVAL;

	unblock(iterator);
	return 0;
}

int Scheduler::suspend()
{
	CHECK_FUNCTION_CONTEXT();

	return suspend(currentThreadControlBlock_);
}

int Scheduler::suspend(const ThreadList::iterator iterator)
{
	return block(suspendedList_, iterator, ThreadState::suspended);
}

void* Scheduler::switchContext(void* const stackPointer)
{
	++contextSwitchCount_;

	auto& stack = getCurrentThreadControlBlock().getStack();

#ifdef DISTORTOS_CHECK_STACK_GUARD_CONTEXT_SWITCH_ENABLE

	if (stack.checkStackGuard() == false)
		FATAL_ERROR("Stack overflow detected!");

#endif	// def DISTORTOS_CHECK_STACK_GUARD_CONTEXT_SWITCH_ENABLE

	stack.setStackPointer(stackPointer);
	currentThreadControlBlock_ = runnableList_.begin();
	getCurrentThreadControlBlock().switchedToHook();
	return getCurrentThreadControlBlock().getStack().getStackPointer();
}

bool Scheduler::tickInterruptHandler()
{
	const InterruptMaskingLock interruptMaskingLock;

#ifdef DISTORTOS_CHECK_STACK_GUARD_SYSTEM_TICK_ENABLE

	if (getCurrentThreadControlBlock().getStack().checkStackGuard() == false)
		FATAL_ERROR("Stack overflow detected!");

#endif	// def DISTORTOS_CHECK_STACK_GUARD_SYSTEM_TICK_ENABLE

	++tickCount_;

	getCurrentThreadControlBlock().getRoundRobinQuantum().decrement();

	// if the object is on the "runnable" list, it uses SchedulingPolicy::roundRobin and it used its round-robin
	// quantum, then do the "rotation": move current thread to the end of same-priority group to implement round-robin
	// scheduling
	if (getCurrentThreadControlBlock().getList() == &runnableList_ &&
			getCurrentThreadControlBlock().getSchedulingPolicy() == SchedulingPolicy::roundRobin &&
			getCurrentThreadControlBlock().getRoundRobinQuantum().isZero() == true)
	{
		getCurrentThreadControlBlock().getRoundRobinQuantum().reset();
		runnableList_.splice(currentThreadControlBlock_);
	}

	softwareTimerSupervisor_.tickInterruptHandler(TickClock::time_point{TickClock::duration{tickCount_}});

	return isContextSwitchRequired();
}

void Scheduler::unblock(const ThreadList::iterator iterator, const UnblockReason unblockReason)
{
	const InterruptMaskingLock interruptMaskingLock;

	unblockInternal(iterator, unblockReason);
	maybeRequestContextSwitch();
}

void Scheduler::yield()
{
	const InterruptMaskingLock interruptMaskingLock;

	runnableList_.splice(currentThreadControlBlock_);
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

	runnableList_.insert(threadControlBlock);
	threadControlBlock.setList(&runnableList_);
	threadControlBlock.setState(ThreadState::runnable);

	return 0;
}

int Scheduler::blockInternal(ThreadList& container, const ThreadList::iterator iterator, const ThreadState state,
		const UnblockFunctor* const unblockFunctor)
{
	auto& threadControlBlock = *iterator;

	if (threadControlBlock.getList() != &runnableList_)
		return EINVAL;

	container.splice(iterator);
	threadControlBlock.setList(&container);
	threadControlBlock.setState(state);
	threadControlBlock.blockHook(unblockFunctor);

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

void Scheduler::unblockInternal(const ThreadList::iterator iterator, const UnblockReason unblockReason)
{
	auto& threadControlBlock = *iterator;
	runnableList_.splice(iterator);
	threadControlBlock.setList(&runnableList_);
	threadControlBlock.setState(ThreadState::runnable);
	threadControlBlock.unblockHook(unblockReason);
}

}	// namespace internal

}	// namespace distortos
