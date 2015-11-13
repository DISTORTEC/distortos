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
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THREADCOMMON_HPP_
