/**
 * \file
 * \brief callOnceTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "callOnceTestCases.hpp"

#include "CallOnceOperationsTestCase.hpp"
#include "CallOnceFunctionTypesTestCase.hpp"

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

/// CallOnceOperationsTestCase instance
const CallOnceOperationsTestCase operationsTestCase;

/// CallOnceFunctionTypesTestCase instance
const CallOnceFunctionTypesTestCase functionTypesTestCase;

/// array with references to TestCase objects related to callOnce()
const TestCaseGroup::Range::value_type callOnceTestCases_[]
{
		TestCaseGroup::Range::value_type{operationsTestCase},
		TestCaseGroup::Range::value_type{functionTypesTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup callOnceTestCases {TestCaseGroup::Range{callOnceTestCases_}};

}	// namespace test

}	// namespace distortos
