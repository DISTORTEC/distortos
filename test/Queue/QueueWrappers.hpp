/**
 * \file
 * \brief QueueWrapper, [Non]RawQueueWrapper and [Static][Raw]{Fifo,Message}QueueWrapper classes header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-10
 */

#ifndef TEST_QUEUE_QUEUEWRAPPERS_HPP_
#define TEST_QUEUE_QUEUEWRAPPERS_HPP_

#include "OperationCountingType.hpp"

#include "distortos/StaticFifoQueue.hpp"
#include "distortos/StaticMessageQueue.hpp"
#include "distortos/StaticRawFifoQueue.hpp"
#include "distortos/StaticRawMessageQueue.hpp"

/// GCC 4.9 is needed for all {Fifo,Message}Queue::*emplace*() functions
#define DISTORTOS_QUEUE_EMPLACE_SUPPORTED	(DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED && \
		DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED)

namespace distortos
{

namespace test
{

/// wrapper for [Raw]{Fifo,Message}Queue
class QueueWrapper
{
public:

	/**
	 * \brief Tests whether pushed and popped data match.
	 *
	 * \param [in] priority1 is the priority used for *push*() or *emplace*()
	 * \param [in] value1 is a reference to OperationCountingType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to OperationCountingType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const = 0;

	/**
	 * \brief Wrapper for OperationCountingType::checkCounters().
	 *
	 * If the wrapped queue is a "raw" queue, then this function should be a stub which returns true, otherwise it
	 * should just call OperationCountingType::checkCounters().
	 */

	virtual bool checkCounters(size_t constructed, size_t copyConstructed, size_t moveConstructed, size_t destructed,
			size_t copyAssigned, size_t moveAssigned, size_t swapped) const = 0;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::emplace() or Raw{Fifo,Message}Queue::push()
	 */

	virtual int emplace(uint8_t priority, OperationCountingType::Value value = {}) const = 0;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for [Raw]{Fifo,Message}Queue::pop()
	 */

	virtual int pop(uint8_t& priority, OperationCountingType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::push(..., const OperationCountingType&) or Raw{Fifo,Message}Queue::push()
	 */

	virtual int push(uint8_t priority, const OperationCountingType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::push(..., OperationCountingType&&) or Raw{Fifo,Message}Queue::push()
	 */

	virtual int push(uint8_t priority, OperationCountingType&& value) const = 0;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryEmplace() or Raw{Fifo,Message}Queue::tryPush()
	 */

	virtual int tryEmplace(uint8_t priority, OperationCountingType::Value value = {}) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryEmplaceFor() or Raw{Fifo,Message}Queue::tryPushFor()
	 */

	virtual int tryEmplaceFor(TickClock::duration duration, uint8_t priority, OperationCountingType::Value value = {})
			const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryEmplaceUntil() or Raw{Fifo,Message}Queue::tryPushUntil()
	 */

	virtual int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority,
			OperationCountingType::Value value = {}) const = 0;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for [Raw]{Fifo,Message}Queue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, OperationCountingType& value) const = 0;

	/**
	 * \brief Wrapper for [Raw]{Fifo,Message}Queue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, OperationCountingType& value) const = 0;

	/**
	 * \brief Wrapper for [Raw]{Fifo,Message}Queue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPush(..., const OperationCountingType&) or
	 * Raw{Fifo,Message}Queue::tryPush()
	 */

	virtual int tryPush(uint8_t priority, const OperationCountingType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPush(..., OperationCountingType&&) or
	 * Raw{Fifo,Message}Queue::tryPush()
	 */

	virtual int tryPush(uint8_t priority, OperationCountingType&& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushFor(..., const OperationCountingType&) or
	 * Raw{Fifo,Message}Queue::tryPushFor()
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const OperationCountingType& value) const =
			0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushFor(..., OperationCountingType&&) or
	 * Raw{Fifo,Message}Queue::tryPushFor()
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, OperationCountingType&& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushUntil(..., const OperationCountingType&) or
	 * Raw{Fifo,Message}Queue::tryPushUntil()
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const OperationCountingType& value)
			const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushUntil(..., OperationCountingType&&) or
	 * Raw{Fifo,Message}Queue::tryPushUntil()
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType&& value) const =
			0;
};

/// common implementation of QueueWrapper for {Fifo,Message}Queue
class NonRawQueueWrapper : public QueueWrapper
{
public:

