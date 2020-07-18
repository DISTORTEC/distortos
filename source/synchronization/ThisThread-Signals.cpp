/**
 * \file
 * \brief ThisThread::Signals namespace implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/ThisThread-Signals.hpp"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/internal/synchronization/SignalsReceiverControlBlock.hpp"

#include "distortos/internal/CHECK_FUNCTION_CONTEXT.hpp"

#include "distortos/InterruptMaskingLock.hpp"
#include "distortos/SignalAction.hpp"
#include "distortos/ThisThread.hpp"
#include "distortos/Thread.hpp"

#include <cerrno>

namespace distortos
{

namespace ThisThread
{

namespace Signals
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// SignalsWaitUnblockFunctor is a functor executed when unblocking a thread that is waiting for signal
class SignalsWaitUnblockFunctor : public internal::UnblockFunctor
{
public:

	/**
	 * \brief SignalsWaitUnblockFunctor's function call operator
	 *
	 * Clears pointer to set of signals that were "waited for".
	 *
	 * \param [in] threadControlBlock is a reference to ThreadControlBlock that is being unblocked
	 * \param [in] unblockReason is the reason of thread unblocking
	 */

	void operator()(internal::ThreadControlBlock& threadControlBlock, internal::UnblockReason) const override
	{
		const auto signalsReceiverControlBlock = threadControlBlock.getSignalsReceiverControlBlock();
		if (signalsReceiverControlBlock == nullptr)
			return;

		signalsReceiverControlBlock->setWaitingSignalSet(nullptr);
	}
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Implementation of distortos::ThisThread::Signals::wait(), distortos::ThisThread::Signals::tryWait() and
 * distortos::ThisThread::Signals::tryWaitUntil().
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 * \param [in] nonBlocking selects whether this function operates in blocking mode (false) or non-blocking mode (true)
 * \param [in] timePoint is a pointer to time point at which the wait for signals will be terminated, used only if
 * blocking mode is selected, nullptr to block without timeout
 *
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EAGAIN - no signal specified by \a signalSet was pending and non-blocking mode was selected;
 * - ENOTSUP - reception of signals is disabled for current thread;
 * - error codes returned by internal::Scheduler::block() (for blocking mode without timeout) /
 * internal::Scheduler::blockUntil() (for blocking mode with timeout);
 * - error codes returned by internal::SignalsReceiverControlBlock::acceptPendingSignal();
 */

std::pair<int, SignalInformation> waitImplementation(const SignalSet& signalSet, const bool nonBlocking,
		const TickClock::time_point* const timePoint)
{
	CHECK_FUNCTION_CONTEXT();

	auto& scheduler = internal::getScheduler();
	const auto signalsReceiverControlBlock = scheduler.getCurrentThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return {ENOTSUP, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};

	const InterruptMaskingLock interruptMaskingLock;

	const auto bitset = signalSet.getBitset();
	auto pendingSignalSet = signalsReceiverControlBlock->getPendingSignalSet();
	auto pendingBitset = pendingSignalSet.getBitset();
	auto intersection = bitset & pendingBitset;

	if (intersection.none() == true)	// none of desired signals is pending, so current thread must be blocked
	{
		if (nonBlocking == true)
			return {EAGAIN, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};

		internal::ThreadList waitingList;

		signalsReceiverControlBlock->setWaitingSignalSet(&signalSet);
		const SignalsWaitUnblockFunctor signalsWaitUnblockFunctor;
		const auto ret = timePoint == nullptr ?
				scheduler.block(waitingList, ThreadState::waitingForSignal, &signalsWaitUnblockFunctor) :
				scheduler.blockUntil(waitingList, ThreadState::waitingForSignal, *timePoint,
						&signalsWaitUnblockFunctor);
		if (ret != 0)
			return {ret, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};

		pendingSignalSet = signalsReceiverControlBlock->getPendingSignalSet();
		pendingBitset = pendingSignalSet.getBitset();
		intersection = bitset & pendingBitset;
	}

	const auto intersectionValue = intersection.to_ulong();
	static_assert(sizeof(intersectionValue) == intersection.size() / 8,
			"Size of intersectionValue doesn't match size of intersection!");
	// GCC builtin - "find first set" - https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
	const auto signalNumber = __builtin_ffsl(intersectionValue) - 1;
	return signalsReceiverControlBlock->acceptPendingSignal(signalNumber);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int generateSignal(const uint8_t signalNumber)
{
	return ThisThread::get().generateSignal(signalNumber);
}

SignalSet getPendingSignalSet()
{
	return ThisThread::get().getPendingSignalSet();
}

std::pair<int, SignalAction> getSignalAction(const uint8_t signalNumber)
{
	CHECK_FUNCTION_CONTEXT();

	const auto signalsReceiverControlBlock =
			internal::getScheduler().getCurrentThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return {ENOTSUP, {}};

	const InterruptMaskingLock interruptMaskingLock;
	return signalsReceiverControlBlock->getSignalAction(signalNumber);
}

SignalSet getSignalMask()
{
	CHECK_FUNCTION_CONTEXT();

	const auto signalsReceiverControlBlock =
			internal::getScheduler().getCurrentThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return SignalSet{SignalSet::full};

	return signalsReceiverControlBlock->getSignalMask();
}

int queueSignal(const uint8_t signalNumber, const sigval value)
{
	return ThisThread::get().queueSignal(signalNumber, value);
}

std::pair<int, SignalAction> setSignalAction(const uint8_t signalNumber, const SignalAction& signalAction)
{
	CHECK_FUNCTION_CONTEXT();

	const auto signalsReceiverControlBlock =
			internal::getScheduler().getCurrentThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return {ENOTSUP, {}};

	const InterruptMaskingLock interruptMaskingLock;
	return signalsReceiverControlBlock->setSignalAction(signalNumber, signalAction);
}

int setSignalMask(const SignalSet signalMask)
{
	CHECK_FUNCTION_CONTEXT();

	const auto signalsReceiverControlBlock =
			internal::getScheduler().getCurrentThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return ENOTSUP;

	return signalsReceiverControlBlock->setSignalMask(signalMask, true);
}

std::pair<int, SignalInformation> tryWait(const SignalSet& signalSet)
{
	return waitImplementation(signalSet, true, nullptr);	// non-blocking mode
}

std::pair<int, SignalInformation> tryWaitFor(const SignalSet& signalSet, const TickClock::duration duration)
{
	return tryWaitUntil(signalSet, TickClock::now() + duration + TickClock::duration{1});
}

std::pair<int, SignalInformation> tryWaitUntil(const SignalSet& signalSet, const TickClock::time_point timePoint)
{
	return waitImplementation(signalSet, false, &timePoint);	// blocking mode, with timeout
}

std::pair<int, SignalInformation> wait(const SignalSet& signalSet)
{
	return waitImplementation(signalSet, false, nullptr);	// blocking mode, no timeout
}

}	// namespace Signals

}	// namespace ThisThread

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1
