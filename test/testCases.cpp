/**
 * \file
 * \brief testCases object definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-27
 */

#include "testCases.hpp"

#include "Thread/threadTestCases.hpp"
#include "Mutex/mutexTestCases.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with references to TestCaseRange objects
const TestCaseRangeRange::value_type testCases_[]
{
		threadTestCases,
		mutexTestCases,
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const TestCaseRangeRange testCases {testCases_};

}	// namespace test

}	// namespace distortos
