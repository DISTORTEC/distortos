/**
 * \file
 * \brief architectureTestCases object definition for ARMv6-M
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "architecture/architectureTestCases.hpp"

#include "TestCaseGroup.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup architectureTestCases {TestCaseGroup::Range{}};

}	// namespace test

}	// namespace distortos
