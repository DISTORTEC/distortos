/**
 * \file
 * \brief Idle thread definition and its low-level initializer
 *
 * \author Copyright (C) 2014-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/memory/DeferredThreadDeleter.hpp"
#include "distortos/internal/memory/getDeferredThreadDeleter.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"
#include "distortos/StaticThread.hpp"

namespace distortos
{

namespace internal
{

namespace
{

void idleThreadFunction();

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// size of idle thread's stack, bytes
#ifdef DISTORTOS_THREAD_DETACH_ENABLE
constexpr size_t idleThreadStackSize {320};
#else	// !def DISTORTOS_THREAD_DETACH_ENABLE
constexpr size_t idleThreadStackSize {128};
#endif	// !def DISTORTOS_THREAD_DETACH_ENABLE

/// type of idle thread
using IdleThread = decltype(makeStaticThread<idleThreadStackSize>(0, idleThreadFunction));

/// storage for idle thread instance
std::aligned_storage<sizeof(IdleThread), alignof(IdleThread)>::type idleThreadStorage;

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Idle thread's function
 */

void idleThreadFunction()
{
	volatile uint64_t i {};

	while (1)
	{
		++i;

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

		getDeferredThreadDeleter().tryCleanup();	/// \todo error handling?

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE
	}
}

/**
 * \brief Low-level initializer of idle thread
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void idleThreadLowLevelInitializer()
{
	auto& idleThread = *new (&idleThreadStorage) IdleThread {0, idleThreadFunction};
	idleThread.start();
}

BIND_LOW_LEVEL_INITIALIZER(20, idleThreadLowLevelInitializer);

}	// namespace

}	// namespace internal

}	// namespace distortos
