/**
 * \file
 * \brief Header of C-API for distortos::Mutex
 *
 * \author Copyright (C) 2017-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_C_API_MUTEX_H_
#define INCLUDE_DISTORTOS_C_API_MUTEX_H_

#include "estd/C-API/IntrusiveList.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/**
 * \addtogroup mutexCApi
 * \{
 */

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of distortos::Mutex
 *
 * Similar to std::mutex - https://en.cppreference.com/w/cpp/thread/mutex
 * Similar to POSIX pthread_mutex_t -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_09 -> 2.9.3 Thread Mutexes
 *
 * \sa distortos::Mutex
 */

struct distortos_Mutex
{
	/** node for intrusive list */
	struct estd_IntrusiveListNode node;

	/** ThreadControlBlock objects blocked on mutex */
	struct estd_IntrusiveList blockedList;

	/** owner of the mutex */
	void* owner;

	/** number of recursive locks, used when mutex type is recursive */
	uint16_t recursiveLocksCount;

	/** priority ceiling of mutex, valid only when protocol_ == Protocol::priorityProtect */
	uint8_t priorityCeiling;

	/** type of mutex and its protocol */
	uint8_t typeProtocol;
};

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

enum
{
	/** no protocol, similar to PTHREAD_PRIO_NONE */
	distortos_Mutex_Protocol_none,
	/** priority inheritance protocol, similar to PTHREAD_PRIO_INHERIT */
	distortos_Mutex_Protocol_priorityInheritance,
	/** priority protection protocol (Immediate Ceiling Priority Protocol), similar to PTHREAD_PRIO_PROTECT */
	distortos_Mutex_Protocol_priorityProtect
};

enum
{
	/** normal mutex, similar to PTHREAD_MUTEX_NORMAL */
	distortos_Mutex_Type_normal,
	/** mutex with additional error checking, similar to PTHREAD_MUTEX_ERRORCHECK */
	distortos_Mutex_Type_errorChecking,
	/** recursive mutex, similar to PTHREAD_MUTEX_RECURSIVE */
	distortos_Mutex_Type_recursive
};

enum
{
	/** shift of "type" subfield, bits */
	distortos_Mutex_typeShift = 0
};

enum
{
	/** shift of "protocol" subfield, bits */
	distortos_Mutex_protocolShift = 4
};

/*---------------------------------------------------------------------------------------------------------------------+
| global defines
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Initializer for distortos_Mutex
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] self is an equivalent of `this` hidden argument
 * \param [in] type is the type of mutex, {distortos_Mutex_Type_normal, distortos_Mutex_Type_errorChecking,
 * distortos_Mutex_Type_recursive}
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when
 * protocol != distortos_Mutex_Protocol_priorityProtect
 */

#define DISTORTOS_MUTEX_INITIALIZER(self, type, protocol, priorityCeiling) \
		{ESTD_INTRUSIVELISTNODE_INITIALIZER((self).node), ESTD_INTRUSIVELIST_INITIALIZER((self).blockedList), \
		NULL, 0, (priorityCeiling), \
		(uint8_t)(((type) == distortos_Mutex_Type_normal || (type) == distortos_Mutex_Type_errorChecking || \
				(type) == distortos_Mutex_Type_recursive ? \
				(uint8_t)(type) : (uint8_t)distortos_Mutex_Type_normal) << distortos_Mutex_typeShift | \
		((protocol) == distortos_Mutex_Protocol_none || (protocol) == distortos_Mutex_Protocol_priorityInheritance || \
				(protocol) == distortos_Mutex_Protocol_priorityProtect ? \
				(uint8_t)(protocol) : (uint8_t)distortos_Mutex_Protocol_none) << distortos_Mutex_protocolShift)}

