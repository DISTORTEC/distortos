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
 * \date 2014-12-06
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

int FifoQueueBase::pushImplementation(Functor& functor)
{
	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = pushSemaphore_.wait();
	if (ret != 0)
		return ret;

	functor(writePosition_);

	if (writePosition_ == storageEnd_)
		writePosition_ = storageBegin_;

	return popSemaphore_.post();
}

}	// namespace scheduler

}	// namespace distortos
