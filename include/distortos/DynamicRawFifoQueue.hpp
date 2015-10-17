/**
 * \file
 * \brief DynamicRawFifoQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-17
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICRAWFIFOQUEUE_HPP_
#define INCLUDE_DISTORTOS_DYNAMICRAWFIFOQUEUE_HPP_

#include "RawFifoQueue.hpp"

#include <memory>

namespace distortos
{

/**
 * \brief DynamicRawFifoQueue class is a wrapper for RawFifoQueue that also provides dynamic storage for queue's
 * contents.
 *
 * \note Objects of this class can be safely casted to (const) reference to RawFifoQueue.
 */

class DynamicRawFifoQueue
{
public:

	/// unique_ptr to uninitialized storage for data
	using StorageUniquePointer = std::unique_ptr<uint8_t>;

	/**
	 * \brief DynamicRawFifoQueue's constructor
	 *
	 * \param [in] elementSize is the size of single queue element, bytes
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	DynamicRawFifoQueue(size_t elementSize, size_t queueSize);

	/**
	 * \brief DynamicRawFifoQueue's destructor
	 */

	~DynamicRawFifoQueue();

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

	template<typename T>
	int pop(T& buffer)
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

	template<typename T>
	int push(const T& data)
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

	template<typename T>
	int tryPop(T& buffer)
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

	template<typename Rep, typename Period, typename T>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, T& buffer)
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

	template<typename Duration, typename T>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, T& buffer)
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

	template<typename T>
	int tryPush(const T& data)
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

	template<typename Rep, typename Period, typename T>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const T& data)
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

	template<typename Duration, typename T>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const T& data)
	{
		return rawFifoQueue_.tryPushUntil(timePoint, data);
	}

	DynamicRawFifoQueue(const DynamicRawFifoQueue&) = delete;
	DynamicRawFifoQueue(DynamicRawFifoQueue&&) = default;
	const DynamicRawFifoQueue& operator=(const DynamicRawFifoQueue&) = delete;
	DynamicRawFifoQueue& operator=(DynamicRawFifoQueue&&) = delete;

private:

	/// unique_ptr to allocated storage for queue's contents
	StorageUniquePointer storageUniquePointer_;

	/// internal RawFifoQueue object
	RawFifoQueue rawFifoQueue_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICRAWFIFOQUEUE_HPP_
