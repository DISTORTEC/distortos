/**
 * \file
 * \brief mutexTestUnlockFromWrongThread() implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "mutexTestUnlockFromWrongThread.hpp"

#include "waitForNextTick.hpp"

#include "distortos/DynamicThread.hpp"
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
constexpr size_t testThreadStackSize {512};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

bool mutexTestUnlockFromWrongThread(Mutex& mutex)
{
	// unlock from a wrong thread - must fail with EPERM immediately
	bool sharedRet {};
	auto unlockThreadObject = makeDynamicThread({testThreadStackSize, UINT8_MAX},
			[&mutex, &sharedRet]()
			{
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
