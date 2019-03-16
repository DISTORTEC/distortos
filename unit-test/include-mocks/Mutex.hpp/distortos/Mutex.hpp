/**
 * \file
 * \brief Mock of Mutex class
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_MUTEX_HPP_DISTORTOS_MUTEX_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_MUTEX_HPP_DISTORTOS_MUTEX_HPP_

#include "unit-test-common.hpp"

#include "distortos/MutexProtocol.hpp"
#include "distortos/MutexType.hpp"
#include "distortos/TickClock.hpp"

#include <climits>

namespace distortos
{

#ifdef DISTORTOS_UNIT_TEST_MUTEXMOCK_USE_WRAPPER
namespace mock
{
#endif	// def DISTORTOS_UNIT_TEST_MUTEXMOCK_USE_WRAPPER

namespace internal
{

class MutexControlBlock
{
public:

	constexpr static uint8_t typeShift {0};
	constexpr static uint8_t protocolShift {typeShift + CHAR_BIT / 2};
};

}	// namespace internal

class Mutex
{
public:

	using Protocol = MutexProtocol;
	using Type = MutexType;

	struct UnitTestTag
	{

	};

	Mutex(UnitTestTag)
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance == nullptr);
		instance = this;
	}

	explicit Mutex(const Type type = Type::normal, const Protocol protocol = Protocol::none,
			const uint8_t priorityCeiling = {})
	{
		getInstance().construct(type, protocol, priorityCeiling);
	}

	explicit Mutex(const Protocol protocol, const uint8_t priorityCeiling = {}) :
			Mutex{Type::normal, protocol, priorityCeiling}
	{

	}

	virtual ~Mutex()
	{
		auto& instance = getInstanceInternal();
		if (instance == this)
			instance = {};
	}

	MAKE_MOCK3(construct, void(Type, Protocol, uint8_t));
	MAKE_MOCK0(lock, int());
	MAKE_MOCK0(tryLock, int());
	MAKE_MOCK1(tryLockFor, int(TickClock::duration));
	MAKE_MOCK1(tryLockUntil, int(TickClock::time_point));
	MAKE_MOCK0(unlock, int());

	static Mutex& getInstance()
	{
		const auto instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		return *instance;
	}

private:

	static Mutex*& getInstanceInternal()
	{
		static Mutex* instance;
		return instance;
	}
};

#ifdef DISTORTOS_UNIT_TEST_MUTEXMOCK_USE_WRAPPER
}	// namespace mock

class Mutex
{
public:

	using Protocol = mock::Mutex::Protocol;
	using Type = mock::Mutex::Type;

	constexpr explicit Mutex(Type = Type::normal, Protocol = Protocol::none, uint8_t = {})
	{

	}

	constexpr explicit Mutex(const Protocol protocol, const uint8_t priorityCeiling = {}) :
			Mutex{Type::normal, protocol, priorityCeiling}
	{

	}

	int lock()
	{
		return mock::Mutex::getInstance().lock();
	}

	int unlock()
	{
		return mock::Mutex::getInstance().unlock();
	}
};

#endif	// def DISTORTOS_UNIT_TEST_MUTEXMOCK_USE_WRAPPER

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_MUTEX_HPP_DISTORTOS_MUTEX_HPP_
