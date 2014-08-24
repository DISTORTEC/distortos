/**
 * \file
 * \brief testThreadFunction() implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-24
 */

#include "testThreadFunction.hpp"

#include "testCases.hpp"
#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void testThreadFunction()
{
	for (const auto& list : testCases)
	{
		for (const auto& testCase : list.get())
		{
			const auto result = testCase.get().run();
			if (result == false)
				while (1);
		}
	}
}

}	// namespace test

}	// namespace distortos
