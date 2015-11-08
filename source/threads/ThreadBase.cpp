/**
 * \file
 * \brief ThreadBase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-08
 */

#include "distortos/ThreadBase.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/synchronization/SignalsReceiverControlBlock.hpp"

#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadBase::ThreadBase(StackStorageUniquePointer&& stackStorageUniquePointer, const size_t size, const uint8_t priority,
		const SchedulingPolicy schedulingPolicy, scheduler::ThreadGroupControlBlock* const threadGroupControlBlock,
		SignalsReceiver* const signalsReceiver) :
		ThreadBase{{std::move(stackStorageUniquePointer), size, threadRunner, *this}, priority, schedulingPolicy,
				threadGroupControlBlock, signalsReceiver}
{

}

ThreadBase::ThreadBase(architecture::Stack&& stack, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		scheduler::ThreadGroupControlBlock* const threadGroupControlBlock, SignalsReceiver* const signalsReceiver) :
		threadControlBlock_{std::move(stack), priority, schedulingPolicy, threadGroupControlBlock, signalsReceiver,
				*this},
		joinSemaphore_{0}
{

}

int ThreadBase::generateSignal(const uint8_t signalNumber)
{
	const auto signalsReceiverControlBlock = threadControlBlock_.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->generateSignal(signalNumber, threadControlBlock_);
}

SignalSet ThreadBase::getPendingSignalSet() const
{
	const auto signalsReceiverControlBlock = threadControlBlock_.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return SignalSet{SignalSet::empty};

	architecture::InterruptMaskingLock interruptMaskingLock;
	return signalsReceiverControlBlock->getPendingSignalSet();
}

int ThreadBase::join()
{
	if (&threadControlBlock_ == &scheduler::getScheduler().getCurrentThreadControlBlock())
		return EDEADLK;

	int ret;
	while ((ret = joinSemaphore_.wait()) == EINTR);
	return ret;
}

int ThreadBase::queueSignal(const uint8_t signalNumber, const sigval value)
{
	const auto signalsReceiverControlBlock = threadControlBlock_.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->queueSignal(signalNumber, value, threadControlBlock_);
}

int ThreadBase::start()
{
	if (getState() != scheduler::ThreadControlBlock::State::New)
		return EINVAL;

	return scheduler::getScheduler().add(threadControlBlock_);
}

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadBase::~ThreadBase()
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private static functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadBase::threadRunner(ThreadBase& threadBase)
{
	threadBase.run();
	scheduler::getScheduler().remove(&ThreadBase::terminationHook);

	while (1);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void ThreadBase::terminationHook()
{
	joinSemaphore_.post();
	threadControlBlock_.setList(nullptr);
}

}	// namespace distortos
