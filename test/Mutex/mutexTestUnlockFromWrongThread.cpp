/**
 * \file
 * \brief mutexTestUnlockFromWrongThread() implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-25
 */

#include "mutexTestUnlockFromWrongThread.hpp"

#include "waitForNextTick.hpp"

#include "distortos/scheduler/StaticThread.hpp"
#include "distortos/Mutex.hpp"

#include <cerrno>

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local constants
+---------------------------------------------------------------------------------------------------------------------*/

/// size of stack for test thread, bytes
constexpr size_t testThreadStackSize {256};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

bool mutexTestUnlockFromWrongThread(distortos::scheduler::Mutex& mutex)
{
	using distortos::scheduler::TickClock;
	using distortos::scheduler::makeStaticThread;

	// unlock from a wrong thread - must fail with EPERM immediately
	bool sharedRet {};
	auto unlockThreadObject = makeStaticThread<testThreadStackSize>(UINT8_MAX,
			[&mutex, &sharedRet]()
			{
				using distortos::scheduler::TickClock;

				const auto start = TickClock::now();
				const auto ret = mutex.unlock();
				sharedRet = ret == EPERM && start == TickClock::now();
			});
	waitForNextTick();
	unlockThreadObject.start();
	unlockThreadObject.join();

	return sharedRet;
}

}	// namespace test

}	// namespace distortos
