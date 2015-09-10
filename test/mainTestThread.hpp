/**
 * \file
 * \brief mainTestThread object declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-10
 */

#ifndef TEST_MAINTESTTHREAD_HPP_
#define TEST_MAINTESTTHREAD_HPP_

#include "distortos/StaticThread.hpp"

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Main test thread function which runs all functional tests.
 */

void mainTestThreadFunction();

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for main test thread, bytes
#ifdef _REENT_SMALL
constexpr size_t mainTestThreadStackSize {10240};
#else
constexpr size_t mainTestThreadStackSize {20480};
#endif	// def _REENT_SMALL

/// type of main test thread
using MainTestThread = decltype(makeStaticThread<mainTestThreadStackSize,
#ifdef CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS
		true, CONFIG_MAIN_THREAD_QUEUED_SIGNALS, CONFIG_MAIN_THREAD_SIGNAL_ACTIONS
#endif	// def CONFIG_MAIN_THREAD_CAN_RECEIVE_SIGNALS
		>(CONFIG_MAIN_THREAD_PRIORITY, mainTestThreadFunction));

/// main test thread object
extern MainTestThread mainTestThread;

}	// namespace test

}	// namespace distortos

#endif	// TEST_MAINTESTTHREAD_HPP_
