/**
 * \file
 * \brief DeferredThreadDeleter class header
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_MEMORY_DEFERREDTHREADDELETER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_MEMORY_DEFERREDTHREADDELETER_HPP_

#include "distortos/distortosConfiguration.h"

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

#include "distortos/Mutex.hpp"

namespace distortos
{

namespace internal
{

/// DeferredThreadDeleter class can be used to defer deletion of dynamic detached threads
class DeferredThreadDeleter
{
public:

	/**
	 * \brief DeferredThreadDeleter's constructor
	 */

	constexpr DeferredThreadDeleter() :
			list_{},
			mutex_{Mutex::Protocol::priorityInheritance},
			notEmpty_{}
	{

	}

	/**
	 * \brief DeferredThreadDeleter's function call operator
	 *
	 * Adds thread to internal list of threads scheduled for deferred deletion and marks the list as "not empty".
	 *
	 * \note The object must be locked (with a successful call to DeferredThreadDeleter::lock()) before this function is
	 * used!
	 *
	 * \param [in] threadControlBlock is a reference to ThreadControlBlock object associated with dynamic and detached
	 * thread that has terminated its execution
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::unlock();
	 */

	int operator()(ThreadControlBlock& threadControlBlock);

	/**
	 * \brief Locks the object, preparing it for adding thread to internal list.
	 *
	 * Locks the mutex that synchronizes access to internal list. Locking (performed in this function) and unlocking
	 * (performed at the end of function call operator) are separated, because the locking must be done while the thread
	 * is still runnable, while the transfer to internal list is performed when the thread is not in this state.
	 *
	 * \note This function must be called before function call operator is used!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::lock();
	 */

	int lock();

	/**
	 * \brief Tries to perform deferred deletion of threads.
	 *
	 * Does nothing is the list is not marked as "not empty". Otherwise this function first tries to lock following two
	 * mutexes:
	 * - mutex that protects dynamic memory allocator;
	 * - mutex that synchronizes access to list of threads scheduled for deferred deletion;
	 * If any Mutex::tryLock() call fails, this function just returns (unlocking any mutexes is necessary). Otherwise
	 * the threads are removed from the list and deleted, while the list's "not empty" marker is cleared.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::tryLock();
	 * - error codes returned by Mutex::unlock();
	 */

	int tryCleanup();

private:

	/**
	 * \brief Internals of tryCleanup().
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::tryLock();
	 * - error codes returned by Mutex::unlock();
	 */

	int tryCleanupInternal();

	/// list of threads scheduled for deferred deletion
	ThreadList::UnsortedIntrusiveList list_;

	/// mutex that synchronizes access to the \a list_
	Mutex mutex_;

	/// true if \a list_ is not empty, false otherwise
	volatile bool notEmpty_;
};

}	// namespace internal

}	// namespace distortos

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE

#endif	// INCLUDE_DISTORTOS_INTERNAL_MEMORY_DEFERREDTHREADDELETER_HPP_
