/**
 * \file
 * \brief SignalsWaitOperationsTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-21
 */

#ifndef TEST_SIGNALS_SIGNALSWAITOPERATIONSTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSWAITOPERATIONSTESTCASE_HPP_

#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests various operations related to waiting for signals.
 *
 * Tests waiting (with ThisThread::Signals::wait(), ThisThread::Signals::tryWait(), ThisThread::Signals::tryWaitFor()
 * and ThisThread::Signals::tryWaitUntil()) for signals in a thread and generation of signals from thread and from
 * interrupt context - these operations must return expected result, cause expected number of context switches and
 * finish within expected time frame.
 */

class SignalsWaitOperationsTestCase : public TestCase
{
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

#endif	// TEST_SIGNALS_SIGNALSWAITOPERATIONSTESTCASE_HPP_
