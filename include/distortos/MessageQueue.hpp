/**
 * \file
 * \brief MessageQueue class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_

#include "distortos/internal/synchronization/MessageQueueBase.hpp"
#include "distortos/internal/synchronization/BoundQueueFunctor.hpp"
#include "distortos/internal/synchronization/CopyConstructQueueFunctor.hpp"
#include "distortos/internal/synchronization/MoveConstructQueueFunctor.hpp"
#include "distortos/internal/synchronization/SwapPopQueueFunctor.hpp"
#include "distortos/internal/synchronization/SemaphoreWaitFunctor.hpp"
#include "distortos/internal/synchronization/SemaphoreTryWaitFunctor.hpp"
#include "distortos/internal/synchronization/SemaphoreTryWaitForFunctor.hpp"
#include "distortos/internal/synchronization/SemaphoreTryWaitUntilFunctor.hpp"

#if __GNUC_PREREQ(5, 1) != 1
// GCC 4.8 doesn't support parameter pack expansion in lambdas
#error "GCC 5.1 is the minimum version supported by distortos"
#endif

namespace distortos
{

/**
 * \brief MessageQueue class is a message queue for thread-thread, thread-interrupt or interrupt-interrupt
 * communication. It supports multiple readers and multiple writers. It is implemented as a wrapper for
 * internal::MessageQueueBase.
 *
 * Similar to POSIX mqd_t - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 *
 * \tparam T is the type of data in queue
 *
 * \ingroup queues
 */

template<typename T>
class MessageQueue
{
public:

	/// type of uninitialized storage for Entry with link
	using EntryStorage = internal::MessageQueueBase::EntryStorage;

	/// type of uninitialized storage for value
	using ValueStorage = internal::MessageQueueBase::ValueStorage<T>;

	/// type of data in queue
	using ValueType = T;

	/// import EntryStorageUniquePointer type from internal::MessageQueueBase class
	using EntryStorageUniquePointer = internal::MessageQueueBase::EntryStorageUniquePointer;

	/// unique_ptr (with deleter) to ValueStorage[]
	using ValueStorageUniquePointer =
			std::unique_ptr<ValueStorage[], internal::MessageQueueBase::ValueStorageUniquePointer::deleter_type>;

	/**
	 * \brief MessageQueue's constructor
	 *
	 * \param [in] entryStorageUniquePointer is a rvalue reference to EntryStorageUniquePointer with storage for queue
	 * entries (sufficiently large for \a maxElements EntryStorage objects) and appropriate deleter
	 * \param [in] valueStorageUniquePointer is a rvalue reference to ValueStorageUniquePointer with storage for queue
	 * elements (sufficiently large for \a maxElements, each sizeof(T) bytes long) and appropriate deleter
	 * \param [in] maxElements is the number of elements in \a entryStorage and \a valueStorage arrays
	 */

	MessageQueue(EntryStorageUniquePointer&& entryStorageUniquePointer,
			ValueStorageUniquePointer&& valueStorageUniquePointer, const size_t maxElements) :
			messageQueueBase_{std::move(entryStorageUniquePointer),
					{valueStorageUniquePointer.release(), valueStorageUniquePointer.get_deleter()},
					sizeof(*valueStorageUniquePointer.get()), maxElements}
	{

	}

	/**
	 * \brief MessageQueue's destructor
	 *
	 * Pops all remaining elements from the queue.
	 */

	~MessageQueue();

	/**
	 * \brief Emplaces the element in the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int emplace(const uint8_t priority, Args&&... args)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return emplaceInternal(semaphoreWaitFunctor, priority, std::forward<Args>(args)...);
	}

	/**
	 * \return maximum number of elements in queue
	 */

