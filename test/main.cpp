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
 * \date 2015-03-07
 */

#include "testThreadFunction.hpp"

#include "distortos/StaticThread.hpp"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
#ifdef _REENT_SMALL
constexpr size_t testThreadStackSize {10240};
#else
constexpr size_t testThreadStackSize {20480};
#endif	// def _REENT_SMALL

/// test thread
auto testThread = distortos::makeStaticThread<testThreadStackSize>(UINT8_MAX / 2, distortos::test::testThreadFunction);

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief main code block
 */

int main()
{
	testThread.start();
	testThread.join();

	return 0;
}
