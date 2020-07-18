/**
 * \file
 * \brief signalsTestSelfOneSignalPending() definition
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "signalsTestSelfOneSignalPending.hpp"

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "distortos/ThisThread-Signals.hpp"
#include "distortos/SignalSet.hpp"

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

bool signalsTestSelfOneSignalPending(const uint8_t signalNumber)
{
#if DISTORTOS_SIGNALS_ENABLE == 1

	SignalSet expectedSignalSet {SignalSet::empty};
	if (expectedSignalSet.add(signalNumber) != 0)
		return false;

	return ThisThread::Signals::getPendingSignalSet().getBitset() == expectedSignalSet.getBitset();

#else	// DISTORTOS_SIGNALS_ENABLE != 1

	static_cast<void>(signalNumber);	// suppress warning
	return false;

#endif	// DISTORTOS_SIGNALS_ENABLE != 1
}

}	// namespace test

}	// namespace distortos
