/**
 * \file
 * \brief threadTestCases object declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_THREAD_THREADTESTCASES_HPP_
#define TEST_THREAD_THREADTESTCASES_HPP_

namespace distortos
{

namespace test
{

class TestCaseGroup;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// group of test cases related to threads
extern const TestCaseGroup threadTestCases;

}	// namespace test

}	// namespace distortos

#endif	// TEST_THREAD_THREADTESTCASES_HPP_
