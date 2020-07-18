/**
 * \file
 * \brief DynamicMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_DYNAMICMESSAGEQUEUE_HPP_

#include "MessageQueue.hpp"

#include "distortos/internal/memory/storageDeleter.hpp"

namespace distortos
{

/**
 * \brief DynamicMessageQueue class is a variant of MessageQueue that has dynamic storage for queue's contents.
 *
 * \tparam T is the type of data in queue
 *
 * \ingroup queues
 */

template<typename T>
class DynamicMessageQueue : public MessageQueue<T>
{
public:

	/// import EntryStorage type from base class
	using typename MessageQueue<T>::EntryStorage;

	/// import ValueStorage type from base class
	using typename MessageQueue<T>::ValueStorage;

	/**
	 * \brief DynamicMessageQueue's constructor
	 *
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	explicit DynamicMessageQueue(size_t queueSize);
};

template<typename T>
DynamicMessageQueue<T>::DynamicMessageQueue(const size_t queueSize) :
		MessageQueue<T>{{new EntryStorage[queueSize], internal::storageDeleter<EntryStorage>},
				{new ValueStorage[queueSize], internal::storageDeleter<ValueStorage>}, queueSize}
{

}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICMESSAGEQUEUE_HPP_
