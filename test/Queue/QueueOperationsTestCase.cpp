/**
 * \file
 * \brief QueueOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-07
 */

#include "QueueOperationsTestCase.hpp"

#include "waitForNextTick.hpp"
#include "OperationCountingType.hpp"

#include "distortos/StaticFifoQueue.hpp"
#include "distortos/StaticMessageQueue.hpp"
#include "distortos/SoftwareTimer.hpp"
#include "distortos/statistics.hpp"

#include "distortos/estd/ReferenceHolder.hpp"

#include <cerrno>

/// GCC 4.9 is needed for all {Fifo,Message}Queue::*emplace*() functions
#define DISTORTOS_QUEUE_EMPLACE_SUPPORTED	(DISTORTOS_FIFOQUEUE_EMPLACE_SUPPORTED && \
		DISTORTOS_MESSAGEQUEUE_EMPLACE_SUPPORTED)

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// type of elements of \a Test{Fifo,Message}Queue
using TestType = OperationCountingType;

/// FifoQueue with \a TestType
using TestFifoQueue = FifoQueue<TestType>;

/// MessageQueue with \a TestType
using TestMessageQueue = MessageQueue<TestType>;

/// wrapper for {Fifo,Message}Queue
class QueueWrapper
{
public:

	/**
	 * \brief Tests whether pushed and popped data match.
	 *
	 * \param [in] priority1 is the priority used for *push*() or *emplace*()
	 * \param [in] value1 is a reference to TestType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to TestType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(uint8_t priority1, const TestType& value1, uint8_t priority2, const TestType& value2) const = 0;

	/**
	 * \brief Wrapper for TestType::checkCounters().
	 *
	 * If the wrapped queue is a "raw" queue, then this function should be a stub which returns true, otherwise it
	 * should just call TestType::checkCounters().
	 */

	virtual bool checkCounters(size_t constructed, size_t copyConstructed, size_t moveConstructed, size_t destructed,
			size_t copyAssigned, size_t moveAssigned, size_t swapped) const = 0;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::emplace()
	 */

	virtual int emplace(uint8_t priority, TestType::Value value = {}) const = 0;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::pop()
	 */

