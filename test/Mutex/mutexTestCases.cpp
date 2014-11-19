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
 * \date 2014-11-19
 */

#include "mutexTestCases.hpp"

#include "MutexPriorityTestCase.hpp"
#include "MutexOperationsTestCase.hpp"
#include "MutexErrorCheckingOperationsTestCase.hpp"
#include "MutexRecursiveOperationsTestCase.hpp"
#include "MutexPriorityProtectOperationsTestCase.hpp"
#include "MutexPriorityInheritanceOperationsTestCase.hpp"
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

/// MutexOperationsTestCase::Implementation instance
const MutexOperationsTestCase::Implementation operationsTestCaseImplementation;

/// MutexOperationsTestCase instance
const MutexOperationsTestCase operationsTestCase {operationsTestCaseImplementation};

/// MutexErrorCheckingOperationsTestCase::Implementation instance
const MutexErrorCheckingOperationsTestCase::Implementation errorCheckingOperationsTestCaseImplementation;

/// MutexErrorCheckingOperationsTestCase instance
const MutexErrorCheckingOperationsTestCase errorCheckingOperationsTestCase
{
		errorCheckingOperationsTestCaseImplementation
};

/// MutexRecursiveOperationsTestCase::Implementation instance
const MutexRecursiveOperationsTestCase::Implementation recursiveOperationsTestCaseImplementation;

/// MutexRecursiveOperationsTestCase instance
const MutexRecursiveOperationsTestCase recursiveOperationsTestCase {recursiveOperationsTestCaseImplementation};

/// MutexPriorityProtectOperationsTestCase::Implementation instance
const MutexPriorityProtectOperationsTestCase::Implementation priorityProtectOperationsTestCaseImplementation;

/// MutexPriorityProtectOperationsTestCase instance
const MutexPriorityProtectOperationsTestCase priorityProtectOperationsTestCase
{
		priorityProtectOperationsTestCaseImplementation
};

/// MutexPriorityInheritanceOperationsTestCase::Implementation instance
const MutexPriorityInheritanceOperationsTestCase::Implementation priorityInheritanceOperationsTestCaseImplementation;

/// MutexPriorityInheritanceOperationsTestCase instance
const MutexPriorityInheritanceOperationsTestCase priorityInheritanceOperationsTestCase
{
		priorityInheritanceOperationsTestCaseImplementation
};

/// MutexPriorityProtocolTestCase instance
const MutexPriorityProtocolTestCase priorityProtocolTestCase;

/// array with references to TestCase objects related to mutexes
const TestCaseRange::value_type mutexTestCases_[]
{
		TestCaseRange::value_type{priorityTestCase},
		TestCaseRange::value_type{operationsTestCase},
		TestCaseRange::value_type{errorCheckingOperationsTestCase},
		TestCaseRange::value_type{recursiveOperationsTestCase},
		TestCaseRange::value_type{priorityProtectOperationsTestCase},
		TestCaseRange::value_type{priorityInheritanceOperationsTestCase},
		TestCaseRange::value_type{priorityProtocolTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange mutexTestCases {mutexTestCases_};

}	// namespace test

}	// namespace distortos
