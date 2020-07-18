/**
 * \file
 * \brief conditionVariableTestCases object definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "conditionVariableTestCases.hpp"

#include "ConditionVariablePriorityTestCase.hpp"
#include "ConditionVariableOperationsTestCase.hpp"

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

/// ConditionVariablePriorityTestCase instance
const ConditionVariablePriorityTestCase priorityTestCase;

/// ConditionVariableOperationsTestCase instance
const ConditionVariableOperationsTestCase operationsTestCase;

/// array with references to TestCase objects related to condition variables
const TestCaseGroup::Range::value_type conditionVariableTestCases_[]
{
		TestCaseGroup::Range::value_type{priorityTestCase},
		TestCaseGroup::Range::value_type{operationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup conditionVariableTestCases {TestCaseGroup::Range{conditionVariableTestCases_}};

}	// namespace test

}	// namespace distortos
