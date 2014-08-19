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
 * \date 2014-08-19
 */

#include "distortos/scheduler/StaticThread.hpp"
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

void* test1(const char* const volatile name)
{
	volatile auto name2 = name;
	(void)name2;

	volatile uint32_t i = 0;
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		const volatile auto nanoseconds =
				distortos::scheduler::schedulerInstance.getTimePoint().time_since_epoch().count();
		(void)nanoseconds;

		++i;
		d = distribution(generator);
		d = sin(cos(d *  M_PI / 180.0) * M_PI / 180.0);

		distortos::scheduler::schedulerInstance.sleepFor(1000);
	}

	return nullptr;
}

/**
 * \brief Test thread
 */

void* test2(const char* const volatile name)
{
	volatile auto name2 = name;
	(void)name2;

	volatile uint32_t i = 0;
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		const volatile auto nanoseconds =
				distortos::scheduler::schedulerInstance.getTimePoint().time_since_epoch().count();
		(void)nanoseconds;

		++i;
		d = distribution(generator);
		d = tan(1 / (tan(d *  M_PI / 180.0) * M_PI / 180.0));

		distortos::scheduler::schedulerInstance.sleepFor(2000);
	}

	return nullptr;
}

/**
 * \brief Test thread
 */

void* test3(const char* const volatile name)
{
	volatile auto name2 = name;
	(void)name2;

	volatile uint32_t i = 0;
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		const volatile auto nanoseconds =
				distortos::scheduler::schedulerInstance.getTimePoint().time_since_epoch().count();
		(void)nanoseconds;

		++i;
		d = distribution(generator);
		d = log10(d * 11);

		distortos::scheduler::schedulerInstance.sleepFor(3000);
	}

	return nullptr;
}

/**
 * \brief Test thread
 */

void* test4(const char* const volatile name)
{
	volatile auto name2 = name;
	(void)name2;

	volatile uint32_t i = 0;
	volatile double d;
	std::default_random_engine generator;
	std::uniform_real_distribution<> distribution {0, M_PI};

	while (1)
	{
		const volatile auto nanoseconds =
				distortos::scheduler::schedulerInstance.getTimePoint().time_since_epoch().count();
		(void)nanoseconds;

		++i;
		d = distribution(generator);
		d = log2(d * 3);

		distortos::scheduler::schedulerInstance.sleepFor(4000);
	}

	return nullptr;
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// test thread 1
auto thread1 = distortos::scheduler::makeStaticThread<2048>(1, test1, "one");

/// test thread 2
auto thread2 = distortos::scheduler::makeStaticThread<2048>(1, test2, "two");

/// test thread 3
auto thread3 = distortos::scheduler::makeStaticThread<2048>(1, test3, "three");

/// test thread 4
auto thread4 = distortos::scheduler::makeStaticThread<2048>(1, test4, "four");

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief main code block
 */

int main()
{
	thread1.start();
	thread2.start();
	thread3.start();
	thread4.start();

	distortos::scheduler::schedulerInstance.start();

	return 0;
}
