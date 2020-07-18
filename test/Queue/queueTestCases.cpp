/**
 * \file
 * \brief queueTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "queueTestCases.hpp"

#include "QueueOperationsTestCase.hpp"
#include "FifoQueuePriorityTestCase.hpp"
#include "MessageQueuePriorityTestCase.hpp"

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

/// QueueOperationsTestCase instance
const QueueOperationsTestCase operationsTestCase;

/// FifoQueuePriorityTestCase instance
const FifoQueuePriorityTestCase fifoQueuePriorityTestCase;

/// MessageQueuePriorityTestCase instance
const MessageQueuePriorityTestCase messageQueuePriorityTestCase;

/// array with references to TestCase objects related to queue
const TestCaseGroup::Range::value_type queueTestCases_[]
{
		TestCaseGroup::Range::value_type{operationsTestCase},
		TestCaseGroup::Range::value_type{fifoQueuePriorityTestCase},
		TestCaseGroup::Range::value_type{messageQueuePriorityTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup queueTestCases {TestCaseGroup::Range{queueTestCases_}};

}	// namespace test

}	// namespace distortos
