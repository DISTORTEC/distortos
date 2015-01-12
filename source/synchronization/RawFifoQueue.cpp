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
 * \date 2015-01-12
 */

#include "distortos/RawFifoQueue.hpp"

#include "distortos/synchronization/SemaphoreWaitFunctor.hpp"
#include "distortos/synchronization/SemaphoreTryWaitFunctor.hpp"
#include "distortos/scheduler/SemaphoreTryWaitForFunctor.hpp"
#include "distortos/scheduler/SemaphoreTryWaitUntilFunctor.hpp"

#include <cstring>
#include <cerrno>

namespace distortos
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// PopFunctor is a functor used for popping of data from the RawFifoQueue
class PopFunctor : public scheduler::FifoQueueBase::Functor
{
public:

	/**
	 * \brief PopFunctor's constructor
	 *
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes
	 */

	constexpr PopFunctor(void* const buffer, const size_t size) :
			buffer_{buffer},
			size_{size}
	{

	}

	/**
	 * \brief Copies the data from RawFifoQueue's storage (with memcpy()) and increments the storage pointer to next
	 * position (using the size of element).
	 *
	 * \param [in,out] storage is a reference to pointer to queue's storage
	 */

	virtual void operator()(void*& storage) const override
	{
		memcpy(buffer_, storage, size_);
		storage = static_cast<uint8_t*>(storage) + size_;
	}

private:

	/// pointer to buffer for popped element
	void* const buffer_;

	/// size of \a buffer_, bytes
	const size_t size_;
};

/// PushFunctor is a functor used for pushing of data to the RawFifoQueue
class PushFunctor : public scheduler::FifoQueueBase::Functor
{
public:

	/**
	 * \brief PushFunctor's constructor
	 *
	 * \param [in] data is a pointer to data that will be pushed to RawFifoQueue
	 * \param [in] size is the size of \a data, bytes
	 */

	constexpr PushFunctor(const void* const data, const size_t size) :
			data_{data},
			size_{size}
	{

	}

	/**
	 * \brief Copies the data to RawFifoQueue's storage (with memcpy()) and increments the storage pointer to next
	 * position (using the size of element).
	 *
	 * \param [in,out] storage is a reference to pointer to queue's storage
	 */

	virtual void operator()(void*& storage) const override
	{
		memcpy(storage, data_, size_);
		storage = static_cast<uint8_t*>(storage) + size_;
	}

private:

	/// pointer to data that will be pushed to RawFifoQueue
	const void* const data_;

	/// size of \a data_, bytes
	const size_t size_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

RawFifoQueue::RawFifoQueue(void* const storage, const size_t elementSize, const size_t maxElements) :
		fifoQueueBase_{storage, static_cast<uint8_t*>(storage) + elementSize * maxElements, maxElements},
		elementSize_{elementSize}
{

}

int RawFifoQueue::pop(void* const buffer, const size_t size)
{
	const scheduler::SemaphoreWaitFunctor semaphoreWaitFunctor;
	return popInternal(semaphoreWaitFunctor, buffer, size);
}

int RawFifoQueue::push(const void* const data, const size_t size)
{
	const scheduler::SemaphoreWaitFunctor semaphoreWaitFunctor;
	return pushInternal(semaphoreWaitFunctor, data, size);
}

int RawFifoQueue::tryPop(void* const buffer, const size_t size)
{
	const scheduler::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
	return popInternal(semaphoreTryWaitFunctor, buffer, size);
}

int RawFifoQueue::tryPopFor(const TickClock::duration duration, void* const buffer, const size_t size)
{
	const scheduler::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
	return popInternal(semaphoreTryWaitForFunctor, buffer, size);
}

int RawFifoQueue::tryPopUntil(const TickClock::time_point timePoint, void* const buffer, const size_t size)
{
	const scheduler::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
	return popInternal(semaphoreTryWaitUntilFunctor, buffer, size);
}

int RawFifoQueue::tryPush(const void* const data, const size_t size)
{
	const scheduler::SemaphoreTryWaitFunctor semaphoreTryWaitFunctor;
	return pushInternal(semaphoreTryWaitFunctor, data, size);
}

int RawFifoQueue::tryPushFor(const TickClock::duration duration, const void* const data, const size_t size)
{
	const scheduler::SemaphoreTryWaitForFunctor semaphoreTryWaitForFunctor {duration};
	return pushInternal(semaphoreTryWaitForFunctor, data, size);
}

int RawFifoQueue::tryPushUntil(const TickClock::time_point timePoint, const void* const data, const size_t size)
{
	const scheduler::SemaphoreTryWaitUntilFunctor semaphoreTryWaitUntilFunctor {timePoint};
	return pushInternal(semaphoreTryWaitUntilFunctor, data, size);
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int RawFifoQueue::popInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, void* const buffer,
		const size_t size)
{
	if (size != elementSize_)
		return EMSGSIZE;

	const PopFunctor popFunctor {buffer, size};
	return fifoQueueBase_.pop(waitSemaphoreFunctor, popFunctor);
}

int RawFifoQueue::pushInternal(const scheduler::SemaphoreFunctor& waitSemaphoreFunctor, const void* const data,
		const size_t size)
{
	if (size != elementSize_)
		return EMSGSIZE;

	const PushFunctor pushFunctor {data, size};
	return fifoQueueBase_.push(waitSemaphoreFunctor, pushFunctor);
}

}	// namespace distortos
