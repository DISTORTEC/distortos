/**
 * \file
 * \brief StaticRawMessageQueue class header
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
 * This class is a replacement for StaticRawMessageQueue with old API and removed StaticRawMessageQueueFromSize. To use
 * this new API modify your code in following way:
 * - old `"StaticRawMessageQueue<T, QueueSize>"` -> `"StaticRawMessageQueue<sizeof(T), QueueSize>"`;
 * - removed `"StaticRawMessageQueueFromSize<ElementSize, QueueSize>"` ->
 * `"StaticRawMessageQueue<ElementSize, QueueSize>"`;
 *
 * Transition schedule:
 * 1. v0.5.0 - `StaticRawMessageQueue<T, QueueSize>` and `StaticRawMessageQueueFromSize<ElementSize, QueueSize>` are
 * converted to deprecated aliases to `StaticRawMessageQueue2<ElementSize, QueueSize>`;
 * 2. v0.6.0 - "old" `StaticRawMessageQueue<T, QueueSize>` and `StaticRawMessageQueueFromSize<ElementSize, QueueSize>`
 * aliases are removed, `StaticRawMessageQueue2<ElementSize, QueueSize>` is renamed to
 * `StaticRawMessageQueue<ElementSize, QueueSize>`, deprecated `StaticRawMessageQueue2<ElementSize, QueueSize>` alias is
 * added;
 * 3. v0.7.0 - deprecated `StaticRawMessageQueue2<ElementSize, QueueSize>` alias is removed;
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

/**
 * \brief StaticRawMessageQueue2 class is a variant of RawMessageQueue that has automatic storage for queue's contents.
 *
 * \deprecated scheduled to be removed after v0.6.0, use `StaticRawMessageQueue<ElementSize, QueueSize>`
 *
 * \tparam ElementSize is the size of single queue element, bytes
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<size_t ElementSize, size_t QueueSize>
using StaticRawMessageQueue2 __attribute__ ((deprecated("Use StaticRawMessageQueue<ElementSize, QueueSize>"))) =
		StaticRawMessageQueue<ElementSize, QueueSize>;

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
