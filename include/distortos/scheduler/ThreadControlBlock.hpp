/**
 * \file
 * \brief ThreadControlBlock class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-23
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_

#include "distortos/scheduler/RoundRobinQuantum.hpp"
#include "distortos/scheduler/ThreadControlBlockList-types.hpp"

#include "distortos/architecture/Stack.hpp"

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

	/// type of object used as storage for ThreadControlBlockList elements - 3 pointers
	using Link = std::array<std::aligned_storage<sizeof(void*), alignof(void*)>::type, 3>;

	/**
	 * \brief ThreadControlBlock constructor.
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	ThreadControlBlock(void* buffer, size_t size, uint8_t priority);

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
	 * Changes priority of thread.
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

protected:

	/**
	 * \brief ThreadControlBlock constructor.
	 *
	 * This constructor is meant for MainThreadControlBlock.
	 *
	 * \param [in] stack is an rvalue reference to stack of main()
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	ThreadControlBlock(architecture::Stack&& stack, uint8_t priority);

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

	/// pointer to list that has this object
	ThreadControlBlockList* list_;

	/// iterator to the element on the list, valid only when list_ != nullptr
	ThreadControlBlockListIterator iterator_;

	/// thread's priority, 0 - lowest, UINT8_MAX - highest
	uint8_t priority_;

	/// round-robin quantum
	RoundRobinQuantum roundRobinQuantum_;

	/// current state of object
	State state_;
};

}	// namespace scheduler

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
