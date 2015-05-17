/**
 * \file
 * \brief SignalsCatcherControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-17
 */

#include "distortos/synchronization/SignalsCatcherControlBlock.hpp"

#include "distortos/synchronization/SignalsReceiverControlBlock.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"
#include "distortos/architecture/requestFunctionExecution.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

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

/**
 * \brief Tries to find SignalsCatcherControlBlock::Association for given signal number in given range.
 *
 * \param [in] begin is a pointer to first element of range of SignalsCatcherControlBlock::Association objects
 * \param [in] end is a pointer to "one past the last" element of range of SignalsCatcherControlBlock::Association
 * objects
 * \param [in] signalNumber is the signal for which the association will be searched
 *
 * \return pointer to found SignalsCatcherControlBlock::Association object, \e end if no match was found
 */

SignalsCatcherControlBlock::Association* findAssociation(SignalsCatcherControlBlock::Association* const begin,
		SignalsCatcherControlBlock::Association* const end, const uint8_t signalNumber)
{
	return std::find_if(begin, end,
			[signalNumber](const SignalsCatcherControlBlock::Association& association)
			{
				return association.first.getBitset()[signalNumber];
			});
}

/**
 * \brief Tries to find SignalsCatcherControlBlock::Association for given SignalAction in given range.
 *
 * \param [in] begin is a pointer to first element of range of SignalsCatcherControlBlock::Association objects
 * \param [in] end is a pointer to "one past the last" element of range of SignalsCatcherControlBlock::Association
 * objects
 * \param [in] signalAction is a reference to SignalAction object for which the association will be searched
 *
 * \return pointer to found SignalsCatcherControlBlock::Association object, \e end if no match was found
 */

SignalsCatcherControlBlock::Association* findAssociation(SignalsCatcherControlBlock::Association* const begin,
		SignalsCatcherControlBlock::Association* const end, const SignalAction& signalAction)
{
	return std::find_if(begin, end,
			[&signalAction](const SignalsCatcherControlBlock::Association& association)
			{
				return association.second.getHandler() == signalAction.getHandler() &&
						association.second.getSignalMask().getBitset() == signalAction.getSignalMask().getBitset();
			});
}

/**
 * \brief Delivers all unmasked signals that are pending/queued for current thread.
 */

void deliverSignals()
{
	const auto signalsReceiverControlBlock =
			scheduler::getScheduler().getCurrentThreadControlBlock().getSignalsReceiverControlBlock();
	if (signalsReceiverControlBlock == nullptr)
		return;	/// \todo error handling?

	if (signalsReceiverControlBlock->deliveryOfSignalsStartedHook() != 0)
		return; /// \todo error handling?

	const auto signalMask = signalsReceiverControlBlock->getSignalMask();
	const auto savedErrno = errno;

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
			{
				SignalSet newSignalMask {signalMask.getBitset() | signalAction.getSignalMask().getBitset()};
				newSignalMask.add(signalNumber);	// signalNumber is valid (checked above)
				// this call may not fail, because SignalsReceiverControlBlock that is used here must support
				// catching/handling of signals - otherwise the call to SignalsReceiverControlBlock::getSignalAction()
				// above would fail
				/// \todo add assertion just to be sure
				signalsReceiverControlBlock->setSignalMask(newSignalMask, false);
				(*handler)(signalInformation);
				signalsReceiverControlBlock->setSignalMask(signalMask, false);	// restore previous signal mask
			}
		}
	}

	errno = savedErrno;	// restore errno value
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, SignalAction> SignalsCatcherControlBlock::getAssociation(const uint8_t signalNumber) const
{
	if (signalNumber >= SignalSet::Bitset{}.size())
		return {EINVAL, {}};

	const auto association = findAssociation(associationsBegin_, associationsEnd_, signalNumber);
	if (association == associationsEnd_)	// there is no association for this signal number?
		return {{}, {}};

	return {{}, association->second};
}

