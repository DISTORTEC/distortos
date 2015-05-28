/**
 * \file
 * \brief architectureTestCases object definition for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-28
 */

#include "architecture/architectureTestCases.hpp"

#include "ARMv7-M-FpuThreadTestCase.hpp"

#include "TestCaseGroup.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// FpuThreadTestCase instance
const FpuThreadTestCase fpuThreadTestCase;

/// array with references to architecture-specific test cases
const TestCaseGroup::Range::value_type threadTestCases_[]
{
		TestCaseGroup::Range::value_type{fpuThreadTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup architectureTestCases {TestCaseGroup::Range{threadTestCases_}};

}	// namespace test

}	// namespace distortos
