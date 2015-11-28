/**
 * \file
 * \brief ThreadCommon class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-28
 */

#ifndef INCLUDE_DISTORTOS_THREADCOMMON_HPP_
#define INCLUDE_DISTORTOS_THREADCOMMON_HPP_

#include "distortos/Semaphore.hpp"
#include "distortos/Thread.hpp"

namespace distortos
{

/**
 * \brief ThreadCommon class implements common functionality of threads
 *
 * \ingroup threads
 */

class ThreadCommon : public Thread
{
public:

	/// unique_ptr (with deleter) to storage for stack
	using StackStorageUniquePointer = architecture::Stack::StorageUniquePointer;

	/**
	 * \brief ThreadCommon's constructor
	 *
	 * \param [in] stackStorageUniquePointer is a rvalue reference to StackStorageUniquePointer with storage for stack
	 * (\a size bytes long) and appropriate deleter
	 * \param [in] size is the size of stack's storage, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] threadGroupControlBlock is a pointer to internal::ThreadGroupControlBlock to which this object will
	 * be added, nullptr to inherit thread group from currently running thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 */

	ThreadCommon(StackStorageUniquePointer&& stackStorageUniquePointer, size_t size, uint8_t priority,
			SchedulingPolicy schedulingPolicy, internal::ThreadGroupControlBlock* threadGroupControlBlock,
			SignalsReceiver* signalsReceiver);

	/**
	 * \brief ThreadCommon's constructor
	 *
	 * \param [in] stack is an rvalue reference to architecture::Stack object which will be adopted for this thread
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] threadGroupControlBlock is a pointer to internal::ThreadGroupControlBlock to which this object will
	 * be added, nullptr to inherit thread group from currently running thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 */

	ThreadCommon(architecture::Stack&& stack, uint8_t priority, SchedulingPolicy schedulingPolicy,
			internal::ThreadGroupControlBlock* threadGroupControlBlock, SignalsReceiver* signalsReceiver);

	/**
	 * \brief ThreadCommon's destructor
	 */

	virtual ~ThreadCommon() override;

	/**
	 * \brief Generates signal for thread.
	 *
	 * Similar to pthread_kill() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_kill.html
	 *
	 * Adds the signalNumber to set of pending signals. If this thread is currently waiting for this signal, it will be
	 * unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be generated, [0; 31]
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 * - ENOTSUP - reception of signals is disabled for this thread;
	 */

	virtual int generateSignal(uint8_t signalNumber) override;

	/**
	 * \return effective priority of thread
	 */

	virtual uint8_t getEffectivePriority() const override;

	/**
	 * \brief Gets set of currently pending signals.
	 *
	 * Similar to sigpending() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigpending.html
	 *
	 * This function shall return the set of signals that are blocked from delivery and are pending on the thread.
	 *
	 * \return set of currently pending signals
	 */

	virtual SignalSet getPendingSignalSet() const override;

	/**
	 * \return priority of thread
	 */

	virtual uint8_t getPriority() const override;

	/**
	 * \return scheduling policy of the thread
	 */

	virtual SchedulingPolicy getSchedulingPolicy() const override;

	/**
	 * \return current state of thread
	 */

	virtual ThreadState getState() const override;

	/**
	 * \brief Waits for thread termination.
	 *
	 * Similar to std::thread::join() - http://en.cppreference.com/w/cpp/thread/thread/join
	 * Similar to POSIX pthread_join() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
	 *
	 * Blocks current thread until this thread finishes its execution. The results of multiple simultaneous calls to
	 * join() on the same target thread are undefined.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EDEADLK - deadlock condition was detected,
	 * - EINVAL - this thread is not joinable,
	 * - ...
	 */

	virtual int join() override;

	/**
	 * \brief Queues signal for thread.
	 *
	 * Similar to sigqueue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigqueue.html
	 *
	 * Adds the signalNumber and signal value (sigval union) to queue of SignalInformation objects. If this thread is
	 * currently waiting for this signal, it will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be queued, [0; 31]
	 * \param [in] value is the signal value
	 *
	 * \return 0 on success, error code otherwise:
	 * - EAGAIN - no resources are available to queue the signal, maximal number of signals is already queued in
	 * associated queue of SignalInformation objects;
	 * - EINVAL - \a signalNumber value is invalid;
	 * - ENOTSUP - reception or queuing of signals are disabled for this thread;
	 */

	virtual int queueSignal(uint8_t signalNumber, sigval value) override;

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

	virtual void setPriority(uint8_t priority, bool alwaysBehind = {}) override;

	/**
	 * param [in] schedulingPolicy is the new scheduling policy of the thread
	 */

	virtual void setSchedulingPolicy(SchedulingPolicy schedulingPolicy) override;

	/**
	 * \brief Starts the thread.
	 *
	 * This operation can be performed on threads in "New" state only.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - thread is already started;
	 * - error codes returned by internal::Scheduler::add();
	 */

	virtual int start() override;

	ThreadCommon(const ThreadCommon&) = delete;
	ThreadCommon(ThreadCommon&&) = default;
	const ThreadCommon& operator=(const ThreadCommon&) = delete;
	ThreadCommon& operator=(ThreadCommon&&) = delete;

protected:

	/**
	 * \return reference to internal ThreadControlBlock object
	 */

	internal::ThreadControlBlock& getThreadControlBlock()
	{
		return threadControlBlock_;
	}

	/**
	 * \return const reference to internal ThreadControlBlock object
	 */

	const internal::ThreadControlBlock& getThreadControlBlock() const
	{
		return threadControlBlock_;
	}

private:

	/**
	 * \brief Termination hook function of thread
	 *
	 * This function is called after run() completes, from Scheduler::remove().
	 */

	virtual void terminationHook() override;

	/// internal ThreadControlBlock object
	internal::ThreadControlBlock threadControlBlock_;

	/// semaphore used by join()
	Semaphore joinSemaphore_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREADCOMMON_HPP_
