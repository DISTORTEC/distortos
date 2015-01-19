/**
 * \file
 * \brief rawMessageQueueTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-19
 */

#include "rawMessageQueueTestCases.hpp"

#include "RawMessageQueuePriorityTestCase.hpp"
#include "RawMessageQueueOperationsTestCase.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// RawMessageQueuePriorityTestCase::Implementation instance
const RawMessageQueuePriorityTestCase::Implementation priorityTestCaseImplementation;

/// RawMessageQueuePriorityTestCase instance
const RawMessageQueuePriorityTestCase priorityTestCase {priorityTestCaseImplementation};

/// RawMessageQueueOperationsTestCase instance
const RawMessageQueueOperationsTestCase operationsTestCase;

/// array with references to TestCase objects related to raw message queue
const TestCaseRange::value_type messageQueueTestCases_[]
{
		TestCaseRange::value_type{priorityTestCase},
		TestCaseRange::value_type{operationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange rawMessageQueueTestCases {messageQueueTestCases_};

}	// namespace test

}	// namespace distortos
