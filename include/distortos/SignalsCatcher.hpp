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
 * \date 2015-10-17
 */

#ifndef INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_
#define INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_

#include "distortos/synchronization/SignalsCatcherControlBlock.hpp"

#include <array>

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
	 * \param [in] storage is a memory block for storage of synchronization::SignalsCatcherControlBlock::Association
	 * objects, sufficiently large for \a storageSize elements
	 * \param [in] storageSize is the number of elements in \a storage array
	 */

	constexpr SignalsCatcher(Storage* const storage, const size_t storageSize) :
			signalsCatcherControlBlock_{storage, storage + storageSize}
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
			SignalsCatcher{storage, sizeof(storage) / sizeof(*storage)}
	{

	}

	/**
	 * \brief SignalsCatcher's constructor
	 *
	 * \param N is the number of elements in \a storage array
	 *
	 * \param [in] storage is a reference to std::array of Storage elements
	 */

	template<size_t N>
	constexpr explicit SignalsCatcher(std::array<Storage, N>& storage) :
			SignalsCatcher{storage.data(), storage.size()}
	{

	}

private:

	/// contained synchronization::SignalsCatcherControlBlock object
	synchronization::SignalsCatcherControlBlock signalsCatcherControlBlock_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_
