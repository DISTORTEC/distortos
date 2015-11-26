/**
 * \file
 * \brief Implementation of newlib's malloc() locking functions
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-26
 */

#include "distortos/internal/memory/mallocLockingInitialization.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace syscalls
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// storage for mutex used by malloc() locking functions
std::aligned_storage<sizeof(Mutex), alignof(Mutex)>::type mallocLockingMutexStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void mallocLockingInitialization()
{
	new (&mallocLockingMutexStorage) Mutex {Mutex::Type::Recursive, Mutex::Protocol::PriorityInheritance};
}

/**
 * \brief Recursively locks malloc()'s mutex.
 */

extern "C" void __malloc_lock()
{
	auto& mallocLockingMutex = *reinterpret_cast<Mutex*>(&mallocLockingMutexStorage);
	mallocLockingMutex.lock();
}

/**
 * \brief Recursively unlocks malloc()'s mutex.
 */

extern "C" void __malloc_unlock()
{
	auto& mallocLockingMutex = *reinterpret_cast<Mutex*>(&mallocLockingMutexStorage);
	mallocLockingMutex.unlock();
}

}	// namespace syscalls

}	// namespace distortos