int SignalsCatcherControlBlock::postGenerate(const uint8_t signalNumber,
		const scheduler::ThreadControlBlock& threadControlBlock) const
{
	const auto getAssociationResult = getAssociation(signalNumber);
	if (getAssociationResult.first != 0)
		return getAssociationResult.first;
	if (getAssociationResult.second.getHandler() == SignalAction{}.getHandler())	// default handler?
		return 0;	// ignore signal

	architecture::requestFunctionExecution(threadControlBlock, deliverSignals);

	return 0;
}

std::pair<int, SignalAction> SignalsCatcherControlBlock::setAssociation(const uint8_t signalNumber,
		const SignalAction& signalAction)
{
	if (signalNumber >= SignalSet::Bitset{}.size())
		return {EINVAL, {}};

	if (signalAction.getHandler() == SignalAction{}.getHandler())	// default handler is being set?
	{
		const auto previousSignalAction = clearAssociation(signalNumber);
		return {{}, previousSignalAction};
	}

	const auto numberAssociation = findAssociation(associationsBegin_, associationsEnd_, signalNumber);
	const auto actionAssociation = findAssociation(associationsBegin_, associationsEnd_, signalAction);

	if (actionAssociation != associationsEnd_)	// there is an association for this SignalAction?
	{
		if (numberAssociation == actionAssociation)	// no change?
			return {{}, signalAction};

		actionAssociation->first.add(signalNumber);
		const auto previousSignalAction = numberAssociation != associationsEnd_ ?
				clearAssociation(signalNumber, *numberAssociation) : SignalAction{};
		return {{}, previousSignalAction};
	}

	// there is no association for this SignalAction?

	SignalSet signalSet {1u << signalNumber};

	// there is no storage for new association if all conditions are true:
	// - there are no free associations,
	// - no Association object was found for signal number or Association object found for signal number has more
	// than one signal number associated.
	if (std::distance(storageBegin_, storageEnd_) == 0 && (numberAssociation == associationsEnd_ ||
			numberAssociation->first.getBitset() == signalSet.getBitset()))
		return {EAGAIN, {}};

	const auto previousSignalAction = numberAssociation != associationsEnd_ ?
			clearAssociation(signalNumber, *numberAssociation) : SignalAction{};
	if (storageBegin_ == storageEnd_)
		abort();	/// \todo replace with assertion
	new (associationsEnd_) Association{signalSet, signalAction};
	++associationsEnd_;
	return {{}, previousSignalAction};
}

void SignalsCatcherControlBlock::setSignalMask(const SignalSet signalMask,
		const SignalsReceiverControlBlock* const owner)
{
	signalMask_ = signalMask;

	if (owner == nullptr)	// delivery of signals should not be requested if any pending signal is unblocked?
		return;

	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto pendingUnblockedBitset = owner->getPendingSignalSet().getBitset() & ~signalMask.getBitset();
	if (pendingUnblockedBitset.none() == true)	// no pending & unblocked signals?
		return;

	architecture::requestFunctionExecution(scheduler::getScheduler().getCurrentThreadControlBlock(), deliverSignals);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

SignalAction SignalsCatcherControlBlock::clearAssociation(const uint8_t signalNumber)
{
	const auto association = findAssociation(associationsBegin_, associationsEnd_, signalNumber);
	if (association == associationsEnd_)	// there is no association for this signal number?
		return {};

	return clearAssociation(signalNumber, *association);
}

SignalAction SignalsCatcherControlBlock::clearAssociation(const uint8_t signalNumber, Association& association)
{
	const auto previousSignalAction = association.second;

	association.first.remove(signalNumber);	// signal number is valid (checked by caller)

	// can this association be removed (it has no more signal numbers associated)?
	if (association.first.getBitset().none() == true)
	{
		const auto& lastAssociation = *(associationsEnd_ - 1);
		association = lastAssociation;	// replace removed association with the last association in the range
		lastAssociation.~Association();
		--associationsEnd_;
	}

	return previousSignalAction;
}

}	// namespace synchronization

}	// namespace distortos
