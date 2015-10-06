/**
 * \file
 * \brief queueTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-06
 */

#include "queueTestCases.hpp"

#include "QueueOperationsTestCase.hpp"
#include "FifoQueuePriorityTestCase.hpp"
#include "RawFifoQueuePriorityTestCase.hpp"
#include "RawFifoQueueOperationsTestCase.hpp"
#include "MessageQueuePriorityTestCase.hpp"
#include "RawMessageQueuePriorityTestCase.hpp"
#include "RawMessageQueueOperationsTestCase.hpp"

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

/// RawFifoQueuePriorityTestCase instance
const RawFifoQueuePriorityTestCase rawFifoQueuePriorityTestCase;

/// RawFifoQueueOperationsTestCase instance
const RawFifoQueueOperationsTestCase rawFifoQueueOperationsTestCase;

/// MessageQueuePriorityTestCase instance
const MessageQueuePriorityTestCase messageQueuePriorityTestCase;

/// RawMessageQueuePriorityTestCase instance
const RawMessageQueuePriorityTestCase rawMessageQueuePriorityTestCase;

/// RawMessageQueueOperationsTestCase instance
const RawMessageQueueOperationsTestCase rawMessageQueueOperationsTestCase;

/// array with references to TestCase objects related to queue
const TestCaseGroup::Range::value_type queueTestCases_[]
{
		TestCaseGroup::Range::value_type{operationsTestCase},
		TestCaseGroup::Range::value_type{fifoQueuePriorityTestCase},
		TestCaseGroup::Range::value_type{rawFifoQueuePriorityTestCase},
		TestCaseGroup::Range::value_type{rawFifoQueueOperationsTestCase},
		TestCaseGroup::Range::value_type{messageQueuePriorityTestCase},
		TestCaseGroup::Range::value_type{rawMessageQueuePriorityTestCase},
		TestCaseGroup::Range::value_type{rawMessageQueueOperationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup queueTestCases {TestCaseGroup::Range{queueTestCases_}};

}	// namespace test

}	// namespace distortos
