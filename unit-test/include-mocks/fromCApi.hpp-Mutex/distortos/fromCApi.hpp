/**
 * \file
 * \brief Mocks of fromCApi() for Mutex
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_MUTEX_DISTORTOS_FROMCAPI_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_MUTEX_DISTORTOS_FROMCAPI_HPP_

#include "unit-test-common.hpp"

extern "C"
{

struct distortos_Mutex;

}	// extern "C"

namespace distortos
{

class Mutex;

class FromCApiMutexMock
{
public:

	FromCApiMutexMock()
	{
		REQUIRE(getInstanceInternal() == nullptr);
		getInstanceInternal() = this;
	}

	~FromCApiMutexMock()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		getInstanceInternal() = {};
	}

	MAKE_CONST_MOCK1(getMutex, distortos::Mutex&(distortos_Mutex&));
	MAKE_CONST_MOCK1(getConstMutex, const distortos::Mutex&(const distortos_Mutex&));

	static const FromCApiMutexMock& getInstance()
	{
		REQUIRE(getInstanceInternal() != nullptr);
		return *getInstanceInternal();
	}

private:

	static const FromCApiMutexMock*& getInstanceInternal()
	{
		static const FromCApiMutexMock* instance;
		return instance;
	}
};

inline static distortos::Mutex& fromCApi(distortos_Mutex& mutex)
{
	return FromCApiMutexMock::getInstance().getMutex(mutex);
}

inline static const distortos::Mutex& fromCApi(const distortos_Mutex& mutex)
{
	return FromCApiMutexMock::getInstance().getConstMutex(mutex);
}

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_MUTEX_DISTORTOS_FROMCAPI_HPP_
