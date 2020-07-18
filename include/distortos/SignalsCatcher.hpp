/**
 * \file
 * \brief SignalsCatcher class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_
#define INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/internal/synchronization/SignalsCatcherControlBlock.hpp"

namespace distortos
{

namespace internal
{

class SignalsReceiverControlBlock;

}	// namespace internal

/// SignalsCatcher class is a container for internal::SignalsCatcherControlBlock
class SignalsCatcher
{
	friend internal::SignalsReceiverControlBlock;

public:

	/// import Storage type alias from internal::SignalsCatcherControlBlock
	using Storage = internal::SignalsCatcherControlBlock::Storage;

	/// import StorageUniquePointer type alias from internal::SignalsCatcherControlBlock
	using StorageUniquePointer = internal::SignalsCatcherControlBlock::StorageUniquePointer;

	/**
	 * \brief SignalsCatcher's constructor
	 *
	 * \param [in] storageUniquePointer is a rvalue reference to StorageUniquePointer with storage for
	 * internal::Association objects (sufficiently large for \a storageSize elements) and appropriate deleter
	 * \param [in] storageSize is the number of elements in \a storage array
	 */

	SignalsCatcher(StorageUniquePointer&& storageUniquePointer, const size_t storageSize) :
			signalsCatcherControlBlock_{std::move(storageUniquePointer), storageSize}
	{

	}

private:

	/// contained internal::SignalsCatcherControlBlock object
	internal::SignalsCatcherControlBlock signalsCatcherControlBlock_;
};

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_SIGNALSCATCHER_HPP_
