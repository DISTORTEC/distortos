/**
 * \file
 * \brief FifoQueueBase class implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-20
 */

#include "distortos/synchronization/FifoQueueBase.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/memory/dummyDeleter.hpp"

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

FifoQueueBase::FifoQueueBase(void* const storage, const size_t elementSize, const size_t maxElements) :
		popSemaphore_{0, maxElements},
		pushSemaphore_{maxElements, maxElements},
		storageUniquePointer_{storage, memory::dummyDeleter},
		storageEnd_{static_cast<uint8_t*>(storage) + elementSize * maxElements},
		readPosition_{storage},
		writePosition_{storage},
		elementSize_{elementSize}
{

}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int FifoQueueBase::popPush(const SemaphoreFunctor& waitSemaphoreFunctor, const QueueFunctor& functor,
		Semaphore& waitSemaphore, Semaphore& postSemaphore, void*& storage)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = waitSemaphoreFunctor(waitSemaphore);
	if (ret != 0)
		return ret;

	functor(storage);

	storage = static_cast<uint8_t*>(storage) + elementSize_;
	if (storage >= storageEnd_)
		storage = storageUniquePointer_.get();

	return postSemaphore.post();
}

}	// namespace synchronization

}	// namespace distortos
