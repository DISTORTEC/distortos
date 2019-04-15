/**
 * \file
 * \brief FifoQueue class header
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_FIFOQUEUE_HPP_

#include "distortos/internal/synchronization/FifoQueueBase.hpp"
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
 * \brief FifoQueue class is a simple FIFO queue for thread-thread, thread-interrupt or interrupt-interrupt
 * communication. It supports multiple readers and multiple writers. It is implemented as a wrapper for
 * internal::FifoQueueBase.
 *
 * \tparam T is the type of data in queue
 *
 * \ingroup queues
 */

template<typename T>
class FifoQueue
{
public:

	/// type of uninitialized storage for data
	using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	/// unique_ptr (with deleter) to Storage[]
	using StorageUniquePointer =
			std::unique_ptr<Storage[], internal::FifoQueueBase::StorageUniquePointer::deleter_type>;

	/// type of data in queue
	using ValueType = T;

	/**
	 * \brief FifoQueue's constructor
	 *
	 * \param [in] storageUniquePointer is a rvalue reference to StorageUniquePointer with storage for queue elements
	 * (sufficiently large for \a maxElements, each sizeof(T) bytes long) and appropriate deleter
	 * \param [in] maxElements is the number of elements in storage array
	 */

	FifoQueue(StorageUniquePointer&& storageUniquePointer, const size_t maxElements) :
			fifoQueueBase_{{storageUniquePointer.release(), storageUniquePointer.get_deleter()}, sizeof(T), maxElements}
	{

	}

	/**
	 * \brief FifoQueue's destructor
	 *
	 * Pops all remaining elements from the queue.
	 */

	~FifoQueue();

	/**
	 * \brief Emplaces the element in the queue.
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int emplace(Args&&... args)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return emplaceInternal(semaphoreWaitFunctor, std::forward<Args>(args)...);
	}

	/**
	 * \brief Pops the oldest (first) element from the queue.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(T& value)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return popInternal(semaphoreWaitFunctor, value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const T& value)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(T&& value)
	{
		const internal::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, std::move(value));
	}

	/**
	 * \brief Tries to emplace the element in the queue.
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int tryEmplace(Args&&... args)
	{
		const internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return emplaceInternal(semaphoreTryWaitFunctor, std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue for a given duration of time.
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without emplacing the element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int tryEmplaceFor(const TickClock::duration duration, Args&&... args)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return emplaceInternal(semaphoreTryWaitForFunctor, std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue for a given duration of time.
	 *
	 * Template variant of FifoQueue::tryEmplaceFor(TickClock::duration, Args&&...).
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
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period, typename... Args>
	int tryEmplaceFor(const std::chrono::duration<Rep, Period> duration, Args&&... args)
	{
		return tryEmplaceFor(std::chrono::duration_cast<TickClock::duration>(duration), std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue until a given time point.
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without emplacing the element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int tryEmplaceUntil(const TickClock::time_point timePoint, Args&&... args)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return emplaceInternal(semaphoreTryWaitUntilFunctor, std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to emplace the element in the queue until a given time point.
	 *
	 * Template variant of FifoQueue::tryEmplaceUntil(TickClock::time_point, Args&&...).
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without emplacing the element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration, typename... Args>
	int tryEmplaceUntil(const std::chrono::time_point<TickClock, Duration> timePoint, Args&&... args)
	{
		return tryEmplaceUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint),
				std::forward<Args>(args)...);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue.
	 *
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPop(T& value)
	{
		internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return popInternal(semaphoreTryWaitFunctor, value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue for a given duration of time.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopFor(const TickClock::duration duration, T& value)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return popInternal(semaphoreTryWaitForFunctor, value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue for a given duration of time.
	 *
	 * Template variant of tryPopFor(TickClock::duration, T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
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
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPopUntil(const TickClock::time_point timePoint, T& value)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return popInternal(semaphoreTryWaitUntilFunctor, value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue until a given time point.
	 *
	 * Template variant of tryPopUntil(TickClock::time_point, T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, T& value)
	{
		return tryPopUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), value);
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const T& value)
	{
		const internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return pushInternal(semaphoreTryWaitFunctor, value);
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(T&& value)
	{
		const internal::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return pushInternal(semaphoreTryWaitFunctor, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(const TickClock::duration duration, const T& value)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return pushInternal(semaphoreTryWaitForFunctor, value);
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Template variant of tryPushFor(TickClock::duration, const T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const T& value)
	{
		return tryPushFor(std::chrono::duration_cast<TickClock::duration>(duration), value);
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] duration is the duration after which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(const TickClock::duration duration, T&& value)
	{
		const internal::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return pushInternal(semaphoreTryWaitForFunctor, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Template variant of tryPushFor(TickClock::duration, T&&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Rep is type of tick counter
	 * \tparam Period is std::ratio type representing the tick period of the clock, seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, T&& value)
	{
		return tryPushFor(std::chrono::duration_cast<TickClock::duration>(duration), std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(const TickClock::time_point timePoint, const T& value)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return pushInternal(semaphoreTryWaitUntilFunctor, value);
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Template variant of tryPushUntil(TickClock::time_point, const T&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const T& value)
	{
		return tryPushUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), value);
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(const TickClock::time_point timePoint, T&& value)
	{
		const internal::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return pushInternal(semaphoreTryWaitUntilFunctor, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Template variant of tryPushUntil(TickClock::time_point, T&&).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \tparam Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, T&& value)
	{
		return tryPushUntil(std::chrono::time_point_cast<TickClock::duration>(timePoint), std::move(value));
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
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return 0 if element was emplaced successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int emplaceInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, Args&&... args);

	/**
	 * \brief Pops the oldest (first) element from the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return 0 if element was popped successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, T& value);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, const T& value);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return 0 if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, T&& value);

	/// contained internal::FifoQueueBase object which implements whole functionality
	internal::FifoQueueBase fifoQueueBase_;
};

template<typename T>
FifoQueue<T>::~FifoQueue()
{
	T value;
	while (tryPop(value) == 0);
}

template<typename T>
template<typename... Args>
int FifoQueue<T>::emplaceInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, Args&&... args)
{
	const auto emplaceFunctor = internal::makeBoundQueueFunctor(
			[&args...](void* const storage)
			{
				new (storage) T{std::forward<Args>(args)...};
			});
	return fifoQueueBase_.push(waitSemaphoreFunctor, emplaceFunctor);
}

template<typename T>
int FifoQueue<T>::popInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, T& value)
{
	const internal::SwapPopQueueFunctor<T> swapPopQueueFunctor {value};
	return fifoQueueBase_.pop(waitSemaphoreFunctor, swapPopQueueFunctor);
}

template<typename T>
int FifoQueue<T>::pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, const T& value)
{
	const internal::CopyConstructQueueFunctor<T> copyConstructQueueFunctor {value};
	return fifoQueueBase_.push(waitSemaphoreFunctor, copyConstructQueueFunctor);
}

template<typename T>
int FifoQueue<T>::pushInternal(const internal::SemaphoreFunctor& waitSemaphoreFunctor, T&& value)
{
	const internal::MoveConstructQueueFunctor<T> moveConstructQueueFunctor {std::move(value)};
	return fifoQueueBase_.push(waitSemaphoreFunctor, moveConstructQueueFunctor);
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FIFOQUEUE_HPP_
