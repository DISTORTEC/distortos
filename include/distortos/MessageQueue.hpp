/**
 * \file
 * \brief MessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-14
 */

#ifndef INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_

#include "distortos/synchronization/MessageQueueBase.hpp"

namespace distortos
{

/**
 * \brief MessageQueue class is a message queue for thread-thread, thread-interrupt or interrupt-interrupt
 * communication. It supports multiple readers and multiple writers. It is implemented as a wrapper for
 * synchronization::MessageQueueBase.
 *
 * Similar to POSIX mqd_t - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 *
 * \param T is the type of data in queue
 */

template<typename T>
class MessageQueue
{
public:

	/// type of uninitialized storage for data
	using Storage = synchronization::MessageQueueBase::Storage<T>;

	/**
	 * \brief MessageQueue's constructor
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in storage array
	 */

	MessageQueue(Storage* const storage, const size_t maxElements) :
			messageQueueBase_{storage, maxElements}
	{

	}

private:

	/// contained synchronization::MessageQueueBase object which implements whole functionality
	synchronization::MessageQueueBase messageQueueBase_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_
