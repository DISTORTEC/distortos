/**
 * \file
 * \brief Implementation of newlib locking
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/newlib/locking.hpp"

#if __GNUC_PREREQ(5, 1) != 1
// GCC 4.x doesn't fully support constexpr constructors
#error "GCC 5.1 is the minimum version supported by distortos"
#endif

#if defined(_RETARGETABLE_LOCKING)

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// lock used by arc4random()
__lock __lock___arc4random_mutex {distortos::Mutex::Protocol::priorityInheritance};

/// lock used by at_quick_exit()
__lock __lock___at_quick_exit_mutex {distortos::Mutex::Protocol::priorityInheritance};

/// recursive lock used by atexit()
__lock __lock___atexit_recursive_mutex {distortos::Mutex::Type::recursive,
		distortos::Mutex::Protocol::priorityInheritance};

/// lock used by telldir(), seekdir() and cleanupdir()
__lock __lock___dd_hash_mutex {distortos::Mutex::Protocol::priorityInheritance};

/// recursive lock used by getenv(), setenv() and unsetenv()
__lock __lock___env_recursive_mutex {distortos::Mutex::Type::recursive,
		distortos::Mutex::Protocol::priorityInheritance};

/// recursive lock used by malloc(), free(), realloc() and calloc()
__lock __lock___malloc_recursive_mutex {distortos::Mutex::Type::recursive,
		distortos::Mutex::Protocol::priorityInheritance};

/// recursive lock used by stdio functions
__lock __lock___sfp_recursive_mutex {distortos::Mutex::Type::recursive,
		distortos::Mutex::Protocol::priorityInheritance};

/// recursive lock used by stdio functions
__lock __lock___sinit_recursive_mutex {distortos::Mutex::Type::recursive,
		distortos::Mutex::Protocol::priorityInheritance};

/// lock used by time zone related functions
__lock __lock___tz_mutex {distortos::Mutex::Protocol::priorityInheritance};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Locks mutex.
 *
 * \param [in] lock is a pointer to mutex
 */

void __retarget_lock_acquire(const _LOCK_T lock)
{
	lock->lock();
}

/**
 * \brief Locks recursive mutex.
 *
 * \param [in] lock is a pointer to recursive mutex
 */

void __retarget_lock_acquire_recursive(const _LOCK_T lock)
{
	lock->lock();
}

/**
 * \brief Deletes mutex.
 *
 * \param [in] lock is a pointer to mutex
 */

void __retarget_lock_close(const _LOCK_T lock)
{
	delete lock;
}

/**
 * \brief Deletes recursive mutex.
 *
 * \param [in] lock is a pointer to recursive mutex
 */

void __retarget_lock_close_recursive(const _LOCK_T lock)
{
	delete lock;
}

/**
 * \brief Allocates and constructs new mutex.
 *
 * \param [in] lock is a pointer to pointer to mutex
 */

void __retarget_lock_init(_LOCK_T* const lock)
{
	*lock = new __lock {distortos::Mutex::Protocol::priorityInheritance};
}

/**
 * \brief Allocates and constructs new recursive mutex.
 *
 * \param [in] lock is a pointer to pointer to recursive mutex
 */

void __retarget_lock_init_recursive(_LOCK_T* const lock)
{
	*lock = new __lock {distortos::Mutex::Type::recursive, distortos::Mutex::Protocol::priorityInheritance};
}

/**
 * \brief Unlocks mutex.
 *
 * \param [in] lock is a pointer to mutex
 */

void __retarget_lock_release(const _LOCK_T lock)
{
	lock->unlock();
}

/**
 * \brief Unlocks recursive mutex.
 *
 * \param [in] lock is a pointer to recursive mutex
 */

void __retarget_lock_release_recursive(const _LOCK_T lock)
{
	lock->unlock();
}

/**
 * \brief Tries to lock mutex.
 *
 * \param [in] lock is a pointer to mutex
 *
 * \return 0 if the caller successfully locked the mutex, error code otherwise:
 * - error codes returned by distortos::Mutex::tryLock();
 */

int __retarget_lock_try_acquire(const _LOCK_T lock)
{
	return lock->tryLock();
}

/**
 * \brief Tries to lock recursive mutex.
 *
 * \param [in] lock is a pointer to recursive mutex
 *
 * \return 0 if the caller successfully locked the mutex, error code otherwise:
 * - error codes returned by distortos::Mutex::tryLock();
 */

int __retarget_lock_try_acquire_recursive(const _LOCK_T lock)
{
	return lock->tryLock();
}

}	// extern "C"

#else	// !defined(_RETARGETABLE_LOCKING)

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

#endif	// !defined(_RETARGETABLE_LOCKING)
