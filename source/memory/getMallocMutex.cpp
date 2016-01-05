/**
 * \file
 * \brief getMallocMutex() definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/memory/getMallocMutex.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// storage for main instance of Mutex used for malloc() and free() locking
std::aligned_storage<sizeof(Mutex), alignof(Mutex)>::type mallocMutexInstanceStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

Mutex& getMallocMutex()
{
	return reinterpret_cast<Mutex&>(mallocMutexInstanceStorage);
}

}	// namespace internal

}	// namespace distortos
