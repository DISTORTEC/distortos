/**
 * \file
 * \brief StaticRawMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-24
 */

#ifndef INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_

#include "distortos/RawMessageQueue.hpp"

#include "distortos/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticRawMessageQueue class is a variant of RawMessageQueue that has automatic storage for queue's contents.
 *
 * \param T is the type of data in queue
 * \param QueueSize is the maximum number of elements in queue
 */

template<typename T, size_t QueueSize>
class StaticRawMessageQueue : public RawMessageQueue
{
public:

	/**
	 * \brief StaticRawMessageQueue's constructor
	 */

	explicit StaticRawMessageQueue() :
			RawMessageQueue{{entryStorage_.data(), memory::dummyDeleter}, {valueStorage_.data(), memory::dummyDeleter},
					sizeof(*valueStorage_.data()), valueStorage_.size()}
	{

	}

private:

	/// storage for queue's entries
	std::array<EntryStorage, QueueSize> entryStorage_;

	/// storage for queue's contents
	std::array<ValueStorage<T>, QueueSize> valueStorage_;
};

/**
 * \brief StaticRawMessageQueueFromSize type alias is a variant of StaticRawMessageQueue which uses size of element
 * (instead of type) as template argument.
 *
 * \param ElementSize is the size of single queue element, bytes
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t ElementSize, size_t QueueSize>
using StaticRawMessageQueueFromSize =
		StaticRawMessageQueue<typename std::aligned_storage<ElementSize, ElementSize>::type, QueueSize>;

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
