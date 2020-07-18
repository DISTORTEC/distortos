/**
 * \file
 * \brief SignalsQueuedOperationsTestCase class implementation
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "SignalsQueuedOperationsTestCase.hpp"

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_SIGNALS_ENABLE == 1

#include "signalsTestSelfOneSignalPending.hpp"

#include "distortos/DynamicThread.hpp"
#include "distortos/ThisThread-Signals.hpp"

#include <cerrno>

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

namespace distortos
{

namespace test
{

#if DISTORTOS_SIGNALS_ENABLE == 1

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {512};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Phase 1 of test case.
 *
 * The same signal number is queued multiple times for current thread (with increasing signal value) until EAGAIN error
 * code is returned, which means that all space for queuing of signals is used. The same number of calls to
 * ThisThread::Signals::tryWait() must succeed and signal value contained in received SignalInformation object must
 * match expected values.
 *
 * \return true if test succeeded, false otherwise
 */

bool phase1()
{
#if defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0

	constexpr uint8_t testSignalNumber {30};
	int testValue {};

	{
		int ret;
		while ((ret = ThisThread::Signals::queueSignal(testSignalNumber, sigval{testValue})) == 0)
		{
			++testValue;
			if (signalsTestSelfOneSignalPending(testSignalNumber) == false)
				return false;
		}

		if (ret != EAGAIN)
			return false;
	}

	const SignalSet signalSet {SignalSet::full};
	for (decltype(testValue) expectedValue {}; expectedValue < testValue; ++expectedValue)
	{
		if (signalsTestSelfOneSignalPending(testSignalNumber) == false)	// the queued signal must still be pending
			return false;

		const auto tryWaitResult = ThisThread::Signals::tryWait(signalSet);
		if (tryWaitResult.first != 0)
			return false;

		auto& signalInformation = tryWaitResult.second;
		if (signalInformation.getSignalNumber() != testSignalNumber ||
				signalInformation.getCode() != SignalInformation::Code::queued ||
				signalInformation.getValue().sival_int != expectedValue)
			return false;
	}

	// after the test no signals may be pending
	if (ThisThread::Signals::getPendingSignalSet().getBitset().none() == false)
		return false;

#endif	// defined(DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS) && DISTORTOS_MAIN_THREAD_QUEUED_SIGNALS > 0

	return true;
}

/**
 * \brief Phase 2 of test case.
 *
 * Tests error cases specific to queuing of signals:
 * - attempt to queue a signal to thread that has enabled reception of signals but disabled queuing of signals must fail
 * with ENOTSUP;
 *
 * \return true if test succeeded, false otherwise
 */

bool phase2()
{
	{
		auto testThread = makeDynamicThread({testThreadStackSize, true, 0, 0, {}}, [](){});
		if (testThread.queueSignal({}, {}) != ENOTSUP)
			return false;
	}

	return true;
}

}	// namespace

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

bool SignalsQueuedOperationsTestCase::run_() const
{
#if DISTORTOS_SIGNALS_ENABLE == 1

	for (const auto& function : {phase1, phase2})
	{
		// initially no signals may be pending
		if (ThisThread::Signals::getPendingSignalSet().getBitset().none() == false)
			return false;

		if (function() == false)
			return false;
	}

	// after the test no signals may be pending
	if (ThisThread::Signals::getPendingSignalSet().getBitset().none() == false)
		return false;

#endif	// DISTORTOS_SIGNALS_ENABLE == 1

	return true;
}

}	// namespace test

}	// namespace distortos
