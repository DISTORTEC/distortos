/**
 * \file
 * \brief QueueWrapper, [Non]RawQueueWrapper and [Dynamic|Static][Raw]{Fifo,Message}QueueWrapper classes implementation
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "QueueWrappers.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| NonRawQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool NonRawQueueWrapper::checkCounters(const size_t constructed, const size_t copyConstructed,
		const size_t moveConstructed, const size_t destructed, const size_t copyAssigned, const size_t moveAssigned,
		const size_t swapped) const
{
	return OperationCountingType::checkCounters(constructed, copyConstructed, moveConstructed, destructed, copyAssigned,
			moveAssigned, swapped);
}

/*---------------------------------------------------------------------------------------------------------------------+
| FifoQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool FifoQueueWrapper::check(uint8_t, const OperationCountingType& value1, uint8_t, const OperationCountingType& value2)
		const
{
	return value1 == value2;
}

int FifoQueueWrapper::emplace(uint8_t, const OperationCountingType::Value value) const
{
	return fifoQueue_.emplace(value);
}

int FifoQueueWrapper::pop(uint8_t& priority, OperationCountingType& value) const
{
	priority = {};
	return fifoQueue_.pop(value);
}

int FifoQueueWrapper::push(uint8_t, const OperationCountingType& value) const
{
	return fifoQueue_.push(value);
}

int FifoQueueWrapper::push(uint8_t, OperationCountingType&& value) const
{
	return fifoQueue_.push(std::move(value));
}

int FifoQueueWrapper::tryEmplace(uint8_t, const OperationCountingType::Value value) const
{
	return fifoQueue_.tryEmplace(value);
}

int FifoQueueWrapper::tryEmplaceFor(const TickClock::duration duration, uint8_t,
		const OperationCountingType::Value value) const
{
	return fifoQueue_.tryEmplaceFor(duration, value);
}

int FifoQueueWrapper::tryEmplaceUntil(const TickClock::time_point timePoint, uint8_t,
		const OperationCountingType::Value value) const
{
	return fifoQueue_.tryEmplaceUntil(timePoint, value);
}

int FifoQueueWrapper::tryPop(uint8_t& priority, OperationCountingType& value) const
{
	priority = {};
	return fifoQueue_.tryPop(value);
}

int FifoQueueWrapper::tryPopFor(const TickClock::duration duration, uint8_t& priority, OperationCountingType& value)
		const
{
	priority = {};
	return fifoQueue_.tryPopFor(duration, value);
}

int FifoQueueWrapper::tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority,
		OperationCountingType& value) const
{
	priority = {};
	return fifoQueue_.tryPopUntil(timePoint, value);
}

int FifoQueueWrapper::tryPush(uint8_t, const OperationCountingType& value) const
{
	return fifoQueue_.tryPush(value);
}

int FifoQueueWrapper::tryPush(uint8_t, OperationCountingType&& value) const
{
	return fifoQueue_.tryPush(std::move(value));
}

int FifoQueueWrapper::tryPushFor(const TickClock::duration duration, uint8_t, const OperationCountingType& value) const
{
	return fifoQueue_.tryPushFor(duration, value);
}

int FifoQueueWrapper::tryPushFor(const TickClock::duration duration, uint8_t, OperationCountingType&& value) const
{
	return fifoQueue_.tryPushFor(duration, std::move(value));
}

int FifoQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, uint8_t, const OperationCountingType& value)
		const
{
	return fifoQueue_.tryPushUntil(timePoint, value);
}

int FifoQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, uint8_t, OperationCountingType&& value) const
{
	return fifoQueue_.tryPushUntil(timePoint, std::move(value));
}

/*---------------------------------------------------------------------------------------------------------------------+
| DynamicFifoQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicFifoQueueWrapper::DynamicFifoQueueWrapper(const size_t queueSize) :
		FifoQueueWrapper{fifoQueue_},
		fifoQueue_{queueSize}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| MessageQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool MessageQueueWrapper::check(const uint8_t priority1, const OperationCountingType& value1, const uint8_t priority2,
		const OperationCountingType& value2) const
{
	return priority1 == priority2 && value1 == value2;
}

int MessageQueueWrapper::emplace(const uint8_t priority, const OperationCountingType::Value value) const
{
	return messageQueue_.emplace(priority, value);
}

int MessageQueueWrapper::pop(uint8_t& priority, OperationCountingType& value) const
{
	return messageQueue_.pop(priority, value);
}

int MessageQueueWrapper::push(const uint8_t priority, const OperationCountingType& value) const
{
	return messageQueue_.push(priority, value);
}

int MessageQueueWrapper::push(const uint8_t priority, OperationCountingType&& value) const
{
	return messageQueue_.push(priority, std::move(value));
}

int MessageQueueWrapper::tryEmplace(const uint8_t priority, const OperationCountingType::Value value) const
{
	return messageQueue_.tryEmplace(priority, value);
}

int MessageQueueWrapper::tryEmplaceFor(const TickClock::duration duration, const uint8_t priority,
		const OperationCountingType::Value value) const
{
	return messageQueue_.tryEmplaceFor(duration, priority, value);
}

int MessageQueueWrapper::tryEmplaceUntil(const TickClock::time_point timePoint, const uint8_t priority,
		const OperationCountingType::Value value) const
{
	return messageQueue_.tryEmplaceUntil(timePoint, priority, value);
}

int MessageQueueWrapper::tryPop(uint8_t& priority, OperationCountingType& value) const
{
	return messageQueue_.tryPop(priority, value);
}

int MessageQueueWrapper::tryPopFor(const TickClock::duration duration, uint8_t& priority, OperationCountingType& value)
		const
{
	return messageQueue_.tryPopFor(duration, priority, value);
}

int MessageQueueWrapper::tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority,
		OperationCountingType& value) const
{
	return messageQueue_.tryPopUntil(timePoint, priority, value);
}

int MessageQueueWrapper::tryPush(const uint8_t priority, const OperationCountingType& value) const
{
	return messageQueue_.tryPush(priority, value);
}

int MessageQueueWrapper::tryPush(const uint8_t priority, OperationCountingType&& value) const
{
	return messageQueue_.tryPush(priority, std::move(value));
}

int MessageQueueWrapper::tryPushFor(const TickClock::duration duration, const uint8_t priority,
		const OperationCountingType& value) const
{
	return messageQueue_.tryPushFor(duration, priority, value);
}

int MessageQueueWrapper::tryPushFor(const TickClock::duration duration, const uint8_t priority,
		OperationCountingType&& value) const
{
	return messageQueue_.tryPushFor(duration, priority, std::move(value));
}

int MessageQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority,
		const OperationCountingType& value) const
{
	return messageQueue_.tryPushUntil(timePoint, priority, value);
}

int MessageQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority,
		OperationCountingType&& value) const
{
	return messageQueue_.tryPushUntil(timePoint, priority, std::move(value));
}

/*---------------------------------------------------------------------------------------------------------------------+
| DynamicMessageQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicMessageQueueWrapper::DynamicMessageQueueWrapper(const size_t queueSize) :
		MessageQueueWrapper{messageQueue_},
		messageQueue_{queueSize}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| RawQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool RawQueueWrapper::checkCounters(size_t, size_t, size_t, size_t, size_t, size_t, size_t) const
{
	return true;
}

int RawQueueWrapper::emplace(const uint8_t priority, const OperationCountingType::Value value) const
{
	return push(priority, &value, sizeof(value));
}

int RawQueueWrapper::pop(uint8_t& priority, OperationCountingType& value) const
{
	OperationCountingType::Value rawValue;
	const auto ret = pop(priority, &rawValue, sizeof(rawValue));
	value = OperationCountingType{rawValue};
	return ret;
}

int RawQueueWrapper::push(const uint8_t priority, const OperationCountingType& value) const
{
	const auto rawValue = value.getValue();
	return push(priority, &rawValue, sizeof(rawValue));
}

int RawQueueWrapper::push(const uint8_t priority, OperationCountingType&& value) const
{
	return push(priority, value);
}

int RawQueueWrapper::tryEmplace(const uint8_t priority, const OperationCountingType::Value value) const
{
	return tryPush(priority, &value, sizeof(value));
}

int RawQueueWrapper::tryEmplaceFor(const TickClock::duration duration, const uint8_t priority,
		const OperationCountingType::Value value) const
{
	return tryPushFor(duration, priority, &value, sizeof(value));
}

int RawQueueWrapper::tryEmplaceUntil(const TickClock::time_point timePoint, const uint8_t priority,
		const OperationCountingType::Value value) const
{
	return tryPushUntil(timePoint, priority, &value, sizeof(value));
}

int RawQueueWrapper::tryPop(uint8_t& priority, OperationCountingType& value) const
{
	OperationCountingType::Value rawValue;
	const auto ret = tryPop(priority, &rawValue, sizeof(rawValue));
	value = OperationCountingType{rawValue};
	return ret;
}

int RawQueueWrapper::tryPopFor(const TickClock::duration duration, uint8_t& priority, OperationCountingType& value)
		const
{
	OperationCountingType::Value rawValue;
	const auto ret = tryPopFor(duration, priority, &rawValue, sizeof(rawValue));
	value = OperationCountingType{rawValue};
	return ret;
}

int RawQueueWrapper::tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority, OperationCountingType& value)
		const
{
	OperationCountingType::Value rawValue;
	const auto ret = tryPopUntil(timePoint, priority, &rawValue, sizeof(rawValue));
	value = OperationCountingType{rawValue};
	return ret;
}

int RawQueueWrapper::tryPush(const uint8_t priority, const OperationCountingType& value) const
{
	const auto rawValue = value.getValue();
	return tryPush(priority, &rawValue, sizeof(rawValue));
}

int RawQueueWrapper::tryPush(const uint8_t priority, OperationCountingType&& value) const
{
	return tryPush(priority, value);
}

int RawQueueWrapper::tryPushFor(const TickClock::duration duration, const uint8_t priority,
		const OperationCountingType& value) const
{
	const auto rawValue = value.getValue();
	return tryPushFor(duration, priority, &rawValue, sizeof(rawValue));
}

int RawQueueWrapper::tryPushFor(const TickClock::duration duration, const uint8_t priority,
		OperationCountingType&& value) const
{
	return tryPushFor(duration, priority, value);
}

int RawQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority,
		const OperationCountingType& value) const
{
	const auto rawValue = value.getValue();
	return tryPushUntil(timePoint, priority, &rawValue, sizeof(rawValue));
}

int RawQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority,
		OperationCountingType&& value) const
{
	return tryPushUntil(timePoint, priority, value);
}

/*---------------------------------------------------------------------------------------------------------------------+
| RawFifoQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool RawFifoQueueWrapper::check(uint8_t, const OperationCountingType& value1, uint8_t,
		const OperationCountingType& value2) const
{
	return value1 == value2;
}

int RawFifoQueueWrapper::pop(uint8_t& priority, void* const buffer, const size_t size) const
{
	priority = {};
	return rawFifoQueue_.pop(buffer, size);
}

int RawFifoQueueWrapper::push(uint8_t, const void* const data, const size_t size) const
{
	return rawFifoQueue_.push(data, size);
}

int RawFifoQueueWrapper::tryPop(uint8_t& priority, void* const buffer, const size_t size) const
{
	priority = {};
	return rawFifoQueue_.tryPop(buffer, size);
}

int RawFifoQueueWrapper::tryPopFor(const TickClock::duration duration, uint8_t& priority, void* const buffer,
		const size_t size) const
{
	priority = {};
	return rawFifoQueue_.tryPopFor(duration, buffer, size);
}

int RawFifoQueueWrapper::tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority, void* const buffer,
		const size_t size) const
{
	priority = {};
	return rawFifoQueue_.tryPopUntil(timePoint, buffer, size);
}

int RawFifoQueueWrapper::tryPush(uint8_t, const void* const data, const size_t size) const
{
	return rawFifoQueue_.tryPush(data, size);
}

int RawFifoQueueWrapper::tryPushFor(const TickClock::duration duration, uint8_t, const void* const data,
		const size_t size) const
{
	return rawFifoQueue_.tryPushFor(duration, data, size);
}

int RawFifoQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, uint8_t, const void* const data,
		const size_t size) const
{
	return rawFifoQueue_.tryPushUntil(timePoint, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------+
| DynamicRawFifoQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicRawFifoQueueWrapper::DynamicRawFifoQueueWrapper(const size_t queueSize) :
		RawFifoQueueWrapper{rawFifoQueue_},
		rawFifoQueue_{sizeof(OperationCountingType::Value), queueSize}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| RawMessageQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool RawMessageQueueWrapper::check(const uint8_t priority1, const OperationCountingType& value1,
		const uint8_t priority2, const OperationCountingType& value2) const
{
	return priority1 == priority2 && value1 == value2;
}

int RawMessageQueueWrapper::pop(uint8_t& priority, void* const buffer, const size_t size) const
{
	return rawMessageQueue_.pop(priority, buffer, size);
}

int RawMessageQueueWrapper::push(const uint8_t priority, const void* const data, const size_t size) const
{
	return rawMessageQueue_.push(priority, data, size);
}

int RawMessageQueueWrapper::tryPop(uint8_t& priority, void* const buffer, const size_t size) const
{
	return rawMessageQueue_.tryPop(priority, buffer, size);
}

int RawMessageQueueWrapper::tryPopFor(const TickClock::duration duration, uint8_t& priority, void* const buffer,
		const size_t size) const
{
	return rawMessageQueue_.tryPopFor(duration, priority, buffer, size);
}

int RawMessageQueueWrapper::tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority, void* const buffer,
		const size_t size) const
{
	return rawMessageQueue_.tryPopUntil(timePoint, priority, buffer, size);
}

int RawMessageQueueWrapper::tryPush(const uint8_t priority, const void* const data, const size_t size) const
{
	return rawMessageQueue_.tryPush(priority, data, size);
}

int RawMessageQueueWrapper::tryPushFor(const TickClock::duration duration, const uint8_t priority,
		const void* const data, const size_t size) const
{
	return rawMessageQueue_.tryPushFor(duration, priority, data, size);
}

int RawMessageQueueWrapper::tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority,
		const void* const data, const size_t size) const
{
	return rawMessageQueue_.tryPushUntil(timePoint, priority, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------+
| DynamicRawMessageQueueWrapper public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicRawMessageQueueWrapper::DynamicRawMessageQueueWrapper(const size_t queueSize) :
		RawMessageQueueWrapper{rawMessageQueue_},
		rawMessageQueue_{sizeof(OperationCountingType::Value), queueSize}
{

}

}	// namespace test

}	// namespace distortos
