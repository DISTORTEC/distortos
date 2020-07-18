/**
 * \file
 * \brief SignalCatchingOperationsTestCase class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALCATCHINGOPERATIONSTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALCATCHINGOPERATIONSTESTCASE_HPP_

#include "SignalsTestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations specific to catching signals.
 *
 * Tests behavior of ThisThread::Signals::getSignalAction(), ThisThread::Signals::getSignalMask(),
 * ThisThread::Signals::setSignalAction() and ThisThread::Signals::setSignalMask().
 */

class SignalCatchingOperationsTestCase : public SignalsTestCaseCommon
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
	 * \brief SignalCatchingOperationsTestCase's constructor
	 */

	constexpr SignalCatchingOperationsTestCase() :
			SignalsTestCaseCommon{testCasePriority_, SignalSet{SignalSet::full}, {}}
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

#endif	// TEST_SIGNALS_SIGNALCATCHINGOPERATIONSTESTCASE_HPP_
