/**
 * \file
 * \brief SoftwareTimerPeriodicTestCase class header
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SOFTWARETIMER_SOFTWARETIMERPERIODICTESTCASE_HPP_
#define TEST_SOFTWARETIMER_SOFTWARETIMERPERIODICTESTCASE_HPP_

#include "TestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests behavior of periodic software timers.
 *
 * Creates 8 periodic software timers and starts them with varying duration and period, asserting that they execute in
 * expected sequence.
 */

class SoftwareTimerPeriodicTestCase : public TestCaseCommon
{
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

#endif	// TEST_SOFTWARETIMER_SOFTWARETIMERPERIODICTESTCASE_HPP_
