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
 * \date 2015-01-01
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

	/**
	 * \brief Pops the oldest (first) element from the queue.
	 *
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of RawFifoQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(void* buffer, size_t size);

	/**
	 * \brief Pops the oldest (first) element from the queue.
	 *
	 * \param T is the type of data popped from the queue
	 *
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int pop(T& buffer)
	{
		return pop(&buffer, sizeof(buffer));
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * \param [in] data is a pointer to data that will be pushed to RawFifoQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawFifoQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const void* data, size_t size);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * \param T is the type of data pushed to the queue
	 *
	 * \param [in] data is a reference to data that will be pushed to RawFifoQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int push(const T& data)
	{
		return push(&data, sizeof(data));
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue.
	 *
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of RawFifoQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPop(void* buffer, size_t size);

	/**
	 * \brief Tries to pop the oldest (first) element from the queue.
	 *
	 * \param T is the type of data popped from the queue
	 *
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int tryPop(T& buffer)
	{
		return tryPop(&buffer, sizeof(buffer));
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * \param [in] data is a pointer to data that will be pushed to RawFifoQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawFifoQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const void* data, size_t size);

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * \param T is the type of data pushed to the queue
	 *
	 * \param [in] data is a reference to data that will be pushed to RawFifoQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int tryPush(const T& data)
	{
		return tryPush(&data, sizeof(data));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] data is a pointer to data that will be pushed to RawFifoQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawFifoQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(TickClock::duration duration, const void* data, size_t size);

private:

	/**
	 * \brief Pops the oldest (first) element from the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of RawFifoQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawFifoQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, void* buffer, size_t size);

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
