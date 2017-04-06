/**
 * \file
 * \brief StaticRawMessageQueue class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

private:

	/// storage for queue's entries
	std::array<EntryStorage, QueueSize> entryStorage_;

	/// storage for queue's contents
	std::array<uint8_t, ElementSize * QueueSize> valueStorage_;
};

namespace deprecated
{

/**
 * \brief StaticRawMessageQueue class is a variant of RawMessageQueue that has automatic storage for queue's contents.
 *
 * \deprecated scheduled to be removed after v0.5.0, use `distortos::StaticRawMessageQueue<sizeof(T), QueueSize>`
 *
 * \tparam T is the type of data in queue
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<typename T, size_t QueueSize>
class StaticRawMessageQueue : public RawMessageQueue
{
public:

	/**
	 * \brief StaticRawMessageQueue's constructor
	 */

	explicit StaticRawMessageQueue() :
			RawMessageQueue{{entryStorage_.data(), internal::dummyDeleter<EntryStorage>},
					{valueStorage_.data(), internal::dummyDeleter<ValueStorage<T>>}, sizeof(*valueStorage_.data()),
					valueStorage_.size()}
	{

	}

private:

	/// storage for queue's entries
	std::array<EntryStorage, QueueSize> entryStorage_;

	/// storage for queue's contents
	std::array<ValueStorage<T>, QueueSize> valueStorage_;
} __attribute__ ((deprecated));

/**
 * \brief StaticRawMessageQueueFromSize type alias is a variant of StaticRawMessageQueue which uses size of element
 * (instead of type) as template argument.
 *
 * \deprecated scheduled to be removed after v0.5.0, use `distortos::StaticRawMessageQueue<ElementSize, QueueSize>`
 *
 * \tparam ElementSize is the size of single queue element, bytes
 * \tparam QueueSize is the maximum number of elements in queue
 */

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80332
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

template<size_t ElementSize, size_t QueueSize>
using StaticRawMessageQueueFromSize __attribute__ ((deprecated)) =
		StaticRawMessageQueue<typename std::aligned_storage<ElementSize, ElementSize>::type, QueueSize>;

#pragma GCC diagnostic pop

}	// namespace deprecated

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
