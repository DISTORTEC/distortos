/**
 * \file
 * \brief StaticSignalsReceiver class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-26
 */

#ifndef INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_

#include "distortos/SignalsReceiver.hpp"

namespace distortos
{

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals.
 *
 * \param QueuedSignals is the max number of queued signals, 0 to disable queuing of signals for this receiver
 */

template<size_t QueuedSignals>
class StaticSignalsReceiver : public SignalsReceiver
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			SignalsReceiver{nullptr}
	{

	}
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
