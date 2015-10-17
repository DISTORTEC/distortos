/**
 * \file
 * \brief StaticMessageQueue class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-17
 */

#ifndef INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_
#define INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_

#include "MessageQueue.hpp"

namespace distortos
{

/**
 * \brief StaticMessageQueue class is a wrapper for MessageQueue that also provides automatic storage for queue's
 * contents.
 *
 * \note Objects of this class can be safely casted to (const) reference to MessageQueue.
 *
 * \param T is the type of data in queue
 * \param QueueSize is the maximum number of elements in queue
 */

template<typename T, size_t QueueSize>
class StaticMessageQueue
{
public:

	/**
	 * \brief StaticMessageQueue's constructor
	 */

	explicit StaticMessageQueue() :
			messageQueue_{entryStorage_, valueStorage_}
	{

	}

	/**
	 * \brief conversion to MessageQueue<T>&
	 *
	 * \return reference to internal MessageQueue<T> object
	 */

	operator MessageQueue<T>&()
	{
		return messageQueue_;
	}

	/**
	 * \brief conversion to const MessageQueue<T>&
	 *
	 * \return const reference to internal MessageQueue<T> object
	 */

	operator const MessageQueue<T>&() const
	{
		return messageQueue_;
	}

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::emplace()
	 */

	template<typename... Args>
	int emplace(const uint8_t priority, Args&&... args)
	{
		return messageQueue_.emplace(priority, std::forward<Args>(args)...);
	}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::pop()
	 */

	int pop(uint8_t& priority, T& value)
	{
		return messageQueue_.pop(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::push(uint8_t, const T&)
	 */

	int push(const uint8_t priority, const T& value)
	{
		return messageQueue_.push(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::push(uint8_t, T&&)
	 */

	int push(const uint8_t priority, T&& value)
	{
		return messageQueue_.push(priority, std::move(value));
	}

#if DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::tryEmplace()
	 */

	template<typename... Args>
	int tryEmplace(const uint8_t priority, Args&&... args)
	{
		return messageQueue_.tryEmplace(priority, std::forward<Args>(args)...);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceFor()
	 */

	template<typename Rep, typename Period, typename... Args>
	int tryEmplaceFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, Args&&... args)
	{
		return messageQueue_.tryEmplaceFor(duration, priority, std::forward<Args>(args)...);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceUntil()
	 */

	template<typename Duration, typename... Args>
	int tryEmplaceUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			Args&&... args)
	{
		return messageQueue_.tryEmplaceUntil(timePoint, priority, std::forward<Args>(args)...);
	}

#endif	// DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::tryPop()
	 */

	int tryPop(uint8_t& priority, T& value)
	{
		return messageQueue_.tryPop(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPopFor()
	 */

	template<typename Rep, typename Period>
	int tryPopFor(const std::chrono::duration<Rep, Period> duration, uint8_t& priority, T& value)
	{
		return messageQueue_.tryPopFor(duration, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPopUntil()
	 */

	template<typename Duration>
	int tryPopUntil(const std::chrono::time_point<TickClock, Duration> timePoint, uint8_t& priority, T& value)
	{
		return messageQueue_.tryPopUntil(timePoint, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPush(uint8_t, const T&)
	 */

	int tryPush(const uint8_t priority, const T& value)
	{
		return messageQueue_.tryPush(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPush(uint8_t, T&&)
	 */

	int tryPush(const uint8_t priority, T&& value)
	{
		return messageQueue_.tryPush(priority, std::move(value));
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(std::chrono::duration<Rep, Period>, uint8_t, const T&)
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, const T& value)
	{
		return messageQueue_.tryPushFor(duration, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(std::chrono::duration<Rep, Period>, uint8_t, T&&)
	 */

	template<typename Rep, typename Period>
	int tryPushFor(const std::chrono::duration<Rep, Period> duration, const uint8_t priority, T&& value)
	{
		return messageQueue_.tryPushFor(duration, priority, std::move(value));
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(std::chrono::time_point<TickClock, Duration>, uint8_t, const T&)
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority,
			const T& value)
	{
		return messageQueue_.tryPushUntil(timePoint, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(std::chrono::time_point<TickClock, Duration>, uint8_t, T&&)
	 */

	template<typename Duration>
	int tryPushUntil(const std::chrono::time_point<TickClock, Duration> timePoint, const uint8_t priority, T&& value)
	{
		return messageQueue_.tryPushUntil(timePoint, priority, std::move(value));
	}

private:

	/// storage for queue's entries
	std::array<typename MessageQueue<T>::EntryStorage, QueueSize> entryStorage_;

	/// storage for queue's contents
	std::array<typename MessageQueue<T>::ValueStorage, QueueSize> valueStorage_;

	/// internal MessageQueue<T> object
	MessageQueue<T> messageQueue_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICMESSAGEQUEUE_HPP_