	/**
	 * \brief Wrapper for OperationCountingType::checkCounters().
	 *
	 * Just calls OperationCountingType::checkCounters().
	 */

	virtual bool checkCounters(size_t constructed, size_t copyConstructed, size_t moveConstructed, size_t destructed,
			size_t copyAssigned, size_t moveAssigned, size_t swapped) const override;
};

/// implementation of QueueWrapper for FifoQueue
class FifoQueueWrapper : public NonRawQueueWrapper
{
public:

	/// FifoQueue with \a OperationCountingType
	using TestFifoQueue = FifoQueue<OperationCountingType>;

	/**
	 * \brief FifoQueueWrapper's constructor
	 *
	 * \param [in] fifoQueue is a reference to wrapped \a TestFifoQueue object
	 */

	constexpr explicit FifoQueueWrapper(TestFifoQueue& fifoQueue) :
			fifoQueue_{fifoQueue}
	{

	}

	/**
	 * \brief Tests whether pushed and popped data match.
	 *
	 * As FifoQueue doesn't support priority, \a priority1 and \a priority2 values are ignored.
	 *
	 * \param [in] priority1 is the priority used for *push*() or *emplace*()
	 * \param [in] value1 is a reference to OperationCountingType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to OperationCountingType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::emplace()
	 */

	virtual int emplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::pop()
	 */

	virtual int pop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::push(..., const OperationCountingType&)
	 */

	virtual int push(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::push(..., OperationCountingType&&)
	 */

	virtual int push(uint8_t priority, OperationCountingType&& value) const override;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::tryEmplace()
	 */

	virtual int tryEmplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryEmplaceFor()
	 */

	virtual int tryEmplaceFor(TickClock::duration duration, uint8_t priority, OperationCountingType::Value value = {})
			const override;

	/**
	 * \brief Wrapper for FifoQueue::tryEmplaceUntil()
	 */

	virtual int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority,
			OperationCountingType::Value value = {}) const override;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value) const
			override;

	/**
	 * \brief Wrapper for FifoQueue::tryPush(..., const OperationCountingType&)
	 */

	virtual int tryPush(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPush(..., OperationCountingType&&)
	 */

	virtual int tryPush(uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushFor(..., const OperationCountingType&)
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const OperationCountingType& value) const
			override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushFor(..., OperationCountingType&&)
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, OperationCountingType&& value) const
			override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushUntil(..., const OperationCountingType&)
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const OperationCountingType& value)
			const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushUntil(..., OperationCountingType&&)
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType&& value) const
			override;

private:

	/// reference to wrapped \a TestFifoQueue object
	TestFifoQueue& fifoQueue_;
};

/**
 * \brief StaticFifoQueueWrapper class is a variant of FifoQueueWrapper that has automatic storage for queue's contents.
 *
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticFifoQueueWrapper : public FifoQueueWrapper
{
public:

	/**
	 * \brief StaticFifoQueueWrapper's constructor
	 */

	constexpr StaticFifoQueueWrapper() :
			FifoQueueWrapper{fifoQueue_}
	{

	}

private:

	/// internal StaticFifoQueue<> object that will be wrapped
	StaticFifoQueue<OperationCountingType, QueueSize> fifoQueue_;
};

/// implementation of QueueWrapper for MessageQueue
class MessageQueueWrapper : public NonRawQueueWrapper
{
public:

	/// MessageQueue with \a OperationCountingType
	using TestMessageQueue = MessageQueue<OperationCountingType>;

	/**
	 * \brief MessageQueueWrapper's constructor
	 *
	 * \param [in] messageQueue is a reference to wrapped \a TestMessageQueue object
	 */

	constexpr explicit MessageQueueWrapper(TestMessageQueue& messageQueue) :
			messageQueue_{messageQueue}
	{

	}

	/**
	 * \brief Tests whether pushed and popped data match.
	 *
	 * \param [in] priority1 is the priority used for *push*() or *emplace*()
	 * \param [in] value1 is a reference to OperationCountingType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to OperationCountingType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::emplace()
	 */

	virtual int emplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::pop()
	 */

	virtual int pop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::push(..., const OperationCountingType&)
	 */

	virtual int push(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::push(..., OperationCountingType&&)
	 */

	virtual int push(uint8_t priority, OperationCountingType&& value) const override;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::tryEmplace()
	 */

	virtual int tryEmplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceFor()
	 */

	virtual int tryEmplaceFor(TickClock::duration duration, uint8_t priority, OperationCountingType::Value value = {})
			const override;

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceUntil()
	 */

	virtual int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority,
			OperationCountingType::Value value = {}) const override;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value) const
			override;

	/**
	 * \brief Wrapper for MessageQueue::tryPush(..., const OperationCountingType&)
	 */

	virtual int tryPush(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPush(..., OperationCountingType&&)
	 */

	virtual int tryPush(uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(..., const OperationCountingType&)
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const OperationCountingType& value) const
			override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(..., OperationCountingType&&)
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, OperationCountingType&& value) const
			override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(..., const OperationCountingType&)
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const OperationCountingType& value)
			const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(..., OperationCountingType&&)
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType&& value) const
			override;

private:

	/// reference to wrapped \a TestMessageQueue object
	TestMessageQueue& messageQueue_;
};

/**
 * \brief StaticMessageQueueWrapper class is a variant of MessageQueueWrapper that has automatic storage for queue's
 * contents.
 *
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticMessageQueueWrapper : public MessageQueueWrapper
{
public:

	/**
	 * \brief StaticMessageQueueWrapper's constructor
	 */

	constexpr StaticMessageQueueWrapper() :
			MessageQueueWrapper{messageQueue_}
	{

	}

private:

	/// internal StaticMessageQueue<> object that will be wrapped
	StaticMessageQueue<OperationCountingType, QueueSize> messageQueue_;
};

/// common implementation of QueueWrapper for Raw{Fifo,Message}Queue
class RawQueueWrapper : public QueueWrapper
{
public:

	/**
	 * \brief Wrapper for OperationCountingType::checkCounters().
	 *
	 * A stub which returns true.
	 */

	virtual bool checkCounters(size_t constructed, size_t copyConstructed, size_t moveConstructed, size_t destructed,
			size_t copyAssigned, size_t moveAssigned, size_t swapped) const override;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Redirects the call to "raw" push().
	 */

	virtual int emplace(uint8_t priority, OperationCountingType::Value value = {}) const final override;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::pop()
	 */

