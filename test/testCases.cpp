/**
 * \file
 * \brief testCases object definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#include "testCases.hpp"

#include "Thread/threadTestCases.hpp"
#include "SoftwareTimer/softwareTimerTestCases.hpp"
#include "Semaphore/semaphoreTestCases.hpp"
#include "Mutex/mutexTestCases.hpp"
#include "ConditionVariable/conditionVariableTestCases.hpp"
#include "FifoQueue/fifoQueueTestCases.hpp"
#include "RawFifoQueue/rawFifoQueueTestCases.hpp"
#include "MessageQueue/messageQueueTestCases.hpp"
#include "RawMessageQueue/rawMessageQueueTestCases.hpp"
#include "Signals/signalsTestCases.hpp"

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

/// array with references to TestCaseGroup objects
const TestCaseRange::value_type groupsWrapper_[]
{
		TestCaseRange::value_type{threadTestCases},
		TestCaseRange::value_type{softwareTimerTestCases},
		TestCaseRange::value_type{semaphoreTestCases},
		TestCaseRange::value_type{mutexTestCases},
		TestCaseRange::value_type{conditionVariableTestCases},
		TestCaseRange::value_type{fifoQueueTestCases},
		TestCaseRange::value_type{rawFifoQueueTestCases},
		TestCaseRange::value_type{messageQueueTestCases},
};

/// range of references to TestCaseGroup objects
const TestCaseRange groupsWrapper {groupsWrapper_};

/// array with references to TestCaseRange objects
const TestCaseRangeRange::value_type testCases_[]
{
		TestCaseRangeRange::value_type{groupsWrapper},
		TestCaseRangeRange::value_type{rawMessageQueueTestCases},
		TestCaseRangeRange::value_type{signalsTestCases},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRangeRange testCases {testCases_};

}	// namespace test

}	// namespace distortos
