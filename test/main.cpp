/**
 * \file
 * \brief Main code block.
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-12
 */

#include "TestCaseGroup.hpp"
#include "testCases.hpp"

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Main code block of test application
 *
 * Runs all the test cases. If any test case fails (TestCase::run() returns false) infinite loop is entered so the
 * situation can be noticed in debugging.
 *
 * \return 0 on success, doesn't return otherwise
 */

int main()
{
	if (distortos::test::testCases.run() == false)
		while (1);

	return 0;
}
