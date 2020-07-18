/**
 * \file
 * \brief testCases object definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "testCases.hpp"

#include "Thread/threadTestCases.hpp"
#include "SoftwareTimer/softwareTimerTestCases.hpp"
#include "Semaphore/semaphoreTestCases.hpp"
#include "Mutex/mutexTestCases.hpp"
#include "ConditionVariable/conditionVariableTestCases.hpp"
#include "Queue/queueTestCases.hpp"
#include "Signals/signalsTestCases.hpp"
#include "CallOnce/callOnceTestCases.hpp"
#include "architecture/architectureTestCases.hpp"

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

/// array with references to TestCase objects
const TestCaseGroup::Range::value_type testCases_[]
{
		TestCaseGroup::Range::value_type{threadTestCases},
		TestCaseGroup::Range::value_type{softwareTimerTestCases},
		TestCaseGroup::Range::value_type{semaphoreTestCases},
		TestCaseGroup::Range::value_type{mutexTestCases},
		TestCaseGroup::Range::value_type{conditionVariableTestCases},
		TestCaseGroup::Range::value_type{queueTestCases},
		TestCaseGroup::Range::value_type{signalsTestCases},
		TestCaseGroup::Range::value_type{callOnceTestCases},
		TestCaseGroup::Range::value_type{architectureTestCases},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup testCases {TestCaseGroup::Range{testCases_}};

}	// namespace test

}	// namespace distortos
