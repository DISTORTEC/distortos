/**
 * \file
 * \brief ThreadControlBlock class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-06-10
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_

#include "distortos/scheduler/RoundRobinQuantum.hpp"
#include "distortos/scheduler/ThreadControlBlockList-types.hpp"
#include "distortos/scheduler/MutexControlBlockList.hpp"

#include "distortos/architecture/Stack.hpp"

#include "distortos/SchedulingPolicy.hpp"

#include "distortos/estd/TypeErasedFunctor.hpp"

namespace distortos
{

class SignalsReceiver;

namespace synchronization
{

class SignalsReceiverControlBlock;

}	// namespace synchronization

namespace scheduler
{

class ThreadControlBlockList;
class ThreadGroupControlBlock;

/// ThreadControlBlock class is a simple description of a Thread
class ThreadControlBlock
{
public:

	/// state of the thread
	enum class State : uint8_t
	{
		/// state in which thread is created, before being added to Scheduler
		New,
		/// thread is runnable
		Runnable,
		/// thread is sleeping
		Sleeping,
		/// thread is blocked on Semaphore
		BlockedOnSemaphore,
		/// thread is suspended
		Suspended,
		/// thread is terminated
		Terminated,
		/// thread is blocked on Mutex
		BlockedOnMutex,
		/// thread is blocked on ConditionVariable
		BlockedOnConditionVariable,
		/// thread is waiting for signal
		WaitingForSignal,
	};

	/// reason of thread unblocking
	enum class UnblockReason : uint8_t
	{
		/// explicit request to unblock the thread - normal unblock
		UnblockRequest,
		/// timeout - unblock via software timer
		Timeout,
	};

	/// type of object used as storage for ThreadControlBlockList elements - 3 pointers
	using Link = std::array<std::aligned_storage<sizeof(void*), alignof(void*)>::type, 3>;

	/// UnblockFunctor is a functor executed when unblocking the thread, it receives two parameter - a reference to
	/// ThreadControlBlock that is being unblocked and the reason of thread unblocking
	class UnblockFunctor : public estd::TypeErasedFunctor<void(ThreadControlBlock&, UnblockReason)>
	{

	};

	/**
	 * \brief ThreadControlBlock constructor.
	 *
	 * \param [in] stack is an rvalue reference to architecture::Stack object which will be adopted for this thread
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] threadGroupControlBlock is a pointer to scheduler::ThreadGroupControlBlock to which this object will
	 * be added, nullptr to inherit thread group from currently running thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 * \param [in] owner is a reference to ThreadBase object that owns this ThreadControlBlock
	 */

	ThreadControlBlock(architecture::Stack&& stack, uint8_t priority, SchedulingPolicy schedulingPolicy,
			ThreadGroupControlBlock* threadGroupControlBlock, SignalsReceiver* signalsReceiver, ThreadBase& owner);

	/**
	 * \brief ThreadControlBlock's destructor
	 */

	~ThreadControlBlock();

	/**
	 * \brief Hook function executed when thread is added to scheduler.
	 *
	 * If threadGroupControlBlock_ is nullptr, it is inherited from currently running thread. Then this object is added
	 * to the thread group (if it is valid).
	 *
	 * \attention This function should be called only by Scheduler::addInternal().
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - inherited thread group is invalid;
	 */

	int addHook();

	/**
	 * \brief Block hook function of thread
	 *
	 * Saves pointer to UnblockFunctor.
	 *
	 * \attention This function should be called only by Scheduler::blockInternal().
	 *
	 * \param [in] unblockFunctor is a pointer to UnblockFunctor which will be executed in unblockHook()
	 */

	void blockHook(const UnblockFunctor* const unblockFunctor)
	{
		unblockFunctor_ = unblockFunctor;
	}

	/**
	 * \return effective priority of ThreadControlBlock
	 */

	uint8_t getEffectivePriority() const
	{
		return std::max(priority_, boostedPriority_);
	}

	/**
	 * \return iterator to the element on the list, valid only when list_ != nullptr
	 */

	ThreadControlBlockListIterator getIterator() const
	{
		return iterator_;
	}

	/**
	 * \return reference to internal storage for list link
	 */

	Link& getLink()
	{
		return link_;
	}

	/**
	 * \return pointer to list that has this object
	 */

	ThreadControlBlockList* getList() const
	{
		return list_;
	}

	/**
	 * \return reference to list of mutex control blocks with enabled priority protocol owned by this thread
	 */

	MutexControlBlockList& getOwnedProtocolMutexControlBlocksList()
	{
		return ownedProtocolMutexControlBlocksList_;
	}

	/**
	 * \return reference to ThreadBase object that owns this ThreadControlBlock
	 */

	ThreadBase& getOwner() const
	{
		return owner_;
	}

	/**
	 * \return priority of ThreadControlBlock
	 */

	uint8_t getPriority() const
	{
		return priority_;
	}

	/**
	 * \return reference to internal RoundRobinQuantum object
	 */

	RoundRobinQuantum& getRoundRobinQuantum()
	{
		return roundRobinQuantum_;
	}

	/**
	 * \return scheduling policy of the thread
	 */

	SchedulingPolicy getSchedulingPolicy() const
	{
		return schedulingPolicy_;
	}

	/**
	 * \return pointer to synchronization::SignalsReceiverControlBlock object for this thread, nullptr if this thread
	 * cannot receive signals
	 */

	synchronization::SignalsReceiverControlBlock* getSignalsReceiverControlBlock() const
	{
		return signalsReceiverControlBlock_;
	}

	/**
	 * \return reference to internal Stack object
	 */

	architecture::Stack& getStack()
	{
		return stack_;
	}

	/**
	 * \return current state of object
	 */

	State getState() const
	{
		return state_;
	}

	/**
	 * \return reference to internal storage for thread group list link
	 */

	Link& getThreadGroupLink()
	{
		return threadGroupLink_;
	}

	/**
	 * \brief Sets the iterator to the element on the list.
	 *
	 * \param [in] iterator is an iterator to the element on the list
	 */

	void setIterator(const ThreadControlBlockListIterator iterator)
	{
		iterator_ = iterator;
	}

	/**
	 * \brief Sets the list that has this object.
	 *
	 * \param [in] list is a pointer to list that has this object
	 */

	void setList(ThreadControlBlockList* const list)
	{
		list_ = list;
	}

	/**
	 * \brief Changes priority of thread.
	 *
	 * If the priority really changes, the position in the thread list is adjusted and context switch may be requested.
	 *
	 * \param [in] priority is the new priority of thread
	 * \param [in] alwaysBehind selects the method of ordering when lowering the priority
	 * - false - the thread is moved to the head of the group of threads with the new priority (default),
	 * - true - the thread is moved to the tail of the group of threads with the new priority.
	 */

	void setPriority(uint8_t priority, bool alwaysBehind = {});

	/**
	 * \param [in] priorityInheritanceMutexControlBlock is a pointer to MutexControlBlock (with PriorityInheritance
	 * protocol) that blocks this thread
	 */

	void setPriorityInheritanceMutexControlBlock(const synchronization::MutexControlBlock* const
			priorityInheritanceMutexControlBlock)
	{
		priorityInheritanceMutexControlBlock_ = priorityInheritanceMutexControlBlock;
	}

	/**
	 * param [in] schedulingPolicy is the new scheduling policy of the thread
	 */

	void setSchedulingPolicy(SchedulingPolicy schedulingPolicy);

	/**
	 * \param [in] state is the new state of object
	 */

	void setState(const State state)
	{
		state_ = state;
	}

	/**
	 * \brief Hook function called when context is switched to this thread.
	 *
	 * Sets global _impure_ptr (from newlib) to thread's \a reent_ member variable.
	 *
	 * \attention This function should be called only by Scheduler::switchContext().
	 */

	void switchedToHook()
	{
		_impure_ptr = &reent_;
	}

	/**
	 * \brief Unblock hook function of thread
	 *
	 * Resets round-robin's quantum and executes unblock functor saved in blockHook().
	 *
	 * \attention This function should be called only by Scheduler::unblockInternal().
	 *
	 * \param [in] unblockReason is the new reason of unblocking of the thread
	 */

	void unblockHook(UnblockReason unblockReason);

	/**
	 * \brief Updates boosted priority of the thread.
	 *
	 * This function should be called after all operations involving this thread and a mutex with enabled priority
	 * protocol.
	 *
	 * \param [in] boostedPriority is the initial boosted priority, this should be effective priority of the thread that
	 * is about to be blocked on a mutex owned by this thread, default - 0
	 */

	void updateBoostedPriority(uint8_t boostedPriority = {});

	ThreadControlBlock(const ThreadControlBlock&) = delete;
	ThreadControlBlock(ThreadControlBlock&&) = default;
	const ThreadControlBlock& operator=(const ThreadControlBlock&) = delete;
	ThreadControlBlock& operator=(ThreadControlBlock&&) = delete;

