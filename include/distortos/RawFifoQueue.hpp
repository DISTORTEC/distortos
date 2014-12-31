/**
 * \file
 * \brief RawFifoQueue class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-12-31
 */

#ifndef INCLUDE_DISTORTOS_RAWFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_RAWFIFOQUEUE_HPP_

#include "distortos/scheduler/FifoQueueBase.hpp"

namespace distortos
{

/// RawFifoQueue class is very similar to FifoQueue, but optimized for binary serializable types (like POD types). Type
/// T can be used with both RawFifoQueue and FifoQueue<T> only when std::is_trivially_copyable<T>::value == true,
/// otherwise only FifoQueue<T> use is safe, while using RawFifoQueue results in undefined behavior.
class RawFifoQueue
{
public:

	/**
	 * \brief RawFifoQueue's constructor
	 *
	 * \param [in] storage is a memory block for elements, sufficiently large for \a maxElements, each \a elementSize
	 * bytes long
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] maxElements is the number of elements in storage memory block
	 */

	RawFifoQueue(void* storage, size_t elementSize, size_t maxElements);

private:

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] data is a pointer to data that will be pushed to RawFifoQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawFifoQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, const void* data, size_t size);

	/// contained scheduler::FifoQueueBase object which implements base functionality
	scheduler::FifoQueueBase fifoQueueBase_;

	/// size of single queue element, bytes
	const size_t elementSize_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_RAWFIFOQUEUE_HPP_
