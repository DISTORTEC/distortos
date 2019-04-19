/**
 * \file
 * \brief ThreadCommon class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/ThreadCommon.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/internal/synchronization/SignalsReceiverControlBlock.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/InterruptMaskingLock.hpp"
#include "distortos/ThreadIdentifier.hpp"

#include <cerrno>

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

ThreadCommon::ThreadCommon(Stack&& stack, const uint8_t priority, const SchedulingPolicy schedulingPolicy,
		ThreadGroupControlBlock* const threadGroupControlBlock, SignalsReceiver* const signalsReceiver) :
				threadControlBlock_{std::move(stack), priority, schedulingPolicy, threadGroupControlBlock,
						signalsReceiver, *this},
				joinSemaphore_{0}
{

}

ThreadCommon::~ThreadCommon()
{

}

#if DISTORTOS_SIGNALS_ENABLE == 1

int ThreadCommon::generateSignal(const uint8_t signalNumber)
{
	auto& threadControlBlock = getThreadControlBlock();
	const auto signalsReceiverControlBlock = threadControlBlock.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->generateSignal(signalNumber, threadControlBlock);
}

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

uint8_t ThreadCommon::getEffectivePriority() const
{
	return getThreadControlBlock().getEffectivePriority();
}

ThreadIdentifier ThreadCommon::getIdentifier() const
{
	const auto& threadControlBlock = getThreadControlBlock();
	return {threadControlBlock, threadControlBlock.getSequenceNumber()};
}

#if DISTORTOS_SIGNALS_ENABLE == 1

SignalSet ThreadCommon::getPendingSignalSet() const
{
	const auto signalsReceiverControlBlock = getThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return SignalSet{SignalSet::empty};

	const InterruptMaskingLock interruptMaskingLock;
	return signalsReceiverControlBlock->getPendingSignalSet();
}

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

uint8_t ThreadCommon::getPriority() const
{
	return getThreadControlBlock().getPriority();
}

SchedulingPolicy ThreadCommon::getSchedulingPolicy() const
{
	return getThreadControlBlock().getSchedulingPolicy();
}

size_t ThreadCommon::getStackHighWaterMark() const
{
	return getThreadControlBlock().getStack().getHighWaterMark();
}

size_t ThreadCommon::getStackSize() const
{
	return getThreadControlBlock().getStack().getSize();
}

ThreadState ThreadCommon::getState() const
{
	return getThreadControlBlock().getState();
}

int ThreadCommon::join()
{
	CHECK_FUNCTION_CONTEXT();

	if (&getThreadControlBlock() == &getScheduler().getCurrentThreadControlBlock())
		return EDEADLK;

	int ret;
	while ((ret = joinSemaphore_.wait()) == EINTR);
	return ret;
}

#if DISTORTOS_SIGNALS_ENABLE == 1

int ThreadCommon::queueSignal(const uint8_t signalNumber, const sigval value)
{
	auto& threadControlBlock = getThreadControlBlock();
	const auto signalsReceiverControlBlock = threadControlBlock.getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->queueSignal(signalNumber, value, threadControlBlock);
}

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

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

void ThreadCommon::exit0Hook()
{

}

void ThreadCommon::exit1Hook()
{
	joinSemaphore_.post();
}

int ThreadCommon::startInternal()
{
	return getScheduler().add(getThreadControlBlock());
}

}	// namespace internal

}	// namespace distortos
