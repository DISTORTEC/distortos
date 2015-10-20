/**
 * \file
 * \brief RawFifoQueue class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-20
 */

#include "distortos/RawFifoQueue.hpp"

#include "distortos/memory/dummyDeleter.hpp"

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

RawFifoQueue::RawFifoQueue(void* const storage, const size_t elementSize, const size_t maxElements) :
		fifoQueueBase_{{storage, memory::dummyDeleter}, elementSize, maxElements}
{

}

int RawFifoQueue::pop(void* const buffer, const size_t size)
{
	const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
	return popInternal(semaphoreWaitFunctor, buffer, size);
}

int RawFifoQueue::push(const void* const data, const size_t size)
{
	const synchronization::SemaphoreWaitFunctor semaphoreWaitFunctor;
	return pushInternal(semaphoreWaitFunctor, data, size);
}

int RawFifoQueue::tryPop(void* const buffer, const size_t size)
{
	const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
	return popInternal(semaphoreTryWaitFunctor, buffer, size);
}

int RawFifoQueue::tryPopFor(const TickClock::duration duration, void* const buffer, const size_t size)
{
	const synchronization::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
	return popInternal(semaphoreTryWaitForFunctor, buffer, size);
}

int RawFifoQueue::tryPopUntil(const TickClock::time_point timePoint, void* const buffer, const size_t size)
{
	const synchronization::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
	return popInternal(semaphoreTryWaitUntilFunctor, buffer, size);
}

int RawFifoQueue::tryPush(const void* const data, const size_t size)
{
	const synchronization::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
	return pushInternal(semaphoreTryWaitFunctor, data, size);
}

int RawFifoQueue::tryPushFor(const TickClock::duration duration, const void* const data, const size_t size)
{
	const synchronization::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
	return pushInternal(semaphoreTryWaitForFunctor, data, size);
}

int RawFifoQueue::tryPushUntil(const TickClock::time_point timePoint, const void* const data, const size_t size)
{
	const synchronization::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
	return pushInternal(semaphoreTryWaitUntilFunctor, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int RawFifoQueue::popInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, void* const buffer,
		const size_t size)
{
	if (size != fifoQueueBase_.getElementSize())
		return EMSGSIZE;

	const synchronization::MemcpyPopQueueFunctor memcpyPopQueueFunctor {buffer, size};
	return fifoQueueBase_.pop(waitSemaphoreFunctor, memcpyPopQueueFunctor);
}

int RawFifoQueue::pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, const void* const data,
		const size_t size)
{
	if (size != fifoQueueBase_.getElementSize())
		return EMSGSIZE;

	const synchronization::MemcpyPushQueueFunctor memcpyPushQueueFunctor {data, size};
	return fifoQueueBase_.push(waitSemaphoreFunctor, memcpyPushQueueFunctor);
}

}	// namespace distortos
