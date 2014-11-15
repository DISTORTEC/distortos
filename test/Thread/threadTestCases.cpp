/**
 * \file
 * \brief threadTestCases object definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-15
 */

#include "threadTestCases.hpp"

#include "ThreadPriorityTestCase.hpp"
#include "ThreadFunctionTypesTestCase.hpp"
#include "ThreadSleepForTestCase.hpp"
#include "ThreadSleepUntilTestCase.hpp"
#include "ThreadRoundRobinSchedulingTestCase.hpp"
#include "ThreadPriorityChangeTestCase.hpp"

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

/// ThreadFunctionTypesTestCase instance
const ThreadFunctionTypesTestCase functionTypesTestCase;

/// ThreadSleepForTestCase instance
const ThreadSleepForTestCase sleepForTestCase;

/// ThreadSleepUntilTestCase instance
const ThreadSleepUntilTestCase sleepUntilTestCase;

/// ThreadRoundRobinSchedulingTestCase instance
const ThreadRoundRobinSchedulingTestCase roundRobinSchedulingTestCase;

/// ThreadPriorityChangeTestCase instance
const ThreadPriorityChangeTestCase priorityChangeTestCase;

/// array with references to TestCase objects related to threads
const TestCaseRange::value_type threadTestCases_[]
{
		TestCaseRange::value_type{priorityTestCase},
		TestCaseRange::value_type{functionTypesTestCase},
		TestCaseRange::value_type{sleepForTestCase},
		TestCaseRange::value_type{sleepUntilTestCase},
		TestCaseRange::value_type{roundRobinSchedulingTestCase},
		TestCaseRange::value_type{priorityChangeTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange threadTestCases {threadTestCases_};

}	// namespace test

}	// namespace distortos
