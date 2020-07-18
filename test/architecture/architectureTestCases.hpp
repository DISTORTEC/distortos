/**
 * \file
 * \brief architectureTestCases object declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_ARCHITECTURE_ARCHITECTURETESTCASES_HPP_
#define TEST_ARCHITECTURE_ARCHITECTURETESTCASES_HPP_

namespace distortos
{

namespace test
{

class TestCaseGroup;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// group of architecture-specific test cases
extern const TestCaseGroup architectureTestCases;

}	// namespace test

}	// namespace distortos

#endif	// TEST_ARCHITECTURE_ARCHITECTURETESTCASES_HPP_
