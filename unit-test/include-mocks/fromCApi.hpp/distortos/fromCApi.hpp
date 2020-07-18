/**
 * \file
 * \brief Mocks of fromCApi()
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_DISTORTOS_FROMCAPI_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_DISTORTOS_FROMCAPI_HPP_

#include "unit-test-common.hpp"

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_INCLUDE_CONDITIONVARIABLE
#include "distortos/C-API/ConditionVariable.h"
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_INCLUDE_CONDITIONVARIABLE

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_INCLUDE_MUTEX
#include "distortos/C-API/Mutex.h"
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_INCLUDE_MUTEX

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_INCLUDE_SEMAPHORE
#include "distortos/C-API/Semaphore.h"
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_INCLUDE_SEMAPHORE

extern "C"
{

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE
struct distortos_ConditionVariable;
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX
struct distortos_Mutex;
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE
struct distortos_Semaphore;
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE

}	// extern "C"

namespace distortos
{

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE
class ConditionVariable;
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX
class Mutex;
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE
class Semaphore;
#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE

class FromCApiMock
{
public:

	FromCApiMock()
	{
		REQUIRE(getInstanceInternal() == nullptr);
		getInstanceInternal() = this;
	}

	~FromCApiMock()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		getInstanceInternal() = {};
	}

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE

	MAKE_CONST_MOCK1(getConditionVariable, distortos::ConditionVariable&(distortos_ConditionVariable&));
	MAKE_CONST_MOCK1(getConstConditionVariable,
			const distortos::ConditionVariable&(const distortos_ConditionVariable&));

#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX

	MAKE_CONST_MOCK1(getMutex, distortos::Mutex&(distortos_Mutex&));
	MAKE_CONST_MOCK1(getConstMutex, const distortos::Mutex&(const distortos_Mutex&));

#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE

	MAKE_CONST_MOCK1(getSemaphore, distortos::Semaphore&(distortos_Semaphore&));
	MAKE_CONST_MOCK1(getConstSemaphore, const distortos::Semaphore&(const distortos_Semaphore&));

#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE

	static const FromCApiMock& getInstance()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		return *getInstanceInternal();
	}

private:

	static const FromCApiMock*& getInstanceInternal()
	{
		static const FromCApiMock* instance;
		return instance;
	}
};

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE

inline static distortos::ConditionVariable& fromCApi(distortos_ConditionVariable& conditionVariable)
{
	return FromCApiMock::getInstance().getConditionVariable(conditionVariable);
}

inline static const distortos::ConditionVariable& fromCApi(const distortos_ConditionVariable& conditionVariable)
{
	return FromCApiMock::getInstance().getConstConditionVariable(conditionVariable);
}

#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_CONDITIONVARIABLE

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX

inline static distortos::Mutex& fromCApi(distortos_Mutex& mutex)
{
	return FromCApiMock::getInstance().getMutex(mutex);
}

inline static const distortos::Mutex& fromCApi(const distortos_Mutex& mutex)
{
	return FromCApiMock::getInstance().getConstMutex(mutex);
}

#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_MUTEX

#ifdef DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE

inline static distortos::Semaphore& fromCApi(distortos_Semaphore& semaphore)
{
	return FromCApiMock::getInstance().getSemaphore(semaphore);
}

inline static const distortos::Semaphore& fromCApi(const distortos_Semaphore& semaphore)
{
	return FromCApiMock::getInstance().getConstSemaphore(semaphore);
}

#endif	// def DISTORTOS_UNIT_TEST_FROMCAPIMOCK_SEMAPHORE

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_DISTORTOS_FROMCAPI_HPP_
