/**
 * \file
 * \brief Main code block.
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

#include "distortos/scheduler/StaticThread.hpp"
#include "distortos/scheduler/Scheduler.hpp"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {10240};

/// test thread
auto testThread =
		distortos::scheduler::makeStaticThread<testThreadStackSize>(UINT8_MAX / 2, distortos::test::testThreadFunction);

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

	distortos::scheduler::schedulerInstance.start();

	return 0;
}
