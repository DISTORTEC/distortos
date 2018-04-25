/**
 * \file
 * \brief Header of C-API for distortos::Semaphore
 *
 * \author Copyright (C) 2017-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_C_API_SEMAPHORE_H_
#define INCLUDE_DISTORTOS_C_API_SEMAPHORE_H_

#include "estd/C-API/IntrusiveList.h"

#include <limits.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/**
 * \addtogroup semaphoreCApi
 * \{
 */

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of distortos::Semaphore
 *
 * Similar to POSIX semaphores - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap04.html#tag_04_16
 *
 * \sa distortos::Semaphore
 */

struct distortos_Semaphore
{
	/** ThreadControlBlock objects blocked on this semaphore */
	struct estd_IntrusiveList blockedList;

	/** internal value of the semaphore */
	unsigned int value;

	/** max value of the semaphore */
	unsigned int maxValue;
};

/*---------------------------------------------------------------------------------------------------------------------+
| global defines
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Initializer for distortos_Semaphore
 *
 * \sa distortos::Semaphore::Semaphore()
 *
 * \param [in] self is an equivalent of `this` hidden argument
 * \param [in] value is the initial value of the semaphore, if this value is greater than \a maxValue, it will be
 * truncated
 * \param [in] maxValue is the max value of the semaphore before post() returns EOVERFLOW
 */

#define DISTORTOS_SEMAPHORE_INITIALIZER(self, value, maxValue) \
		{ESTD_INTRUSIVELIST_INITIALIZER((self).blockedList), (value) < (maxValue) ? (value) : (maxValue), (maxValue)}

/**
 * \brief C-API equivalent of distortos::Semaphore's constructor
 *
 * \sa distortos::Semaphore::Semaphore()
 *
 * \param [in] name is the name of the object that will be instantiated
 * \param [in] value is the initial value of the semaphore, if this value is greater than \a maxValue, it will be
 * truncated
 * \param [in] maxValue is the max value of the semaphore before post() returns EOVERFLOW
 */

#define DISTORTOS_SEMAPHORE_CONSTRUCT_1(name, value, maxValue) \
		struct distortos_Semaphore name = DISTORTOS_SEMAPHORE_INITIALIZER(name, value, maxValue)

/**
 * \brief C-API equivalent of distortos::Semaphore's constructor, maxValue == UINT_MAX
 *
 * \sa distortos::Semaphore::Semaphore()
 *
 * \param [in] name is the name of the object that will be instantiated
 * \param [in] value is the initial value of the semaphore, if this value is greater than UINT_MAX, it will be truncated
 */

#define DISTORTOS_SEMAPHORE_CONSTRUCT(name, value)	DISTORTOS_SEMAPHORE_CONSTRUCT_1(name, value, UINT_MAX)

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief C-API equivalent of distortos::Semaphore's constructor
 *
 * Similar to sem_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html#
 *
 * \sa distortos::Semaphore::Semaphore()
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 * \param [in] value is the initial value of the semaphore, if this value is greater than \a maxValue, it will be
 * truncated
 * \param [in] maxValue is the max value of the semaphore before post() returns EOVERFLOW
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a semaphore is invalid;
 */

int distortos_Semaphore_construct_1(struct distortos_Semaphore* semaphore, unsigned int value, unsigned int maxValue);

/**
 * \brief C-API equivalent of distortos::Semaphore's constructor, maxValue == UINT_MAX
 *
 * Similar to sem_init() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html#
 *
 * \sa distortos::Semaphore::Semaphore()
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 * \param [in] value is the initial value of the semaphore, if this value is greater than UINT_MAX, it will be truncated
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a semaphore is invalid;
 */

static inline int distortos_Semaphore_construct(struct distortos_Semaphore* const semaphore, const unsigned int value)
{
	return distortos_Semaphore_construct_1(semaphore, value, UINT_MAX);
}

/**
 * \brief C-API equivalent of distortos::Semaphore's destructor
 *
 * Similar to sem_destroy() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html#
 *
 * \sa distortos::Semaphore::~Semaphore()
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a semaphore is invalid;
 */

int distortos_Semaphore_destruct(struct distortos_Semaphore* semaphore);

/**
 * \brief C-API equivalent of distortos::Semaphore::getValue()
 *
 * Similar to sem_getvalue() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_getvalue.html#
 *
 * \sa distortos::Semaphore::getValue()
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 * \param [out] value is a pointer to variable into which semaphore's value will be written
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a semaphore and/or value are invalid;
 */

int distortos_Semaphore_getValue(const struct distortos_Semaphore* semaphore, unsigned int* value);

/**
 * \brief C-API equivalent of distortos::Semaphore::post()
 *
 * Similar to sem_post() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_post.html#
 *
 * \sa distortos::Semaphore::post()
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a semaphore value is invalid;
 * - EOVERFLOW - the maximum allowable value for a semaphore would be exceeded;
 */

int distortos_Semaphore_post(struct distortos_Semaphore* semaphore);

/**
 * \brief C-API equivalent of distortos::Semaphore::tryWait()
 *
 * Similar to sem_trywait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html#
 *
 * \sa distortos::Semaphore::tryWait()
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 *
 * \return 0 on success, error code otherwise:
 * - EAGAIN - semaphore was already locked, so it cannot be immediately locked by the distortos_Semaphore_tryWait()
 * operation;
 * - EINVAL - \a semaphore value is invalid;
 */

int distortos_Semaphore_tryWait(struct distortos_Semaphore* semaphore);

/**
 * \brief C-API equivalent of distortos::Semaphore::tryWaitFor()
 *
 * Similar to sem_timedwait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_timedwait.html#
 *
 * \sa distortos::Semaphore::tryWaitFor()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 * \param [in] duration is the duration in system ticks after which the wait will be terminated without locking the
 * semaphore
 *
 * \return 0 on success, error code otherwise:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - EINVAL - \a semaphore value is invalid;
 * - ETIMEDOUT - the semaphore could not be locked before the specified timeout expired;
 */

int distortos_Semaphore_tryWaitFor(struct distortos_Semaphore* semaphore, int64_t duration);

/**
 * \brief C-API equivalent of distortos::Semaphore::tryWaitUntil()
 *
 * Similar to sem_timedwait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_timedwait.html#
 *
 * \sa distortos::Semaphore::tryWaitUntil()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 * \param [in] timePoint is the time point in system ticks at which the wait will be terminated without locking the
 * semaphore
 *
 * \return 0 on success, error code otherwise:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - EINVAL - \a semaphore value is invalid;
 * - ETIMEDOUT - the semaphore could not be locked before the specified timeout expired;
 */

int distortos_Semaphore_tryWaitUntil(struct distortos_Semaphore* semaphore, int64_t timePoint);

/**
 * \brief C-API equivalent of distortos::Semaphore::wait()
 *
 * Similar to sem_wait() - http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html#
 *
 * \sa distortos::Semaphore::wait()
 *
 * \warning This function must not be called from interrupt context!
 *
 * \param [in] semaphore is a pointer to distortos_Semaphore object
 *
 * \return 0 on success, error code otherwise:
 * - EINTR - the wait was interrupted by an unmasked, caught signal;
 * - EINVAL - \a semaphore value is invalid;
 */

int distortos_Semaphore_wait(struct distortos_Semaphore* semaphore);

/**
 * \}
 */

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* INCLUDE_DISTORTOS_C_API_SEMAPHORE_H_ */
