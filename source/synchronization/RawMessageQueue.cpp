/**
 * \file
 * \brief RawMessageQueue class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-22
 */

#include "distortos/RawMessageQueue.hpp"

#include "distortos/synchronization/MemcpyPopQueueFunctor.hpp"
#include "distortos/synchronization/MemcpyPushQueueFunctor.hpp"
#include "distortos/synchronization/SemaphoreWaitFunctor.hpp"
#include "distortos/synchronization/SemaphoreTryWaitFunctor.hpp"
#include "distortos/synchronization/SemaphoreTryWaitForFunctor.hpp"
#include "distortos/synchronization/SemaphoreTryWaitUntilFunctor.hpp"

#include <cstring>
#include <cerrno>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

RawMessageQueue::RawMessageQueue(EntryStorageUniquePointer&& entryStorageUniquePointer,
		memory::StorageUniquePointer&& valueStorageUniquePointer, const size_t elementSize, const size_t maxElements) :
		messageQueueBase_{std::move(entryStorageUniquePointer), std::move(valueStorageUniquePointer), elementSize,
				maxElements},
		elementSize_{elementSize}
{

}

int RawMessageQueue::pop(uint8_t& priority, void* const buffer, const size_t size)
{
	const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
	return popInternal(semaphoreWaitFunctor, priority, buffer, size);
}

int RawMessageQueue::push(const uint8_t priority, const void* const data, const size_t size)
{
	const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
	return pushInternal(semaphoreWaitFunctor, priority, data, size);
}

int RawMessageQueue::tryPop(uint8_t& priority, void* const buffer, const size_t size)
{
	const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
	return popInternal(semaphoreTryWaitFunctor, priority, buffer, size);
}

int RawMessageQueue::tryPopFor(const TickClock::duration duration, uint8_t& priority, void* const buffer,
		const size_t size)
{
	const synchronization::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
	return popInternal(semaphoreTryWaitForFunctor, priority, buffer, size);
}

int RawMessageQueue::tryPopUntil(const TickClock::time_point timePoint, uint8_t& priority, void* const buffer,
		const size_t size)
{
	const synchronization::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
	return popInternal(semaphoreTryWaitUntilFunctor, priority, buffer, size);
}

int RawMessageQueue::tryPush(const uint8_t priority, const void* const data, const size_t size)
{
	const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
	return pushInternal(semaphoreTryWaitFunctor, priority, data, size);
}

int RawMessageQueue::tryPushFor(const TickClock::duration duration, const uint8_t priority, const void* const data,
		const size_t size)
{
	const synchronization::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
	return pushInternal(semaphoreTryWaitForFunctor, priority, data, size);
}

int RawMessageQueue::tryPushUntil(const TickClock::time_point timePoint, const uint8_t priority, const void* const data,
		const size_t size)
{
	const synchronization::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
	return pushInternal(semaphoreTryWaitUntilFunctor, priority, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int RawMessageQueue::popInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, uint8_t& priority,
		void* const buffer, const size_t size)
{
	if (size != elementSize_)
		return EMSGSIZE;

	const synchronization::MemcpyPopQueueFunctor memcpyPopQueueFunctor {buffer, size};
	return messageQueueBase_.pop(waitSemaphoreFunctor, priority, memcpyPopQueueFunctor);
}

int RawMessageQueue::pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		const void* const data, const size_t size)
{
	if (size != elementSize_)
		return EMSGSIZE;

	const synchronization::MemcpyPushQueueFunctor memcpyPushQueueFunctor {data, size};
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, memcpyPushQueueFunctor);
}

}	// namespace distortos
