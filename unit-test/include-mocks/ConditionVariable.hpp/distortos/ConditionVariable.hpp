/**
 * \file
 * \brief Mock of ConditionVariable class
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_CONDITIONVARIABLE_HPP_DISTORTOS_CONDITIONVARIABLE_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_CONDITIONVARIABLE_HPP_DISTORTOS_CONDITIONVARIABLE_HPP_

#include "unit-test-common.hpp"

#include "distortos/Mutex.hpp"

namespace distortos
{

class ConditionVariable
{
public:

	struct UnitTestTag
	{

	};

	ConditionVariable()
	{
		REQUIRE(getProxyInstance() != nullptr);
		getProxyInstance()->construct();
	}

	ConditionVariable(UnitTestTag)
	{

	}

	virtual ~ConditionVariable() = default;

	MAKE_MOCK0(construct, void());
	MAKE_MOCK0(notifyAll, void());
	MAKE_MOCK0(notifyOne, void());
	MAKE_MOCK1(wait, int(Mutex&));
	MAKE_MOCK2(waitFor, int(Mutex&, TickClock::duration));
	MAKE_MOCK2(waitUntil, int(Mutex&, TickClock::time_point));

	static ConditionVariable*& getProxyInstance()
	{
		static ConditionVariable* proxyInstance;
		return proxyInstance;
	}
};

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_CONDITIONVARIABLE_HPP_DISTORTOS_CONDITIONVARIABLE_HPP_
