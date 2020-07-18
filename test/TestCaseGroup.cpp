/**
 * \file
 * \brief TestCaseGroup class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "TestCaseGroup.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

bool TestCaseGroup::run_() const
{
	for (auto& testCase : range_)
		if (testCase.get().run() == false)
			return false;

	return true;
}

}	// namespace test

}	// namespace distortos
