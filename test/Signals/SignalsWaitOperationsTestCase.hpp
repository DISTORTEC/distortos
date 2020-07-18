/**
 * \file
 * \brief SignalsWaitOperationsTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALSWAITOPERATIONSTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSWAITOPERATIONSTESTCASE_HPP_

#include "SignalsTestCaseCommon.hpp"

#include "abortSignalHandler.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests various operations related to waiting for signals.
 *
 * Tests waiting (with ThisThread::Signals::wait(), ThisThread::Signals::tryWait(), ThisThread::Signals::tryWaitFor()
 * and ThisThread::Signals::tryWaitUntil()) for signals in a thread and generation/queuing of signals from thread and
 * from interrupt context - these operations must return expected result, cause expected number of context switches and
 * finish within expected time frame. Behavior or ignoring signals and various error cases are also tested.
 */

class SignalsWaitOperationsTestCase : public SignalsTestCaseCommon
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX / 2};

public:

	/**
	 * \brief SignalsWaitOperationsTestCase's constructor
	 */

	constexpr SignalsWaitOperationsTestCase() :
			SignalsTestCaseCommon{testCasePriority_, SignalSet{SignalSet::full},
					{abortSignalHandler, SignalSet{SignalSet::full}}}
	{

	}

private:

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	bool run_() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSWAITOPERATIONSTESTCASE_HPP_
