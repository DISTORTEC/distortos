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

#include "distortos/ThreadCommon.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadCommon::ThreadCommon(StackStorageUniquePointer&& stackStorageUniquePointer, const size_t size,
		const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		scheduler::ThreadGroupControlBlock* const threadGroupControlBlock, SignalsReceiver* const signalsReceiver) :
		ThreadCommon{{std::move(stackStorageUniquePointer), size, threadRunner, *this}, priority, schedulingPolicy,
				threadGroupControlBlock, signalsReceiver}
{

}

ThreadCommon::ThreadCommon(architecture::Stack&& stack, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		scheduler::ThreadGroupControlBlock* const threadGroupControlBlock, SignalsReceiver* const signalsReceiver) :
		Thread{},
		threadControlBlock_{std::move(stack), priority, schedulingPolicy, threadGroupControlBlock, signalsReceiver,
				*this}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

scheduler::ThreadControlBlock& ThreadCommon::getThreadControlBlock()
{
	return threadControlBlock_;
}

const scheduler::ThreadControlBlock& ThreadCommon::getThreadControlBlock() const
{
	return threadControlBlock_;
}

}	// namespace distortos