/**
 * \brief C-API equivalent of distortos::Mutex's constructor
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] name is the name of the object that will be instantiated
 * \param [in] type is the type of mutex, {distortos_Mutex_Type_normal, distortos_Mutex_Type_errorChecking,
 * distortos_Mutex_Type_recursive}
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when
 * protocol != distortos_Mutex_Protocol_priorityProtect
 */

#define DISTORTOS_MUTEX_CONSTRUCT_3(name, type, protocol, priorityCeiling) \
		struct distortos_Mutex name = DISTORTOS_MUTEX_INITIALIZER(name, type, protocol, priorityCeiling)

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, type = distortos_Mutex_Type_normal
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] name is the name of the object that will be instantiated
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when
 * protocol != distortos_Mutex_Protocol_priorityProtect
 */

#define DISTORTOS_MUTEX_CONSTRUCT_2PC(name, protocol, priorityCeiling) \
		DISTORTOS_MUTEX_CONSTRUCT_3(name, distortos_Mutex_Type_normal, protocol, priorityCeiling)

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, priorityCeiling = 0
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] name is the name of the object that will be instantiated
 * \param [in] type is the type of mutex, {distortos_Mutex_Type_normal, distortos_Mutex_Type_errorChecking,
 * distortos_Mutex_Type_recursive}
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 */

#define DISTORTOS_MUTEX_CONSTRUCT_2TP(name, type, protocol) \
		DISTORTOS_MUTEX_CONSTRUCT_3(name, type, protocol, 0)

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, type = distortos_Mutex_Type_normal, priorityCeiling = 0
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] name is the name of the object that will be instantiated
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 */

#define DISTORTOS_MUTEX_CONSTRUCT_1P(name, protocol) \
		DISTORTOS_MUTEX_CONSTRUCT_3(name, distortos_Mutex_Type_normal, protocol, 0)

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, protocol = distortos_Mutex_Protocol_none,
 * priorityCeiling = 0
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] name is the name of the object that will be instantiated
 * \param [in] type is the type of mutex, {distortos_Mutex_Type_normal, distortos_Mutex_Type_errorChecking,
 * distortos_Mutex_Type_recursive}
 */

#define DISTORTOS_MUTEX_CONSTRUCT_1T(name, type) \
		DISTORTOS_MUTEX_CONSTRUCT_3(name, type, distortos_Mutex_Protocol_none, 0)

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, type = distortos_Mutex_Type_normal,
 * protocol = distortos_Mutex_Protocol_none, priorityCeiling = 0
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] name is the name of the object that will be instantiated
 */

#define DISTORTOS_MUTEX_CONSTRUCT(name) \
		DISTORTOS_MUTEX_CONSTRUCT_3(name, distortos_Mutex_Type_normal, distortos_Mutex_Protocol_none, 0)

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of distortos::Mutex's constructor
 *
 * Similar to std::mutex::mutex() - https://en.cppreference.com/w/cpp/thread/mutex/mutex
 * Similar to pthread_mutex_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 * \param [in] type is the type of mutex, {distortos_Mutex_Type_normal, distortos_Mutex_Type_errorChecking,
 * distortos_Mutex_Type_recursive}
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when
 * protocol != distortos_Mutex_Protocol_priorityProtect
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a mutex and/or \a type and/or \a protocol are invalid;
 */

int distortos_Mutex_construct_3(struct distortos_Mutex* mutex, uint8_t type, uint8_t protocol, uint8_t priorityCeiling);

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, type = distortos_Mutex_Type_normal
 *
 * Similar to std::mutex::mutex() - https://en.cppreference.com/w/cpp/thread/mutex/mutex
 * Similar to pthread_mutex_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 * \param [in] priorityCeiling is the priority ceiling of mutex, ignored when
 * protocol != distortos_Mutex_Protocol_priorityProtect
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a mutex and/or \a protocol are invalid;
 */

