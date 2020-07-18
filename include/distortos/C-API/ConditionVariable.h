/**
 * \file
 * \brief Header of C-API for distortos::ConditionVariable
 *
 * \author Copyright (C) 2017-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_C_API_CONDITIONVARIABLE_H_
#define INCLUDE_DISTORTOS_C_API_CONDITIONVARIABLE_H_

#include "estd/C-API/IntrusiveList.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

struct distortos_Mutex;

/**
 * \addtogroup conditionVariableCApi
 * \{
 */

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of distortos::ConditionVariable
 *
 * Similar to std::condition_variable - https://en.cppreference.com/w/cpp/thread/condition_variable
 * Similar to POSIX pthread_cond_t
 *
 * \sa distortos::ConditionVariable
 */

struct distortos_ConditionVariable
{
	/** ThreadControlBlock objects blocked on this condition variable */
	struct estd_IntrusiveList blockedList;
};

/*---------------------------------------------------------------------------------------------------------------------+
| global defines
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Initializer for distortos_ConditionVariable
 *
 * \sa distortos::ConditionVariable::ConditionVariable()
 *
 * \param [in] self is an equivalent of `this` hidden argument
 */

#define DISTORTOS_CONDITIONVARIABLE_INITIALIZER(self)	{ESTD_INTRUSIVELIST_INITIALIZER((self).blockedList)}

/**
 * \brief C-API equivalent of distortos::ConditionVariable's constructor
 *
 * \sa distortos::ConditionVariable::ConditionVariable()
 *
 * \param [in] name is the name of the object that will be instantiated
 */

#define DISTORTOS_CONDITIONVARIABLE_CONSTRUCT(name) \
		struct distortos_ConditionVariable name = DISTORTOS_CONDITIONVARIABLE_INITIALIZER(name)

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of distortos::ConditionVariable's constructor
 *
 * Similar to std::condition_variable::condition_variable() -
 * https://en.cppreference.com/w/cpp/thread/condition_variable/condition_variable
 * Similar to pthread_cond_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_init.html
 *
 * \sa distortos::ConditionVariable::ConditionVariable()
 *
 * \param [in] conditionVariable is a pointer to distortos_ConditionVariable object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a conditionVariable is invalid;
 */

int distortos_ConditionVariable_construct(struct distortos_ConditionVariable* conditionVariable);

/**
 * \brief C-API equivalent of distortos::ConditionVariable's destructor
 *
 * Similar to std::condition_variable::~condition_variable() -
 * https://en.cppreference.com/w/cpp/thread/condition_variable/~condition_variable
 * Similar to pthread_cond_destroy() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_destroy.html
 *
 * \sa distortos::ConditionVariable::~ConditionVariable()
 *
 * \param [in] conditionVariable is a pointer to distortos_ConditionVariable object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a conditionVariable is invalid;
 */

int distortos_ConditionVariable_destruct(struct distortos_ConditionVariable* conditionVariable);

/**
 * \brief C-API equivalent of distortos::ConditionVariable::notifyAll()
 *
 * Similar to std::condition_variable::notify_all() -
 * https://en.cppreference.com/w/cpp/thread/condition_variable/notify_all
 * Similar to pthread_cond_broadcast() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_signal.html
 *
 * \sa distortos::ConditionVariable::notifyAll()
 *
 * \param [in] conditionVariable is a pointer to distortos_ConditionVariable object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a conditionVariable is invalid;
 */

int distortos_ConditionVariable_notifyAll(struct distortos_ConditionVariable* conditionVariable);

/**
 * \brief C-API equivalent of distortos::ConditionVariable::notifyOne()
 *
 * Similar to std::condition_variable::notify_one() -
 * https://en.cppreference.com/w/cpp/thread/condition_variable/notify_one
 * Similar to pthread_cond_signal() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_signal.html
 *
 * \sa distortos::ConditionVariable::notifyOne()
 *
 * \param [in] conditionVariable is a pointer to distortos_ConditionVariable object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a conditionVariable is invalid;
 */

int distortos_ConditionVariable_notifyOne(struct distortos_ConditionVariable* conditionVariable);

/**
 * \brief C-API equivalent of distortos::ConditionVariable::wait()
 *
 * Similar to std::condition_variable::wait() - https://en.cppreference.com/w/cpp/thread/condition_variable/wait
 * Similar to pthread_cond_wait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html
 *
 * \sa distortos::ConditionVariable::wait()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] conditionVariable is a pointer to distortos_ConditionVariable object
 * \param [in] mutex is a pointer to mutex which must be owned by calling thread
 *
 * \return 0 if the wait was completed successfully, error code otherwise:
 * - EINVAL - \a conditionVariable and/or \a mutex are invalid;
 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
 */

int distortos_ConditionVariable_wait(struct distortos_ConditionVariable* conditionVariable,
		struct distortos_Mutex* mutex);

/**
 * \brief C-API equivalent of distortos::ConditionVariable::waitFor()
 *
 * Similar to std::condition_variable::wait_for() -
 * https://en.cppreference.com/w/cpp/thread/condition_variable/wait_for
 * Similar to pthread_cond_timedwait() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
 *
 * \sa distortos::ConditionVariable::waitFor()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] conditionVariable is a pointer to distortos_ConditionVariable object
 * \param [in] mutex is a pointer to mutex which must be owned by calling thread
 * \param [in] duration is the duration after which the wait for notification will be terminated
 *
 * \return 0 if the wait was completed successfully, error code otherwise:
 * - EINVAL - \a conditionVariable and/or \a mutex are invalid;
 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
 * - ETIMEDOUT - no notification was received before the specified timeout expired;
 */

int distortos_ConditionVariable_waitFor(struct distortos_ConditionVariable* conditionVariable,
		struct distortos_Mutex* mutex, int64_t duration);

/**
 * \brief C-API equivalent of distortos::ConditionVariable::waitUntil()
 *
 * Similar to std::condition_variable::wait_until() -
 * https://en.cppreference.com/w/cpp/thread/condition_variable/wait_until
 * Similar to pthread_cond_timedwait() -
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html#
 *
 * \sa distortos::ConditionVariable::waitUntil()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] conditionVariable is a pointer to distortos_ConditionVariable object
 * \param [in] mutex is a pointer to mutex which must be owned by calling thread
 * \param [in] timePoint is the time point at which the wait for notification will be terminated
 *
 * \return 0 if the wait was completed successfully, error code otherwise:
 * - EINVAL - \a conditionVariable and/or \a mutex are invalid;
 * - EPERM - the mutex type is errorChecking or recursive, and the current thread does not own the mutex;
 * - ETIMEDOUT - no notification was received before the specified timeout expired;
 */

int distortos_ConditionVariable_waitUntil(struct distortos_ConditionVariable* conditionVariable,
		struct distortos_Mutex* mutex, int64_t timePoint);

/**
 * \}
 */

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* INCLUDE_DISTORTOS_C_API_CONDITIONVARIABLE_H_ */
