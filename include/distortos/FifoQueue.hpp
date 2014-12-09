/**
 * \file
 * \brief FifoQueue class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-12-09
 */

#ifndef INCLUDE_DISTORTOS_FIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_FIFOQUEUE_HPP_

#include "distortos/scheduler/FifoQueueBase.hpp"

namespace distortos
{

/**
 * \brief FifoQueue class is a simple FIFO queue for thread-thread, thread-interrupt or interrupt-interrupt
 * communication. It supports multiple readers and multiple writers. It is implemented as a thin wrapper for
 * scheduler::FifoQueueBase.
 *
 * \param T is the type of data in queue
 */

template<typename T>
class FifoQueue : private scheduler::FifoQueueBase
{
public:

	/// type of uninitialized storage for data
	using Storage = Storage<T>;

	/**
	 * \brief FifoQueue's constructor
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in storage array
	 */

	FifoQueue(Storage* const storage, const size_t maxElements) :
			FifoQueueBase{storage, maxElements, TypeTag<T>{}}
	{

	}

	/**
	 * \brief Pops the oldest (first) element from the queue.
	 *
	 * Wrapper for scheduler::FifoQueueBase::pop(T&)
	 *
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(T& value)
	{
		return FifoQueueBase::pop(value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Wrapper for scheduler::FifoQueueBase::push(const T&)
	 *
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const T& value)
	{
		return FifoQueueBase::push(value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Wrapper for scheduler::FifoQueueBase::push(T&&)
	 *
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(T&& value)
	{
		return FifoQueueBase::push(std::move(value));
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPop(T&)
	 *
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPop(T& value)
	{
		return FifoQueueBase::tryPop(value);
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPush(const T&)
	 *
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const T& value)
	{
		return FifoQueueBase::tryPush(value);
	}
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FIFOQUEUE_HPP_