static inline int distortos_Mutex_construct_2pc(struct distortos_Mutex* const mutex, const uint8_t protocol,
		const uint8_t priorityCeiling)
{
	return distortos_Mutex_construct_3(mutex, distortos_Mutex_Type_normal, protocol, priorityCeiling);
}

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, priorityCeiling = 0
 *
 * Similar to std::mutex::mutex() - https://en.cppreference.com/w/cpp/thread/mutex/mutex
 * Similar to pthread_mutex_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 * \param [in] type is the type of mutex, {distortos_Mutex_Type_normal, distortos_Mutex_Type_errorChecking,
 * distortos_Mutex_Type_recursive}
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a mutex and/or \a type and/or \a protocol are invalid;
 */

static inline int distortos_Mutex_construct_2tp(struct distortos_Mutex* const mutex, const uint8_t type,
		const uint8_t protocol)
{
	return distortos_Mutex_construct_3(mutex, type, protocol, 0);
}

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, type = distortos_Mutex_Type_normal, priorityCeiling = 0
 *
 * Similar to std::mutex::mutex() - https://en.cppreference.com/w/cpp/thread/mutex/mutex
 * Similar to pthread_mutex_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 * \param [in] protocol is the mutex protocol, {distortos_Mutex_Protocol_none,
 * distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityProtect}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a mutex and/or \a protocol are invalid;
 */

static inline int distortos_Mutex_construct_1p(struct distortos_Mutex* const mutex, const uint8_t protocol)
{
	return distortos_Mutex_construct_3(mutex, distortos_Mutex_Type_normal, protocol, 0);
}

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, protocol = distortos_Mutex_Protocol_none,
 * priorityCeiling = 0
 *
 * Similar to std::mutex::mutex() - https://en.cppreference.com/w/cpp/thread/mutex/mutex
 * Similar to pthread_mutex_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 * \param [in] type is the type of mutex, {distortos_Mutex_Type_normal, distortos_Mutex_Type_errorChecking,
 * distortos_Mutex_Type_recursive}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a mutex and/or \a type are invalid;
 */

static inline int distortos_Mutex_construct_1t(struct distortos_Mutex* const mutex, const uint8_t type)
{
	return distortos_Mutex_construct_3(mutex, type, distortos_Mutex_Protocol_none, 0);
}

/**
 * \brief C-API equivalent of distortos::Mutex's constructor, type = distortos_Mutex_Type_normal,
 * protocol = distortos_Mutex_Protocol_none, priorityCeiling = 0
 *
 * Similar to std::mutex::mutex() - https://en.cppreference.com/w/cpp/thread/mutex/mutex
 * Similar to pthread_mutex_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
 *
 * \sa distortos::Mutex::Mutex()
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a mutex is invalid;
 */

static inline int distortos_Mutex_construct(struct distortos_Mutex* const mutex)
{
	return distortos_Mutex_construct_3(mutex, distortos_Mutex_Type_normal, distortos_Mutex_Protocol_none, 0);
}

/**
 * \brief C-API equivalent of distortos::Mutex's destructor
 *
 * Similar to std::mutex::~mutex() - https://en.cppreference.com/w/cpp/thread/mutex/~mutex
 * Similar to pthread_mutex_destroy() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_destroy.html
 *
 * \sa distortos::Mutex::~Mutex()
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a mutex is invalid;
 */

int distortos_Mutex_destruct(struct distortos_Mutex* mutex);

/**
 * \brief C-API equivalent of distortos::Mutex::lock()
 *
 * Similar to std::mutex::lock() - https://en.cppreference.com/w/cpp/thread/mutex/lock
 * Similar to pthread_mutex_lock() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html#
 *
 * \sa distortos::Mutex::lock()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 *
 * \return 0 on success, error code otherwise:
 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
 * exceeded;
 * - EDEADLK - the mutex type is errorChecking and the current thread already owns the mutex;
 * - EINVAL - the mutex was created with the protocol attribute having the value priorityProtect and the calling
 * thread's priority is higher than the mutex's current priority ceiling;
 * - EINVAL - \a mutex value is invalid;
 */

