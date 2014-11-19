/**
 * \file
 * \brief semaphoreTestCases object definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-19
 */

#include "semaphoreTestCases.hpp"

#include "SemaphorePriorityTestCase.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// SemaphorePriorityTestCase::Implementation instance
const SemaphorePriorityTestCase::Implementation priorityTestCaseImplementation;

/// SemaphorePriorityTestCase instance
const SemaphorePriorityTestCase priorityTestCase {priorityTestCaseImplementation};

/// array with references to TestCase objects related to semaphores
const TestCaseRange::value_type semaphoreTestCases_[]
{
		TestCaseRange::value_type{priorityTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange semaphoreTestCases {semaphoreTestCases_};

}	// namespace test

}	// namespace distortos
