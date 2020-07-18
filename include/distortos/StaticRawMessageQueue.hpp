/**
 * \file
 * \brief StaticRawMessageQueue class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_

#include "distortos/RawMessageQueue.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticRawMessageQueue class is a variant of RawMessageQueue that has automatic storage for queue's contents.
 *
 * \tparam ElementSize is the size of single queue element, bytes
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<size_t ElementSize, size_t QueueSize>
class StaticRawMessageQueue : public RawMessageQueue
{
public:

	/**
	 * \brief StaticRawMessageQueue's constructor
	 */

	explicit StaticRawMessageQueue() :
			RawMessageQueue{{entryStorage_.data(), internal::dummyDeleter<EntryStorage>},
					{valueStorage_.data(), internal::dummyDeleter<uint8_t>}, ElementSize, QueueSize}
	{

	}

	/**
	 * \return maximum number of elements in queue
	 */

	constexpr static size_t getCapacity()
	{
		return QueueSize;
	}

	/**
	 * \return size of single queue element, bytes
	 */

	constexpr static size_t getElementSize()
	{
		return ElementSize;
	}

private:

	/// storage for queue's entries
	std::array<EntryStorage, QueueSize> entryStorage_;

	/// storage for queue's contents
	std::array<uint8_t, ElementSize * QueueSize> valueStorage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
