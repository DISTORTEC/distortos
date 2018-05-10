/**
 * \file
 * \brief StaticRawFifoQueue and StaticRawFifoQueue2 classes header
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_

#include "RawFifoQueue.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticRawFifoQueue2 class is a variant of RawFifoQueue that has automatic storage for queue's contents.
 *
 * This class is a replacement for StaticRawFifoQueue and StaticRawFifoQueueFromSize. To use this new API modify your
 * code in following way:
 * - `'StaticRawFifoQueue<T, QueueSize>'` -> `'StaticRawFifoQueue2<sizeof(T), QueueSize>'`;
 * - `'StaticRawFifoQueueFromSize<ElementSize, QueueSize>'` -> `'StaticRawFifoQueue2<ElementSize, QueueSize>'`;
 *
 * Transition schedule:
 * 1. v0.5.0 - `StaticRawFifoQueue<T, QueueSize>` and `StaticRawFifoQueueFromSize<ElementSize, QueueSize>` are converted
 * to deprecated aliases to `StaticRawFifoQueue2<ElementSize, QueueSize>`;
 * 2. v0.6.0 - "old" `StaticRawFifoQueue<T, QueueSize>` and `StaticRawFifoQueueFromSize<ElementSize, QueueSize>` aliases
 * are removed, `StaticRawFifoQueue2<ElementSize, QueueSize>` is renamed to
 * `StaticRawFifoQueue<ElementSize, QueueSize>`, deprecated `StaticRawFifoQueue2<ElementSize, QueueSize>` alias is
 * added;
 * 3. v0.7.0 - deprecated `StaticRawFifoQueue2<ElementSize, QueueSize>` alias is removed;
 *
 * \tparam ElementSize is the size of single queue element, bytes
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<size_t ElementSize, size_t QueueSize>
class StaticRawFifoQueue2 : public RawFifoQueue
{
public:

	/**
	 * \brief StaticRawFifoQueue2's constructor
	 */

	explicit StaticRawFifoQueue2() :
			RawFifoQueue{{storage_.data(), internal::dummyDeleter<uint8_t>}, ElementSize, QueueSize}
	{

	}

private:

	/// storage for queue's contents
	std::array<uint8_t, ElementSize * QueueSize> storage_;
};

/**
 * \brief StaticRawFifoQueue class is a variant of RawFifoQueue that has automatic storage for queue's contents.
 *
 * \deprecated scheduled to be removed after v0.5.0, use `StaticRawFifoQueue2<sizeof(T), QueueSize>`
 *
 * \tparam T is the type of data in queue
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<typename T, size_t QueueSize>
using StaticRawFifoQueue __attribute__ ((deprecated("Use StaticRawFifoQueue2<sizeof(T), QueueSize>"))) =
		StaticRawFifoQueue2<sizeof(T), QueueSize>;

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_
