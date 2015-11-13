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

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/synchronization/SignalsReceiverControlBlock.hpp"

#include <cerrno>

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

int ThreadCommon::generateSignal(const uint8_t signalNumber)
{
	auto& threadControlBlock = getThreadControlBlock();
	const auto signalsReceiverControlBlock = threadControlBlock.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->generateSignal(signalNumber, threadControlBlock);
}

uint8_t ThreadCommon::getEffectivePriority() const
{
	return getThreadControlBlock().getEffectivePriority();
}

SignalSet ThreadCommon::getPendingSignalSet() const
{
	const auto signalsReceiverControlBlock = getThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return SignalSet{SignalSet::empty};

	architecture::InterruptMaskingLock interruptMaskingLock;
	return signalsReceiverControlBlock->getPendingSignalSet();
}

uint8_t ThreadCommon::getPriority() const
{
	return getThreadControlBlock().getPriority();
}

SchedulingPolicy ThreadCommon::getSchedulingPolicy() const
{
	return getThreadControlBlock().getSchedulingPolicy();
}

scheduler::ThreadControlBlock::State ThreadCommon::getState() const
{
	return getThreadControlBlock().getState();
}

int ThreadCommon::queueSignal(const uint8_t signalNumber, const sigval value)
{
	auto& threadControlBlock = getThreadControlBlock();
	const auto signalsReceiverControlBlock = threadControlBlock.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->queueSignal(signalNumber, value, threadControlBlock);
}

void ThreadCommon::setPriority(const uint8_t priority, const bool alwaysBehind)
{
	getThreadControlBlock().setPriority(priority, alwaysBehind);
}

void ThreadCommon::setSchedulingPolicy(const SchedulingPolicy schedulingPolicy)
{
	getThreadControlBlock().setSchedulingPolicy(schedulingPolicy);
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
