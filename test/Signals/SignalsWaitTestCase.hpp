/**
 * \file
 * \brief SignalsWaitTestCase class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALSWAITTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSWAITTESTCASE_HPP_

#include "SignalsTestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests waiting for signals
 *
 * Starts 8 small threads which wait for any signals, asserting that they receive the same signals that were generated
 * or queued (ordered numerically), start and finish in the expected order, using exact number of context switches.
 */

class SignalsWaitTestCase : public SignalsTestCaseCommon
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX / 2};

public:

	/**
	 * \brief SignalsWaitTestCase's constructor
	 */

	constexpr SignalsWaitTestCase() :
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

#endif	// TEST_SIGNALS_SIGNALSWAITTESTCASE_HPP_