	virtual int pop(uint8_t& priority, TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::push(..., const TestType&)
	 */

	virtual int push(uint8_t priority, const TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::push(..., TestType&&)
	 */

	virtual int push(uint8_t priority, TestType&& value) const = 0;

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryEmplace()
	 */

	virtual int tryEmplace(uint8_t priority, TestType::Value value = {}) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryEmplaceFor()
	 */

	virtual int tryEmplaceFor(TickClock::duration duration, uint8_t priority, TestType::Value value = {}) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryEmplaceUntil()
	 */

	virtual int tryEmplaceUntil(TickClock::time_point timePoint, uint8_t priority, TestType::Value value = {}) const =
			0;

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPopFor()
	 */

	virtual int tryPopFor(TickClock::duration duration, uint8_t& priority, TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPopUntil()
	 */

	virtual int tryPopUntil(TickClock::time_point timePoint, uint8_t& priority, TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPush(..., const TestType&)
	 */

	virtual int tryPush(uint8_t priority, const TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPush(..., TestType&&)
	 */

	virtual int tryPush(uint8_t priority, TestType&& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushFor(..., const TestType&)
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, const TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushFor(..., TestType&&)
	 */

	virtual int tryPushFor(TickClock::duration duration, uint8_t priority, TestType&& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushUntil(..., const TestType&)
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, const TestType& value) const = 0;

	/**
	 * \brief Wrapper for {Fifo,Message}Queue::tryPushUntil(..., TestType&&)
	 */

	virtual int tryPushUntil(TickClock::time_point timePoint, uint8_t priority, TestType&& value) const = 0;
};

/// common implementation of QueueWrapper for {Fifo,Message}Queue
class NonRawQueueWrapper : public QueueWrapper
{
public:

	/**
	 * \brief Wrapper for TestType::checkCounters().
	 *
	 * Just calls TestType::checkCounters().
	 */

	virtual bool checkCounters(const size_t constructed, const size_t copyConstructed, const size_t moveConstructed,
			const size_t destructed, const size_t copyAssigned, const size_t moveAssigned, const size_t swapped) const
			override
	{
		return TestType::checkCounters(constructed, copyConstructed, moveConstructed, destructed, copyAssigned,
				moveAssigned, swapped);
	}
};

/// implementation of QueueWrapper for FifoQueue
class FifoQueueWrapper : public NonRawQueueWrapper
{
public:

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
	 * \param [in] value1 is a reference to TestType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to TestType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(uint8_t, const TestType& value1, uint8_t, const TestType& value2) const override
	{
		return value1 == value2;
	}

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::emplace()
	 */

	virtual int emplace(uint8_t, const TestType::Value value = {}) const override
	{
		return fifoQueue_.emplace(value);
	}

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::pop()
	 */

	virtual int pop(uint8_t& priority, TestType& value) const override
	{
		priority = {};
		return fifoQueue_.pop(value);
	}

	/**
	 * \brief Wrapper for FifoQueue::push(..., const TestType&)
	 */

	virtual int push(uint8_t, const TestType& value) const override
	{
		return fifoQueue_.push(value);
	}

	/**
	 * \brief Wrapper for FifoQueue::push(..., TestType&&)
	 */

	virtual int push(uint8_t, TestType&& value) const override
	{
		return fifoQueue_.push(std::move(value));
	}

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::tryEmplace()
	 */

	virtual int tryEmplace(uint8_t, const TestType::Value value = {}) const override
	{
		return fifoQueue_.tryEmplace(value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryEmplaceFor()
	 */

	virtual int tryEmplaceFor(const TickClock::duration duration, uint8_t, const TestType::Value value = {}) const
			override
	{
		return fifoQueue_.tryEmplaceFor(duration, value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryEmplaceUntil()
	 */

	virtual int tryEmplaceUntil(const TickClock::time_point timePoint, uint8_t, const TestType::Value value = {}) const
			override
	{
		return fifoQueue_.tryEmplaceUntil(timePoint, value);
	}

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for FifoQueue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, TestType& value) const override
	{
		priority = {};
		return fifoQueue_.tryPop(value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPopFor()
	 */

	virtual int tryPopFor(const TickClock::duration duration, uint8_t& priority, TestType& value) const override
	{
		priority = {};
		return fifoQueue_.tryPopFor(duration, value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPopUntil()
	 */

	virtual int tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority, TestType& value) const override
	{
		priority = {};
		return fifoQueue_.tryPopUntil(timePoint, value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPush(..., const TestType&)
	 */

	virtual int tryPush(uint8_t, const TestType& value) const override
	{
		return fifoQueue_.tryPush(value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPush(..., TestType&&)
	 */

	virtual int tryPush(uint8_t, TestType&& value) const override
	{
		return fifoQueue_.tryPush(std::move(value));
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPushFor(..., const TestType&)
	 */

	virtual int tryPushFor(const TickClock::duration duration, uint8_t, const TestType& value) const override
	{
		return fifoQueue_.tryPushFor(duration, value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPushFor(..., TestType&&)
	 */

	virtual int tryPushFor(const TickClock::duration duration, uint8_t, TestType&& value) const override
	{
		return fifoQueue_.tryPushFor(duration, std::move(value));
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPushUntil(..., const TestType&)
	 */

	virtual int tryPushUntil(const TickClock::time_point timePoint, uint8_t, const TestType& value) const override
	{
		return fifoQueue_.tryPushUntil(timePoint, value);
	}

	/**
	 * \brief Wrapper for FifoQueue::tryPushUntil(..., TestType&&)
	 */

	virtual int tryPushUntil(const TickClock::time_point timePoint, uint8_t, TestType&& value) const override
	{
		return fifoQueue_.tryPushUntil(timePoint, std::move(value));
	}

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
	StaticFifoQueue<TestType, QueueSize> fifoQueue_;
};

/// implementation of QueueWrapper for MessageQueue
class MessageQueueWrapper : public NonRawQueueWrapper
{
public:

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
	 * \param [in] value1 is a reference to TestType object used for *push*() or *emplace*()
	 * \param [in] priority2 is the priority returned by *pop*()
	 * \param [in] value2 is a reference to TestType object returned by *pop*()
	 *
	 * \return true if pushed and popped data matches, false otherwise
	 */

	virtual bool check(const uint8_t priority1, const TestType& value1, const uint8_t priority2, const TestType& value2)
			const override
	{
		return priority1 == priority2 && value1 == value2;
	}

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::emplace()
	 */

	virtual int emplace(const uint8_t priority, const TestType::Value value = {}) const override
	{
		return messageQueue_.emplace(priority, value);
	}

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::pop()
	 */

	virtual int pop(uint8_t& priority, TestType& value) const override
	{
		return messageQueue_.pop(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::push(..., const TestType&)
	 */

	virtual int push(const uint8_t priority, const TestType& value) const override
	{
		return messageQueue_.push(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::push(..., TestType&&)
	 */

	virtual int push(const uint8_t priority, TestType&& value) const override
	{
		return messageQueue_.push(priority, std::move(value));
	}

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::tryEmplace()
	 */

	virtual int tryEmplace(const uint8_t priority, const TestType::Value value = {}) const override
	{
		return messageQueue_.tryEmplace(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceFor()
	 */

	virtual int tryEmplaceFor(const TickClock::duration duration, const uint8_t priority,
			const TestType::Value value = {}) const override
	{
		return messageQueue_.tryEmplaceFor(duration, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryEmplaceUntil()
	 */

	virtual int tryEmplaceUntil(const TickClock::time_point timePoint, const uint8_t priority,
			const TestType::Value value = {}) const override
	{
		return messageQueue_.tryEmplaceUntil(timePoint, priority, value);
	}

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	/**
	 * \brief Wrapper for MessageQueue::tryPop()
	 */

	virtual int tryPop(uint8_t& priority, TestType& value) const override
	{
		return messageQueue_.tryPop(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPopFor()
	 */

	virtual int tryPopFor(const TickClock::duration duration, uint8_t& priority, TestType& value) const override
	{
		return messageQueue_.tryPopFor(duration, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPopUntil()
	 */

	virtual int tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority, TestType& value) const override
	{
		return messageQueue_.tryPopUntil(timePoint, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPush(..., const TestType&)
	 */

	virtual int tryPush(const uint8_t priority, const TestType& value) const override
	{
		return messageQueue_.tryPush(priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPush(..., TestType&&)
	 */

	virtual int tryPush(const uint8_t priority, TestType&& value) const override
	{
		return messageQueue_.tryPush(priority, std::move(value));
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(..., const TestType&)
	 */

	virtual int tryPushFor(const TickClock::duration duration, const uint8_t priority, const TestType& value) const
			override
	{
		return messageQueue_.tryPushFor(duration, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushFor(..., TestType&&)
	 */

	virtual int tryPushFor(const TickClock::duration duration, const uint8_t priority, TestType&& value) const override
	{
		return messageQueue_.tryPushFor(duration, priority, std::move(value));
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(..., const TestType&)
	 */

	virtual int tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority, const TestType& value) const
			override
	{
		return messageQueue_.tryPushUntil(timePoint, priority, value);
	}

	/**
	 * \brief Wrapper for MessageQueue::tryPushUntil(..., TestType&&)
	 */

	virtual int tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority, TestType&& value) const
			override
	{
		return messageQueue_.tryPushUntil(timePoint, priority, std::move(value));
	}

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
	StaticMessageQueue<TestType, QueueSize> messageQueue_;
};

/// ReferenceHolder with const QueueWrapper
using QueueWrapperHolder = estd::ReferenceHolder<const QueueWrapper>;

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// single duration used in tests
constexpr auto singleDuration = TickClock::duration{1};

/// long duration used in tests
constexpr auto longDuration = singleDuration * 10;

/// expected number of context switches in waitForNextTick(): main -> idle -> main
constexpr decltype(statistics::getContextSwitchCount()) waitForNextTickContextSwitchCount {2};

/// expected number of context switches in phase1 block involving tryEmplaceFor(), tryEmplaceUntil(), tryPopFor(),
/// tryPopUntil(), tryPushFor() or tryPushUntil() (excluding waitForNextTick()): 1 - main thread blocks on queue
/// (main -> idle), 2 - main thread wakes up (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase1TryForUntilContextSwitchCount {2};

/// expected number of context switches in phase3 and phase4 block involving software timer (excluding
/// waitForNextTick()): 1 - main thread blocks on queue (main -> idle), 2 - main thread is unblocked by interrupt
/// (idle -> main)
constexpr decltype(statistics::getContextSwitchCount()) phase34SoftwareTimerContextSwitchCount {2};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Tests QueueWrapper::tryPop() when queue is empty - it must fail immediately and return EAGAIN
 *
 * \param [in] queueWrapper is a reference to QueueWrapper that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenEmpty(const QueueWrapper& queueWrapper)
{
	// queue is empty, so tryPop(T&) should fail immediately
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	TestType testValue {};	// 1 construction
	const auto ret = queueWrapper.tryPop(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start && queueWrapper.checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Tests QueueWrapper::tryPop() when queue is not empty - it must succeed immediately
 *
 * \param [in] queueWrapper is a reference to QueueWrapper that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPopWhenNotEmpty(const QueueWrapper& queueWrapper)
{
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	uint8_t priority {};
	TestType testValue {};	// 1 construction
	const auto ret = queueWrapper.tryPop(priority, testValue);	// 1 swap, 1 destruction
	return ret == 0 && start == TickClock::now() && queueWrapper.checkCounters(1, 0, 0, 1, 0, 0, 1) == true;
}

/**
 * \brief Tests QueueWrapper::tryPush(..., const T&) when queue is full - it must fail immediately and return EAGAIN
 *
 * \param [in] queueWrapper is a reference to QueueWrapper that will be tested
 *
 * \return true if test succeeded, false otherwise
 */

bool testTryPushWhenFull(const QueueWrapper& queueWrapper)
{
	// queue is full, so tryPush(..., const T&) should fail immediately
	TestType::resetCounters();
	waitForNextTick();
	const auto start = TickClock::now();
	const uint8_t priority {};
	const TestType testValue {};	// 1 construction
	const auto ret = queueWrapper.tryPush(priority, testValue);
	return ret == EAGAIN && TickClock::now() == start && queueWrapper.checkCounters(1, 0, 0, 0, 0, 0, 0) == true;
}

/**
 * \brief Phase 1 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly return some error when dealing with full
 * or empty queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
	// size 0, so queues are both full and empty at the same time
	StaticFifoQueueWrapper<0> fifoQueueWrapper;
	StaticMessageQueueWrapper<0> messageQueueWrapper;
	const QueueWrapperHolder queueWrappers[]
	{
			QueueWrapperHolder{fifoQueueWrapper},
			QueueWrapperHolder{messageQueueWrapper},
	};

	for (auto& queueWrapperHolder : queueWrappers)
	{
		auto& queueWrapper = queueWrapperHolder.get();
		const uint8_t constPriority {};
		const TestType constTestValue {};
		uint8_t nonConstPriority {};
		TestType nonConstTestValue {};

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is both full and empty, so tryPush(..., T&&) should fail immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryPush(constPriority, TestType{});	// 1 construction, 1 destruction
			if (ret != EAGAIN || start != TickClock::now() || queueWrapper.checkCounters(1, 0, 0, 1, 0, 0, 0) != true)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryPushFor(..., const T&) should time-out at expected time
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryPushFor(singleDuration, constPriority, constTestValue);
			const auto realDuration = TickClock::now() - start;
			if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
					queueWrapper.checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryPushFor(..., T&&) should time-out at expected time
			const auto start = TickClock::now();
			// 1 construction, 1 destruction
			const auto ret = queueWrapper.tryPushFor(singleDuration, constPriority, TestType{});
			const auto realDuration = TickClock::now() - start;
			if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
					queueWrapper.checkCounters(1, 0, 0, 1, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryPushUntil(..., const T&) should time-out at exact expected time
			const auto requestedTimePoint = TickClock::now() + singleDuration;
			const auto ret = queueWrapper.tryPushUntil(requestedTimePoint, constPriority, constTestValue);
			if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
					queueWrapper.checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryPushUntil(..., T&&) should time-out at exact expected time
			const auto requestedTimePoint = TickClock::now() + singleDuration;
			// 1 construction, 1 destruction
			const auto ret = queueWrapper.tryPushUntil(requestedTimePoint, constPriority, TestType{});
			if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
					queueWrapper.checkCounters(1, 0, 0, 1, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryPopFor(..., T&) should time-out at expected time
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
			const auto realDuration = TickClock::now() - start;
			if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
					queueWrapper.checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryPopUntil(..., T&) should time-out at exact expected time
			const auto requestedTimePoint = TickClock::now() + singleDuration;
			const auto ret = queueWrapper.tryPopUntil(requestedTimePoint, nonConstPriority, nonConstTestValue);
			if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
					queueWrapper.checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

		{
			// queue is both full and empty, so tryEmplace(..., Args&&...) should fail immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryEmplace(constPriority);
			if (ret != EAGAIN || start != TickClock::now() || queueWrapper.checkCounters(0, 0, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryEmplaceFor(..., Args&&...) should time-out at expected time
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryEmplaceFor(singleDuration, constPriority);
			const auto realDuration = TickClock::now() - start;
			if (ret != ETIMEDOUT || realDuration != singleDuration + decltype(singleDuration){1} ||
					queueWrapper.checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();

			// queue is both full and empty, so tryEmplaceUntil(..., Args&&...) should time-out at exact expected time
			const auto requestedTimePoint = TickClock::now() + singleDuration;
			const auto ret = queueWrapper.tryEmplaceUntil(requestedTimePoint, constPriority);
			if (ret != ETIMEDOUT || requestedTimePoint != TickClock::now() ||
					queueWrapper.checkCounters(0, 0, 0, 0, 0, 0, 0) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase1TryForUntilContextSwitchCount)
				return false;
		}

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	}

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests whether all tryEmplace*(), tryPush*() and tryPop*() functions properly send data via non-full or non-empty
 * queue.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	StaticFifoQueueWrapper<1> fifoQueueWrapper;
	StaticMessageQueueWrapper<1> messageQueueWrapper;
	const QueueWrapperHolder queueWrappers[]
	{
			QueueWrapperHolder{fifoQueueWrapper},
			QueueWrapperHolder{messageQueueWrapper},
	};

	for (auto& queueWrapperHolder : queueWrappers)
	{
		auto& queueWrapper = queueWrapperHolder.get();
		const uint8_t constPriority {};
		const TestType constTestValue {};
		uint8_t nonConstPriority {};
		TestType nonConstTestValue {};

		{
			// queue is not full, so tryPush(..., const T&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryPush(constPriority, constTestValue);	// 1 copy construction
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenNotEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not full, so tryPush(..., T&&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			// 1 construction, 1 move construction, 1 destruction
			const auto ret = queueWrapper.tryPush(constPriority, TestType{});
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenNotEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not full, so tryPushFor(..., const T&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			// 1 copy construction
			const auto ret = queueWrapper.tryPushFor(singleDuration, constPriority, constTestValue);
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not empty, so tryPopFor(..., T&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			// 1 swap, 1 destruction
			const auto ret = queueWrapper.tryPopFor(singleDuration, nonConstPriority, nonConstTestValue);
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
				return false;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not full, so tryPushFor(..., T&&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			// 1 construction, 1 move construction, 1 destruction
			const auto ret = queueWrapper.tryPushFor(singleDuration, constPriority, TestType{});
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenNotEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not full, so tryPushUntil(..., const T&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			// 1 copy construction
			const auto ret = queueWrapper.tryPushUntil(start + singleDuration, constPriority, constTestValue);
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not empty, so tryPopUntil(..., T&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			// 1 swap, 1 destruction
			const auto ret = queueWrapper.tryPopUntil(start + singleDuration, nonConstPriority, nonConstTestValue);
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(0, 0, 0, 1, 0, 0, 1) != true)
				return false;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not full, so tryPushUntil(..., T&&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			// 1 construction, 1 move construction, 1 destruction
			const auto ret = queueWrapper.tryPushUntil(start + singleDuration, constPriority, TestType{});
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(1, 0, 1, 1, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenNotEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

		{
			// queue is not full, so tryEmplace(..., Args&&...) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryEmplace(constPriority);	// 1 construction
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenNotEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not full, so tryEmplaceFor(..., Args&&...) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryEmplaceFor(singleDuration, constPriority);	// 1 construction
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenNotEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			// queue is not full, so tryEmplaceUntil(..., Args&&...) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryEmplaceUntil(start + singleDuration, constPriority);	// 1 construction
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(1, 0, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			const auto ret = testTryPushWhenFull(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenNotEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	}

	return true;
}

/**
 * \brief Phase 3 of test case.
 *
 * Tests interrupt -> thread communication scenario. Main (current) thread waits for data to become available in queue.
 * Software timer pushes some values to the same queue at specified time point from interrupt context, main thread is
 * expected to receive these values (with pop(), tryPopFor() and tryPopUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase3()
{
	StaticFifoQueueWrapper<1> fifoQueueWrapper;
	StaticMessageQueueWrapper<1> messageQueueWrapper;
	const QueueWrapperHolder queueWrappers[]
	{
			QueueWrapperHolder{fifoQueueWrapper},
			QueueWrapperHolder{messageQueueWrapper},
	};

	for (auto& queueWrapperHolder : queueWrappers)
	{
		auto& queueWrapper = queueWrapperHolder.get();
		uint8_t sharedMagicPriority {};
		TestType sharedMagicValue {};
		auto softwareTimer = makeSoftwareTimer(
				[&queueWrapper, &sharedMagicPriority, &sharedMagicValue]()
				{
					queueWrapper.tryPush(sharedMagicPriority, sharedMagicValue);
				});

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			sharedMagicPriority = 0x93;
			sharedMagicValue = TestType{0x2f5be1a4};	// 1 construction, 1 move assignment, 1 destruction
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

			// queue is currently empty, but pop() should succeed at expected time
			uint8_t priority {};
			TestType testValue {};	// 1 construction
			const auto ret = queueWrapper.pop(priority, testValue);	// 1 swap, 1 destruction
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(sharedMagicPriority, sharedMagicValue, priority, testValue) == false ||
					queueWrapper.checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			sharedMagicPriority = 0x01;
			sharedMagicValue = TestType{0xc1fe105a};	// 1 construction, 1 move assignment, 1 destruction
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

			// queue is currently empty, but tryPopFor() should succeed at expected time
			uint8_t priority {};
			TestType testValue {};	// 1 construction
			// 1 swap, 1 destruction
			const auto ret = queueWrapper.tryPopFor(wakeUpTimePoint - TickClock::now() + longDuration, priority,
					testValue);
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(sharedMagicPriority, sharedMagicValue, priority, testValue) == false ||
					queueWrapper.checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			sharedMagicPriority = 0x48;
			sharedMagicValue = TestType{0xda0e4e30};	// 1 construction, 1 move assignment, 1 destruction
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 copy construction

			// queue is currently empty, but tryPopUntil() should succeed at expected time
			uint8_t priority {};
			TestType testValue {};	// 1 construction
			// 1 swap, 1 destruction
			const auto ret = queueWrapper.tryPopUntil(wakeUpTimePoint + longDuration, priority, testValue);
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(sharedMagicPriority, sharedMagicValue, priority, testValue) == false ||
					queueWrapper.checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			const auto ret = testTryPopWhenEmpty(queueWrapper);
			if (ret != true)
				return ret;
		}

	}

	return true;
}

/**
 * \brief Phase 4 of test case.
 *
 * Tests thread -> interrupt communication scenario. Main (current) thread pushes data to queue (which is initially
 * full). Software timer pops first value (which should match the one pushed previously) from the same queue at
 * specified time point from interrupt context, main thread is expected to succeed in pushing new value (with emplace(),
 * push(), tryEmplaceFor(), tryEmplaceUntil(), tryPushFor() and tryPushUntil()) in the same moment.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase4()
{
	StaticFifoQueueWrapper<1> fifoQueueWrapper;
	StaticMessageQueueWrapper<1> messageQueueWrapper;
	const QueueWrapperHolder queueWrappers[]
	{
			QueueWrapperHolder{fifoQueueWrapper},
			QueueWrapperHolder{messageQueueWrapper},
	};

	for (auto& queueWrapperHolder : queueWrappers)
	{
		auto& queueWrapper = queueWrapperHolder.get();
		uint8_t receivedPriority {};
		TestType receivedTestValue {};
		auto softwareTimer = makeSoftwareTimer(
				[&queueWrapper, &receivedPriority, &receivedTestValue]()
				{
					queueWrapper.tryPop(receivedPriority, receivedTestValue);
				});

		uint8_t currentMagicPriority {0xc9};
		TestType currentMagicValue {0xa810b166};

		{
			// queue is not full, so push(..., const T&) must succeed immediately
			TestType::resetCounters();
			waitForNextTick();
			const auto start = TickClock::now();
			const auto ret = queueWrapper.tryPush(currentMagicPriority, currentMagicValue);	// 1 copy construction
			if (ret != 0 || start != TickClock::now() || queueWrapper.checkCounters(0, 1, 0, 0, 0, 0, 0) != true)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but push(..., const T&) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0x96;
			currentMagicValue = TestType{0xc9e7e479};	// 1 construction, 1 move assignment, 1 destruction
			const auto ret = queueWrapper.push(currentMagicPriority, currentMagicValue);	// 1 copy construction
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but push(..., T&&) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0x06;
			currentMagicValue = TestType{0x51607941};	// 1 construction, 1 move assignment, 1 destruction
			// 1 copy construction, 1 move construction, 1 destruction
			const auto ret = queueWrapper.push(currentMagicPriority, TestType{currentMagicValue});
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but tryPushFor(..., const T&) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0xcc;
			currentMagicValue = TestType{0xb9f4b42e};	// 1 construction, 1 move assignment, 1 destruction
			const auto ret = queueWrapper.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
					currentMagicPriority, currentMagicValue);	// 1 copy construction
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but tryPushFor(..., T&&) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0xf6;
			currentMagicValue = TestType{0xbb0bfe00};	// 1 construction, 1 move assignment, 1 destruction
			// 1 copy construction, 1 move construction, 1 destruction
			const auto ret = queueWrapper.tryPushFor(wakeUpTimePoint - TickClock::now() + longDuration,
					currentMagicPriority, TestType{currentMagicValue});
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but tryPushUntil(..., const T&) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0x2e;
			currentMagicValue = TestType{0x25eb4357};	// 1 construction, 1 move assignment, 1 destruction
			const auto ret = queueWrapper.tryPushUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
					currentMagicValue);	// 1 copy construction
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(1, 2, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but tryPushUntil(..., T&&) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0xb6;
			currentMagicValue = TestType{0x625652d7};	// 1 construction, 1 move assignment, 1 destruction
			const auto ret = queueWrapper.tryPushUntil(wakeUpTimePoint + longDuration, currentMagicPriority,
					TestType{currentMagicValue});	// 1 copy construction, 1 move construction, 1 destruction
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(1, 2, 1, 3, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

#if DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but emplace(..., Args&&...) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0xe7;
			const TestType::Value value = 0x8de61877;
			currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
			const auto ret = queueWrapper.emplace(currentMagicPriority, value);	// 1 construction
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but tryEmplaceFor(..., Args&&...) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0x98;
			const TestType::Value value = 0x2b2cd349;
			currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
			const auto ret = queueWrapper.tryEmplaceFor(wakeUpTimePoint - TickClock::now() + longDuration,
					currentMagicPriority, value);	// 1 construction
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

		{
			TestType::resetCounters();
			waitForNextTick();

			const auto contextSwitchCount = statistics::getContextSwitchCount();
			const auto wakeUpTimePoint = TickClock::now() + longDuration;
			softwareTimer.start(wakeUpTimePoint);	// in timer: 1 swap, 1 destruction

			// queue is currently full, but tryEmplaceUntil(..., Args&&...) should succeed at expected time
			const decltype(currentMagicPriority) expectedPriority {currentMagicPriority};
			const decltype(currentMagicValue) expectedTestValue {currentMagicValue};	// 1 copy construction
			currentMagicPriority = 0xa5;
			const TestType::Value value = 0x7df8502a;
			currentMagicValue = TestType{value};	// 1 construction, 1 move assignment, 1 destruction
			// 1 construction
			const auto ret = queueWrapper.tryEmplaceUntil(wakeUpTimePoint + longDuration, currentMagicPriority, value);
			const auto wokenUpTimePoint = TickClock::now();
			if (ret != 0 || wakeUpTimePoint != wokenUpTimePoint ||
					queueWrapper.check(expectedPriority, expectedTestValue, receivedPriority, receivedTestValue) ==
							false || queueWrapper.checkCounters(2, 1, 0, 2, 0, 1, 1) != true ||
					statistics::getContextSwitchCount() - contextSwitchCount != phase34SoftwareTimerContextSwitchCount)
				return false;
		}

#endif	// DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1 || DOXYGEN == 1

	}

	return true;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool QueueOperationsTestCase::run_() const
{
	constexpr auto emplace = DISTORTOS_QUEUE_EMPLACE_SUPPORTED == 1;

	constexpr size_t queueTypes {2};
	constexpr auto phase1ExpectedContextSwitchCount = queueTypes * (emplace == true ?
			12 * waitForNextTickContextSwitchCount + 8 * phase1TryForUntilContextSwitchCount :
			9 * waitForNextTickContextSwitchCount + 6 * phase1TryForUntilContextSwitchCount);
	constexpr auto phase2ExpectedContextSwitchCount = queueTypes * (emplace == true ?
			36 * waitForNextTickContextSwitchCount : 24 * waitForNextTickContextSwitchCount);
	constexpr auto phase3ExpectedContextSwitchCount = queueTypes * (6 * waitForNextTickContextSwitchCount +
			3 * phase34SoftwareTimerContextSwitchCount);
	constexpr auto phase4ExpectedContextSwitchCount = queueTypes * (emplace == true ?
			10 * waitForNextTickContextSwitchCount + 9 * phase34SoftwareTimerContextSwitchCount :
			7 * waitForNextTickContextSwitchCount + 6 * phase34SoftwareTimerContextSwitchCount);
	constexpr auto expectedContextSwitchCount = phase1ExpectedContextSwitchCount + phase2ExpectedContextSwitchCount +
			phase3ExpectedContextSwitchCount + phase4ExpectedContextSwitchCount;

	const auto contextSwitchCount = statistics::getContextSwitchCount();

	for (const auto& function : {phase1, phase2, phase3, phase4})
	{
		const auto ret = function();
		if (ret != true)
			return ret;
	}

	if (statistics::getContextSwitchCount() - contextSwitchCount != expectedContextSwitchCount)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
