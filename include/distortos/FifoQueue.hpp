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
 * \date 2014-12-11
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
class FifoQueue
{
public:

	/// type of uninitialized storage for data
	using Storage = scheduler::FifoQueueBase::Storage<T>;

	/**
	 * \brief FifoQueue's constructor
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in storage array
	 */

	FifoQueue(Storage* const storage, const size_t maxElements) :
			fifoQueueBase_{storage, maxElements, scheduler::FifoQueueBase::TypeTag<T>{}}
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
		return fifoQueueBase_.pop(value);
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
		return fifoQueueBase_.push(value);
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
		return fifoQueueBase_.push(std::move(value));
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
		return fifoQueueBase_.tryPop(value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue for a given duration of time.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPopFor(TickClock::duration, T&)
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopFor(const TickClock::duration duration, T& value)
	{
		return fifoQueueBase_.tryPopFor(duration, value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue for a given duration of time.
	 *
	 * Template variant of tryPopFor(TickClock::duration, T&).
	 *
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, T& value)
	{
		return tryPopFor(std::chrono::duration_cast<TickClock::duration>(duration), value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue until a given time point.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPopUntil(TickClock::time_point, T&)
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopUntil(const TickClock::time_point timePoint, T& value)
	{
		return fifoQueueBase_.tryPopUntil(timePoint, value);
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
		return fifoQueueBase_.tryPush(value);
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPush(T&&)
	 *
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(T&& value)
	{
		return fifoQueueBase_.tryPush(std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPushFor(TickClock::duration, const T&)
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(const TickClock::duration duration, const T& value)
	{
		return fifoQueueBase_.tryPushFor(duration, value);
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPushFor(TickClock::duration, T&&)
	 *
	 * \param [in] duration is the duration after which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(const TickClock::duration duration, T&& value)
	{
		return fifoQueueBase_.tryPushFor(duration, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPushUntil(TickClock::time_point, const T&)
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(const TickClock::time_point timePoint, const T& value)
	{
		return fifoQueueBase_.tryPushUntil(timePoint, value);
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Wrapper for scheduler::FifoQueueBase::tryPushUntil(TickClock::time_point, T&&)
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(const TickClock::time_point timePoint, T&& value)
	{
		return fifoQueueBase_.tryPushUntil(timePoint, std::move(value));
	}

private:

	/// contained scheduler::FifoQueueBase object which implements whole functionality
	scheduler::FifoQueueBase fifoQueueBase_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FIFOQUEUE_HPP_