private:

	/**
	 * \brief Repositions the thread on the list it's currently on.
	 *
	 * This function should be called when thread's effective priority changes.
	 *
	 * \attention list_ must not be nullptr
	 *
	 * \param [in] loweringBefore selects the method of ordering when lowering the priority (it must be false when the
	 * priority is raised!):
	 * - true - the thread is moved to the head of the group of threads with the new priority, this is accomplished by
	 * temporarily boosting effective priority by 1,
	 * - false - the thread is moved to the tail of the group of threads with the new priority.
	 */

	void reposition(bool loweringBefore);

	/// internal stack object
	architecture::Stack stack_;

	/// storage for list link
	Link link_;

	/// storage for thread group list link
	Link threadGroupLink_;

	/// reference to ThreadBase object that owns this ThreadControlBlock
	ThreadBase& owner_;

	/// list of mutex control blocks with enabled priority protocol owned by this thread
	MutexControlBlockList ownedProtocolMutexControlBlocksList_;

	/// pointer to MutexControlBlock (with PriorityInheritance protocol) that blocks this thread
	const synchronization::MutexControlBlock* priorityInheritanceMutexControlBlock_;

	/// pointer to list that has this object
	ThreadControlBlockList* list_;

	/// iterator to the element on the list, valid only when list_ != nullptr
	ThreadControlBlockListIterator iterator_;

	/// pointer to ThreadGroupControlBlock with which this object is associated
	ThreadGroupControlBlock* threadGroupControlBlock_;

	/// pointer to ThreadGroupControlBlock's list that has this object
	ThreadControlBlockUnsortedList* threadGroupList_;

	/// iterator to the element on the ThreadGroupControlBlock's list, valid only when threadGroupList_ != nullptr
	ThreadControlBlockListIterator threadGroupIterator_;

	/// functor executed in unblockHook()
	const UnblockFunctor* unblockFunctor_;

	/// pointer to synchronization::SignalsReceiverControlBlock object for this thread, nullptr if this thread cannot
	/// receive signals
	synchronization::SignalsReceiverControlBlock* signalsReceiverControlBlock_;

	/// newlib's _reent structure with thread-specific data
	_reent reent_;

	/// thread's priority, 0 - lowest, UINT8_MAX - highest
	uint8_t priority_;

	/// thread's boosted priority, 0 - no boosting
	uint8_t boostedPriority_;

	/// round-robin quantum
	RoundRobinQuantum roundRobinQuantum_;

	/// scheduling policy of the thread
	SchedulingPolicy schedulingPolicy_;

	/// current state of object
	State state_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
