/**
 * \file
 * \brief SignalsCatchingTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALSCATCHINGTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSCATCHINGTESTCASE_HPP_

#include "SignalsTestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations specific to catching of signals.
 *
 * Generates or queues various signals in different scenarios, asserting all events occur in the expected order, using
 * exact number of context switches.
 *
 * Three scenarios are tested:
 * - current thread generating/queuing signals for itself;
 * - interrupt generating/queuing signals for current thread;
 * - second test thread generating/queuing signals for main test thread;
 */

class SignalsCatchingTestCase : public SignalsTestCaseCommon
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX / 2};

public:

	/**
	 * \return priority at which this test case should be executed
	 */

	constexpr static uint8_t getTestCasePriority()
	{
		return testCasePriority_;
	}

	/**
	 * \brief SignalsCatchingTestCase's constructor
	 */

	constexpr SignalsCatchingTestCase() :
			SignalsTestCaseCommon{testCasePriority_, SignalSet{SignalSet::empty}, {}}
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

#endif	// TEST_SIGNALS_SIGNALSCATCHINGTESTCASE_HPP_
