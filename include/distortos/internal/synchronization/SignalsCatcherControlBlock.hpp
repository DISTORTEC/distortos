/**
 * \file
 * \brief SignalsCatcherControlBlock class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/SignalAction.hpp"

#include <memory>

namespace distortos
{

namespace internal
{

class SignalsReceiverControlBlock;
class ThreadControlBlock;

/// SignalsCatcherControlBlock class is a structure required by threads for "catching" and "handling" of signals
class SignalsCatcherControlBlock
{
public:

	/// association of signal numbers (as SignalSet) with SignalAction
	using Association = std::pair<SignalSet, SignalAction>;

	/// type of uninitialized storage for Association objects
	using Storage = std::aligned_storage<sizeof(Association), alignof(Association)>::type;

	/// unique_ptr (with deleter) to Storage[]
	using StorageUniquePointer = std::unique_ptr<Storage[], void(&)(Storage*)>;

	/**
	 * \brief SignalsCatcherControlBlock's constructor
	 *
	 * \param [in] storageUniquePointer is a rvalue reference to StorageUniquePointer with storage for Association
	 * objects (sufficiently large for \a storageSize elements) and appropriate deleter
	 * \param [in] storageSize is the number of elements in \a storage array
	 */

	SignalsCatcherControlBlock(StorageUniquePointer&& storageUniquePointer, size_t storageSize);

	/**
	 * \brief SignalsCatcherControlBlock's destructor
	 */

	~SignalsCatcherControlBlock();

	/**
	 * \brief Part of SignalsReceiverControlBlock::afterGenerateQueueUnlocked() specific to catching unmasked signals.
	 *
	 * This function does nothing if the request is for non-current thread of execution. Otherwise it delivers signals
	 * to current thread.
	 *
	 * \note It is assumed that some unmasked signal with non-default signal handler is pending.
	 *
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 */

	void afterGenerateQueueUnlocked(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief Part of SignalsReceiverControlBlock::beforeGenerateQueue() specific to catching unmasked signals.
	 *
	 * This function does nothing if the request is for current thread of execution. Otherwise it requests delivery of
	 * signals to associated thread.
	 *
	 * \note It is assumed that some unmasked signal with non-default signal handler will be pending before leaving
	 * critical section.
	 *
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by requestDeliveryOfSignals();
	 */

	int beforeGenerateQueue(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief Hook function executed when delivery of signals is started.
	 *
	 * Clears "delivery pending" flag.
	 *
	 * \attention This function should be called only by SignalsReceiverControlBlock::deliveryOfSignalsFinishedHook().
	 */

	void deliveryOfSignalsStartedHook()
	{
		deliveryIsPending_ = false;
	}

	/**
	 * \brief Gets SignalAction associated with given signal number.
	 *
	 * \param [in] signalNumber is the signal for which the association is requested, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that is associated with
	 * \a signalNumber, default-constructed object if no association was found; error codes:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	std::pair<int, SignalAction> getAssociation(uint8_t signalNumber) const;

	/**
	 * \return SignalSet with signal mask for associated thread
	 */

	SignalSet getSignalMask() const
	{
		return signalMask_;
	}

	/**
	 * \brief Sets association for given signal number.
	 *
	 * \param [in] signalNumber is the signal for which the association will be set, [0; 31]
	 * \param [in] signalAction is a reference to SignalAction that will be associated with given signal number, object
	 * in internal storage is copy-constructed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that was associated with
	 * \a signalNumber, default-constructed object if no association was found; error codes:
	 * - EAGAIN - no resources are available to associate \a signalNumber with \a signalAction;
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	std::pair<int, SignalAction> setAssociation(uint8_t signalNumber, const SignalAction& signalAction);

	/**
	 * \brief Sets signal mask for associated thread.
	 *
	 * If any pending signal is unblocked and \a owner doesn't equal nullptr, then pending and unblocked signals will be
	 * delivered.
	 *
	 * \param [in] signalMask is the SignalSet with new signal mask for associated thread
	 * \param [in] owner selects whether pending and unblocked signals will be delivered (pointer to owner
	 * SignalsReceiverControlBlock object) or not (nullptr)
	 */

	void setSignalMask(SignalSet signalMask, const SignalsReceiverControlBlock* owner);

	SignalsCatcherControlBlock(const SignalsCatcherControlBlock&) = delete;
	SignalsCatcherControlBlock(SignalsCatcherControlBlock&&) = default;
	const SignalsCatcherControlBlock& operator=(const SignalsCatcherControlBlock&) = delete;
	SignalsCatcherControlBlock& operator=(SignalsCatcherControlBlock&&) = delete;

private:

	/**
	 * \brief Clears association for given signal number.
	 *
	 * \param [in] signalNumber is the signal for which the association will be cleared, [0; 31]
	 *
	 * \return SignalAction that was associated with \a signalNumber, default-constructed object if no association was
	 * found
	 */

	SignalAction clearAssociation(uint8_t signalNumber);

	/**
	 * \brief Clears given association for given signal number.
	 *
	 * \param [in] signalNumber is the signal for which the association will be cleared, [0; 31]
	 * \param [in] association is a reference to Association object from <em>[associationsBegin_; associationsEnd_)</em>
	 * range that will be removed
	 *
	 * \return SignalAction from \a association
	 */

	SignalAction clearAssociation(uint8_t signalNumber, Association& association);

	/**
	 * \return pointer to first element of range of Association objects
	 */

	Association* getAssociationsBegin() const
	{
		return reinterpret_cast<Association*>(storageUniquePointer_.get());
	}

	/**
	 * \brief Requests delivery of signals to associated thread.
	 *
	 * Delivery of signals (via special function executed in the associated thread) is requested only if it's not
	 * already pending. The thread is unblocked if it was blocked.
	 *
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by architecture::requestFunctionExecution();
	 */

	int requestDeliveryOfSignals(ThreadControlBlock& threadControlBlock);

	/// storage for Association objects
	StorageUniquePointer storageUniquePointer_;

	/// SignalSet with signal mask for associated thread
	SignalSet signalMask_;

	/// union binds \a associationsEnd_ and \a storageBegin_ - these point to the same address
	union
	{
		/// pointer to "one past the last" element of range of Association objects
		Association* associationsEnd_;

		/// pointer to first element of range of Storage objects
		Storage* storageBegin_;
	};

	/// pointer to "one past the last" element of range of Storage objects
	Storage* storageEnd_;

	/// true if signal delivery is pending, false otherwise
	bool deliveryIsPending_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_
