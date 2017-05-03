/**
 * \file
 * \brief getMallocMutex() definition
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/memory/getMallocMutex.hpp"

#include "distortos/Mutex.hpp"

#if __GNUC_PREREQ(5, 1) != 1
// GCC 4.x doesn't fully support constexpr constructors
#error "GCC 5.1 is the minimum version supported by distortos"
#endif

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// main instance of Mutex used for malloc() and free() locking
Mutex mallocMutexInstance {Mutex::Type::recursive, Mutex::Protocol::priorityInheritance};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

Mutex& getMallocMutex()
{
	return mallocMutexInstance;
}

}	// namespace internal

}	// namespace distortos
