/**
 * \file
 * \brief SignalsQueuedOperationsTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALSQUEUEDOPERATIONSTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSQUEUEDOPERATIONSTESTCASE_HPP_

#include "SignalsTestCaseCommon.hpp"

#include "abortSignalHandler.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations specific to queued signals.
 *
 * Tests queuing of signals and error cases specific to queued signals.
 */

class SignalsQueuedOperationsTestCase : public SignalsTestCaseCommon
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX / 2};

public:

	/**
	 * \brief SignalsQueuedOperationsTestCase's constructor
	 */

	constexpr SignalsQueuedOperationsTestCase() :
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

#endif	// TEST_SIGNALS_SIGNALSQUEUEDOPERATIONSTESTCASE_HPP_
