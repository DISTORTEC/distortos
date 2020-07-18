/**
 * \file
 * \brief mutexTestCases object definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "mutexTestCases.hpp"

#include "MutexPriorityTestCase.hpp"
#include "MutexOperationsTestCase.hpp"
#include "MutexErrorCheckingOperationsTestCase.hpp"
#include "MutexRecursiveOperationsTestCase.hpp"
#include "MutexPriorityProtectOperationsTestCase.hpp"
#include "MutexPriorityInheritanceOperationsTestCase.hpp"
#include "MutexPriorityProtocolTestCase.hpp"

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

/// MutexPriorityTestCase instance
const MutexPriorityTestCase priorityTestCase;

/// MutexOperationsTestCase instance
const MutexOperationsTestCase operationsTestCase;

/// MutexErrorCheckingOperationsTestCase instance
const MutexErrorCheckingOperationsTestCase errorCheckingOperationsTestCase;

/// MutexRecursiveOperationsTestCase instance
const MutexRecursiveOperationsTestCase recursiveOperationsTestCase;

/// MutexPriorityProtectOperationsTestCase instance
const MutexPriorityProtectOperationsTestCase priorityProtectOperationsTestCase;

/// MutexPriorityInheritanceOperationsTestCase instance
const MutexPriorityInheritanceOperationsTestCase priorityInheritanceOperationsTestCase;

/// MutexPriorityProtocolTestCase instance
const MutexPriorityProtocolTestCase priorityProtocolTestCase;

/// array with references to TestCase objects related to mutexes
const TestCaseGroup::Range::value_type mutexTestCases_[]
{
		TestCaseGroup::Range::value_type{priorityTestCase},
		TestCaseGroup::Range::value_type{operationsTestCase},
		TestCaseGroup::Range::value_type{errorCheckingOperationsTestCase},
		TestCaseGroup::Range::value_type{recursiveOperationsTestCase},
		TestCaseGroup::Range::value_type{priorityProtectOperationsTestCase},
		TestCaseGroup::Range::value_type{priorityInheritanceOperationsTestCase},
		TestCaseGroup::Range::value_type{priorityProtocolTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseGroup mutexTestCases {TestCaseGroup::Range{mutexTestCases_}};

}	// namespace test

}	// namespace distortos
