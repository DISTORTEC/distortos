/**
 * \file
 * \brief fifoQueueTestCases object definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "fifoQueueTestCases.hpp"

#include "FifoQueuePriorityTestCase.hpp"
#include "FifoQueueOperationsTestCase.hpp"

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

/// FifoQueuePriorityTestCase::Implementation instance
const FifoQueuePriorityTestCase::Implementation priorityTestCaseImplementation;

/// FifoQueuePriorityTestCase instance
const FifoQueuePriorityTestCase priorityTestCase {priorityTestCaseImplementation};

/// FifoQueueOperationsTestCase instance
const FifoQueueOperationsTestCase operationsTestCase;

/// array with references to TestCase objects related to FIFO queue
const TestCaseGroup::Range::value_type fifoQueueTestCases_[]
{
		TestCaseGroup::Range::value_type{priorityTestCase},
		TestCaseGroup::Range::value_type{operationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup fifoQueueTestCases {TestCaseGroup::Range{fifoQueueTestCases_}};

}	// namespace test

}	// namespace distortos
