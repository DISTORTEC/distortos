/**
 * \file
 * \brief architectureTestCases object definition for ARMv7-M and ARMv8-M
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "architecture/architectureTestCases.hpp"

#include "ARMv7-M-ARMv8-M-FpuThreadTestCase.hpp"
#include "ARMv7-M-ARMv8-M-FpuSignalTestCase.hpp"

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

/// FpuSignalTestCase instance
const FpuSignalTestCase fpuSignalTestCase;

/// array with references to architecture-specific test cases
const TestCaseGroup::Range::value_type threadTestCases_[]
{
		TestCaseGroup::Range::value_type{fpuThreadTestCase},
		TestCaseGroup::Range::value_type{fpuSignalTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup architectureTestCases {TestCaseGroup::Range{threadTestCases_}};

}	// namespace test

}	// namespace distortos
