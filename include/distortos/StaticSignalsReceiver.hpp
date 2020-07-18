/**
 * \file
 * \brief StaticSignalsReceiver class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/SignalInformationQueueWrapper.hpp"
#include "distortos/SignalsCatcher.hpp"
#include "distortos/SignalsReceiver.hpp"

#include "distortos/internal/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * \tparam QueuedSignals is the max number of queued signals, 0 to disable queuing of signals for this receiver
 * \tparam SignalActions is the max number of different SignalAction objects, 0 to disable catching of signals for this
 * receiver
 */

template<size_t QueuedSignals, size_t SignalActions>
class StaticSignalsReceiver : public SignalsReceiver
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			SignalsReceiver{&signalInformationQueueWrapper_, &signalsCatcher_},
			signalInformationQueueWrapper_{{signalInformationQueueWrapperStorage_.data(),
					internal::dummyDeleter<SignalInformationQueueWrapper::Storage>},
					signalInformationQueueWrapperStorage_.size()},
			signalsCatcher_{{signalsCatcherStorage_.data(), internal::dummyDeleter<SignalsCatcher::Storage>},
					signalsCatcherStorage_.size()}
	{

	}

private:

	/// storage for \a signalInformationQueueWrapper_
	std::array<SignalInformationQueueWrapper::Storage, QueuedSignals> signalInformationQueueWrapperStorage_;

	/// internal SignalInformationQueueWrapper object
	SignalInformationQueueWrapper signalInformationQueueWrapper_;

	/// storage for \a signalsCatcher_
	std::array<SignalsCatcher::Storage, SignalActions> signalsCatcherStorage_;

	/// internal SignalsCatcher object
	SignalsCatcher signalsCatcher_;
};

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * \tparam QueuedSignals is the max number of queued signals
 *
 * Specialization for receiver with enabled queuing (QueuedSignals != 0) and disabled catching (SignalActions == 0) of
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
			SignalsReceiver{&signalInformationQueueWrapper_, nullptr},
			signalInformationQueueWrapper_{{signalInformationQueueWrapperStorage_.data(),
					internal::dummyDeleter<SignalInformationQueueWrapper::Storage>},
					signalInformationQueueWrapperStorage_.size()}
	{

	}

private:

	/// storage for \a signalInformationQueueWrapper_
	std::array<SignalInformationQueueWrapper::Storage, QueuedSignals> signalInformationQueueWrapperStorage_;

	/// internal SignalInformationQueueWrapper object
	SignalInformationQueueWrapper signalInformationQueueWrapper_;
};

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * \tparam SignalActions is the max number of different SignalAction objects
 *
 * Specialization for receiver with disabled queuing (QueuedSignals == 0) and enabled catching (SignalActions != 0) of
 * signals
 */

template<size_t SignalActions>
class StaticSignalsReceiver<0, SignalActions> : public SignalsReceiver
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			SignalsReceiver{nullptr, &signalsCatcher_},
			signalsCatcher_{{signalsCatcherStorage_.data(), internal::dummyDeleter<SignalsCatcher::Storage>},
					signalsCatcherStorage_.size()}
	{

	}

private:

	/// storage for \a signalsCatcher_
	std::array<SignalsCatcher::Storage, SignalActions> signalsCatcherStorage_;

	/// internal SignalsCatcher object
	SignalsCatcher signalsCatcher_;
};

/**
 * \brief StaticSignalsReceiver class is a templated interface for SignalsReceiver that has automatic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * Specialization for receiver with disabled queuing (QueuedSignals == 0) and catching (SignalActions == 0) of signals
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

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
