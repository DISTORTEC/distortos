/**
 * \file
 * \brief ThisThread::Signals namespace header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-17
 */

#ifndef INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_
#define INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_

#include <utility>

#include <cstdint>

namespace distortos
{

class SignalSet;

namespace ThisThread
{

/// Signals namespace groups signals-related functions operating on current thread
namespace Signals
{

/**
 * \brief Waits for signal.
 *
 * Similar to sigwait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sigwait.html
 *
 * This function shall select the lowest pending signal from provided set, atomically clear it from current thread's set
 * of pending signals and return that signal number. If no signal in provided set is pending at the time of the call,
 * the thread shall be suspended until one or more becomes pending.
 *
 * \param [in] signalSet is a reference to set of signals that will be waited for
 *
 * \return pair with return code (0 on success, error code otherwise) and signal number of signal that was accepted
 */

std::pair<int, uint8_t> wait(const SignalSet& signalSet);

}	// namespace Signals

}	// namespace ThisThread

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_THISTHREAD_SIGNALS_HPP_
