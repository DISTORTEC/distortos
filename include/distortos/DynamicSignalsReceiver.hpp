/**
 * \file
 * \brief DynamicSignalsReceiver class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICSIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_DYNAMICSIGNALSRECEIVER_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/SignalInformationQueueWrapper.hpp"
#include "distortos/SignalsCatcher.hpp"
#include "distortos/SignalsReceiver.hpp"

namespace distortos
{

/**
 * \brief DynamicSignalsReceiver class is a templated interface for SignalsReceiver that has dynamic storage for queued
 * signals and SignalAction associations required for catching signals.
 */

class DynamicSignalsReceiver : public SignalsReceiver
{
public:

	/**
	 * \brief DynamicSignalsReceiver's constructor
	 *
	 * \param [in] queuedSignals is the max number of queued signals, 0 to disable queuing of signals for this receiver
	 * \param [in] signalActions is the max number of different SignalAction objects, 0 to disable catching of signals
	 * for this receiver
	 */

	DynamicSignalsReceiver(size_t queuedSignals, size_t signalActions);

private:

	/// internal SignalInformationQueueWrapper object
	SignalInformationQueueWrapper signalInformationQueueWrapper_;

	/// internal SignalsCatcher object
	SignalsCatcher signalsCatcher_;
};

}	// namespace distortos

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_DYNAMICSIGNALSRECEIVER_HPP_
