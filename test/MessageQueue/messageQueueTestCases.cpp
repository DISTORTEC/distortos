/**
 * \file
 * \brief messageQueueTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "messageQueueTestCases.hpp"

#include "MessageQueuePriorityTestCase.hpp"
#include "MessageQueueOperationsTestCase.hpp"

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

/// MessageQueuePriorityTestCase::Implementation instance
const MessageQueuePriorityTestCase::Implementation priorityTestCaseImplementation;

/// MessageQueuePriorityTestCase instance
const MessageQueuePriorityTestCase priorityTestCase {priorityTestCaseImplementation};

/// MessageQueueOperationsTestCase instance
const MessageQueueOperationsTestCase operationsTestCase;

/// array with references to TestCase objects related to message queue
const TestCaseGroup::Range::value_type messageQueueTestCases_[]
{
		TestCaseGroup::Range::value_type{priorityTestCase},
		TestCaseGroup::Range::value_type{operationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup messageQueueTestCases {TestCaseGroup::Range{messageQueueTestCases_}};

}	// namespace test

}	// namespace distortos
