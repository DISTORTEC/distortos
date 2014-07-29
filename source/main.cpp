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
 * \date 2014-07-29
 */

#include "distortos/scheduler/ThreadControlBlock.hpp"
#include "distortos/scheduler/schedulerInstance.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include <random>

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
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		++i;
		d = distribution(generator);
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
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		++i;
		d = distribution(generator);
		d = tan(1 / (tan(d *  M_PI / 180.0) * M_PI / 180.0));
	}

	return nullptr;
}

/**
 * \brief Test thread
 */

void * test3(void *)
{
	volatile uint32_t i = 0;
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		++i;
		d = distribution(generator);
		d = log10(d * 11);
	}

	return nullptr;
}

/**
 * \brief Test thread
 */

void * test4(void *)
{
	volatile uint32_t i = 0;
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		++i;
		d = distribution(generator);
		d = log2(d * 3);
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

/// stack for thread3
uint64_t stack3[128];

/// stack for thread4
uint64_t stack4[128];

/// TCB with thread1
distortos::scheduler::ThreadControlBlock tcb1 {stack1, sizeof(stack1), test1, nullptr};

/// TCB with thread2
distortos::scheduler::ThreadControlBlock tcb2 {stack2, sizeof(stack2), test2, nullptr};

/// TCB with thread3
distortos::scheduler::ThreadControlBlock tcb3 {stack3, sizeof(stack3), test3, nullptr};

/// TCB with thread4
distortos::scheduler::ThreadControlBlock tcb4 {stack4, sizeof(stack4), test4, nullptr};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief main code block
 */

int main()
{
	distortos::scheduler::schedulerInstance.threadControlBlockList.emplace_back(tcb1);
	distortos::scheduler::schedulerInstance.threadControlBlockList.emplace_back(tcb2);
	distortos::scheduler::schedulerInstance.threadControlBlockList.emplace_back(tcb3);
	distortos::scheduler::schedulerInstance.threadControlBlockList.emplace_back(tcb4);

	distortos::scheduler::schedulerInstance.start();

	return 0;
}
