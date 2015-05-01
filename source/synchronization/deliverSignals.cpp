/**
 * \file
 * \brief deliverSignals() definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-01
 */

#include "distortos/synchronization/deliverSignals.hpp"

#include "distortos/synchronization/SignalsReceiverControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/SignalAction.hpp"
#include "distortos/SignalInformation.hpp"

#include <cerrno>

namespace distortos
{

namespace synchronization
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Accepts first pending and unblocked signal.
 *
 * \param [in] signalsReceiverControlBlock is a reference to SignalsReceiverControlBlock associated with current thread
 * \param [in] signalMask is the signal mask of current thread
 *
 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted signal;
 * error codes:
 * - EAGAIN - no unblocked signal was pending;
 * - error codes returned by SignalsReceiverControlBlock::acceptPendingSignal();
 */

std::pair<int, SignalInformation> acceptPendingUnblockedSignal(SignalsReceiverControlBlock& signalsReceiverControlBlock,
		const SignalSet signalMask)
{
	const auto pendingSignalSet = signalsReceiverControlBlock.getPendingSignalSet();
	const auto pendingUnblockedBitset = pendingSignalSet.getBitset() & ~signalMask.getBitset();
	if (pendingUnblockedBitset.none() == true)	// no pending & unblocked signals?
		return {EAGAIN, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};

	const auto pendingUnblockedValue = pendingUnblockedBitset.to_ulong();
	static_assert(sizeof(pendingUnblockedValue) == pendingUnblockedBitset.size() / 8,
			"Size of pendingUnblockedValue doesn't match size of pendingUnblockedBitset!");
	// GCC builtin - "find first set" - https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
	const auto signalNumber = __builtin_ffsl(pendingUnblockedValue) - 1;
	return signalsReceiverControlBlock.acceptPendingSignal(signalNumber);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void deliverSignals()
{
	const auto signalsReceiverControlBlock =
			scheduler::getScheduler().getCurrentThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return;	/// \todo error handling?

	const auto signalMask = signalsReceiverControlBlock->getSignalMask();

	int ret;
	SignalInformation signalInformation {uint8_t{}, SignalInformation::Code{}, sigval{}};
	while (std::tie(ret, signalInformation) = acceptPendingUnblockedSignal(*signalsReceiverControlBlock, signalMask),
			ret == 0)
	{
		const auto signalNumber = signalInformation.getSignalNumber();
		SignalAction signalAction;
		std::tie(ret, signalAction) = signalsReceiverControlBlock->getSignalAction(signalNumber);
		if (ret == 0)
		{
			const auto handler = signalAction.getHandler();
			if (handler != nullptr)
				(*handler)(signalInformation);
		}
	}
}

}	// namespace synchronization

}	// namespace distortos
