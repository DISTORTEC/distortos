/**
 * \file
 * \brief SignalsTestCaseCommon class implementation
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "SignalsTestCaseCommon.hpp"

#include "distortos/SignalAction.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include <tuple>

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| protected functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsTestCaseCommon::finalize() const
{
#if defined(CONFIG_MAIN_THREAD_SIGNAL_ACTIONS) && CONFIG_MAIN_THREAD_SIGNAL_ACTIONS > 0

	if (ThisThread::Signals::setSignalMask(SignalSet{SignalSet::empty}) != 0)
		return false;

	for (uint8_t signalNumber {}; signalNumber < SignalSet::Bitset{}.size(); ++signalNumber)
	{
		int ret;
		std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(signalNumber, {});
		if (ret != 0)
			return false;
	}

#endif	// defined(CONFIG_MAIN_THREAD_SIGNAL_ACTIONS) && CONFIG_MAIN_THREAD_SIGNAL_ACTIONS > 0

	return PrioritizedTestCase::finalize();
}

bool SignalsTestCaseCommon::initialize() const
{
	if (PrioritizedTestCase::initialize() == false)
		return false;

#if defined(CONFIG_MAIN_THREAD_SIGNAL_ACTIONS) && CONFIG_MAIN_THREAD_SIGNAL_ACTIONS > 0

	for (uint8_t signalNumber {}; signalNumber < SignalSet::Bitset{}.size(); ++signalNumber)
	{
		int ret;
		std::tie(ret, std::ignore) = ThisThread::Signals::setSignalAction(signalNumber, signalAction_);
		if (ret != 0)
			return false;
	}

	if (ThisThread::Signals::setSignalMask(signalMask_) != 0)
		return false;

#endif	// defined(CONFIG_MAIN_THREAD_SIGNAL_ACTIONS) && CONFIG_MAIN_THREAD_SIGNAL_ACTIONS > 0

	return true;
}

}	// namespace test

}	// namespace distortos
