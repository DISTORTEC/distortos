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
 * \date 2014-08-24
 */

#ifndef INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SCHEDULER_THREADCONTROLBLOCK_HPP_

#include "distortos/scheduler/RoundRobinQuantum.hpp"
#include "distortos/scheduler/threadRunner.hpp"

#include "distortos/architecture/Stack.hpp"

#include <cstdint>

namespace distortos
{

namespace scheduler
{

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

	/**
	 * \brief ThreadControlBlock constructor.
	 *
	 * \param [in] buffer is a pointer to stack's buffer
	 * \param [in] size is the size of stack's buffer, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 */

	ThreadControlBlock(void* buffer, size_t size, uint8_t priority);

	/// \return reference to internal Stack object
	architecture::Stack& getStack() { return stack_; }

	/// \return const reference to internal Stack object
	const architecture::Stack& getStack() const { return stack_; }

	/// \return priority of ThreadControlBlock
	uint8_t getPriority() const { return priority_; }

	/// \return reference to internal RoundRobinQuantum object
	RoundRobinQuantum& getRoundRobinQuantum() { return roundRobinQuantum_; }

	/// \return const reference to internal RoundRobinQuantum object
	const RoundRobinQuantum& getRoundRobinQuantum() const { return roundRobinQuantum_; }

	/// \return current state of object
	State getState() const { return state_; }

	/// \param [in] state is the new state of object
	void setState(const State state) { state_ = state; }

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
	 * \brief ThreadControlBlock's destructor
	 *
	 * \note Polymorphic objects of ThreadControlBlock type must not be deleted via pointer/reference
	 */

	~ThreadControlBlock()
	{

	}

private:

	friend void threadRunner(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief "Run" function of thread
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void run_() = 0;

	/**
	 * \brief Termination hook function of thread
	 *
	 * This function is called after run_() completes.
	 *
	 * This should be overridden by derived classes.
	 */

	virtual void terminationHook_() = 0;

	/// internal stack object
	architecture::Stack stack_;

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
