/**
 * \file
 * \brief Definitions of fromCApi() converter functions
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FROMCAPI_HPP_
#define INCLUDE_DISTORTOS_FROMCAPI_HPP_

extern "C"
{

struct distortos_ConditionVariable;
struct distortos_Mutex;
struct distortos_Semaphore;

}	// extern "C"

namespace distortos
{

class ConditionVariable;
class Mutex;
class Semaphore;

/**
 * \brief Casts C-API distortos_ConditionVariable to distortos::ConditionVariable.
 *
 * \param [in] conditionVariable is a reference to distortos_ConditionVariable object
 *
 * \return reference to distortos::ConditionVariable object, casted from \a conditionVariable
 */

inline static distortos::ConditionVariable& fromCApi(distortos_ConditionVariable& conditionVariable)
{
	return reinterpret_cast<distortos::ConditionVariable&>(conditionVariable);
}

/**
 * \brief Casts const C-API distortos_ConditionVariable to const distortos::ConditionVariable.
 *
 * \param [in] conditionVariable is a const reference to distortos_ConditionVariable object
 *
 * \return const reference to distortos::ConditionVariable object, casted from \a conditionVariable
 */

inline static const distortos::ConditionVariable& fromCApi(const distortos_ConditionVariable& conditionVariable)
{
	return reinterpret_cast<const distortos::ConditionVariable&>(conditionVariable);
}

/**
 * \brief Casts C-API distortos_Mutex to distortos::Mutex.
 *
 * \param [in] mutex is a reference to distortos_Mutex object
 *
 * \return reference to distortos::Mutex object, casted from \a mutex
 */

inline static distortos::Mutex& fromCApi(distortos_Mutex& mutex)
{
	return reinterpret_cast<distortos::Mutex&>(mutex);
}

/**
 * \brief Casts const C-API distortos_Mutex to const distortos::Mutex.
 *
 * \param [in] mutex is a const reference to distortos_Mutex object
 *
 * \return const reference to distortos::Mutex object, casted from \a mutex
 */

inline static const distortos::Mutex& fromCApi(const distortos_Mutex& mutex)
{
	return reinterpret_cast<const distortos::Mutex&>(mutex);
}

/**
 * \brief Casts C-API distortos_Semaphore to distortos::Semaphore.
 *
 * \param [in] semaphore is a reference to distortos_Semaphore object
 *
 * \return reference to distortos::Semaphore object, casted from \a semaphore
 */

inline static distortos::Semaphore& fromCApi(distortos_Semaphore& semaphore)
{
	return reinterpret_cast<distortos::Semaphore&>(semaphore);
}

/**
 * \brief Casts const C-API distortos_Semaphore to const distortos::Semaphore.
 *
 * \param [in] semaphore is a const reference to distortos_Semaphore object
 *
 * \return const reference to distortos::Semaphore object, casted from \a semaphore
 */

inline static const distortos::Semaphore& fromCApi(const distortos_Semaphore& semaphore)
{
	return reinterpret_cast<const distortos::Semaphore&>(semaphore);
}

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FROMCAPI_HPP_
