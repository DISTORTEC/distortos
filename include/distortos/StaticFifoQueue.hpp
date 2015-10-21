/**
 * \file
 * \brief StaticFifoQueue class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-21
 */

#ifndef INCLUDE_DISTORTOS_STATICFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICFIFOQUEUE_HPP_

#include "FifoQueue.hpp"

#include "distortos/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticFifoQueue class is a variant of FifoQueue that has automatic storage for queue's contents.
 *
 * \param T is the type of data in queue
 * \param QueueSize is the maximum number of elements in queue
 */

template<typename T, size_t QueueSize>
class StaticFifoQueue : public FifoQueue<T>
{
public:

	/**
	 * \brief StaticFifoQueue's constructor
	 */

	explicit StaticFifoQueue() :
			FifoQueue<T>{{storage_.data(), memory::dummyDeleter}, storage_.size()}
	{

	}

private:

	/// storage for queue's contents
	std::array<typename FifoQueue<T>::Storage, QueueSize> storage_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICFIFOQUEUE_HPP_
