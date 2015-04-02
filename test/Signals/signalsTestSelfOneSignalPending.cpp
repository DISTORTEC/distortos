/**
 * \file
 * \brief signalsTestSelfOneSignalPending() definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-02
 */

#include "signalsTestSelfOneSignalPending.hpp"

#include "distortos/ThisThread-Signals.hpp"
#include "distortos/SignalSet.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

bool signalsTestSelfOneSignalPending(const uint8_t signalNumber)
{
	SignalSet expectedSignalSet {SignalSet::empty};
	if (expectedSignalSet.add(signalNumber) != 0)
		return false;

	return ThisThread::Signals::getPendingSignalSet().getBitset() == expectedSignalSet.getBitset();
}

}	// namespace test

}	// namespace distortos
