/**
 * \file
 * \brief SignalsReceiver class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-26
 */

#ifndef INCLUDE_DISTORTOS_SIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_SIGNALSRECEIVER_HPP_

#include "distortos/synchronization/SignalsReceiverControlBlock.hpp"

namespace distortos
{

namespace scheduler
{

class ThreadControlBlock;

}	// namespace scheduler

/// SignalsReceiver class is a container for synchronization::SignalsReceiverControlBlock
class SignalsReceiver
{
	friend class scheduler::ThreadControlBlock;

public:

	/**
	 * \brief SignalsReceiver's constructor
	 */

	SignalsReceiver() :
			signalsReceiverControlBlock_{nullptr}
	{

	}

private:

	/// contained synchronization::SignalsReceiverControlBlock object
	synchronization::SignalsReceiverControlBlock signalsReceiverControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALSRECEIVER_HPP_
