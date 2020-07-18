/**
 * \file
 * \brief FifoQueueBase class implementation
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/synchronization/FifoQueueBase.hpp"

#include "distortos/InterruptMaskingLock.hpp"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

FifoQueueBase::FifoQueueBase(StorageUniquePointer&& storageUniquePointer, const size_t elementSize,
		const size_t maxElements) :
		popSemaphore_{0, maxElements},
		pushSemaphore_{maxElements, maxElements},
		storageUniquePointer_{std::move(storageUniquePointer)},
		storageEnd_{static_cast<uint8_t*>(storageUniquePointer_.get()) + elementSize * maxElements},
		readPosition_{storageUniquePointer_.get()},
		writePosition_{storageUniquePointer_.get()},
		elementSize_{elementSize}
{

}

FifoQueueBase::~FifoQueueBase()
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int FifoQueueBase::popPush(const SemaphoreFunctor& waitSemaphoreFunctor, const QueueFunctor& functor,
		Semaphore& waitSemaphore, Semaphore& postSemaphore, void*& storage)
{
	const InterruptMaskingLock interruptMaskingLock;

	const auto ret = waitSemaphoreFunctor(waitSemaphore);
	if (ret != 0)
		return ret;

	functor(storage);

	storage = static_cast<uint8_t*>(storage) + elementSize_;
	if (storage >= storageEnd_)
		storage = storageUniquePointer_.get();

	return postSemaphore.post();
}

}	// namespace internal

}	// namespace distortos
