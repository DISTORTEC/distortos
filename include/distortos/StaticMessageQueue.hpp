/**
 * \file
 * \brief StaticMessageQueue class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_

#include "MessageQueue.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticMessageQueue class is a variant of MessageQueue that has automatic storage for queue's contents.
 *
 * \tparam T is the type of data in queue
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<typename T, size_t QueueSize>
class StaticMessageQueue : public MessageQueue<T>
{
public:

	/// import EntryStorage type from base class
	using typename MessageQueue<T>::EntryStorage;

	/// import ValueStorage type from base class
	using typename MessageQueue<T>::ValueStorage;

	/**
	 * \brief StaticMessageQueue's constructor
	 */

	explicit StaticMessageQueue() :
			MessageQueue<T>{{entryStorage_.data(), internal::dummyDeleter<EntryStorage>},
					{valueStorage_.data(), internal::dummyDeleter<ValueStorage>}, valueStorage_.size()}
	{

	}

	/**
	 * \return maximum number of elements in queue
	 */

	constexpr static size_t getCapacity()
	{
		return QueueSize;
	}

private:

	/// storage for queue's entries
	std::array<EntryStorage, QueueSize> entryStorage_;

	/// storage for queue's contents
	std::array<ValueStorage, QueueSize> valueStorage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_
