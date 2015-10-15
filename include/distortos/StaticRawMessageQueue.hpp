
/**
 * \file
 * \brief StaticRawMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-15
 */

#ifndef INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_

#include "distortos/RawMessageQueue.hpp"

namespace distortos
{

/**
 * \brief StaticRawMessageQueue class is a wrapper for RawMessageQueue that also provides automatic storage for queue's
 * contents.
 *
 * \note Objects of this class can be safely casted to (const) reference to RawMessageQueue.
 *
 * \param T is the type of data in queue
 * \param QueueSize is the maximum number of elements in queue
 */

template<typename T, size_t QueueSize>
class StaticRawMessageQueue
{
public:

	/**
	 * \brief StaticRawMessageQueue's constructor
	 */

	explicit StaticRawMessageQueue() :
			rawMessageQueue_{entryStorage_, valueStorage_}
	{

	}

	/**
	 * \brief conversion to RawMessageQueue&
	 *
	 * \return reference to internal RawMessageQueue object
	 */

	operator RawMessageQueue&()
	{
		return rawMessageQueue_;
	}

	/**
	 * \brief conversion to const RawMessageQueue&
	 *
	 * \return const reference to internal RawMessageQueue object
	 */

	operator const RawMessageQueue&() const
	{
		return rawMessageQueue_;
	}

	/**
	 * \brief Wrapper for RawMessageQueue::pop(uint8_t&, void*, size_t)
	 */

	int pop(uint8_t& priority, void* const buffer, const size_t size)
	{
		return rawMessageQueue_.pop(priority, buffer, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::pop(uint8_t&, T&)
	 */

	template<typename U>
	int pop(uint8_t& priority, U& buffer)
	{
		return rawMessageQueue_.pop(priority, buffer);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::push(uint8_t, const void*, size_t)
	 */

	int push(const uint8_t priority, const void* const data, const size_t size)
	{
		return rawMessageQueue_.push(priority, data, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::push(uint8_t, const T&)
	 */

	template<typename U>
	int push(const uint8_t priority, const U& data)
	{
		return rawMessageQueue_.push(priority, data);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPop(uint8_t&, void*, size_t)
	 */

	int tryPop(uint8_t& priority, void* const buffer, const size_t size)
	{
		return rawMessageQueue_.tryPop(priority, buffer, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPop(uint8_t&, T&)
	 */

	template<typename U>
	int tryPop(uint8_t& priority, U& buffer)
	{
		return rawMessageQueue_.tryPop(priority, buffer);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPopFor(std::chrono::duration<Rep, Period>, uint8_t&, void*, size_t)
	 */

	template<typename Rep, typename Period>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, uint8_t& priority, void* const buffer,
			const size_t size)
	{
		return rawMessageQueue_.tryPopFor(duration, priority, buffer, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPopFor(std::chrono::duration<Rep, Period>, uint8_t&, T&)
	 */

	template<typename Rep, typename Period, typename U>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, uint8_t& priority, U& buffer)
	{
		return rawMessageQueue_.tryPopFor(duration, priority, buffer);
	}

	/**
	 * \brief Wrapper for
	 * RawMessageQueue::tryPopUntil(std::chrono::time_point<TickClock, Duration>, uint8_t&, void*, size_t)
	 */

	template<typename Duration>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, uint8_t& priority, void* const buffer,
			const size_t size)
	{
		return rawMessageQueue_.tryPopUntil(timePoint, priority, buffer, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPopUntil(std::chrono::time_point<TickClock, Duration>, uint8_t&, T&)
	 */

	template<typename Duration, typename U>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, uint8_t& priority, U& buffer)
	{
		return rawMessageQueue_.tryPopUntil(timePoint, priority, buffer);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPush(uint8_t, const void*, size_t)
	 */

	int tryPush(const uint8_t priority, const void* const data, const size_t size)
	{
		return rawMessageQueue_.tryPush(priority, data, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPush(uint8_t, const T&)
	 */

	template<typename U>
	int tryPush(const uint8_t priority, const U& data)
	{
		return rawMessageQueue_.tryPush(priority, data);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPushFor(std::chrono::duration<Rep, Period>, uint8_t, const void*, size_t)
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, const void* const data,
			const size_t size)
	{
		return rawMessageQueue_.tryPushFor(duration, priority, data, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPushFor(std::chrono::duration<Rep, Period>, uint8_t, const T&)
	 */

	template<typename Rep, typename Period, typename U>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, const U& data)
	{
		return rawMessageQueue_.tryPushFor(duration, priority, data);
	}

	/**
	 * \brief Wrapper for
	 * RawMessageQueue::tryPushUntil(std::chrono::time_point<TickClock, Duration>, uint8_t, const void*, size_t)
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			const void* const data, const size_t size)
	{
		return rawMessageQueue_.tryPushUntil(timePoint, priority, data, size);
	}

	/**
	 * \brief Wrapper for RawMessageQueue::tryPushUntil(std::chrono::time_point<TickClock, Duration>, uint8_t, const T&)
	 */

	template<typename Duration, typename U>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			const U& data)
	{
		return rawMessageQueue_.tryPushUntil(timePoint, priority, data);
	}

private:

	/// storage for queue's entries
	std::array<RawMessageQueue::EntryStorage, QueueSize> entryStorage_;

	/// storage for queue's contents
	std::array<typename std::aligned_storage<sizeof(T), alignof(T)>::type, QueueSize> valueStorage_;

	/// internal RawMessageQueue object
	RawMessageQueue rawMessageQueue_;
};

/**
 * \brief StaticRawMessageQueueFromSize type alias is a variant of StaticRawMessageQueue which uses size of element
 * (instead of type) as template argument.
 *
 * \param ElementSize is the size of single queue element, bytes
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t ElementSize, size_t QueueSize>
using StaticRawMessageQueueFromSize =
		StaticRawMessageQueue<typename std::aligned_storage<ElementSize, ElementSize>::type, QueueSize>;

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICRAWMESSAGEQUEUE_HPP_
