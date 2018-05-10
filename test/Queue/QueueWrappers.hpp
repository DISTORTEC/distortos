/**
 * \file
 * \brief QueueWrapper, [Non]RawQueueWrapper and [Dynamic|Static][Raw]{Fifo,Message}QueueWrapper classes header
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_QUEUE_QUEUEWRAPPERS_HPP_
#define TEST_QUEUE_QUEUEWRAPPERS_HPP_

#include "OperationCountingType.hpp"

#include "distortos/DynamicFifoQueue.hpp"
#include "distortos/DynamicMessageQueue.hpp"
#include "distortos/DynamicRawFifoQueue.hpp"
#include "distortos/DynamicRawMessageQueue.hpp"
#include "distortos/StaticFifoQueue.hpp"
#include "distortos/StaticMessageQueue.hpp"
#include "distortos/StaticRawFifoQueue.hpp"
#include "distortos/StaticRawMessageQueue.hpp"

namespace distortos
{

namespace test
{

/// wrapper for [Raw]{Fifo,Message}Queue
class QueueWrapper
{
public:

	/**
	 * \brief QueueWrapper's destructor
	 */

	virtual ~QueueWrapper() = 0;

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

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::emplace() or Raw{Fifo,Message}Queue::push()
	 */

	virtual int emplace(uint8_t priority, OperationCountingType::Value value = {}) const = 0;

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

/// unique_ptr with QueueWrapper
using QueueWrapperUniquePointer = std::unique_ptr<QueueWrapper>;

/// common implementation of QueueWrapper for {Fifo,Message}Queue
class NonRawQueueWrapper : public QueueWrapper
{
public:

	/**
	 * \brief Wrapper for OperationCountingType::checkCounters().
	 *
	 * Just calls OperationCountingType::checkCounters().
	 */

	bool checkCounters(size_t constructed, size_t copyConstructed, size_t moveConstructed, size_t destructed,
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

	bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

	/**
	 * \brief Wrapper for FifoQueue::emplace()
	 */

	int emplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

	/**
	 * \brief Wrapper for FifoQueue::pop()
	 */

	int pop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::push(..., const OperationCountingType&)
	 */

	int push(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::push(..., OperationCountingType&&)
	 */

	int push(uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryEmplace()
	 */

	int tryEmplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryEmplaceFor()
	 */

	int tryEmplaceFor(TickClock::duration duration, uint8_t priority, OperationCountingType::Value value = {}) const
			override;

	/**
	 * \brief Wrapper for FifoQueue::tryEmplaceUntil()
	 */

	int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType::Value value = {})
			const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPop()
	 */

	int tryPop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPopFor()
	 */

	int tryPopFor(TickClock::duration duration, uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPopUntil()
	 */

	int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPush(..., const OperationCountingType&)
	 */

	int tryPush(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPush(..., OperationCountingType&&)
	 */

	int tryPush(uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushFor(..., const OperationCountingType&)
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushFor(..., OperationCountingType&&)
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushUntil(..., const OperationCountingType&)
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const OperationCountingType& value) const
			override;

	/**
	 * \brief Wrapper for FifoQueue::tryPushUntil(..., OperationCountingType&&)
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType&& value) const override;

private:

	/// reference to wrapped \a TestFifoQueue object
	TestFifoQueue& fifoQueue_;
};

/**
 * \brief DynamicFifoQueueWrapper class is a variant of FifoQueueWrapper that has dynamic storage for queue's contents.
 */

class DynamicFifoQueueWrapper : public FifoQueueWrapper
{
public:

	/// DynamicFifoQueue with \a OperationCountingType
	using TestDynamicFifoQueue = DynamicFifoQueue<OperationCountingType>;

	/**
	 * \brief DynamicFifoQueueWrapper's constructor
	 *
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	explicit DynamicFifoQueueWrapper(size_t queueSize);

private:

	/// internal TestDynamicFifoQueue object that will be wrapped
	TestDynamicFifoQueue fifoQueue_;
};

/**
 * \brief StaticFifoQueueWrapper class is a variant of FifoQueueWrapper that has automatic storage for queue's contents.
 *
 * \tparam QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticFifoQueueWrapper : public FifoQueueWrapper
{
public:

	/// StaticFifoQueue with \a OperationCountingType and storage for \a QueueSize elements
	using TestStaticFifoQueue = StaticFifoQueue<OperationCountingType, QueueSize>;

	/**
	 * \brief StaticFifoQueueWrapper's constructor
	 */

	StaticFifoQueueWrapper() :
			FifoQueueWrapper{fifoQueue_}
	{

	}

private:

	/// internal TestStaticFifoQueue object that will be wrapped
	TestStaticFifoQueue fifoQueue_;
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

	bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

	/**
	 * \brief Wrapper for MessageQueue::emplace()
	 */

	int emplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

	/**
	 * \brief Wrapper for MessageQueue::pop()
	 */

	int pop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::push(..., const OperationCountingType&)
	 */

	int push(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::push(..., OperationCountingType&&)
	 */

	int push(uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryEmplace()
	 */

	int tryEmplace(uint8_t priority, OperationCountingType::Value value = {}) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceFor()
	 */

	int tryEmplaceFor(TickClock::duration duration, uint8_t priority, OperationCountingType::Value value = {}) const
			override;

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceUntil()
	 */

	int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType::Value value = {})
			const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPop()
	 */

	int tryPop(uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPopFor()
	 */

	int tryPopFor(TickClock::duration duration, uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPopUntil()
	 */

	int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPush(..., const OperationCountingType&)
	 */

	int tryPush(uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPush(..., OperationCountingType&&)
	 */

	int tryPush(uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(..., const OperationCountingType&)
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, const OperationCountingType& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(..., OperationCountingType&&)
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, OperationCountingType&& value) const override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(..., const OperationCountingType&)
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const OperationCountingType& value) const
			override;

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(..., OperationCountingType&&)
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType&& value) const override;

private:

	/// reference to wrapped \a TestMessageQueue object
	TestMessageQueue& messageQueue_;
};

/**
 * \brief DynamicMessageQueueWrapper class is a variant of MessageQueueWrapper that has dynamic storage for queue's
 * contents.
 */

class DynamicMessageQueueWrapper : public MessageQueueWrapper
{
public:

	/// DynamicMessageQueue with \a OperationCountingType
	using TestDynamicMessageQueue = DynamicMessageQueue<OperationCountingType>;

	/**
	 * \brief DynamicMessageQueueWrapper's constructor
	 *
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	explicit DynamicMessageQueueWrapper(size_t queueSize);

private:

	/// internal TestDynamicMessageQueue object that will be wrapped
	TestDynamicMessageQueue messageQueue_;
};

/**
 * \brief StaticMessageQueueWrapper class is a variant of MessageQueueWrapper that has automatic storage for queue's
 * contents.
 *
 * \tparam QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticMessageQueueWrapper : public MessageQueueWrapper
{
public:

	/// StaticMessageQueue with \a OperationCountingType and storage for \a QueueSize elements
	using TestStaticMessageQueue = StaticMessageQueue<OperationCountingType, QueueSize>;

	/**
	 * \brief StaticMessageQueueWrapper's constructor
	 */

	StaticMessageQueueWrapper() :
			MessageQueueWrapper{messageQueue_}
	{

	}

private:

	/// internal TestStaticMessageQueue object that will be wrapped
	TestStaticMessageQueue messageQueue_;
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

	bool checkCounters(size_t constructed, size_t copyConstructed, size_t moveConstructed, size_t destructed,
			size_t copyAssigned, size_t moveAssigned, size_t swapped) const override;

	/**
	 * \brief Redirects the call to "raw" push().
	 */

	int emplace(uint8_t priority, OperationCountingType::Value value = {}) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::pop()
	 */

	virtual int pop(uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" pop().
	 */

	int pop(uint8_t& priority, OperationCountingType& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::push()
	 */

	virtual int push(uint8_t priority, const void* data, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" push().
	 */

	int push(uint8_t priority, const OperationCountingType& value) const final override;

	/**
	 * \brief Redirects the call to push(..., const OperationCountingType&).
	 */

	int push(uint8_t priority, OperationCountingType&& value) const final override;

	/**
	 * \brief Redirects the call to "raw" tryPush().
	 */

	int tryEmplace(uint8_t priority, OperationCountingType::Value value = {}) const final override;

	/**
	 * \brief Redirects the call to "raw" tryPushFor().
	 */

	int tryEmplaceFor(TickClock::duration duration, uint8_t priority, OperationCountingType::Value value = {}) const
			final override;

	/**
	 * \brief Redirects the call to "raw" tryPushUntil().
	 */

	int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType::Value value = {})
			const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPop().
	 */

	int tryPop(uint8_t& priority, OperationCountingType& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPopFor().
	 */

	int tryPopFor(TickClock::duration duration, uint8_t& priority, OperationCountingType& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, void* buffer, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPopUntil().
	 */

	int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value) const final
			override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPush()
	 */

	virtual int tryPush(uint8_t priority, const void* data, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPush().
	 */

	int tryPush(uint8_t priority, const OperationCountingType& value) const final override;

	/**
	 * \brief Redirects the call to tryPush(..., const OperationCountingType&).
	 */

	int tryPush(uint8_t priority, OperationCountingType&& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPushFor()
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const void* data, size_t size) const = 0;

	/**
	 * \brief Redirects the call to "raw" tryPushFor().
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, const OperationCountingType& value) const final
			override;

	/**
	 * \brief Redirects the call to tryPushFor(..., const OperationCountingType&).
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, OperationCountingType&& value) const final override;

	/**
	 * \brief Wrapper for Raw{Fifo,Message}Queue::tryPushUntil()
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const void* data, size_t size) const =
			0;

	/**
	 * \brief Redirects the call to "raw" tryPushUntil().
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const OperationCountingType& value) const final
			override;

	/**
	 * \brief Redirects the call to tryPushUntil(..., const OperationCountingType&).
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, OperationCountingType&& value) const final
			override;
};

/// unique_ptr with RawQueueWrapper
using RawQueueWrapperUniquePointer = std::unique_ptr<RawQueueWrapper>;

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

	bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::pop()
	 */

	int pop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::push()
	 */

	int push(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPop()
	 */

	int tryPop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopFor()
	 */

	int tryPopFor(TickClock::duration duration, uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPopUntil()
	 */

	int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPush()
	 */

	int tryPush(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushFor()
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawFifoQueue::tryPushUntil()
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const void* data, size_t size) const override;

private:

	/// reference to wrapped \a RawFifoQueue object
	RawFifoQueue& rawFifoQueue_;
};

/**
 * \brief DynamicRawFifoQueueWrapper class is a variant of RawFifoQueueWrapper that has dynamic storage for queue's
 * contents.
 */

class DynamicRawFifoQueueWrapper : public RawFifoQueueWrapper
{
public:

	/**
	 * \brief DynamicFifoQueueWrapper's constructor
	 *
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	explicit DynamicRawFifoQueueWrapper(size_t queueSize);

private:

	/// internal DynamicRawFifoQueue object that will be wrapped
	DynamicRawFifoQueue rawFifoQueue_;
};

/**
 * \brief StaticRawFifoQueueWrapper class is a variant of RawFifoQueueWrapper that has automatic storage for queue's
 * contents.
 *
 * \tparam QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticRawFifoQueueWrapper : public RawFifoQueueWrapper
{
public:

	/// StaticRawFifoQueue with \a OperationCountingType::Value and storage for \a QueueSize elements
	using TestStaticRawFifoQueue = StaticRawFifoQueue<sizeof(OperationCountingType::Value), QueueSize>;

	/**
	 * \brief StaticFifoQueueWrapper's constructor
	 */

	StaticRawFifoQueueWrapper() :
			RawFifoQueueWrapper{rawFifoQueue_}
	{

	}

private:

	/// internal TestStaticRawFifoQueue object that will be wrapped
	TestStaticRawFifoQueue rawFifoQueue_;
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

	bool check(uint8_t priority1, const OperationCountingType& value1, uint8_t priority2,
			const OperationCountingType& value2) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::pop()
	 */

	int pop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::push()
	 */

	int push(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPop()
	 */

	int tryPop(uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPopFor()
	 */

	int tryPopFor(TickClock::duration duration, uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPopUntil()
	 */

	int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, void* buffer, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPush()
	 */

	int tryPush(uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPushFor()
	 */

	int tryPushFor(TickClock::duration duration, uint8_t priority, const void* data, size_t size) const override;

	/**
	 * \brief Wrapper for RawMessageQueue::tryPushUntil()
	 */

	int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const void* data, size_t size) const override;

private:

	/// reference to wrapped \a RawMessageQueue object
	RawMessageQueue& rawMessageQueue_;
};

/**
 * \brief DynamicRawMessageQueueWrapper class is a variant of RawMessageQueueWrapper that has dynamic storage for
 * queue's contents.
 */

class DynamicRawMessageQueueWrapper : public RawMessageQueueWrapper
{
public:

	/**
	 * \brief DynamicMessageQueueWrapper's constructor
	 *
	 * \param [in] queueSize is the maximum number of elements in queue
	 */

	explicit DynamicRawMessageQueueWrapper(size_t queueSize);

private:

	/// internal DynamicRawMessageQueue object that will be wrapped
	DynamicRawMessageQueue rawMessageQueue_;
};

/**
 * \brief StaticRawMessageQueueWrapper class is a variant of RawMessageQueueWrapper that has automatic storage for
 * queue's contents.
 *
 * \tparam QueueSize is the maximum number of elements in queue
 */

template<size_t QueueSize>
class StaticRawMessageQueueWrapper : public RawMessageQueueWrapper
{
public:

	/// StaticRawMessageQueue with \a OperationCountingType::Value and storage for \a QueueSize elements
	using TestStaticRawMessageQueue = StaticRawMessageQueue<sizeof(OperationCountingType::Value), QueueSize>;

	/**
	 * \brief StaticMessageQueueWrapper's constructor
	 */

	StaticRawMessageQueueWrapper() :
			RawMessageQueueWrapper{rawMessageQueue_}
	{

	}

private:

	/// internal TestStaticRawMessageQueue object that will be wrapped
	TestStaticRawMessageQueue rawMessageQueue_;
};

/**
 * \brief RawQueueWrapper's factory function
 *
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \param [in] dynamic selects whether static (false) or dynamic (true) queue type will be constructed
 * \param [in] fifo selects whether message (false) or fifo (true) queue type will be constructed
 *
 * \return RawQueueWrapperUniquePointer with constructed object - {Dynamic,Static}Raw{Fifo,Message}QueueWrapper
 */

template<size_t QueueSize>
RawQueueWrapperUniquePointer makeRawQueueWrapper(const bool dynamic, const bool fifo)
{
	if (dynamic == false)
	{
		if (fifo == false)
			return RawQueueWrapperUniquePointer {new StaticRawMessageQueueWrapper<QueueSize>};
		else	// if (fifo != false)
			return RawQueueWrapperUniquePointer {new StaticRawFifoQueueWrapper<QueueSize>};
	}
	else	// if (dynamic != false)
	{
		if (fifo == false)
			return RawQueueWrapperUniquePointer {new DynamicRawMessageQueueWrapper {QueueSize}};
		else	// if (fifo != false)
			return RawQueueWrapperUniquePointer {new DynamicRawFifoQueueWrapper {QueueSize}};
	}
}

/**
 * \brief QueueWrapper's factory function
 *
 * \tparam QueueSize is the maximum number of elements in queue
 *
 * \param [in] dynamic selects whether static (false) or dynamic (true) queue type will be constructed
 * \param [in] raw selects whether non-raw (false) or raw (true) queue type will be constructed
 * \param [in] fifo selects whether message (false) or fifo (true) queue type will be constructed
 *
 * \return QueueWrapperUniquePointer with constructed object - {Dynamic,Static}[Raw]{Fifo,Message}QueueWrapper
 */

template<size_t QueueSize>
QueueWrapperUniquePointer makeQueueWrapper(const bool dynamic, const bool raw, const bool fifo)
{
	if (raw == true)
		return makeRawQueueWrapper<QueueSize>(dynamic, fifo);

	if (dynamic == false)
	{
		if (fifo == false)
			return QueueWrapperUniquePointer {new StaticMessageQueueWrapper<QueueSize>};
		else	// if (fifo != false)
			return QueueWrapperUniquePointer {new StaticFifoQueueWrapper<QueueSize>};
	}
	else	// if (dynamic != false)
	{
		if (fifo == false)
			return QueueWrapperUniquePointer {new DynamicMessageQueueWrapper {QueueSize}};
		else	// if (fifo != false)
			return QueueWrapperUniquePointer {new DynamicFifoQueueWrapper {QueueSize}};
	}
}

}	// namespace test

}	// namespace distortos

#endif	// TEST_QUEUE_QUEUEWRAPPERS_HPP_
