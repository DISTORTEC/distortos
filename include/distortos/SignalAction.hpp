/**
 * \file
 * \brief SignalAction class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SIGNALACTION_HPP_
#define INCLUDE_DISTORTOS_SIGNALACTION_HPP_

#include "distortos/SignalSet.hpp"

namespace distortos
{

class SignalInformation;

/**
 * \brief SignalAction class contains information needed to handle signal that was caught
 *
 * Similar to \a sigaction - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/signal.h.html
 *
 * \ingroup signals
 */

class SignalAction
{
public:

	/// handler function
	using Handler = void(const SignalInformation&);

	/**
	 * \brief SignalAction's constructor which uses "default" signal handler.
	 */

	constexpr SignalAction() :
			signalMask_{SignalSet::empty},
			handler_{}
	{

	}

	/**
	 * \brief SignalAction's constructor.
	 *
	 * \param [in] handler is a reference to handler function (similar to \a sa_sigaction member of \a sigaction)
	 * \param [in] signalMask is the additional set of signals to be masked during execution of signal-catching function
	 * (similar to \a sa_mask member of \a sigaction)
	 */

	constexpr SignalAction(Handler& handler, const SignalSet signalMask) :
			signalMask_{signalMask},
			handler_{&handler}
	{

	}

	/**
	 * \return pointer to handler function (similar to \a sa_sigaction member of \a sigaction), nullptr if use of
	 * default handler was configured (similar to \a SIG_DFL)
	 */

	Handler* getHandler() const
	{
		return handler_;
	}

	/**
	 * \return additional set of signals to be masked during execution of signal-catching function (similar to
	 * \a sa_mask member of \a sigaction)
	 */

	SignalSet getSignalMask() const
	{
		return signalMask_;
	}

private:

	/// additional set of signals to be masked during execution of signal-catching function (similar to \a sa_mask
	/// member of \a sigaction)
	SignalSet signalMask_;

	/// pointer to handler function (similar to \a sa_sigaction member of \a sigaction), nullptr to use default handler
	/// (similar to \a SIG_DFL)
	Handler* handler_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALACTION_HPP_
