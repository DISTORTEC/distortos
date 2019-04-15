/**
 * \file
 * \brief SignalInformationQueueWrapper class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SIGNALINFORMATIONQUEUEWRAPPER_HPP_
#define INCLUDE_DISTORTOS_SIGNALINFORMATIONQUEUEWRAPPER_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/internal/synchronization/SignalInformationQueue.hpp"

namespace distortos
{

namespace internal
{

class SignalsReceiverControlBlock;

}	// namespace internal

/// SignalInformationQueueWrapper class is a container for internal::SignalInformationQueue
class SignalInformationQueueWrapper
{
	friend internal::SignalsReceiverControlBlock;

public:

	/// import Storage type alias from internal::SignalInformationQueue
	using Storage = internal::SignalInformationQueue::Storage;

	/// import StorageUniquePointer type alias from internal::SignalInformationQueue
	using StorageUniquePointer = internal::SignalInformationQueue::StorageUniquePointer;

	/**
	 * \brief SignalInformationQueueWrapper's constructor
	 *
	 * \param [in] storageUniquePointer is a rvalue reference to StorageUniquePointer with storage for queue elements
	 * (sufficiently large for \a maxElements Storage objects) and appropriate deleter
	 * \param [in] maxElements is the number of elements in \a storage array
	 */

	SignalInformationQueueWrapper(StorageUniquePointer&& storageUniquePointer, const size_t maxElements) :
			signalInformationQueue_{std::move(storageUniquePointer), maxElements}
	{

	}

private:

	/// contained internal::SignalInformationQueue object
	internal::SignalInformationQueue signalInformationQueue_;
};

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_SIGNALINFORMATIONQUEUEWRAPPER_HPP_
