/**
 * \file
 * \brief SignalInformationQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-30
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALINFORMATIONQUEUE_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALINFORMATIONQUEUE_HPP_

#include "distortos/allocators/PoolAllocator.hpp"
#include "distortos/allocators/FeedablePool.hpp"

#include "distortos/SignalInformation.hpp"

#include <forward_list>

namespace distortos
{

class SignalSet;

namespace synchronization
{

/// SignalInformationQueue class can be used for queuing of SignalInformation objects
class SignalInformationQueue
{
public:

	/// link and SignalInformation
	using LinkAndSignalInformation = std::pair<void*, SignalInformation>;

	/// type of uninitialized storage for SignalInformation with link
	using Storage =
			typename std::aligned_storage<sizeof(LinkAndSignalInformation), alignof(LinkAndSignalInformation)>::type;

	/**
	 * \brief SignalInformationQueue's constructor
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in \a storage array
	 */

	SignalInformationQueue(Storage* storage, size_t maxElements);

	/**
	 * \return set of currently queued signals
	 */

	SignalSet getQueuedSignalSet() const;

private:

	/// type of allocator used by \a List
	using PoolAllocator = allocators::PoolAllocator<SignalInformation, allocators::FeedablePool>;

	/// type of container with SignalInformation objects
	using List = std::forward_list<SignalInformation, PoolAllocator>;

	/// PoolAllocator::Pool used by \a poolAllocator_
	PoolAllocator::Pool pool_;

	/// PoolAllocator used by \a signalInformationList_ and \a freeSignalInformationList_
	PoolAllocator poolAllocator_;

	/// list of queued SignalInformation objects
	List signalInformationList_;

	/// list of "free" SignalInformation objects
	List freeSignalInformationList_;
};

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALINFORMATIONQUEUE_HPP_
