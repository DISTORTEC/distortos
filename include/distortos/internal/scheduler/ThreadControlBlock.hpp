/**
 * \file
 * \brief ThreadControlBlock class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_

#include "distortos/internal/scheduler/RoundRobinQuantum.hpp"
#include "distortos/internal/scheduler/ThreadListNode.hpp"

#include "distortos/internal/synchronization/MutexList.hpp"

#include "distortos/architecture/Stack.hpp"

#include "distortos/SchedulingPolicy.hpp"
#include "distortos/ThreadState.hpp"

#include "estd/TypeErasedFunctor.hpp"

namespace distortos
{

class SignalsReceiver;

namespace internal
{

class SignalsReceiverControlBlock;
class ThreadList;
class ThreadGroupControlBlock;

/// ThreadControlBlock class is a simple description of a Thread
class ThreadControlBlock : public ThreadListNode
{
public:

	/// reason of thread unblocking
	enum class UnblockReason : uint8_t
	{
		/// explicit request to unblock the thread - normal unblock
		unblockRequest,
		/// timeout - unblock via software timer
		timeout,
		/// signal handler - unblock to deliver unmasked signal
		signal,
	};

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
	 * \param [in] threadGroupControlBlock is a pointer to ThreadGroupControlBlock to which this object will be added,
	 * nullptr to inherit thread group from currently running thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 * \param [in] owner is a reference to Thread object that owns this ThreadControlBlock
	 */

	ThreadControlBlock(architecture::Stack&& stack, uint8_t priority, SchedulingPolicy schedulingPolicy,
			ThreadGroupControlBlock* threadGroupControlBlock, SignalsReceiver* signalsReceiver, Thread& owner);

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
	 * \return pointer to list that has this object
	 */

	ThreadList* getList() const
	{
		return list_;
	}

	/**
	 * \return reference to list of mutexes (mutex control blocks) with enabled priority protocol owned by this thread
	 */

	MutexList& getOwnedProtocolMutexList()
	{
		return ownedProtocolMutexList_;
	}

	/**
	 * \return reference to Thread object that owns this ThreadControlBlock
	 */

	Thread& getOwner() const
	{
		return owner_;
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
	 * \return pointer to SignalsReceiverControlBlock object for this thread, nullptr if this thread cannot receive
	 * signals
	 */

	SignalsReceiverControlBlock* getSignalsReceiverControlBlock() const
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

	ThreadState getState() const
	{
		return state_;
	}

	/**
	 * \brief Sets the list that has this object.
	 *
	 * \param [in] list is a pointer to list that has this object
	 */

	void setList(ThreadList* const list)
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

	void setPriorityInheritanceMutexControlBlock(const MutexControlBlock* const priorityInheritanceMutexControlBlock)
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

	void setState(const ThreadState state)
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

	/// reference to Thread object that owns this ThreadControlBlock
	Thread& owner_;

	/// list of mutexes (mutex control blocks) with enabled priority protocol owned by this thread
	MutexList ownedProtocolMutexList_;

	/// pointer to MutexControlBlock (with PriorityInheritance protocol) that blocks this thread
	const MutexControlBlock* priorityInheritanceMutexControlBlock_;

	/// pointer to list that has this object
	ThreadList* list_;

	/// pointer to ThreadGroupControlBlock with which this object is associated
	ThreadGroupControlBlock* threadGroupControlBlock_;

	/// functor executed in unblockHook()
	const UnblockFunctor* unblockFunctor_;

	/// pointer to SignalsReceiverControlBlock object for this thread, nullptr if this thread cannot receive signals
	SignalsReceiverControlBlock* signalsReceiverControlBlock_;

	/// newlib's _reent structure with thread-specific data
	_reent reent_;

	/// round-robin quantum
	RoundRobinQuantum roundRobinQuantum_;

	/// scheduling policy of the thread
	SchedulingPolicy schedulingPolicy_;

	/// current state of object
	ThreadState state_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_THREADCONTROLBLOCK_HPP_
