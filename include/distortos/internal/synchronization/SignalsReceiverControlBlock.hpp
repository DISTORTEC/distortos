/**
 * \file
 * \brief SignalsReceiverControlBlock class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SIGNALSRECEIVERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SIGNALSRECEIVERCONTROLBLOCK_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/SignalSet.hpp"

#include <cstdint>

union sigval;

namespace distortos
{

class SignalAction;
class SignalInformation;
class SignalInformationQueueWrapper;
class SignalsCatcher;

namespace internal
{

class SignalInformationQueue;
class SignalsCatcherControlBlock;
class ThreadControlBlock;

/// SignalsReceiverControlBlock class is a structure required by threads for "receiving" of signals
class SignalsReceiverControlBlock
{
public:

	/**
	 * \brief SignalsReceiverControlBlock's constructor
	 *
	 * \param [in] signalInformationQueueWrapper is a pointer to SignalInformationQueueWrapper for this receiver,
	 * nullptr to disable queuing of signals for this receiver
	 * \param [in] signalsCatcher is a pointer to SignalsCatcher for this receiver, nullptr if this receiver cannot
	 * catch/handle signals
	 */

	explicit SignalsReceiverControlBlock(SignalInformationQueueWrapper* signalInformationQueueWrapper,
			SignalsCatcher* signalsCatcher);

	/**
	 * \brief Accepts (clears) one of signals that are pending.
	 *
	 * This should be called when the signal is "accepted".
	 *
	 * \param [in] signalNumber is the signal that will be accepted, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalInformation object for accepted
	 * signal; error codes:
	 * - EAGAIN - no signal specified by \a signalNumber was pending;
	 * - error codes returned by SignalInformationQueue::acceptQueuedSignal() (except EAGAIN);
	 * - error codes returned by SignalSet::remove();
	 * - error codes returned by SignalSet::test();
	 */

	std::pair<int, SignalInformation> acceptPendingSignal(uint8_t signalNumber);

	/**
	 * \brief Hook function executed when delivery of signals is started.
	 *
	 * Calls SignalsCatcherControlBlock::deliveryOfSignalsStartedHook().
	 *
	 * \attention This function should be called only by the function that delivers signals (<em>deliverSignals()</em>).
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOTSUP - catching/handling of signals is disabled for this receiver;
	 */

	int deliveryOfSignalsStartedHook() const;

	/**
	 * \brief Generates signal for associated thread.
	 *
	 * Similar to pthread_kill() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_kill.html
	 *
	 * Adds the signalNumber to set of pending signals. If associated thread is currently waiting for this signal, it
	 * will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be generated, [0; 31]
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by afterGenerateQueueLocked();
	 * - error codes returned by beforeGenerateQueue();
	 * - error codes returned by isSignalIgnored();
	 */

	int generateSignal(uint8_t signalNumber, ThreadControlBlock& threadControlBlock);

	/**
	 * \return set of currently pending signals
	 */

	SignalSet getPendingSignalSet() const;

	/**
	 * \brief Gets SignalAction associated with given signal number.
	 *
	 * Similar to sigaction() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaction.html
	 *
	 * \param [in] signalNumber is the signal for which the association is requested, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that is associated with
	 * \a signalNumber, default-constructed object if no association was found; error codes:
	 * - ENOTSUP - catching/handling of signals is disabled for this receiver;
	 * - error codes returned by SignalsCatcherControlBlock::getAssociation();
	 */

	std::pair<int, SignalAction> getSignalAction(uint8_t signalNumber) const;

	/**
	 * \brief Gets signal mask for associated thread.
	 *
	 * Similar to pthread_sigmask() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html#
	 *
	 * \return SignalSet with signal mask for associated thread
	 */

	SignalSet getSignalMask() const;

	/**
	 * \brief Queues signal for associated thread.
	 *
	 * Similar to sigqueue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigqueue.html
	 *
	 * Queues the signalNumber and signal value (sigval union) in associated SignalInformationQueue object. If
	 * associated thread is currently waiting for this signal, it will be unblocked.
	 *
	 * \param [in] signalNumber is the signal that will be queued, [0; 31]
	 * \param [in] value is the signal value
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - EAGAIN - no resources are available to queue the signal, the limit of signals which may be queued has been
	 * reached;
	 * - ENOTSUP - queuing of signals is disabled for this receiver;
	 * - error codes returned by afterGenerateQueueLocked();
	 * - error codes returned by beforeGenerateQueue();
	 * - error codes returned by isSignalIgnored();
	 */

	int queueSignal(uint8_t signalNumber, sigval value, ThreadControlBlock& threadControlBlock) const;

	/**
	 * \brief Sets association for given signal number.
	 *
	 * Similar to sigaction() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaction.html
	 *
	 * \param [in] signalNumber is the signal for which the association will be set, [0; 31]
	 * \param [in] signalAction is a reference to SignalAction that will be associated with given signal number, object
	 * in internal storage is copy-constructed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that was associated with
	 * \a signalNumber, default-constructed object if no association was found; error codes:
	 * - ENOTSUP - catching/handling of signals is disabled for this receiver;
	 * - error codes returned by acceptPendingSignal() (except EAGAIN);
	 * - error codes returned by SignalsCatcherControlBlock::setAssociation();
	 */

	std::pair<int, SignalAction> setSignalAction(uint8_t signalNumber, const SignalAction& signalAction);

	/**
	 * \brief Sets signal mask for associated thread.
	 *
	 * Similar to pthread_sigmask() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html#
	 *
	 * \param [in] signalMask is the SignalSet with new signal mask for associated thread
	 * \param [in] deliver selects whether pending and unblocked signals will be delivered (true) or not (false)
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOTSUP - catching/handling of signals is disabled for this receiver;
	 */

	int setSignalMask(SignalSet signalMask, bool deliver);

	/**
	 * \param [in] signalSet is a pointer to set of signals that will be "waited for", nullptr when wait was terminated
	 */

	void setWaitingSignalSet(const SignalSet* const signalSet)
	{
		waitingSignalSet_ = signalSet;
	}

