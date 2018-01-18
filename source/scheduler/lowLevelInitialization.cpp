/**
 * \file
 * \brief internal::lowLevelInitialization() definition
 *
 * \author Copyright (C) 2014-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/scheduler/lowLevelInitialization.hpp"

#include "distortos/StaticThread.hpp"

#include "distortos/internal/scheduler/idleThreadFunction.hpp"

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of idle thread's stack, bytes
#ifdef CONFIG_THREAD_DETACH_ENABLE
constexpr size_t idleThreadStackSize {320};
#else	// !def CONFIG_THREAD_DETACH_ENABLE
constexpr size_t idleThreadStackSize {128};
#endif	// !def CONFIG_THREAD_DETACH_ENABLE

/// type of idle thread
using IdleThread = decltype(makeStaticThread<idleThreadStackSize>(0, idleThreadFunction));

/// storage for idle thread instance
std::aligned_storage<sizeof(IdleThread), alignof(IdleThread)>::type idleThreadStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
	auto& idleThread = *new (&idleThreadStorage) IdleThread {0, idleThreadFunction};
	idleThread.start();
}

}	// namespace internal

}	// namespace distortos
