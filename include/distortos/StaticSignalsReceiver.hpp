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
 * \date 2015-10-24
 */

#ifndef INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
#define INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_

#include "distortos/SignalInformationQueueWrapper.hpp"
#include "distortos/SignalsCatcher.hpp"
#include "distortos/SignalsReceiver.hpp"

#include "distortos/memory/dummyDeleter.hpp"

namespace distortos
{

/**
 * \brief StaticSignalsReceiver class is a templated wrapper for SignalsReceiver that also provides automatic storage
 * for queued signals and SignalAction associations required for catching signals.
 *
 * \note Objects of this class can be safely casted to (const) reference to SignalsReceiver.
 *
 * \param QueuedSignals is the max number of queued signals, 0 to disable queuing of signals for this receiver
 * \param SignalActions is the max number of different SignalAction objects, 0 to disable catching of signals for this
 * receiver
 */

template<size_t QueuedSignals, size_t SignalActions>
class StaticSignalsReceiver
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			signalInformationQueueWrapper_{{signalInformationQueueWrapperStorage_.data(), memory::dummyDeleter},
					signalInformationQueueWrapperStorage_.size()},
			signalsCatcher_{signalsCatcherStorage_.data(), signalsCatcherStorage_.size()},
			signalsReceiver_{&signalInformationQueueWrapper_, &signalsCatcher_}
	{

	}

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

private:

	/// storage for \a signalInformationQueueWrapper_
	std::array<SignalInformationQueueWrapper::Storage, QueuedSignals> signalInformationQueueWrapperStorage_;

	/// internal SignalInformationQueueWrapper object
	SignalInformationQueueWrapper signalInformationQueueWrapper_;

	/// storage for \a signalsCatcher_
	std::array<SignalsCatcher::Storage, SignalActions> signalsCatcherStorage_;

	/// internal SignalsCatcher object
	SignalsCatcher signalsCatcher_;

	/// internal SignalsReceiver object
	SignalsReceiver signalsReceiver_;
};

/**
 * \brief StaticSignalsReceiver class is a templated wrapper for SignalsReceiver that also provides automatic storage
 * for queued signals and SignalAction associations required for catching signals.
 *
 * \note Objects of this class can be safely casted to (const) reference to SignalsReceiver.
 *
 * \param QueuedSignals is the max number of queued signals
 *
 * Specialization for receiver with enabled queuing (QueuedSignals != 0) and disabled catching (SignalActions == 0) of
 * signals
 */

template<size_t QueuedSignals>
class StaticSignalsReceiver<QueuedSignals, 0>
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			signalInformationQueueWrapper_{{signalInformationQueueWrapperStorage_.data(), memory::dummyDeleter},
					signalInformationQueueWrapperStorage_.size()},
			signalsReceiver_{&signalInformationQueueWrapper_, nullptr}
	{

	}

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

private:

	/// storage for \a signalInformationQueueWrapper_
	std::array<SignalInformationQueueWrapper::Storage, QueuedSignals> signalInformationQueueWrapperStorage_;

	/// internal SignalInformationQueueWrapper object
	SignalInformationQueueWrapper signalInformationQueueWrapper_;

	/// internal SignalsReceiver object
	SignalsReceiver signalsReceiver_;
};

/**
 * \brief StaticSignalsReceiver class is a templated wrapper for SignalsReceiver that also provides automatic storage
 * for queued signals and SignalAction associations required for catching signals.
 *
 * \note Objects of this class can be safely casted to (const) reference to SignalsReceiver.
 *
 * \param SignalActions is the max number of different SignalAction objects
 *
 * Specialization for receiver with disabled queuing (QueuedSignals == 0) and enabled catching (SignalActions != 0) of
 * signals
 */

template<size_t SignalActions>
class StaticSignalsReceiver<0, SignalActions>
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			signalsCatcher_{signalsCatcherStorage_.data(), signalsCatcherStorage_.size()},
			signalsReceiver_{nullptr, &signalsCatcher_}
	{

	}

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

private:

	/// storage for \a signalsCatcher_
	std::array<SignalsCatcher::Storage, SignalActions> signalsCatcherStorage_;

	/// internal SignalsCatcher object
	SignalsCatcher signalsCatcher_;

	/// internal SignalsReceiver object
	SignalsReceiver signalsReceiver_;
};

/**
 * \brief StaticSignalsReceiver class is a templated wrapper for SignalsReceiver that also provides automatic storage
 * for queued signals and SignalAction associations required for catching signals.
 *
 * \note Objects of this class can be safely casted to (const) reference to SignalsReceiver.
 *
 * Specialization for receiver with disabled queuing (QueuedSignals == 0) and catching (SignalActions == 0) of signals
 */

template<>
class StaticSignalsReceiver<0, 0>
{
public:

	/**
	 * \brief StaticSignalsReceiver's constructor
	 */

	StaticSignalsReceiver() :
			signalsReceiver_{nullptr, nullptr}
	{

	}

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

private:

	/// internal SignalsReceiver object
	SignalsReceiver signalsReceiver_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_STATICSIGNALSRECEIVER_HPP_
