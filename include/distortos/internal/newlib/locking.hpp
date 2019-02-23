/**
 * \file
 * \brief Header for newlib locking
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_NEWLIB_LOCKING_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_NEWLIB_LOCKING_HPP_

#include "distortos/Mutex.hpp"

#include <sys/lock.h>

#if defined(_RETARGETABLE_LOCKING)

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// newlib's lock
struct __lock : public distortos::Mutex
{
	using Mutex::Mutex;
};

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

extern __lock __lock___malloc_recursive_mutex;

}	// extern "C"

#endif	// defined(_RETARGETABLE_LOCKING)

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \return reference to main instance of Mutex used for malloc() and free() locking
 */

constexpr Mutex& getMallocMutex()
{
#if defined(_RETARGETABLE_LOCKING)

	return __lock___malloc_recursive_mutex;

#else	// !defined(_RETARGETABLE_LOCKING)

	extern Mutex mallocMutexInstance;
	return mallocMutexInstance;

#endif	// !defined(_RETARGETABLE_LOCKING)
}

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_NEWLIB_LOCKING_HPP_
