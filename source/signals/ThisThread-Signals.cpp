/**
 * \file
 * \brief ThisThread::Signals namespace implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-21
 */

#include "distortos/ThisThread-Signals.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

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
class SignalsWaitUnblockFunctor : public scheduler::ThreadControlBlock::UnblockFunctor
{
public:

	/**
	 * \brief SignalsWaitUnblockFunctor's constructor
	 *
	 * \param [in] pendingSignalSet is a reference to SignalSet that will be used to return saved pending signal set
	 */

	constexpr explicit SignalsWaitUnblockFunctor(SignalSet& pendingSignalSet) :
			pendingSignalSet_(pendingSignalSet)
	{

	}

	/**
	 * \brief SignalsWaitUnblockFunctor's function call operator
	 *
	 * Saves pending signal set of unblocked thread.
	 *
	 * \param [in] threadControlBlock is a reference to ThreadControlBlock that is being unblocked
	 */

	void operator()(scheduler::ThreadControlBlock& threadControlBlock) const override
	{
		pendingSignalSet_ = threadControlBlock.getPendingSignalSet();
	}

private:

	/// reference to SignalSet that will be used to return saved pending signal set
	SignalSet& pendingSignalSet_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, uint8_t> wait(const SignalSet& signalSet)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	auto& scheduler = scheduler::getScheduler();
	auto& currentThreadControlBlock = scheduler.getCurrentThreadControlBlock();

	const auto bitset = signalSet.getBitset();
	auto pendingSignalSet = currentThreadControlBlock.getPendingSignalSet();
	auto pendingBitset = pendingSignalSet.getBitset();
	auto intersection = bitset & pendingBitset;

	if (intersection.none() == true)	// none of desired signals is pending, so current thread must be blocked
	{
		scheduler::ThreadControlBlockList waitingList {scheduler.getThreadControlBlockListAllocator(),
				scheduler::ThreadControlBlock::State::WaitingForSignal};

		currentThreadControlBlock.setWaitingSignalSet(&signalSet);
		const SignalsWaitUnblockFunctor signalsWaitUnblockFunctor {pendingSignalSet};
		const auto ret = scheduler.block(waitingList, &signalsWaitUnblockFunctor);
		if (ret != 0)
			return {ret, {}};

		pendingBitset = pendingSignalSet.getBitset();
		intersection = bitset & pendingBitset;
	}

	const auto intersectionValue = intersection.to_ulong();
	// GCC builtin - "find first set" - https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
	const auto signalNumber = __builtin_ffsl(intersectionValue) - 1;
	const auto ret = currentThreadControlBlock.acceptPendingSignal(signalNumber);
	return {ret, signalNumber};
}

}	// namespace Signals

}	// namespace ThisThread

}	// namespace distortos
