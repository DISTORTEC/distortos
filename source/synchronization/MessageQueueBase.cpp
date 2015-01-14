/**
 * \file
 * \brief MessageQueueBase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-14
 */

#include "distortos/synchronization/MessageQueueBase.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

MessageQueueBase::MessageQueueBase(const size_t maxElements) :
		popSemaphore_{0, maxElements},
		pushSemaphore_{maxElements, maxElements},
		pool_{},
		poolAllocator_{pool_},
		entryList_{poolAllocator_},
		freeEntryList_{poolAllocator_}
{

}

int MessageQueueBase::popPush(const SemaphoreFunctor& waitSemaphoreFunctor, const InternalFunctor& internalFunctor,
		Semaphore& waitSemaphore, Semaphore& postSemaphore)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = waitSemaphoreFunctor(waitSemaphore);
	if (ret != 0)
		return ret;

	internalFunctor(entryList_, freeEntryList_);

	return postSemaphore.post();
}

}	// namespace synchronization

}	// namespace distortos
