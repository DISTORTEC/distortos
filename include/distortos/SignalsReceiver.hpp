/**
 * \file
 * \brief SignalsReceiver class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_SIGNALSRECEIVER_HPP_

#include "distortos/internal/synchronization/SignalsReceiverControlBlock.hpp"

namespace distortos
{

namespace internal
{

class ThreadControlBlock;

}	// namespace internal

/// SignalsReceiver class is a container for internal::SignalsReceiverControlBlock
class SignalsReceiver
{
	friend class internal::ThreadControlBlock;

public:

	/**
	 * \brief SignalsReceiver's constructor
	 *
	 * \param [in] signalInformationQueueWrapper is a pointer to SignalInformationQueueWrapper for this receiver,
	 * nullptr to disable queuing of signals for this receiver
	 * \param [in] signalsCatcher is a pointer to SignalsCatcher for this receiver, nullptr if this receiver cannot
	 * catch/handle signals
	 */

	explicit SignalsReceiver(SignalInformationQueueWrapper* const signalInformationQueueWrapper,
			SignalsCatcher* const signalsCatcher) :
			signalsReceiverControlBlock_{signalInformationQueueWrapper, signalsCatcher}
	{

	}

private:

	/// contained internal::SignalsReceiverControlBlock object
	internal::SignalsReceiverControlBlock signalsReceiverControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALSRECEIVER_HPP_
