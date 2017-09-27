/**
 * \file
 * \brief Mocks of fromCApi()
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_DISTORTOS_FROMCAPI_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_DISTORTOS_FROMCAPI_HPP_

#include "unit-test-common.hpp"

extern "C"
{

struct distortos_Semaphore;

}	// extern "C"

namespace distortos
{

class Semaphore;

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

	MAKE_CONST_MOCK1(getSemaphore, distortos::Semaphore&(distortos_Semaphore&));
	MAKE_CONST_MOCK1(getConstSemaphore, const distortos::Semaphore&(const distortos_Semaphore&));

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

inline static distortos::Semaphore& fromCApi(distortos_Semaphore& semaphore)
{
	return FromCApiMock::getInstance().getSemaphore(semaphore);
}

inline static const distortos::Semaphore& fromCApi(const distortos_Semaphore& semaphore)
{
	return FromCApiMock::getInstance().getConstSemaphore(semaphore);
}

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_FROMCAPI_HPP_DISTORTOS_FROMCAPI_HPP_
