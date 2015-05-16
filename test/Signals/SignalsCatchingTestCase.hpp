/**
 * \file
 * \brief SignalsCatchingTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
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
 * Currently only the simplest scenario is tested - current thread generating/queuing signals for itself.
 */

class SignalsCatchingTestCase : public SignalsTestCaseCommon
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX / 2};

public:

	/**
	 * \brief SignalsCatchingTestCase's constructor
	 */

	constexpr SignalsCatchingTestCase() :
			SignalsTestCaseCommon{testCasePriority_}
	{

	}

private:

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	virtual bool run_() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSCATCHINGTESTCASE_HPP_
