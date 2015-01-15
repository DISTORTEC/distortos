/**
 * \file
 * \brief StaticMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-15
 */

#ifndef INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_

#include "MessageQueue.hpp"

namespace distortos
{

/**
 * \brief StaticMessageQueue class is a variant of MessageQueue that has automatic storage for queue's contents.
 *
 * \param T is the type of data in queue
 * \param QueueSize is the maximum number of elements in queue
 */

template<typename T, size_t QueueSize>
class StaticMessageQueue : public MessageQueue<T>
{
public:

	/**
	 * \brief StaticMessageQueue's constructor
	 */

	explicit StaticMessageQueue() :
			MessageQueue<T>{storage_}
	{

	}

private:

	/// storage for queue's contents
	std::array<typename MessageQueue<T>::Storage, QueueSize> storage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_
