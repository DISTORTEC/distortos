/**
 * \file
 * \brief ThreadPriorityTestCase class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_THREAD_THREADPRIORITYTESTCASE_HPP_
#define TEST_THREAD_THREADPRIORITYTESTCASE_HPP_

#include "TestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests priority scheduling of threads.
 *
 * Starts 8 small threads (in various order) with varying priorities, asserting that they are executed in the right
 * order.
 */

class ThreadPriorityTestCase : public TestCaseCommon
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

#endif	// TEST_THREAD_THREADPRIORITYTESTCASE_HPP_
