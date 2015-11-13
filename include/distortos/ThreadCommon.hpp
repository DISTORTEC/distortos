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
 * \date 2015-11-13
 */

#ifndef INCLUDE_DISTORTOS_THREADCOMMON_HPP_
#define INCLUDE_DISTORTOS_THREADCOMMON_HPP_

#include "distortos/Thread.hpp"

namespace distortos
{

/// ThreadCommon class implements common functionality of threads
class ThreadCommon : public Thread
{
public:

	/**
	 * \brief ThreadCommon's constructor
	 *
	 * \param [in] stackStorageUniquePointer is a rvalue reference to StackStorageUniquePointer with storage for stack
	 * (\a size bytes long) and appropriate deleter
	 * \param [in] size is the size of stack's storage, bytes
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] threadGroupControlBlock is a pointer to scheduler::ThreadGroupControlBlock to which this object will
	 * be added, nullptr to inherit thread group from currently running thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 */

	ThreadCommon(StackStorageUniquePointer&& stackStorageUniquePointer, size_t size, uint8_t priority,
			SchedulingPolicy schedulingPolicy, scheduler::ThreadGroupControlBlock* threadGroupControlBlock,
			SignalsReceiver* signalsReceiver);

	/**
	 * \brief ThreadCommon's constructor
	 *
	 * \param [in] stack is an rvalue reference to architecture::Stack object which will be adopted for this thread
	 * \param [in] priority is the thread's priority, 0 - lowest, UINT8_MAX - highest
	 * \param [in] schedulingPolicy is the scheduling policy of the thread
	 * \param [in] threadGroupControlBlock is a pointer to scheduler::ThreadGroupControlBlock to which this object will
	 * be added, nullptr to inherit thread group from currently running thread
	 * \param [in] signalsReceiver is a pointer to SignalsReceiver object for this thread, nullptr to disable reception
	 * of signals for this thread
	 */

	ThreadCommon(architecture::Stack&& stack, uint8_t priority, SchedulingPolicy schedulingPolicy,
			scheduler::ThreadGroupControlBlock* threadGroupControlBlock, SignalsReceiver* signalsReceiver);

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

protected:

	/**
	 * \return reference to internal ThreadControlBlock object
	 */

	virtual scheduler::ThreadControlBlock& getThreadControlBlock() override;

	/**
	 * \return const reference to internal ThreadControlBlock object
	 */

	virtual const scheduler::ThreadControlBlock& getThreadControlBlock() const override;

private:

	/// internal ThreadControlBlock object
	scheduler::ThreadControlBlock threadControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREADCOMMON_HPP_
