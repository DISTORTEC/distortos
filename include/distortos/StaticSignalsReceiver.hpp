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
 * \date 2015-04-16
 */

#ifndef INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_

#include "distortos/SignalsReceiver.hpp"
#include "distortos/StaticSignalInformationQueueWrapper.hpp"

namespace distortos
{

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * \param QueuedSignals is the max number of queued signals, 0 to disable queuing of signals for this receiver
 * \param CaughtSignals is the max number of caught signals, 0 to disable catching of signals for this receiver
 */

template<size_t QueuedSignals, size_t CaughtSignals>
class StaticSignalsReceiver : public SignalsReceiver
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			SignalsReceiver{&staticSignalInformationQueueWrapper_, nullptr},
			staticSignalInformationQueueWrapper_{}
	{

	}

private:

	/// internal StaticSignalInformationQueueWrapper object
	StaticSignalInformationQueueWrapper<QueuedSignals> staticSignalInformationQueueWrapper_;
};

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * \param QueuedSignals is the max number of queued signals
 *
 * Specialization for receiver with enabled queuing (QueuedSignals != 0) and disabled catching (CaughtSignals == 0) of
 * signals
 */

template<size_t QueuedSignals>
class StaticSignalsReceiver<QueuedSignals, 0> : public SignalsReceiver
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			SignalsReceiver{&staticSignalInformationQueueWrapper_, nullptr},
			staticSignalInformationQueueWrapper_{}
	{

	}

private:

	/// internal StaticSignalInformationQueueWrapper object
	StaticSignalInformationQueueWrapper<QueuedSignals> staticSignalInformationQueueWrapper_;
};

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * Specialization for receiver with disabled queuing (QueuedSignals == 0) and catching (CaughtSignals == 0) of signals
 */

template<>
class StaticSignalsReceiver<0, 0> : public SignalsReceiver
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			SignalsReceiver{nullptr, nullptr}
	{

	}
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