int distortos_Mutex_lock(struct distortos_Mutex* mutex);

/**
 * \brief C-API equivalent of distortos::Mutex::tryLock()
 *
 * Similar to std::mutex::try_lock() - https://en.cppreference.com/w/cpp/thread/mutex/try_lock
 * Similar to pthread_mutex_trylock() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html#
 *
 * \sa distortos::Mutex::tryLock()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 *
 * \return 0 if the caller successfully locked the mutex, error code otherwise:
 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
 * exceeded;
 * - EBUSY - the mutex could not be acquired because it was already locked;
 * - EINVAL - the mutex was created with the protocol attribute having the value priorityProtect and the calling
 * thread's priority is higher than the mutex's current priority ceiling;
 * - EINVAL - \a mutex value is invalid;
 */

int distortos_Mutex_tryLock(struct distortos_Mutex* mutex);

/**
 * \brief C-API equivalent of distortos::Mutex::tryLockFor()
 *
 * Similar to std::timed_mutex::try_lock_for() - https://en.cppreference.com/w/cpp/thread/timed_mutex/try_lock_for
 * Similar to pthread_mutex_timedlock() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_timedlock.html#
 *
 * \sa distortos::Mutex::tryLockFor()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 * \param [in] duration is the duration in system ticks after which the wait will be terminated without locking the
 * mutex
 *
 * \return 0 if the caller successfully locked the mutex, error code otherwise:
 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
 * exceeded;
 * - EDEADLK - the mutex type is errorChecking and the current thread already owns the mutex;
 * - EINVAL - the mutex was created with the protocol attribute having the value priorityProtect and the calling
 * thread's priority is higher than the mutex's current priority ceiling;
 * - EINVAL - \a mutex value is invalid;
 * - ETIMEDOUT - the mutex could not be locked before the specified timeout expired;
 */

int distortos_Mutex_tryLockFor(struct distortos_Mutex* mutex, int64_t duration);

/**
 * \brief C-API equivalent of distortos::Mutex::tryLockUntil()
 *
 * Similar to std::timed_mutex::try_lock_until() -
 * https://en.cppreference.com/w/cpp/thread/timed_mutex/try_lock_until
 * Similar to pthread_mutex_timedlock() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_timedlock.html#
 *
 * \sa distortos::Mutex::tryLockUntil()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 * \param [in] timePoint is the time point in system ticks at which the wait will be terminated without locking the
 * mutex
 *
 * \return 0 if the caller successfully locked the mutex, error code otherwise:
 * - EAGAIN - the mutex could not be acquired because the maximum number of recursive locks for mutex has been
 * exceeded;
 * - EDEADLK - the mutex type is errorChecking and the current thread already owns the mutex;
 * - EINVAL - the mutex was created with the protocol attribute having the value priorityProtect and the calling
 * thread's priority is higher than the mutex's current priority ceiling;
 * - EINVAL - \a mutex value is invalid;
 * - ETIMEDOUT - the mutex could not be locked before the specified timeout expired;
 */

int distortos_Mutex_tryLockUntil(struct distortos_Mutex* mutex, int64_t timePoint);

/**
 * \brief C-API equivalent of distortos::Mutex::unlock()
 *
 * Similar to std::mutex::unlock() - https://en.cppreference.com/w/cpp/thread/mutex/unlock
 * Similar to pthread_mutex_unlock() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html#
 *
 * \sa distortos::Mutex::unlock()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] mutex is a pointer to distortos_Mutex object
 *
 * \return 0 if the caller successfully unlocked the mutex, error code otherwise:
 * - EINVAL - \a mutex value is invalid;
 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
 */

int distortos_Mutex_unlock(struct distortos_Mutex* mutex);

/**
 * \}
 */

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* INCLUDE_DISTORTOS_C_API_MUTEX_H_ */
