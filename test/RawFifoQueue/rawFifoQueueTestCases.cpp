/**
 * \file
 * \brief rawFifoQueueTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-09
 */

#include "rawFifoQueueTestCases.hpp"

#include "RawFifoQueuePriorityTestCase.hpp"
#include "RawFifoQueueOperationsTestCase.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// RawFifoQueuePriorityTestCase::Implementation instance
const RawFifoQueuePriorityTestCase::Implementation priorityTestCaseImplementation;

/// RawFifoQueuePriorityTestCase instance
const RawFifoQueuePriorityTestCase priorityTestCase {priorityTestCaseImplementation};

/// RawFifoQueueOperationsTestCase instance
const RawFifoQueueOperationsTestCase operationsTestCase;

/// array with references to TestCase objects related to raw FIFO queue
const TestCaseRange::value_type fifoQueueTestCases_[]
{
		TestCaseRange::value_type{priorityTestCase},
		TestCaseRange::value_type{operationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange rawFifoQueueTestCases {fifoQueueTestCases_};

}	// namespace test

}	// namespace distortos
