/**
 * \file
 * \brief SignalsCatcherControlBlock class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-09
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_

#include "distortos/SignalAction.hpp"

namespace distortos
{

namespace scheduler
{

class ThreadControlBlock;

}	// namespace scheduler

namespace synchronization
{

/// SignalsCatcherControlBlock class is a structure required by threads for "catching" and "handling" of signals
class SignalsCatcherControlBlock
{
public:

	/// association of signal number with SignalAction
	using Association = std::pair<uint8_t, SignalAction>;

	/// type of uninitialized storage for Association objects
	using Storage = std::aligned_storage<sizeof(Association), alignof(Association)>::type;

	/**
	 * \brief SignalsCatcherControlBlock's constructor
	 *
	 * \param [in] storageBegin is a pointer to first element of storage for Association objects
	 * \param [in] storageEnd is a pointer to "one past the last" element of storage for Association objects
	 */

	constexpr SignalsCatcherControlBlock(Storage* const storageBegin, Storage* const storageEnd) :
			signalMask_{SignalSet::empty},
			associationsBegin_{reinterpret_cast<decltype(associationsBegin_)>(storageBegin)},
			storageBegin_{storageBegin},
			storageEnd_{storageEnd}
	{

	}

	/**
	 * \brief Gets SignalAction associated with given signal number.
	 *
	 * \param [in] signalNumber is the signal for which the association is requested, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that is associated with
	 * \a signalNumber, default-constructed object if no association was found;
	 * error codes:
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
	 * \brief Part of SignalsReceiverControlBlock::postGenerate() specific to catching unmasked signals.
	 *
	 * Requests delivery of signals to associated thread if there is some non-default signal handler for the signal.
	 *
	 * \param [in] signalNumber is the unmasked signal that was generated, [0; 31]
	 * \param [in] threadControlBlock is a reference to associated ThreadControlBlock
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	int postGenerate(uint8_t signalNumber, const scheduler::ThreadControlBlock& threadControlBlock) const;

	/**
	 * \brief Sets association for given signal number.
	 *
	 * \param [in] signalNumber is the signal for which the association will be set, [0; 31]
	 * \param [in] signalAction is a reference to SignalAction that will be associated with given signal number, object
	 * in internal storage is copy-constructed
	 *
	 * \return pair with return code (0 on success, error code otherwise) and SignalAction that was associated with
	 * \a signalNumber, default-constructed object if no association was found;
	 * error codes:
	 * - EAGAIN - no resources are available to associate \a signalNumber with \a signalAction;
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	std::pair<int, SignalAction> setAssociation(uint8_t signalNumber, const SignalAction& signalAction);

	/**
	 * \param [in] signalMask is the SignalSet with new signal mask for associated thread
	 */

	void setSignalMask(const SignalSet signalMask)
	{
		signalMask_ = signalMask;
	}

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

	/// SignalSet with signal mask for associated thread
	SignalSet signalMask_;

	/// pointer to first element of range of Association objects
	Association* associationsBegin_;

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
};

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_