private:

	/**
	 * \brief Actions executed from within critical section after signal is generated with generateSignal() or queued
	 * with queueSignal().
	 *
	 * Does nothing if the signal is not masked. Otherwise unblocks associated thread if it is currently waiting for the
	 * signal that was generated/queued.
	 *
	 * \param [in] signalNumber is the signal that was generated/queued, [0; 31]
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 */

	void afterGenerateQueueLocked(uint8_t signalNumber, ThreadControlBlock& threadControlBlock) const;

	/**
	 * \brief Actions executed out of critical section after signal is generated with generateSignal() or queued with
	 * queueSignal().
	 *
	 * \param [in] signalNumber is the signal that was generated/queued, [0; 31]
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 */

	void afterGenerateQueueUnlocked(uint8_t signalNumber, ThreadControlBlock& threadControlBlock) const;

	/**
	 * \brief Actions executed before signal is generated with generateSignal() or queued with queueSignal().
	 *
	 * Requests delivery it the generated/queued signal is not masked.
	 *
	 * \param [in] signalNumber is the signal that will be generated/queued, [0; 31]
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by SignalsCatcherControlBlock::beforeGenerateQueue();
	 */

	int beforeGenerateQueue(uint8_t signalNumber, ThreadControlBlock& threadControlBlock) const;

	/**
	 * \brief Checks whether signal is ignored.
	 *
	 * Signal is ignored if it has no SignalAction object associated. Signal is never ignored if catching/handling of
	 * signals is disabled for this receiver.
	 *
	 * \param [in] signalNumber is the signal for which the check will be performed, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and boolean telling whether the signal is
	 * ignored (true) or not (false); error codes:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	std::pair<int, bool> isSignalIgnored(uint8_t signalNumber) const;

	/// set of pending signals
	SignalSet pendingSignalSet_;

	/// pointer to set of "waited for" signals, nullptr if associated thread is not waiting for any signals
	const SignalSet* waitingSignalSet_;

	/// pointer to SignalsCatcherControlBlock for this receiver, nullptr if this receiver cannot catch/handle signals
	SignalsCatcherControlBlock* signalsCatcherControlBlock_;

	/// pointer to SignalInformationQueue for this receiver, nullptr if this receiver cannot queue signals
	SignalInformationQueue* signalInformationQueue_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SIGNALSRECEIVERCONTROLBLOCK_HPP_
