/**
 * \file
 * \brief StaticRawFifoQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-21
 */

#ifndef INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_

#include "RawFifoQueue.hpp"

#include "distortos/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticRawFifoQueue class is a wrapper for RawFifoQueue that also provides automatic storage for queue's
 * contents.
 *
 * \note Objects of this class can be safely casted to (const) reference to RawFifoQueue.
 *
 * \param T is the type of data in queue
 * \param QueueSize is the maximum number of elements in queue
 */

template<typename T, size_t QueueSize>
class StaticRawFifoQueue
{
public:

	/**
	 * \brief StaticRawFifoQueue's constructor
	 */

	explicit StaticRawFifoQueue() :
			rawFifoQueue_{{storage_.data(), memory::dummyDeleter}, sizeof(*storage_.data()), storage_.size()}
	{

	}

	/**
	 * \brief conversion to RawFifoQueue&
	 *
	 * \return reference to internal RawFifoQueue object
	 */

	operator RawFifoQueue&()
	{
		return rawFifoQueue_;
	}

	/**
	 * \brief conversion to const RawFifoQueue&
	 *
	 * \return const reference to internal RawFifoQueue object
	 */

	operator const RawFifoQueue&() const
	{
		return rawFifoQueue_;
	}

	/**
	 * \brief Wrapper for RawFifoQueue::pop(void*, size_t)
	 */

	int pop(void* const buffer, const size_t size)
	{
		return rawFifoQueue_.pop(buffer, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::pop(T&)
	 */

	template<typename U>
	int pop(U& buffer)
	{
		return rawFifoQueue_.pop(buffer);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::push(const void*, size_t)
	 */

	int push(const void* const data, const size_t size)
	{
		return rawFifoQueue_.push(data, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::push(const T&)
	 */

	template<typename U>
	int push(const U& data)
	{
		return rawFifoQueue_.push(data);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPop(void*, size_t)
	 */

	int tryPop(void* const buffer, const size_t size)
	{
		return rawFifoQueue_.tryPop(buffer, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPop(T&)
	 */

	template<typename U>
	int tryPop(U& buffer)
	{
		return rawFifoQueue_.tryPop(buffer);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopFor(std::chrono::duration<Rep, Period>, void*, size_t)
	 */

	template<typename Rep, typename Period>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, void* const buffer, const size_t size)
	{
		return rawFifoQueue_.tryPopFor(duration, buffer, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopFor(std::chrono::duration<Rep, Period>, T&)
	 */

	template<typename Rep, typename Period, typename U>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, U& buffer)
	{
		return rawFifoQueue_.tryPopFor(duration, buffer);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopUntil(std::chrono::time_point<TickClock, Duration>, void*, size_t)
	 */

	template<typename Duration>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, void* const buffer, const size_t size)
	{
		return rawFifoQueue_.tryPopUntil(timePoint, buffer, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopUntil(std::chrono::time_point<TickClock, Duration>, T&)
	 */

	template<typename Duration, typename U>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, U& buffer)
	{
		return rawFifoQueue_.tryPopUntil(timePoint, buffer);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPush(const void*, size_t)
	 */

	int tryPush(const void* const data, const size_t size)
	{
		return rawFifoQueue_.tryPush(data, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPush(const T&)
	 */

	template<typename U>
	int tryPush(const U& data)
	{
		return rawFifoQueue_.tryPush(data);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushFor(std::chrono::duration<Rep, Period>, const void*, size_t)
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const void* const data, const size_t size)
	{
		return rawFifoQueue_.tryPushFor(duration, data, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushFor(std::chrono::duration<Rep, Period>, const T&)
	 */

	template<typename Rep, typename Period, typename U>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const U& data)
	{
		return rawFifoQueue_.tryPushFor(duration, data);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushUntil(std::chrono::time_point<TickClock, Duration>, const void*, size_t)
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const void* const data,
			const size_t size)
	{
		return rawFifoQueue_.tryPushUntil(timePoint, data, size);
	}

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushUntil(std::chrono::time_point<TickClock, Duration>, const T&)
	 */

	template<typename Duration, typename U>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const U& data)
	{
		return rawFifoQueue_.tryPushUntil(timePoint, data);
	}

private:

	/// storage for queue's contents
	std::array<typename std::aligned_storage<sizeof(T), alignof(T)>::type, QueueSize> storage_;

	/// internal RawFifoQueue object
	RawFifoQueue rawFifoQueue_;
};

/**
 * \brief StaticRawFifoQueueFromSize type alias is a variant of StaticRawFifoQueue which uses size of element (instead
 * of type) as template argument.
 *
 * \param ElementSize is the size of single queue element, bytes
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t ElementSize, size_t QueueSize>
using StaticRawFifoQueueFromSize =
		StaticRawFifoQueue<typename std::aligned_storage<ElementSize, ElementSize>::type, QueueSize>;

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWFIFOQUEUE_HPP_