	virtual int pop(uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" pop().
	 */

	virtual int pop(uint8_t& priority, OperationCountingType& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::push()
	 */

	virtual int push(uint8_t priority, const void* data, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" push().
	 */

	virtual int push(uint8_t priority, const OperationCountingType& value) const final override;

	/**
	 * \brief Redirects the call to push(..., const OperationCountingType&).
	 */

	virtual int push(uint8_t priority, OperationCountingType&& value) const final override;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Redirects the call to "raw" tryPush().
	 */

	virtual int tryEmplace(uint8_t priority, OperationCountingType::Value value = {}) const final override;

	/**
	 * \brief Redirects the call to "raw" tryPushFor().
	 */

	virtual int tryEmplaceFor(TickClock::duration duration, uint8_t priority, OperationCountingType::Value value = {})
			const final override;

	/**
	 * \brief Redirects the call to "raw" tryPushUntil().
	 */

	virtual int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority,
			OperationCountingType::Value value = {}) const final override;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPop().
	 */

	virtual int tryPop(uint8_t& priority, OperationCountingType& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPopFor().
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, OperationCountingType& value) const final
			override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPopUntil().
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value) const
			final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPush()
	 */

	virtual int tryPush(uint8_t priority, const void* data, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPush().
	 */

	virtual int tryPush(uint8_t priority, const OperationCountingType& value) const final override;

	/**
	 * \brief Redirects the call to tryPush(..., const OperationCountingType&).
	 */

	virtual int tryPush(uint8_t priority, OperationCountingType&& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPushFor()
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const void* data, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPushFor().
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const OperationCountingType& value) const
			final override;

	/**
	 * \brief Redirects the call to tryPushFor(..., const OperationCountingType&).
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, OperationCountingType&& value) const final
			override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPushUntil()
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const void* data, size_t size) const =
			0;

	/**
	 * \brief Redirects the call to "raw" tryPushUntil().
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const OperationCountingType& value)
			const final override;

	/**
	 * \brief Redirects the call to tryPushUntil(..., const OperationCountingType&).
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType&& value) const
			final override;
};

/// implementation of RawQueueWrapper for RawFifoQueue
class RawFifoQueueWrapper : public RawQueueWrapper
{
public:

	/**
	 * \brief RawFifoQueueWrapper's constructor
	 *
	 * \param [in] rawFifoQueue is a reference to wrapped \a RawFifoQueue object
	 */

	constexpr explicit RawFifoQueueWrapper(RawFifoQueue& rawFifoQueue) :
			rawFifoQueue_{rawFifoQueue}
	{

	}

	/**
	 * \brief Tests whether pushed and popped data match.
	 *
	 * As RawFifoQueue doesn't support priority, \a priority1 and \a priority2 values are ignored.
	 *
	 * \param [in] priority1 is the priority used for *push*() or *emplace*()
	 * \param [in] value1 is a reference to OperationCountingType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to OperationCountingType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::pop()
	 */

	virtual int pop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::push()
	 */

	virtual int push(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, void* buffer, size_t size) const
			override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPush()
	 */

	virtual int tryPush(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushFor()
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const void* data, size_t size) const
			override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushUntil()
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const void* data, size_t size) const
			override;

private:

	/// reference to wrapped \a RawFifoQueue object
	RawFifoQueue& rawFifoQueue_;
};

/**
 * \brief StaticRawFifoQueueWrapper class is a variant of RawFifoQueueWrapper that has automatic storage for queue's
 * contents.
 *
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticRawFifoQueueWrapper : public RawFifoQueueWrapper
{
public:

	/**
	 * \brief StaticFifoQueueWrapper's constructor
	 */

	constexpr StaticRawFifoQueueWrapper() :
			RawFifoQueueWrapper{rawFifoQueue_}
	{

	}

private:

	/// internal StaticRawFifoQueue<> object that will be wrapped
	StaticRawFifoQueue<OperationCountingType::Value, QueueSize> rawFifoQueue_;
};

/// implementation of RawQueueWrapper for RawMessageQueue
class RawMessageQueueWrapper : public RawQueueWrapper
{
public:

	/**
	 * \brief RawMessageQueueWrapper's constructor
	 *
	 * \param [in] rawMessageQueue is a reference to wrapped \a RawMessageQueue object
	 */

	constexpr explicit RawMessageQueueWrapper(RawMessageQueue& rawMessageQueue) :
			rawMessageQueue_{rawMessageQueue}
	{

	}

	/**
	 * \brief Tests whether pushed and popped data match.
	 *
	 * \param [in] priority1 is the priority used for *push*() or *emplace*()
	 * \param [in] value1 is a reference to OperationCountingType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to OperationCountingType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::pop()
	 */

	virtual int pop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::push()
	 */

	virtual int push(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, void* buffer, size_t size) const
			override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPush()
	 */

	virtual int tryPush(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPushFor()
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const void* data, size_t size) const
			override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPushUntil()
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const void* data, size_t size) const
			override;

private:

	/// reference to wrapped \a RawMessageQueue object
	RawMessageQueue& rawMessageQueue_;
};

/**
 * \brief StaticRawMessageQueueWrapper class is a variant of RawMessageQueueWrapper that has automatic storage for
 * queue's contents.
 *
 * \param QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticRawMessageQueueWrapper : public RawMessageQueueWrapper
{
public:

	/**
	 * \brief StaticMessageQueueWrapper's constructor
	 */

	constexpr StaticRawMessageQueueWrapper() :
			RawMessageQueueWrapper{rawMessageQueue_}
	{

	}

private:

	/// internal StaticRawMessageQueue<> object that will be wrapped
	StaticRawMessageQueue<OperationCountingType::Value, QueueSize> rawMessageQueue_;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_QUEUE_QUEUEWRAPPERS_HPP_
