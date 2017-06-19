/**
 * \file
 * \brief Implementation of newlib locking
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/newlib/locking.hpp"

#include "distortos/Mutex.hpp"

#if __GNUC_PREREQ(5, 1) != 1
// GCC 4.x doesn't fully support constexpr constructors
#error "GCC 5.1 is the minimum version supported by distortos"
#endif

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// main instance of Mutex used for malloc() and free() locking
Mutex mallocMutexInstance {Mutex::Type::recursive, Mutex::Protocol::priorityInheritance};

}	// namespace internal

}	// namespace distortos

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Recursively locks Mutex used for malloc() and free() locking.
 */

void __malloc_lock()
{
	distortos::internal::getMallocMutex().lock();
}

/**
 * \brief Recursively unlocks Mutex used for malloc() and free() locking.
 */

void __malloc_unlock()
{
	distortos::internal::getMallocMutex().unlock();
}

}	// extern "C"
