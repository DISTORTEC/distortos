/**
 * \file
 * \brief SignalsCatcher class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-10
 */

#ifndef INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_
#define INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_

#include "distortos/synchronization/SignalsCatcherControlBlock.hpp"

namespace distortos
{

namespace synchronization
{

class SignalsReceiverControlBlock;

}	// namespace synchronization

/// SignalsCatcher class is a container for synchronization::SignalsCatcherControlBlock
class SignalsCatcher
{
	friend class synchronization::SignalsReceiverControlBlock;

public:

	/**
	 * \brief SignalsCatcher's constructor
	 */

	constexpr SignalsCatcher() :
			signalsCatcherControlBlock_{}
	{

	}

private:

	/// contained synchronization::SignalsCatcherControlBlock object
	synchronization::SignalsCatcherControlBlock signalsCatcherControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_
