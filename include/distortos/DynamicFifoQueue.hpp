/**
 * \file
 * \brief DynamicFifoQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_DYNAMICFIFOQUEUE_HPP_

#include "FifoQueue.hpp"

#include "distortos/internal/memory/storageDeleter.hpp"

namespace distortos
{

/**
 * \brief DynamicFifoQueue class is a variant of FifoQueue that has dynamic storage for queue's contents.
 *
 * \tparam T is the type of data in queue
 *
 * \ingroup queues
 */

template<typename T>
class DynamicFifoQueue : public FifoQueue<T>
{
public:

	/// import Storage type from base class
	using typename FifoQueue<T>::Storage;

	/**
	 * \brief DynamicFifoQueue's constructor
	 *
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	explicit DynamicFifoQueue(size_t queueSize);
};

template<typename T>
DynamicFifoQueue<T>::DynamicFifoQueue(const size_t queueSize) :
		FifoQueue<T>{{new Storage[queueSize], internal::storageDeleter<Storage>}, queueSize}
{

}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICFIFOQUEUE_HPP_
