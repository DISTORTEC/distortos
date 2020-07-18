/**
 * \file
 * \brief ThreadIdentifier class implementation
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/ThreadIdentifier.hpp"

#include "distortos/internal/scheduler/RunnableThread.hpp"
#include "distortos/internal/scheduler/ThreadControlBlock.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Thread* ThreadIdentifier::getThread() const
{
	if (threadControlBlock_ == nullptr || threadControlBlock_->getSequenceNumber() != sequenceNumber_)
		return nullptr;

	return &threadControlBlock_->getOwner();
}

}	// namespace distortos
