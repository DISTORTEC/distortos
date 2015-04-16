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
 * \date 2015-04-16
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

	/// import Storage type alias from synchronization::SignalsCatcherControlBlock
	using Storage = synchronization::SignalsCatcherControlBlock::Storage;

	/**
	 * \brief SignalsCatcher's constructor
	 *
	 * \param [in] storageBegin is a pointer to first element of storage for
	 * synchronization::SignalsCatcherControlBlock::Association objects
	 * \param [in] storageEnd is a pointer to "one past the last" element of storage for
	 * synchronization::SignalsCatcherControlBlock::Association objects
	 */

	constexpr SignalsCatcher(Storage* const storageBegin, Storage* const storageEnd) :
			signalsCatcherControlBlock_{storageBegin, storageEnd}
	{

	}

	/**
	 * \brief SignalsCatcher's constructor
	 *
	 * \param N is the number of elements in \a storage array
	 *
	 * \param [in] storage is a reference to array of Storage elements
	 */

	template<size_t N>
	constexpr explicit SignalsCatcher(Storage (& storage)[N]) :
			SignalsCatcher{std::begin(storage), std::end(storage)}
	{

	}

private:

	/// contained synchronization::SignalsCatcherControlBlock object
	synchronization::SignalsCatcherControlBlock signalsCatcherControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_
