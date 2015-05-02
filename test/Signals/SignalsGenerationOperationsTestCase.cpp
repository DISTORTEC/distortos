/**
 * \file
 * \brief SignalsGenerationOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-02
 */

#include "SignalsGenerationOperationsTestCase.hpp"

#include "signalsTestSelfOneSignalPending.hpp"

#include "distortos/ThisThread-Signals.hpp"
#include "distortos/SignalSet.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsGenerationOperationsTestCase::run_() const
{
	if (ThisThread::Signals::setSignalMask(SignalSet{SignalSet::full}) != 0)
		return false;

	constexpr uint8_t testSignalNumber {2};

	if (ThisThread::Signals::getPendingSignalSet().getBitset().none() == false)	// initially no signals may be pending
		return false;

	for (size_t i {}; i < 100; ++i)
	{
		if (ThisThread::Signals::generateSignal(testSignalNumber) != 0)
			return false;

		if (signalsTestSelfOneSignalPending(testSignalNumber) == false)
			return false;
	}

	const SignalSet signalSet {SignalSet::full};
	const auto tryWaitResult = ThisThread::Signals::tryWait(signalSet);
	if (tryWaitResult.first != 0)
		return false;

	auto& signalInformation = tryWaitResult.second;
	if (signalInformation.getSignalNumber() != testSignalNumber ||
			signalInformation.getCode() != SignalInformation::Code::Generated)
		return false;

	// after the test no signals may be pending
	if (ThisThread::Signals::getPendingSignalSet().getBitset().none() == false)
		return false;

	return true;
}

}	// namespace test

}	// namespace distortos
