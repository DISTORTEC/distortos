/**
 * \file
 * \brief SignalsReceiverControlBlock class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/synchronization/SignalsReceiverControlBlock.hpp"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/assert.h"
#include "distortos/InterruptMaskingLock.hpp"
#include "distortos/SignalsCatcher.hpp"
#include "distortos/SignalInformationQueueWrapper.hpp"

#include <cerrno>

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SignalsReceiverControlBlock::SignalsReceiverControlBlock(SignalInformationQueueWrapper* const
		signalInformationQueueWrapper, SignalsCatcher* const signalsCatcher) :
		pendingSignalSet_{SignalSet::empty},
		waitingSignalSet_{},
		signalsCatcherControlBlock_{signalsCatcher != nullptr ? &signalsCatcher->signalsCatcherControlBlock_ : nullptr},
		signalInformationQueue_
		{
				signalInformationQueueWrapper != nullptr ? &signalInformationQueueWrapper->signalInformationQueue_ :
						nullptr
		}
{

}

std::pair<int, SignalInformation> SignalsReceiverControlBlock::acceptPendingSignal(const uint8_t signalNumber)
{
	if (signalInformationQueue_ != nullptr)
	{
		const auto acceptQueuedSignalResult = signalInformationQueue_->acceptQueuedSignal(signalNumber);
		if (acceptQueuedSignalResult.first != EAGAIN)	// signal was accepted or fatal error?
			return acceptQueuedSignalResult;
	}

	const auto testResult = pendingSignalSet_.test(signalNumber);
	if (testResult.first != 0)
		return {testResult.first, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};
	if (testResult.second == false)
		return {EAGAIN, SignalInformation{uint8_t{}, SignalInformation::Code{}, sigval{}}};
	const auto ret = pendingSignalSet_.remove(signalNumber);
	return {ret, SignalInformation{signalNumber, SignalInformation::Code::generated, sigval{}}};
}

int SignalsReceiverControlBlock::deliveryOfSignalsStartedHook() const
{
	if (signalsCatcherControlBlock_ == nullptr)
		return ENOTSUP;

	signalsCatcherControlBlock_->deliveryOfSignalsStartedHook();
	return 0;
}

int SignalsReceiverControlBlock::generateSignal(const uint8_t signalNumber, ThreadControlBlock& threadControlBlock)
{
	{
		const InterruptMaskingLock interruptMaskingLock;

		const auto isSignalIgnoredResult = isSignalIgnored(signalNumber);
		if (isSignalIgnoredResult.first != 0)
			return isSignalIgnoredResult.first;
		if (isSignalIgnoredResult.second == true)	// is signal ignored?
			return 0;

		{
			const auto ret = beforeGenerateQueue(signalNumber, threadControlBlock);
			if (ret != 0)
				return ret;
		}

		pendingSignalSet_.add(signalNumber);	// signal number is valid (checked above)

		afterGenerateQueueLocked(signalNumber, threadControlBlock);
	}

	afterGenerateQueueUnlocked(signalNumber, threadControlBlock);
	return 0;
}

SignalSet SignalsReceiverControlBlock::getPendingSignalSet() const
{
	const auto pendingSignalSet = pendingSignalSet_;
	if (signalInformationQueue_ == nullptr)
		return pendingSignalSet;

	const auto queuedSignalSet = signalInformationQueue_->getQueuedSignalSet();
	return SignalSet{pendingSignalSet.getBitset() | queuedSignalSet.getBitset()};
}

std::pair<int, SignalAction> SignalsReceiverControlBlock::getSignalAction(const uint8_t signalNumber) const
{
	if (signalsCatcherControlBlock_ == nullptr)
		return {ENOTSUP, {}};

	return signalsCatcherControlBlock_->getAssociation(signalNumber);
}

SignalSet SignalsReceiverControlBlock::getSignalMask() const
{
	if (signalsCatcherControlBlock_ == nullptr)
		return SignalSet{SignalSet::full};

	return signalsCatcherControlBlock_->getSignalMask();
}

int SignalsReceiverControlBlock::queueSignal(const uint8_t signalNumber, const sigval value,
		ThreadControlBlock& threadControlBlock) const
{
	if (signalInformationQueue_ == nullptr)
		return ENOTSUP;

	{
		const InterruptMaskingLock interruptMaskingLock;

		const auto isSignalIgnoredResult = isSignalIgnored(signalNumber);
		if (isSignalIgnoredResult.first != 0)
			return isSignalIgnoredResult.first;
		if (isSignalIgnoredResult.second == true)	// is signal ignored?
			return 0;

		if (signalInformationQueue_->canQueueSignal() == false)
			return EAGAIN;

		{
			const auto ret = beforeGenerateQueue(signalNumber, threadControlBlock);
			if (ret != 0)
				return ret;
		}
		{
			const auto ret = signalInformationQueue_->queueSignal(signalNumber, value);
			assert(ret == 0);
		}

		afterGenerateQueueLocked(signalNumber, threadControlBlock);
	}

	afterGenerateQueueUnlocked(signalNumber, threadControlBlock);
	return 0;
}

std::pair<int, SignalAction> SignalsReceiverControlBlock::setSignalAction(const uint8_t signalNumber,
		const SignalAction& signalAction)
{
	if (signalsCatcherControlBlock_ == nullptr)
		return {ENOTSUP, {}};

	if (signalAction.getHandler() == SignalAction{}.getHandler())	// new signal action is to ignore the signal?
	{
		int ret;
		// discard all pending signals
		// this can be done before the signal is actually changed, because clearing association (changing signal action
		// to "ignore") may not fail (if the signal number is valid)
		while (std::tie(ret, std::ignore) = acceptPendingSignal(signalNumber), ret == 0);
		if (ret != EAGAIN)	// fatal error?
			return {ret, {}};
	}

	return signalsCatcherControlBlock_->setAssociation(signalNumber, signalAction);
}

int SignalsReceiverControlBlock::setSignalMask(const SignalSet signalMask, const bool deliver)
{
	if (signalsCatcherControlBlock_ == nullptr)
		return ENOTSUP;

	signalsCatcherControlBlock_->setSignalMask(signalMask, deliver == true ? this : nullptr);
	return 0;
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void SignalsReceiverControlBlock::afterGenerateQueueLocked(const uint8_t signalNumber,
		ThreadControlBlock& threadControlBlock) const
{
	assert(signalNumber < SignalSet::Bitset{}.size());

	if (signalsCatcherControlBlock_ != nullptr)
	{
		const auto signalMask = signalsCatcherControlBlock_->getSignalMask();
		const auto testResult = signalMask.test(signalNumber);
		if (testResult.second == false)	// signal is not masked?
			return;
	}

	if (waitingSignalSet_ == nullptr)
		return;

	const auto testResult = waitingSignalSet_->test(signalNumber);
	if (testResult.second == false)	// signalNumber is not "waited for"?
		return;

	getScheduler().unblock(ThreadList::iterator{threadControlBlock});
}

void SignalsReceiverControlBlock::afterGenerateQueueUnlocked(const uint8_t signalNumber,
		ThreadControlBlock& threadControlBlock) const
{
	assert(signalNumber < SignalSet::Bitset{}.size());

	if (signalsCatcherControlBlock_ == nullptr)
		return;

	const auto testResult = signalsCatcherControlBlock_->getSignalMask().test(signalNumber);
	if (testResult.second == true)	// signal is masked?
		return;

	signalsCatcherControlBlock_->afterGenerateQueueUnlocked(threadControlBlock);
}

int SignalsReceiverControlBlock::beforeGenerateQueue(const uint8_t signalNumber,
		ThreadControlBlock& threadControlBlock) const
{
	assert(signalNumber < SignalSet::Bitset{}.size());

	if (signalsCatcherControlBlock_ == nullptr)
		return 0;

	const auto testResult = signalsCatcherControlBlock_->getSignalMask().test(signalNumber);
	if (testResult.second == true)	// signal is masked?
		return 0;

	return signalsCatcherControlBlock_->beforeGenerateQueue(threadControlBlock);
}

std::pair<int, bool> SignalsReceiverControlBlock::isSignalIgnored(const uint8_t signalNumber) const
{
	if (signalNumber >= SignalSet::Bitset{}.size())
		return {EINVAL, {}};

	if (signalsCatcherControlBlock_ == nullptr)
		return {{}, false};

	SignalAction signalAction;
	// this will never fail, because signal number is valid (checked above)
	std::tie(std::ignore, signalAction) = signalsCatcherControlBlock_->getAssociation(signalNumber);

	// default handler == signal is ignored
	return {{}, signalAction.getHandler() == SignalAction{}.getHandler() ? true : false};
}

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1
