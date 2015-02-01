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
 * \date 2015-02-01
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_

#include "distortos/scheduler/RoundRobinQuantum.hpp"
#include "distortos/scheduler/ThreadControlBlockList-types.hpp"
#include "distortos/scheduler/MutexControlBlockList.hpp"

#include "distortos/architecture/Stack.hpp"

#include "distortos/SchedulingPolicy.hpp"

#include "distortos/estd/TypeErasedFunctor.hpp"

#include <functional>

namespace distortos
{

namespace scheduler
{

class ThreadControlBlockList;

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

	/// UnblockFunctor is a functor executed when unblocking the thread, it receives one parameter - a reference to
	/// ThreadControlBlock that is being unblocked
	using UnblockFunctor = estd::TypeErasedFunctor<void(ThreadControlBlock&)>;

	/**
	 * \brief ThreadControlBlock constructor.
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 */

	ThreadControlBlock(void* buffer, size_t size, uint8_t priority, SchedulingPolicy schedulingPolicy);

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
	 * \return const reference to internal storage for list link
	 */

	const Link& getLink() const
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
	 * \return const reference to list of mutex control blocks with enabled priority protocol owned by this thread
	 */

	const MutexControlBlockList& getOwnedProtocolMutexControlBlocksList() const
	{
		return ownedProtocolMutexControlBlocksList_;
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
	 * \return const reference to internal RoundRobinQuantum object
	 */

	const RoundRobinQuantum& getRoundRobinQuantum() const
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
	 * \return reference to internal Stack object
	 */

	architecture::Stack& getStack()
	{
		return stack_;
	}

	/**
	 * \return const reference to internal Stack object
	 */

	const architecture::Stack& getStack() const
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
	 * \return reason of previous unblocking of the thread
	 */

	UnblockReason getUnblockReason() const
	{
		return unblockReason_;
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
	 * \brief Termination hook function of thread
	 *
	 * \attention This function should be called only by Scheduler::remove().
	 */

	void terminationHook()
	{
		terminationHook_();
	}

	/**
	 * \brief Unblock hook function of thread
	 *
	 * Resets round-robin's quantum, sets unblock reason and executes unblock functor saved in blockHook().
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

protected:

	/**
	 * \brief ThreadControlBlock constructor.
	 *
	 * This constructor is meant for MainThreadControlBlock.
	 *
	 * \param [in] stack is an rvalue reference to stack of main()
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 */

	ThreadControlBlock(architecture::Stack&& stack, uint8_t priority, SchedulingPolicy schedulingPolicy);

	/**
	 * \brief ThreadControlBlock's destructor
	 *
	 * \note Polymorphic objects of ThreadControlBlock type must not be deleted via pointer/reference
	 */

	~ThreadControlBlock()
	{

	}

private:

	/**
	 * \brief Thread runner function - entry point of threads.
	 *
	 * After return from actual thread function, thread is terminated, so this function never returns.
	 *
	 * \param [in] threadControlBlock is a reference to ThreadControlBlock object that is being run
	 */

	static void threadRunner(ThreadControlBlock& threadControlBlock) __attribute__ ((noreturn));

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

	/**
	 * \brief "Run" function of thread
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void run() = 0;

	/**
	 * \brief Termination hook function of thread
	 *
	 * This function is called after run() completes.
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void terminationHook_() = 0;

	/// internal stack object
	architecture::Stack stack_;

	/// storage for list link
	Link link_;

	/// list of mutex control blocks with enabled priority protocol owned by this thread
	MutexControlBlockList ownedProtocolMutexControlBlocksList_;

	/// pointer to MutexControlBlock (with PriorityInheritance protocol) that blocks this thread
	const synchronization::MutexControlBlock* priorityInheritanceMutexControlBlock_;

	/// pointer to list that has this object
	ThreadControlBlockList* list_;

	/// iterator to the element on the list, valid only when list_ != nullptr
	ThreadControlBlockListIterator iterator_;

	/// information related to unblocking
	union
	{
		/// functor executed in unblockHook() - valid only when thread is blocked
		const UnblockFunctor* unblockFunctor_;

		/// reason of previous unblocking of the thread - valid only when thread is not blocked
		UnblockReason unblockReason_;
	};

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
