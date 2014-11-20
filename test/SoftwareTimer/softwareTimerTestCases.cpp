/**
 * \file
 * \brief softwareTimerTestCases object definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-20
 */

#include "softwareTimerTestCases.hpp"

#include "SoftwareTimerOrderingTestCase.hpp"
#include "SoftwareTimerOperationsTestCase.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// SoftwareTimerOrderingTestCase instance
const SoftwareTimerOrderingTestCase orderingTestCase;

/// SoftwareTimerOperationsTestCase instance
const SoftwareTimerOperationsTestCase operationsTestCase;

/// array with references to TestCase objects related to software timers
const TestCaseRange::value_type softwareTimerTestCases_[]
{
		TestCaseRange::value_type{orderingTestCase},
		TestCaseRange::value_type{operationsTestCase},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRange softwareTimerTestCases {softwareTimerTestCases_};

}	// namespace test

}	// namespace distortos
