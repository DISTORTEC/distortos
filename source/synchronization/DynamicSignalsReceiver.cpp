/**
 * \file
 * \brief DynamicSignalsReceiver class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/DynamicSignalsReceiver.hpp"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/internal/memory/storageDeleter.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

DynamicSignalsReceiver::DynamicSignalsReceiver(const size_t queuedSignals, const size_t signalActions) :
		SignalsReceiver{queuedSignals != 0 ? &signalInformationQueueWrapper_ : nullptr,
				signalActions != 0 ? &signalsCatcher_ : nullptr},
		signalInformationQueueWrapper_{{queuedSignals != 0 ? new SignalInformationQueueWrapper::Storage[queuedSignals] :
				nullptr, internal::storageDeleter<SignalInformationQueueWrapper::Storage>}, queuedSignals},
		signalsCatcher_{{signalActions != 0 ? new SignalsCatcher::Storage[signalActions] : nullptr,
				internal::storageDeleter<SignalsCatcher::Storage>}, signalActions}
{

}

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1
