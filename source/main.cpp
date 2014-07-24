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
 * \date 2014-07-24
 */

#include "distortos/scheduler/ThreadControlBlock.hpp"
#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"
#include "distortos/architecture.hpp"

#include <cstdint>
#include <cmath>

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Test thread
 */

void * test1(void *)
{
	volatile uint32_t i = 0;
	volatile double d = 1;

	while (1)
	{
		++i;
		++d;
		d = sin(cos(d *  M_PI / 180.0) * M_PI / 180.0);
	}

	return nullptr;
}

/**
 * \brief Test thread
 */

void * test2(void *)
{
	volatile uint32_t i = 0;
	volatile double d = 1;

	while (1)
	{
		++i;
		++d;
		d = tan(1 / (tan(d *  M_PI / 180.0) * M_PI / 180.0));
	}

	return nullptr;
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// stack for thread1
uint64_t stack1[128];

/// stack for thread2
uint64_t stack2[128];

/// TCB with thread1
distortos::scheduler::ThreadControlBlock tcb1 {stack1, sizeof(stack1), test1, nullptr};

/// TCB with thread2
distortos::scheduler::ThreadControlBlock tcb2 {stack2, sizeof(stack2), test2, nullptr};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief main code block
 */

int main()
{
	distortos::scheduler::schedulerInstance.threadControlBlocks[0] = &tcb1;
	distortos::scheduler::schedulerInstance.threadControlBlocks[1] = &tcb2;
	distortos::scheduler::schedulerInstance.currentThreadControlBlock = &tcb1;

	distortos::architecture::startScheduling();

	return 0;
}
