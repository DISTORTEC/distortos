/**
 * \file
 * \brief threadTestCases object definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "threadTestCases.hpp"

#include "ThreadPriorityTestCase.hpp"
#include "ThreadOperationsTestCase.hpp"
#include "ThreadFunctionTypesTestCase.hpp"
#include "ThreadSleepForTestCase.hpp"
#include "ThreadSleepUntilTestCase.hpp"
#include "ThreadSchedulingPolicyTestCase.hpp"
#include "ThreadPriorityChangeTestCase.hpp"

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

/// ThreadPriorityTestCase instance
const ThreadPriorityTestCase priorityTestCase;

/// SoftwareTimerOperationsTestCase instance
const ThreadOperationsTestCase operationsTestCase;

/// ThreadFunctionTypesTestCase instance
const ThreadFunctionTypesTestCase functionTypesTestCase;

/// ThreadSleepForTestCase instance
const ThreadSleepForTestCase sleepForTestCase;

/// ThreadSleepUntilTestCase instance
const ThreadSleepUntilTestCase sleepUntilTestCase;

/// ThreadSchedulingPolicyTestCase instance
const ThreadSchedulingPolicyTestCase schedulingPolicyTestCase;

/// ThreadPriorityChangeTestCase instance
const ThreadPriorityChangeTestCase priorityChangeTestCase;

/// array with references to TestCase objects related to threads
const TestCaseGroup::Range::value_type threadTestCases_[]
{
		TestCaseGroup::Range::value_type{priorityTestCase},
		TestCaseGroup::Range::value_type{operationsTestCase},
		TestCaseGroup::Range::value_type{functionTypesTestCase},
		TestCaseGroup::Range::value_type{sleepForTestCase},
		TestCaseGroup::Range::value_type{sleepUntilTestCase},
		TestCaseGroup::Range::value_type{schedulingPolicyTestCase},
		TestCaseGroup::Range::value_type{priorityChangeTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup threadTestCases {TestCaseGroup::Range{threadTestCases_}};

}	// namespace test

}	// namespace distortos
