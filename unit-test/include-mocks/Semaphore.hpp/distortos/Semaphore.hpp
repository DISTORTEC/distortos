/**
 * \file
 * \brief Mock of Semaphore class
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_SEMAPHORE_HPP_DISTORTOS_SEMAPHORE_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_SEMAPHORE_HPP_DISTORTOS_SEMAPHORE_HPP_

#include "unit-test-common.hpp"

#include "distortos/TickClock.hpp"

namespace distortos
{

class Semaphore
{
public:

	using Value = unsigned int;

	Semaphore() = default;

	Semaphore(const Value value, const Value maxValue = std::numeric_limits<Value>::max())
	{
		REQUIRE(getProxyInstance() != nullptr);
		getProxyInstance()->construct(value, maxValue);
	}

	virtual ~Semaphore()
	{

	}

	MAKE_MOCK2(construct, void(Value, Value));
	MAKE_CONST_MOCK0(getValue, Value());
	MAKE_MOCK0(post, int());
	MAKE_MOCK0(tryWait, int());
	MAKE_MOCK1(tryWaitFor, int(TickClock::duration));
	MAKE_MOCK1(tryWaitUntil, int(TickClock::time_point));
	MAKE_MOCK0(wait, int());

	static Semaphore*& getProxyInstance()
	{
		static Semaphore* proxyInstance;
		return proxyInstance;
	}
};

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_SEMAPHORE_HPP_DISTORTOS_SEMAPHORE_HPP_
