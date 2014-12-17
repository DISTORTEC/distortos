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
 * \date 2014-12-18
 */

#ifndef INCLUDE_DISTORTOS_FIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_FIFOQUEUE_HPP_

#include "distortos/scheduler/FifoQueueBase.hpp"

#include "distortos/scheduler/SemaphoreWaitFunctor.hpp"
#include "distortos/scheduler/SemaphoreTryWaitFunctor.hpp"
#include "distortos/scheduler/SemaphoreTryWaitForFunctor.hpp"
#include "distortos/scheduler/SemaphoreTryWaitUntilFunctor.hpp"

namespace distortos
{

/**
 * \brief FifoQueue class is a simple FIFO queue for thread-thread, thread-interrupt or interrupt-interrupt
 * communication. It supports multiple readers and multiple writers. It is implemented as a wrapper for
 * scheduler::FifoQueueBase.
 *
 * \param T is the type of data in queue
 */

template<typename T>
class FifoQueue
{
public:

	/// type of uninitialized storage for data
	using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	/**
	 * \brief FifoQueue's constructor
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in storage array
	 */

	FifoQueue(Storage* const storage, const size_t maxElements) :
			fifoQueueBase_{storage, storage + maxElements, maxElements}
	{

	}

	/**
	 * \brief Pops the oldest (first) element from the queue.
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
		const scheduler::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return popInternal(semaphoreWaitFunctor, value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const T& value)
	{
		const scheduler::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, value);
	}

	/**
	 * \brief Pushes the element to the queue.
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
		const scheduler::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, std::move(value));
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue.
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
		scheduler::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return popInternal(semaphoreTryWaitFunctor, value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue for a given duration of time.
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
		const scheduler::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return popInternal(semaphoreTryWaitForFunctor, value);
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
		const scheduler::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return popInternal(semaphoreTryWaitUntilFunctor, value);
	}

	/**
	 * \brief Tries to pop the oldest (first) element from the queue until a given time point.
	 *
	 * Template variant of tryPopUntil(TickClock::time_point, T&).
	 *
	 * \param Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without popping the element
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
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
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const T& value)
	{
		const scheduler::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return pushInternal(semaphoreTryWaitFunctor, value);
	}

	/**
	 * \brief Tries to push the element to the queue.
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
		const scheduler::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return pushInternal(semaphoreTryWaitFunctor, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
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
		const scheduler::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return pushInternal(semaphoreTryWaitForFunctor, value);
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Template variant of tryPushFor(TickClock::duration, const T&).
	 *
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
		const scheduler::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return pushInternal(semaphoreTryWaitForFunctor, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Template variant of tryPushFor(TickClock::duration, T&&).
	 *
	 * \param Rep is type of tick counter
	 * \param Period is std::ratio type representing the tick period of the clock, in seconds
	 *
	 * \param [in] duration is the duration after which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitUntil();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushUntil(const TickClock::time_point timePoint, const T& value)
	{
		const scheduler::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return pushInternal(semaphoreTryWaitUntilFunctor, value);
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Template variant of tryPushUntil(TickClock::time_point, const T&).
	 *
	 * \param Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
		const scheduler::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
		return pushInternal(semaphoreTryWaitUntilFunctor, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue until a given time point.
	 *
	 * Template variant of tryPushUntil(TickClock::time_point, T&&).
	 *
	 * \param Duration is a std::chrono::duration type used to measure duration
	 *
	 * \param [in] timePoint is the time point at which the call will be terminated without pushing the element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
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
	 * \brief BoundedFunctor is a type-erased scheduler::FifoQueueBase::Functor which calls its bounded functor to
	 * execute actions on queue's storage and deals with the pointer increments
	 *
	 * \param F is the type of bounded functor, it will be called with <em>Storage*</em> as only argument
	 */

	template<typename F>
	class BoundedFunctor : public scheduler::FifoQueueBase::Functor
	{
	public:

		/**
		 * \brief BoundedFunctor's constructor
		 *
		 * \param [in] boundedFunctor is a rvalue reference to bounded functor which will be used to move-construct
		 * internal bounded functor
		 */

		constexpr explicit BoundedFunctor(F&& boundedFunctor) :
				boundedFunctor_{std::move(boundedFunctor)}
		{

		}

		/**
		 * \brief Calls the bounded functor which will execute some action on queue's storage (like copy-constructing,
		 * swapping, destroying, emplacing, ...) and increments the storage pointer to next position (using the actual
		 * size of element)
		 *
		 * \param [in,out] storage is a reference to pointer to queue's storage - after executing bounded functor, the
		 * pointer will be incremented to next position (using the actual size of element)
		 */

		virtual void operator()(void*& storage) const override
		{
			auto typedStorage = static_cast<Storage*>(storage);
			boundedFunctor_(typedStorage);
			storage = typedStorage + 1;
		}

	private:

		/// bounded functor
		F boundedFunctor_;
	};

	/**
	 * \brief Helper factory function to make BoundedFunctor object with partially deduced template arguments
	 *
	 * \param F is the type of bounded functor, it will be called with <em>Storage*</em> as only argument
	 *
	 * \param [in] boundedFunctor is a rvalue reference to bounded functor which will be used to move-construct internal
	 * bounded functor
	 *
	 * \return BoundedFunctor object with partially deduced template arguments
	 */

	template<typename F>
	constexpr static BoundedFunctor<F> makeBoundedFunctor(F&& boundedFunctor)
	{
		return BoundedFunctor<F>{std::move(boundedFunctor)};
	}

	/**
	 * \brief Pops the oldest (first) element from the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a popSemaphore_
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, T& value);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, const T& value);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, T&& value);

	/// contained scheduler::FifoQueueBase object which implements whole functionality
	scheduler::FifoQueueBase fifoQueueBase_;
};

template<typename T>
int FifoQueue<T>::popInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, T& value)
{
	const auto swapFunctor = makeBoundedFunctor(
			[&value](Storage* const storage)
			{
				auto& swappedValue = *reinterpret_cast<T*>(storage);
				using std::swap;
				swap(value, swappedValue);
				swappedValue.~T();
			});
	return fifoQueueBase_.pop(waitSemaphoreFunctor, swapFunctor);
}

template<typename T>
int FifoQueue<T>::pushInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, const T& value)
{
	const auto copyFunctor = makeBoundedFunctor(
			[&value](Storage* const storage)
			{
				new (storage) T{value};
			});
	return fifoQueueBase_.push(waitSemaphoreFunctor, copyFunctor);
}

template<typename T>
int FifoQueue<T>::pushInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, T&& value)
{
	const auto moveFunctor = makeBoundedFunctor(
			[&value](Storage* const storage)
			{
				new (storage) T{std::move(value)};
			});
	return fifoQueueBase_.push(waitSemaphoreFunctor, moveFunctor);
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FIFOQUEUE_HPP_
