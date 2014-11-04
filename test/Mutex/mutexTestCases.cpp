/**
 * \file
 * \brief mutexTestCases object definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-04
 */

#include "mutexTestCases.hpp"

#include "MutexPriorityTestCase.hpp"
#include "MutexOperationsTestCase.hpp"
#include "MutexErrorCheckingOperationsTestCase.hpp"
#include "MutexRecursiveOperationsTestCase.hpp"
#include "MutexPriorityProtocolTestCase.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// MutexPriorityTestCase instance
const MutexPriorityTestCase priorityTestCase;

/// MutexOperationsTestCase instance
const MutexOperationsTestCase operationsTestCase;

/// MutexErrorCheckingOperationsTestCase instance
const MutexErrorCheckingOperationsTestCase errorCheckingOperationsTestCase;

/// MutexRecursiveOperationsTestCase instance
const MutexRecursiveOperationsTestCase recursiveOperationsTestCase;

/// MutexPriorityProtocolTestCase instance
const MutexPriorityProtocolTestCase priorityProtocolTestCase;

/// array with references to TestCase objects related to mutexes
const TestCaseRange::value_type mutexTestCases_[]
{
		priorityTestCase,
		operationsTestCase,
		errorCheckingOperationsTestCase,
		recursiveOperationsTestCase,
		priorityProtocolTestCase,
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange mutexTestCases {mutexTestCases_};

}	// namespace test

}	// namespace distortos
