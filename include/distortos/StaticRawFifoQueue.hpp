/**
 * \file
 * \brief StaticRawFifoQueue class header
 *
 * \author Copyright (C) 2015-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_

#include "RawFifoQueue.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

#include <array>

namespace distortos
{

/**
 * \brief StaticRawFifoQueue class is a variant of RawFifoQueue that has automatic storage for queue's contents.
 *
 * \tparam ElementSize is the size of single queue element, bytes
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<size_t ElementSize, size_t QueueSize>
class StaticRawFifoQueue : public RawFifoQueue
{
public:

	/**
	 * \brief StaticRawFifoQueue's constructor
	 */

	explicit StaticRawFifoQueue() :
			RawFifoQueue{{storage_.data(), internal::dummyDeleter<uint8_t>}, ElementSize, QueueSize}
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

	/// storage for queue's contents
	std::array<uint8_t, ElementSize * QueueSize> storage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_
