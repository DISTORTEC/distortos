/**
 * \file
 * \brief DynamicSignalsReceiver class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-17
 */

#ifndef INCLUDE_DISTORTOS_DYNAMICSIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_DYNAMICSIGNALSRECEIVER_HPP_

#include "distortos/SignalInformationQueueWrapper.hpp"
#include "distortos/SignalsCatcher.hpp"
#include "distortos/SignalsReceiver.hpp"

#include <memory>

namespace distortos
{

/**
 * \brief DynamicSignalsReceiver class is a wrapper for SignalsReceiver that also provides dynamic storage for queued
 * signals and SignalAction associations required for catching signals.
 *
 * \note Objects of this class can be safely casted to (const) reference to SignalsReceiver.
 */

class DynamicSignalsReceiver
{
public:

	/// unique_ptr to SignalInformationQueueWrapper::Storage
	using SignalInformationStorageUniquePointer = std::unique_ptr<SignalInformationQueueWrapper::Storage>;

	/// unique_ptr to SignalsCatcher::Storage
	using SignalsCatcherStorageUniquePointer = std::unique_ptr<SignalsCatcher::Storage>;

	/**
	 * \brief DynamicSignalsReceiver's constructor
	 *
	 * \param [in] queuedSignals is the max number of queued signals, 0 to disable queuing of signals for this receiver
	 * \param [in] signalActions is the max number of different SignalAction objects, 0 to disable catching of signals
	 * for this receiver
	 */

	DynamicSignalsReceiver(size_t queuedSignals, size_t signalActions);

	/**
	 * \brief DynamicSignalsReceiver's destructor
	 */

	~DynamicSignalsReceiver();

	/**
	 * \brief conversion to SignalsReceiver&
	 *
	 * \return reference to internal SignalsReceiver object
	 */

	operator SignalsReceiver&()
	{
		return signalsReceiver_;
	}

	/**
	 * \brief conversion to const SignalsReceiver&
	 *
	 * \return const reference to internal SignalsReceiver object
	 */

	operator const SignalsReceiver&() const
	{
		return signalsReceiver_;
	}

	DynamicSignalsReceiver(const DynamicSignalsReceiver&) = delete;
	DynamicSignalsReceiver(DynamicSignalsReceiver&&) = default;
	const DynamicSignalsReceiver& operator=(const DynamicSignalsReceiver&) = delete;
	DynamicSignalsReceiver& operator=(DynamicSignalsReceiver&&) = delete;

private:

	/// unique_ptr to allocated storage for \a signalInformationQueueWrapper_
	SignalInformationStorageUniquePointer signalInformationStorageUniquePointer_;

	/// internal SignalInformationQueueWrapper object
	SignalInformationQueueWrapper signalInformationQueueWrapper_;

	/// unique_ptr to allocated storage for \a signalsCatcher_
	SignalsCatcherStorageUniquePointer signalsCatcherStorageUniquePointer_;

	/// internal SignalsCatcher object
	SignalsCatcher signalsCatcher_;

	/// internal SignalsReceiver object
	SignalsReceiver signalsReceiver_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DYNAMICSIGNALSRECEIVER_HPP_
