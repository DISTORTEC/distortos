/**
 * \file
 * \brief Mock of Semaphore class
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
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

#ifdef DISTORTOS_UNIT_TEST_SEMAPHOREMOCK_USE_WRAPPER

namespace mock
{

#endif	// def DISTORTOS_UNIT_TEST_SEMAPHOREMOCK_USE_WRAPPER

class Semaphore
{
public:

	using Value = unsigned int;

	Semaphore()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance == nullptr);
		instance = this;
	}

	explicit Semaphore(const Value value, const Value maxValue = std::numeric_limits<Value>::max())
	{
		getInstance().construct(value, maxValue);
	}

	virtual ~Semaphore()
	{
		auto& instance = getInstanceInternal();
		if (instance == this)
			instance = {};
	}

	MAKE_MOCK2(construct, void(Value, Value));
	MAKE_CONST_MOCK0(getMaxValue, Value());
	MAKE_CONST_MOCK0(getValue, Value());
	MAKE_MOCK0(post, int());
	MAKE_MOCK0(tryWait, int());
	MAKE_MOCK1(tryWaitFor, int(TickClock::duration));
	MAKE_MOCK1(tryWaitUntil, int(TickClock::time_point));
	MAKE_MOCK0(wait, int());

	static Semaphore& getInstance()
	{
		const auto instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		return *instance;
	}

private:

	static Semaphore*& getInstanceInternal()
	{
		static Semaphore* instance;
		return instance;
	}
};

#ifdef DISTORTOS_UNIT_TEST_SEMAPHOREMOCK_USE_WRAPPER

}	// namespace mock

class Semaphore
{
public:

	using Value = mock::Semaphore::Value;

	constexpr explicit Semaphore(Value, Value = std::numeric_limits<Value>::max())
	{

	}

	int post()
	{
		return mock::Semaphore::getInstance().post();
	}

	int wait()
	{
		return mock::Semaphore::getInstance().wait();
	}
};

#endif	// def DISTORTOS_UNIT_TEST_SEMAPHOREMOCK_USE_WRAPPER

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_SEMAPHORE_HPP_DISTORTOS_SEMAPHORE_HPP_
