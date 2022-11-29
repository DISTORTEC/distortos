/**
 * \file
 * \brief StaticFifoQueue class header
 *
 * \author Copyright (C) 2014-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICFIFOQUEUE_HPP_

#include "FifoQueue.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

#include <array>

namespace distortos
{

/**
 * \brief StaticFifoQueue class is a variant of FifoQueue that has automatic storage for queue's contents.
 *
 * \tparam T is the type of data in queue
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \ingroup queues
 */

template<typename T, size_t QueueSize>
class StaticFifoQueue : public FifoQueue<T>
{
public:

	/// import Storage type from base class
	using typename FifoQueue<T>::Storage;

	/**
	 * \brief StaticFifoQueue's constructor
	 */

	explicit StaticFifoQueue() :
			FifoQueue<T>{{storage_.data(), internal::dummyDeleter<Storage>}, storage_.size()}
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

	/// storage for queue's contents
	std::array<Storage, QueueSize> storage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICFIFOQUEUE_HPP_
