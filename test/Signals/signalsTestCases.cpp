/**
 * \file
 * \brief signalsTestCases object definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "signalsTestCases.hpp"

#include "SignalsWaitTestCase.hpp"
#include "SignalsWaitOperationsTestCase.hpp"
#include "SignalsGenerationOperationsTestCase.hpp"
#include "SignalsQueuedOperationsTestCase.hpp"
#include "SignalsCatchingTestCase.hpp"
#include "SignalCatchingOperationsTestCase.hpp"
#include "SignalsInterruptionTestCase.hpp"

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

/// SignalsWaitTestCase instance
const SignalsWaitTestCase waitTestCase;

/// SignalsWaitOperationsTestCase instance
const SignalsWaitOperationsTestCase waitOperationsTestCase;

/// SignalsGenerationOperationsTestCase instance
const SignalsGenerationOperationsTestCase generationOperationsTestCase;

/// SignalsQueuedOperationsTestCase instance
const SignalsQueuedOperationsTestCase queuedOperationsTestCase;

/// SignalsCatchingTestCase instance
const SignalsCatchingTestCase catchingTestCase;

/// SignalCatchingOperationsTestCase instance
const SignalCatchingOperationsTestCase catchingOperationsTestCase;

/// SignalsInterruptionTestCase instance
const SignalsInterruptionTestCase interruptionTestCase;

/// array with references to TestCase objects related to signals
const TestCaseGroup::Range::value_type messageQueueTestCases_[]
{
		TestCaseGroup::Range::value_type{waitTestCase},
		TestCaseGroup::Range::value_type{waitOperationsTestCase},
		TestCaseGroup::Range::value_type{generationOperationsTestCase},
		TestCaseGroup::Range::value_type{queuedOperationsTestCase},
		TestCaseGroup::Range::value_type{catchingTestCase},
		TestCaseGroup::Range::value_type{catchingOperationsTestCase},
		TestCaseGroup::Range::value_type{interruptionTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup signalsTestCases {TestCaseGroup::Range{messageQueueTestCases_}};

}	// namespace test

}	// namespace distortos