	size_t getCapacity() const
	{
		return messageQueueBase_.getCapacity();
	}

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(uint8_t& priority, T& value)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return popInternal(semaphoreWaitFunctor, priority, value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const uint8_t priority, const T& value)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, priority, value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const uint8_t priority, T&& value)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, priority, std::move(value));
	}

	/**
	 * \brief Tries to emplace the element in the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int tryEmplace(const uint8_t priority, Args&&... args)
	{
		const internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return emplaceInternal(semaphoreTryWaitFunctor, priority, std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue for a given duration of time.
	 *
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without emplacing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int tryEmplaceFor(const TickClock::duration duration, const uint8_t priority, Args&&... args)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return emplaceInternal(semaphoreTryWaitForFunctor, priority, std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue for a given duration of time.
	 *
	 * Template variant of MessageQueue::tryEmplaceFor(TickClock::duration, uint8_t, Args&&...).
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without emplacing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period, typename... Args>
	int tryEmplaceFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, Args&&... args)
	{
		return tryEmplaceFor(std::chrono::duration_cast<TickClock::duration>(duration), priority,
				std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue until a given time point.
	 *
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without emplacing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int tryEmplaceUntil(const TickClock::time_point timePoint, const uint8_t priority, Args&&... args)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return emplaceInternal(semaphoreTryWaitUntilFunctor, priority, std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue until a given time point.
	 *
	 * Template variant of FifoQueue::tryEmplaceUntil(TickClock::time_point, uint8_t, Args&&...).
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without emplacing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration, typename... Args>
	int tryEmplaceUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			Args&&... args)
	{
		return tryEmplaceUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority,
				std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to pop oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPop(uint8_t& priority, T& value)
	{
		const internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return popInternal(semaphoreTryWaitFunctor, priority, value);
	}

	/**
	 * \brief Tries to pop oldest element with highest priority from the queue for a given duration of time.
	 *
	 * Similar to mq_timedreceive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopFor(const TickClock::duration duration, uint8_t& priority, T& value)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return popInternal(semaphoreTryWaitForFunctor, priority, value);
	}

	/**
	 * \brief Tries to pop oldest element with highest priority from the queue for a given duration of time.
	 *
	 * Template variant of tryPopFor(TickClock::duration, uint8_t&, T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, uint8_t& priority, T& value)
	{
		return tryPopFor(std::chrono::duration_cast<TickClock::duration>(duration), priority, value);
	}

	/**
	 * \brief Tries to pop oldest element with highest priority from the queue until a given time point.
	 *
	 * Similar to mq_timedreceive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority, T& value)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return popInternal(semaphoreTryWaitUntilFunctor, priority, value);
	}

	/**
	 * \brief Tries to pop oldest element with highest priority from the queue until a given time point.
	 *
	 * Template variant of tryPopUntil(TickClock::time_point, uint8_t&, T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, uint8_t& priority, T& value)
	{
		return tryPopUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority, value);
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const uint8_t priority, const T& value)
	{
		const internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return pushInternal(semaphoreTryWaitFunctor, priority, value);
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const uint8_t priority, T&& value)
	{
		const internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return pushInternal(semaphoreTryWaitFunctor, priority, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(const TickClock::duration duration, const uint8_t priority, const T& value)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return pushInternal(semaphoreTryWaitForFunctor, priority, value);
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Template variant of tryPushFor(TickClock::duration, uint8_t, const T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, const T& value)
	{
		return tryPushFor(std::chrono::duration_cast<TickClock::duration>(duration), priority, value);
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(const TickClock::duration duration, const uint8_t priority, T&& value)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return pushInternal(semaphoreTryWaitForFunctor, priority, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Template variant of tryPushFor(TickClock::duration, uint8_t, T&&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, T&& value)
	{
		return tryPushFor(std::chrono::duration_cast<TickClock::duration>(duration), priority, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority, const T& value)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return pushInternal(semaphoreTryWaitUntilFunctor, priority, value);
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Template variant of tryPushUntil(TickClock::time_point, uint8_t, const T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			const T& value)
	{
		return tryPushUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority, value);
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority, T&& value)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return pushInternal(semaphoreTryWaitUntilFunctor, priority, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Template variant of tryPushUntil(TickClock::time_point, uint8_t, T&&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority, T&& value)
	{
		return tryPushUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), priority, std::move(value));
	}

private:

	/**
	 * \brief Emplaces the element in the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int emplaceInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, Args&&... args);

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, T& value);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, const T& value);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, T&& value);

	/// contained internal::MessageQueueBase object which implements whole functionality
	internal::MessageQueueBase messageQueueBase_;
};

template<typename T>
MessageQueue<T>::~MessageQueue()
{
	uint8_t priority;
	T value;
	while (tryPop(priority, value) == 0);
}

template<typename T>
template<typename... Args>
int MessageQueue<T>::emplaceInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		Args&&... args)
{
	const auto emplaceFunctor = internal::makeBoundQueueFunctor(
			[&args...](void* const storage)
			{
				new (storage) T{std::forward<Args>(args)...};
			});
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, emplaceFunctor);
}

template<typename T>
int MessageQueue<T>::popInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, T& value)
{
	const internal::SwapPopQueueFunctor<T> swapPopQueueFunctor {value};
	return messageQueueBase_.pop(waitSemaphoreFunctor, priority, swapPopQueueFunctor);
}

template<typename T>
int MessageQueue<T>::pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		const T& value)
{
	const internal::CopyConstructQueueFunctor<T> copyConstructQueueFunctor {value};
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, copyConstructQueueFunctor);
}

template<typename T>
int MessageQueue<T>::pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		T&& value)
{
	const internal::MoveConstructQueueFunctor<T> moveConstructQueueFunctor {std::move(value)};
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, moveConstructQueueFunctor);
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_
