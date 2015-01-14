/**
 * \file
 * \brief MessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-14
 */

#ifndef INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_

#include "distortos/synchronization/MessageQueueBase.hpp"
#include "distortos/synchronization/SemaphoreWaitFunctor.hpp"
#include "distortos/synchronization/SemaphoreTryWaitFunctor.hpp"
#include "distortos/synchronization/SemaphoreTryWaitForFunctor.hpp"

namespace distortos
{

/// GCC 4.9 is needed for all MessageQueue::*emplace*() functions - earlier versions don't support parameter pack
/// expansion in lambdas
#define DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED	__GNUC_PREREQ(4, 9)

/**
 * \brief MessageQueue class is a message queue for thread-thread, thread-interrupt or interrupt-interrupt
 * communication. It supports multiple readers and multiple writers. It is implemented as a wrapper for
 * synchronization::MessageQueueBase.
 *
 * Similar to POSIX mqd_t - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html
 *
 * \param T is the type of data in queue
 */

template<typename T>
class MessageQueue
{
public:

	/// type of uninitialized storage for data
	using Storage = synchronization::MessageQueueBase::Storage<T>;

	/**
	 * \brief MessageQueue's constructor
	 *
	 * \param [in] storage is an array of Storage elements
	 * \param [in] maxElements is the number of elements in storage array
	 */

	MessageQueue(Storage* const storage, const size_t maxElements) :
			messageQueueBase_{storage, maxElements}
	{

	}

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Emplaces the element in the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \param Args are types of arguments for constructor of T
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return zero if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int emplace(const uint8_t priority, Args&&... args)
	{
		const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return emplaceInternal(semaphoreWaitFunctor, priority, std::forward<Args>(args)...);
	}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Pops oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int pop(uint8_t& priority, T& value)
	{
		const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return popInternal(semaphoreWaitFunctor, priority, value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const uint8_t priority, const T& value)
	{
		const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, priority, value);
	}

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a rvalue reference to object that will be pushed, value in queue's storage is
	 * move-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::wait();
	 * - error codes returned by Semaphore::post();
	 */

	int push(const uint8_t priority, T&& value)
	{
		const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
		return pushInternal(semaphoreWaitFunctor, priority, std::move(value));
	}

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Tries to emplace the element in the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \param Args are types of arguments for constructor of T
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return zero if element was emplaced successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int tryEmplace(const uint8_t priority, Args&&... args)
	{
		const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return emplaceInternal(semaphoreTryWaitFunctor, priority, std::forward<Args>(args)...);
	}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Tries to pop oldest element with highest priority from the queue.
	 *
	 * Similar to mq_receive() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
	 *
	 * \param [out] priority is a reference to variable that will be used to return priority of popped value
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 *
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPop(uint8_t& priority, T& value)
	{
		const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return popInternal(semaphoreTryWaitFunctor, priority, value);
	}

	/**
	 * \brief Tries to push the element to the queue.
	 *
	 * Similar to mq_send() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const uint8_t priority, const T& value)
	{
		const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
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
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWait();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPush(const uint8_t priority, T&& value)
	{
		const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
		return pushInternal(semaphoreTryWaitFunctor, priority, std::move(value));
	}

	/**
	 * \brief Tries to push the element to the queue for a given duration of time.
	 *
	 * Similar to mq_timedsend() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html#
	 *
	 * \param [in] duration is the duration after which the wait will be terminated without pushing the element
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by Semaphore::tryWaitFor();
	 * - error codes returned by Semaphore::post();
	 */

	int tryPushFor(const TickClock::duration duration, const uint8_t priority, const T& value)
	{
		const synchronization::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
		return pushInternal(semaphoreTryWaitForFunctor, priority, value);
	}

private:

	/**
	 * \brief BoundedFunctor is a type-erased synchronization::MessageQueueBase::Functor which calls its bounded functor
	 * to execute actions on queue's storage
	 *
	 * \param F is the type of bounded functor, it will be called with <em>void*</em> as only argument
	 */

	template<typename F>
	class BoundedFunctor : public synchronization::MessageQueueBase::Functor
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
		 * swapping, destroying, emplacing, ...)
		 *
		 * \param [in,out] storage is a pointer to storage with/for element
		 */

		virtual void operator()(void* storage) const override
		{
			boundedFunctor_(storage);
		}

	private:

		/// bounded functor
		F boundedFunctor_;
	};

	/**
	 * \brief Helper factory function to make BoundedFunctor object with partially deduced template arguments
	 *
	 * \param F is the type of bounded functor, it will be called with <em>void*</em> as only argument
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

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Emplaces the element in the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \note This function requires GCC 4.9.
	 *
	 * \param Args are types of arguments for constructor of T
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] args are arguments for constructor of T
	 *
	 * \return zero if element was emplaced successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	template<typename... Args>
	int emplaceInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority,
			Args&&... args);

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

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
	 * \return zero if element was popped successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int popInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority, T& value);

	/**
	 * \brief Pushes the element to the queue.
	 *
	 * Internal version - builds the Functor object.
	 *
	 * \param [in] waitSemaphoreFunctor is a reference to SemaphoreFunctor which will be executed with \a pushSemaphore_
	 * \param [in] priority is the priority of new element
	 * \param [in] value is a reference to object that will be pushed, value in queue's storage is copy-constructed
	 *
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, const T& value);

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
	 * \return zero if element was pushed successfully, error code otherwise:
	 * - error codes returned by \a waitSemaphoreFunctor's operator() call;
	 * - error codes returned by Semaphore::post();
	 */

	int pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t priority, T&& value);

	/// contained synchronization::MessageQueueBase object which implements whole functionality
	synchronization::MessageQueueBase messageQueueBase_;
};

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

template<typename T>
template<typename... Args>
int MessageQueue<T>::emplaceInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor,
		const uint8_t priority, Args&&... args)
{
	const auto emplaceFunctor = makeBoundedFunctor(
			[&args...](void* const storage)
			{
				new (storage) T{std::forward<Args>(args)...};
			});
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, emplaceFunctor);
}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

template<typename T>
int MessageQueue<T>::popInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority,
		T& value)
{
	const auto swapFunctor = makeBoundedFunctor(
			[&value](void* const storage)
			{
				auto& swappedValue = *reinterpret_cast<T*>(storage);
				using std::swap;
				swap(value, swappedValue);
				swappedValue.~T();
			});
	return messageQueueBase_.pop(waitSemaphoreFunctor, priority, swapFunctor);
}

template<typename T>
int MessageQueue<T>::pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		const T& value)
{
	const auto copyFunctor = makeBoundedFunctor(
			[&value](void* const storage)
			{
				new (storage) T{value};
			});
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, copyFunctor);
}

template<typename T>
int MessageQueue<T>::pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		T&& value)
{
	const auto moveFunctor = makeBoundedFunctor(
			[&value](void* const storage)
			{
				new (storage) T{std::move(value)};
			});
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, moveFunctor);
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MESSAGEQUEUE_HPP_
