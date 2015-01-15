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
 * \date 2015-01-15
 */

#include "messageQueueTestCases.hpp"

#include "MessageQueuePriorityTestCase.hpp"

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

/// array with references to TestCase objects related to message queue
const TestCaseRange::value_type messageQueueTestCases_[]
{
		TestCaseRange::value_type{priorityTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange messageQueueTestCases {messageQueueTestCases_};

}	// namespace test

}	// namespace distortos
