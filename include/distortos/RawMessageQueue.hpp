/**
 * \file
 * \brief RawMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-18
 */

#ifndef INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_

#include "distortos/synchronization/MessageQueueBase.hpp"

namespace distortos
{

/**
 * \brief RawMessageQueue class is very similar to MessageQueue, but optimized for binary serializable types (like POD
 * types). Type T can be used with both RawMessageQueue and MessageQueue<T> only when
 * std::is_trivially_copyable<T>::value == true, otherwise only MessageQueue<T> use is safe, while using RawMessageQueue
 * results in undefined behavior.
 *
 * Similar to POSIX mqd_t - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 */

class RawMessageQueue
{
public:

	/// type of uninitialized storage for Entry with link
	using EntryStorage = synchronization::MessageQueueBase::EntryStorage;

	/**
	 * \brief RawMessageQueue's constructor
	 *
	 * \param [in] entryStorage is an array of EntryStorage elements
	 * \param [in] valueStorage is a memory block for elements, sufficiently large for \a maxElements, each
	 * \a elementSize bytes long
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] maxElements is the number of elements in \a entryStorage array and \a valueStorage memory block
	 */

	RawMessageQueue(EntryStorage* const entryStorage, void* const valueStorage, const size_t elementSize,
			const size_t maxElements) :
			messageQueueBase_{entryStorage, valueStorage, elementSize, maxElements},
			elementSize_{elementSize}
	{

	}

	/**
	 * \brief RawMessageQueue's constructor
	 *
	 * \param T is the type of data in queue
	 * \param N is the number of elements in \a entryStorage array and \a valueStorage memory block
	 *
	 * \param [in] entryStorage is a reference to an array of \a N EntryStorage elements
	 * \param [in] valueStorage is a reference to array that will be used as storage for \a N elements, each sizeof(T)
	 * bytes long
	 */

	template<typename T, size_t N>
	RawMessageQueue(EntryStorage (& entryStorage)[N], T (& valueStorage)[N]) :
			RawMessageQueue{entryStorage, valueStorage, sizeof(*valueStorage),
					sizeof(valueStorage) / sizeof(*valueStorage)}
	{

	}

	/**
	 * \brief RawMessageQueue's constructor
	 *
	 * \param T is the type of data in queue
	 * \param N is the number of elements in \a entryStorage array and \a valueStorage memory block
	 *
	 * \param [in] entryStorage is a reference to an std::array of \a N EntryStorage elements
	 * \param [in] valueStorage is a reference to std::array that will be used as storage for \a N elements, each
	 * sizeof(T) bytes long
	 */

	template<typename T, size_t N>
	RawMessageQueue(std::array<EntryStorage, N>& entryStorage, std::array<T, N>& valueStorage) :
			RawMessageQueue{entryStorage.data(), valueStorage.data(), sizeof(*valueStorage.data()), valueStorage.size()}
	{

	}

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(uint8_t& priority, void* buffer, size_t size);

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param T is the type of data popped from the queue
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(uint8_t priority, const void* data, size_t size);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param T is the type of data pushed to the queue
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPop(uint8_t& priority, void* buffer, size_t size);

	/**
	 * \brief Tries to pop the oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param T is the type of data popped from the queue
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * Similar to mq_timedreceive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * Similar to mq_timedreceive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 * \param T is the type of data popped from the queue
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a reference to object that will be used to return popped value
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * Similar to mq_timedreceive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * \param Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes - must be equal to the \a elementSize attribute of
	 * RawMessageQueue
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * \brief Tries to push the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(uint8_t priority, const void* data, size_t size);

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param T is the type of data pushed to the queue
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 * \param T is the type of data pushed to the queue
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * \param Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes - must be equal to the \a elementSize attribute of RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param Duration is a std::chrono::duration type used to measure duration
	 * \param T is the type of data pushed to the queue
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] data is a reference to data that will be pushed to RawMessageQueue
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * \return zero if element was popped successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, void* buffer,
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
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - EMSGSIZE - \a size doesn't match the \a elementSize attribute of RawMessageQueue;
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, const void* data,
			size_t size);

	/// contained synchronization::MessageQueueBase object which implements base functionality
	synchronization::MessageQueueBase messageQueueBase_;

	/// size of single queue element, bytes
	const size_t elementSize_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_RAWMESSAGEQUEUE_HPP_
