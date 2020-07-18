/**
 * \file
 * \brief RawMessageQueue class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_

#include "distortos/internal/synchronization/MessageQueueBase.hpp"

namespace distortos
{

/**
 * \brief RawMessageQueue class is very similar to MessageQueue, but optimized for binary serializable types (like POD
 * types).
 *
 * Type \a T can be used with both RawMessageQueue and MessageQueue only when
 * <em>std::is_trivially_copyable<T>::value == true</em>, otherwise only MessageQueue use is safe, while using
 * RawMessageQueue results in undefined behavior.
 *
 * Similar to POSIX mqd_t - https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 *
 * \ingroup queues
 */

class RawMessageQueue
{
public:

	/// type of uninitialized storage for Entry with link
	using EntryStorage = internal::MessageQueueBase::EntryStorage;

	/// import EntryStorageUniquePointer type from internal::MessageQueueBase class
	using EntryStorageUniquePointer = internal::MessageQueueBase::EntryStorageUniquePointer;

	/**
	 * \brief type of uninitialized storage for value
	 *
	 * \tparam T is the type of data in queue
	 */

	template<typename T>
	using ValueStorage = internal::MessageQueueBase::ValueStorage<T>;

	/// unique_ptr (with deleter) to storage for value
	using ValueStorageUniquePointer = internal::MessageQueueBase::ValueStorageUniquePointer;

	/**
	 * \brief RawMessageQueue's constructor
	 *
	 * \param [in] entryStorageUniquePointer is a rvalue reference to EntryStorageUniquePointer with storage for queue
	 * entries (sufficiently large for \a maxElements EntryStorage objects) and appropriate deleter
	 * \param [in] valueStorageUniquePointer is a rvalue reference to ValueStorageUniquePointer with storage for queue
	 * elements (sufficiently large for \a maxElements, each \a elementSize bytes long) and appropriate deleter
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] maxElements is the number of elements in \a entryStorage array and \a valueStorage memory block
	 */

	RawMessageQueue(EntryStorageUniquePointer&& entryStorageUniquePointer,
			ValueStorageUniquePointer&& valueStorageUniquePointer, size_t elementSize, size_t maxElements);

	/**
	 * \return maximum number of elements in queue
	 */

	size_t getCapacity() const
	{
		return messageQueueBase_.getCapacity();
	}

	/**
	 * \return size of single queue element, bytes
	 */

	size_t getElementSize() const
	{
		return elementSize_;
	}

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(uint8_t& priority, void* buffer, size_t size);

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam T is the type of data popped from the queue
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int pop(uint8_t& priority, T& buffer)
	{
		return pop(priority, &buffer, sizeof(buffer));
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(uint8_t priority, const void* data, size_t size);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam T is the type of data pushed to the queue
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int push(const uint8_t priority, const T& data)
	{
		return push(priority, &data, sizeof(data));
	}

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPop(uint8_t& priority, void* buffer, size_t size);

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \tparam T is the type of data popped from the queue
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int tryPop(uint8_t& priority, T& buffer)
	{
		return tryPop(priority, &buffer, sizeof(buffer));
	}

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue for a given duration of time.
	 *
	 * Similar to mq_timedreceive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopFor(TickClock::duration duration, uint8_t& priority, void* buffer, size_t size);

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue for a given duration of time.
	 *
	 * Template variant of tryPopFor(TickClock::duration, uint8_t&, void*, size_t).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, uint8_t& priority, void* const buffer,
			const size_t size)
	{
		return tryPopFor(std::chrono::duration_cast<TickClock::duration>(duration), priority, buffer, size);
	}

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue for a given duration of time.
	 *
	 * Similar to mq_timedreceive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 * \tparam T is the type of data popped from the queue
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period, typename T>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, uint8_t& priority, T& buffer)
	{
		return tryPopFor(std::chrono::duration_cast<TickClock::duration>(duration), priority, &buffer, sizeof(buffer));
	}

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue until a given time point.
	 *
	 * Similar to mq_timedreceive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, void* buffer, size_t size);

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue until a given time point.
	 *
	 * Template variant of tryPopUntil(TickClock::time_point, uint8_t&, void*, size_t).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, uint8_t& priority, void* const buffer,
			const size_t size)
	{
		return tryPopUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority, buffer, size);
	}

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue until a given time point.
	 *
	 * Similar to mq_timedreceive() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 * \tparam T is the type of data popped from the queue
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration, typename T>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, uint8_t& priority, T& buffer)
	{
		return tryPopUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority, &buffer,
				sizeof(buffer));
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Similar to mq_send() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(uint8_t priority, const void* data, size_t size);

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Similar to mq_send() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \tparam T is the type of data pushed to the queue
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename T>
	int tryPush(const uint8_t priority, const T& data)
	{
		return tryPush(priority, &data, sizeof(data));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Similar to mq_timedsend() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, const void* data, size_t size);

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Template variant of tryPushFor(TickClock::duration, uint8_t, const void*, size_t).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, const void* const data,
			const size_t size)
	{
		return tryPushFor(std::chrono::duration_cast<TickClock::duration>(duration), priority, data, size);
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Similar to mq_timedsend() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 * \tparam T is the type of data pushed to the queue
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period, typename T>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, const T& data)
	{
		return tryPushFor(std::chrono::duration_cast<TickClock::duration>(duration), priority, &data, sizeof(data));
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Similar to mq_timedsend() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const void* data, size_t size);

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Template variant of tryPushUntil(TickClock::time_point, uint8_t, const void*, size_t).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			const void* const data, const size_t size)
	{
		return tryPushUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority, data, size);
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Similar to mq_timedsend() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 * \tparam T is the type of data pushed to the queue
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - sizeof(T) doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration, typename T>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			const T& data)
	{
		return tryPushUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority, &data,
				sizeof(data));
	}

private:

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, void* buffer,
			size_t size);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, const void* data,
			size_t size);

	/// contained internal::MessageQueueBase object which implements base functionality
	internal::MessageQueueBase messageQueueBase_;

	/// size of single queue element, bytes
	const size_t elementSize_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_
