/**
 * \file
 * \brief FifoQueueBase class implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-12-10
 */

#include "distortos/scheduler/FifoQueueBase.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

namespace distortos
{

namespace scheduler
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int FifoQueueBase::popPushImplementation(const SemaphoreFunctor& waitSemaphoreFunctor, const Functor& functor,
		Semaphore& waitSemaphore, Semaphore& postSemaphore, void*& storage)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = waitSemaphoreFunctor(waitSemaphore);
	if (ret != 0)
		return ret;

	functor(storage);

	if (storage == storageEnd_)
		storage = storageBegin_;

	return postSemaphore.post();
}

}	// namespace scheduler

}	// namespace distortos
