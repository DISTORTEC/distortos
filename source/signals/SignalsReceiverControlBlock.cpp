/**
 * \file
 * \brief SignalsReceiverControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-14
 */

#include "distortos/signals/SignalsReceiverControlBlock.hpp"

#include "distortos/scheduler/ThreadControlBlock.hpp"

#include <cerrno>

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SignalsReceiverControlBlock::acceptPendingSignal(const uint8_t signalNumber) const
{
	if (threadControlBlock_ == nullptr)
		return ENOTSUP;

	return threadControlBlock_->acceptPendingSignal(signalNumber);
}

void SignalsReceiverControlBlock::setWaitingSignalSet(const SignalSet* const signalSet) const
{
	if (threadControlBlock_ == nullptr)
		return;

	threadControlBlock_->setWaitingSignalSet(signalSet);
}

}	// namespace synchronization

}	// namespace distortos
