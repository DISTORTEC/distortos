/**
 * \file
 * \brief Mutex C-API test cases
 *
 * This test checks whether mutex C-API functions properly call appropriate functions from distortos::Mutex class.
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/fromCApi.hpp"
#include "distortos/Mutex.hpp"
#include "distortos/C-API/Mutex.h"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// invalid value for mutex protocol
constexpr uint8_t invalidProtocol {UINT8_MAX};

/// invalid value for mutex type
constexpr uint8_t invalidType {UINT8_MAX};

/// associates "input" value of mutex protocol with the expected mutex protocol value in the object
const std::pair<uint8_t, uint8_t> protocolAssociations0[]
{
		{distortos_Mutex_Protocol_none, distortos_Mutex_Protocol_none},
		{distortos_Mutex_Protocol_priorityInheritance, distortos_Mutex_Protocol_priorityInheritance},
		{distortos_Mutex_Protocol_priorityProtect, distortos_Mutex_Protocol_priorityProtect},
		{invalidProtocol, distortos_Mutex_Protocol_none},
};

/// associates C-API mutex protocol with appropriate distortos::Mutex::Protocol
const std::pair<uint8_t, distortos::Mutex::Protocol> protocolAssociations1[]
{
		{distortos_Mutex_Protocol_none, distortos::Mutex::Protocol::none},
		{distortos_Mutex_Protocol_priorityInheritance, distortos::Mutex::Protocol::priorityInheritance},
		{distortos_Mutex_Protocol_priorityProtect, distortos::Mutex::Protocol::priorityProtect},
};

/// associates "input" value of mutex type with the expected mutex type value in the object
const std::pair<uint8_t, uint8_t> typeAssociations0[]
{
		{distortos_Mutex_Type_normal, distortos_Mutex_Type_normal},
		{distortos_Mutex_Type_errorChecking, distortos_Mutex_Type_errorChecking},
		{distortos_Mutex_Type_recursive, distortos_Mutex_Type_recursive},
		{invalidType, distortos_Mutex_Type_normal},
};

/// associates C-API mutex type with appropriate distortos::Mutex::Type
const std::pair<uint8_t, distortos::Mutex::Type> typeAssociations1[]
{
		{distortos_Mutex_Type_normal, distortos::Mutex::Type::normal},
		{distortos_Mutex_Type_errorChecking, distortos::Mutex::Type::errorChecking},
		{distortos_Mutex_Type_recursive, distortos::Mutex::Type::recursive},
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing DISTORTOS_MUTEX_INITIALIZER()", "[initializer]")
{
	constexpr uint8_t randomValue {0xc9};

	for (auto& typeAssociation : typeAssociations0)
		for (auto& protocolAssociation : protocolAssociations0)
		{
			const distortos_Mutex mutex = DISTORTOS_MUTEX_INITIALIZER(mutex, typeAssociation.first,
					protocolAssociation.first, randomValue);
			REQUIRE(mutex.typeProtocol == (typeAssociation.second << distortos_Mutex_typeShift |
					protocolAssociation.second << distortos_Mutex_protocolShift));
			REQUIRE(mutex.priorityCeiling == randomValue);
		}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_3()", "[construct]")
{
	constexpr uint8_t randomValue {0x2a};

	for (auto& typeAssociation : typeAssociations0)
		for (auto& protocolAssociation : protocolAssociations0)
		{
			DISTORTOS_MUTEX_CONSTRUCT_3(mutex, typeAssociation.first, protocolAssociation.first,
					randomValue);
			REQUIRE(mutex.typeProtocol == (typeAssociation.second << distortos_Mutex_typeShift |
					protocolAssociation.second << distortos_Mutex_protocolShift));
			REQUIRE(mutex.priorityCeiling == randomValue);
		}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_2PC()", "[construct]")
{
	constexpr uint8_t randomValue {0xf7};

	for (auto& protocolAssociation : protocolAssociations0)
	{
		DISTORTOS_MUTEX_CONSTRUCT_2PC(mutex, protocolAssociation.first, randomValue);
		REQUIRE(mutex.typeProtocol == (distortos_Mutex_Type_normal << distortos_Mutex_typeShift |
				protocolAssociation.second << distortos_Mutex_protocolShift));
		REQUIRE(mutex.priorityCeiling == randomValue);
	}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_2TP()", "[construct]")
{
	for (auto& typeAssociation : typeAssociations0)
		for (auto& protocolAssociation : protocolAssociations0)
		{
			DISTORTOS_MUTEX_CONSTRUCT_2TP(mutex, typeAssociation.first, protocolAssociation.first);
			REQUIRE(mutex.typeProtocol == (typeAssociation.second << distortos_Mutex_typeShift |
					protocolAssociation.second << distortos_Mutex_protocolShift));
			REQUIRE(mutex.priorityCeiling == 0);
		}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_1P()", "[construct]")
{
	for (auto& protocolAssociation : protocolAssociations0)
	{
		DISTORTOS_MUTEX_CONSTRUCT_1P(mutex, protocolAssociation.first);
		REQUIRE(mutex.typeProtocol == (distortos_Mutex_Type_normal << distortos_Mutex_typeShift |
				protocolAssociation.second << distortos_Mutex_protocolShift));
		REQUIRE(mutex.priorityCeiling == 0);
	}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_1T()", "[construct]")
{
	for (auto& typeAssociation : typeAssociations0)
	{
		DISTORTOS_MUTEX_CONSTRUCT_1T(mutex, typeAssociation.first);
		REQUIRE(mutex.typeProtocol == (typeAssociation.second << distortos_Mutex_typeShift |
				distortos_Mutex_Protocol_none << distortos_Mutex_protocolShift));
		REQUIRE(mutex.priorityCeiling == 0);
	}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT()", "[construct]")
{
	DISTORTOS_MUTEX_CONSTRUCT(mutex);
	REQUIRE(mutex.typeProtocol == (distortos_Mutex_Type_normal << distortos_Mutex_typeShift |
			distortos_Mutex_Protocol_none << distortos_Mutex_protocolShift));
	REQUIRE(mutex.priorityCeiling == 0);
}

TEST_CASE("Testing distortos_Mutex_construct_3()", "[construct]")
{
	constexpr uint8_t randomValue {0x47};

	{
		distortos_Mutex mutex;
		REQUIRE(distortos_Mutex_construct_3(nullptr, distortos_Mutex_Type_normal, distortos_Mutex_Protocol_none,
				randomValue) == EINVAL);
		REQUIRE(distortos_Mutex_construct_3(&mutex, invalidType, distortos_Mutex_Protocol_none, randomValue) == EINVAL);
		REQUIRE(distortos_Mutex_construct_3(&mutex, distortos_Mutex_Type_normal, invalidProtocol,
				randomValue) == EINVAL);
	}

	for (auto& typeAssociation : typeAssociations1)
		for (auto& protocolAssociation : protocolAssociations1)
		{
			distortos::FromCApiMock fromCApiMock;
			distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
			std::aligned_storage<sizeof(distortos::Mutex), alignof(distortos::Mutex)>::type storage;

			REQUIRE_CALL(mutexMock, construct(typeAssociation.second, protocolAssociation.second, randomValue));
			REQUIRE(distortos_Mutex_construct_3(reinterpret_cast<distortos_Mutex*>(&storage), typeAssociation.first,
					protocolAssociation.first, randomValue) == 0);

			reinterpret_cast<distortos::Mutex*>(&storage)->~Mutex();
		}
}

TEST_CASE("Testing distortos_Mutex_construct_2pc()", "[construct]")
{
	constexpr uint8_t randomValue {0x69};

	{
		distortos_Mutex mutex;
		REQUIRE(distortos_Mutex_construct_2pc(nullptr, distortos_Mutex_Protocol_none, randomValue) == EINVAL);
		REQUIRE(distortos_Mutex_construct_2pc(&mutex, invalidProtocol, randomValue) == EINVAL);
	}

	for (auto& protocolAssociation : protocolAssociations1)
	{
		distortos::FromCApiMock fromCApiMock;
		distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
		std::aligned_storage<sizeof(distortos::Mutex), alignof(distortos::Mutex)>::type storage;

		REQUIRE_CALL(mutexMock, construct(distortos::Mutex::Type::normal, protocolAssociation.second, randomValue));
		REQUIRE(distortos_Mutex_construct_2pc(reinterpret_cast<distortos_Mutex*>(&storage), protocolAssociation.first,
				randomValue) == 0);

		reinterpret_cast<distortos::Mutex*>(&storage)->~Mutex();
	}
}

TEST_CASE("Testing distortos_Mutex_construct_2tp()", "[construct]")
{
	{
		distortos_Mutex mutex;
		REQUIRE(distortos_Mutex_construct_2tp(nullptr, distortos_Mutex_Type_normal,
				distortos_Mutex_Protocol_none) == EINVAL);
		REQUIRE(distortos_Mutex_construct_2tp(&mutex, invalidType, distortos_Mutex_Protocol_none) == EINVAL);
		REQUIRE(distortos_Mutex_construct_2tp(&mutex, distortos_Mutex_Type_normal, invalidProtocol) == EINVAL);
	}

	for (auto& typeAssociation : typeAssociations1)
		for (auto& protocolAssociation : protocolAssociations1)
		{
			distortos::FromCApiMock fromCApiMock;
			distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
			std::aligned_storage<sizeof(distortos::Mutex), alignof(distortos::Mutex)>::type storage;

			REQUIRE_CALL(mutexMock, construct(typeAssociation.second, protocolAssociation.second, 0));
			REQUIRE(distortos_Mutex_construct_2tp(reinterpret_cast<distortos_Mutex*>(&storage), typeAssociation.first,
					protocolAssociation.first) == 0);

			reinterpret_cast<distortos::Mutex*>(&storage)->~Mutex();
		}
}

TEST_CASE("Testing distortos_Mutex_construct_1p()", "[construct]")
{
	{
		distortos_Mutex mutex;
		REQUIRE(distortos_Mutex_construct_1p(nullptr, distortos_Mutex_Protocol_none) == EINVAL);
		REQUIRE(distortos_Mutex_construct_1p(&mutex, invalidProtocol) == EINVAL);
	}

	for (auto& protocolAssociation : protocolAssociations1)
	{
		distortos::FromCApiMock fromCApiMock;
		distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
		std::aligned_storage<sizeof(distortos::Mutex), alignof(distortos::Mutex)>::type storage;

		REQUIRE_CALL(mutexMock, construct(distortos::Mutex::Type::normal, protocolAssociation.second, 0));
		REQUIRE(distortos_Mutex_construct_1p(reinterpret_cast<distortos_Mutex*>(&storage),
				protocolAssociation.first) == 0);

		reinterpret_cast<distortos::Mutex*>(&storage)->~Mutex();
	}
}

TEST_CASE("Testing distortos_Mutex_construct_1t()", "[construct]")
{
	{
		distortos_Mutex mutex;
		REQUIRE(distortos_Mutex_construct_1t(nullptr, distortos_Mutex_Type_normal) == EINVAL);
		REQUIRE(distortos_Mutex_construct_1t(&mutex, invalidType) == EINVAL);
	}

	for (auto& typeAssociation : typeAssociations1)
	{
		distortos::FromCApiMock fromCApiMock;
		distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
		std::aligned_storage<sizeof(distortos::Mutex), alignof(distortos::Mutex)>::type storage;

		REQUIRE_CALL(mutexMock, construct(typeAssociation.second, distortos::Mutex::Protocol::none, 0));
		REQUIRE(distortos_Mutex_construct_1t(reinterpret_cast<distortos_Mutex*>(&storage), typeAssociation.first) == 0);

		reinterpret_cast<distortos::Mutex*>(&storage)->~Mutex();
	}
}

TEST_CASE("Testing distortos_Mutex_construct()", "[construct]")
{
	REQUIRE(distortos_Mutex_construct(nullptr) == EINVAL);

	distortos::FromCApiMock fromCApiMock;
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	std::aligned_storage<sizeof(distortos::Mutex), alignof(distortos::Mutex)>::type storage;

	REQUIRE_CALL(mutexMock, construct(distortos::Mutex::Type::normal, distortos::Mutex::Protocol::none, 0));
	REQUIRE(distortos_Mutex_construct(reinterpret_cast<distortos_Mutex*>(&storage)) == 0);

	reinterpret_cast<distortos::Mutex*>(&storage)->~Mutex();
}

TEST_CASE("Testing distortos_Mutex_destruct()", "[destruct]")
{
	distortos::FromCApiMock fromCApiMock;
	trompeloeil::deathwatched<distortos::Mutex> mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_Mutex mutex;

	REQUIRE(distortos_Mutex_destruct(nullptr) == EINVAL);

	{
		REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
		REQUIRE_DESTRUCTION(mutexMock);
		REQUIRE(distortos_Mutex_destruct(&mutex) == 0);
	}
}

TEST_CASE("Testing distortos_Mutex_lock()", "[lock]")
{
	distortos::FromCApiMock fromCApiMock;
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_Mutex mutex;

	REQUIRE(distortos_Mutex_lock(nullptr) == EINVAL);

	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	REQUIRE_CALL(mutexMock, lock()).RETURN(EAGAIN);
	REQUIRE(distortos_Mutex_lock(&mutex) == EAGAIN);
}

TEST_CASE("Testing distortos_Mutex_tryLock()", "[tryLock]")
{
	distortos::FromCApiMock fromCApiMock;
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_Mutex mutex;

	REQUIRE(distortos_Mutex_tryLock(nullptr) == EINVAL);

	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	REQUIRE_CALL(mutexMock, tryLock()).RETURN(EBUSY);
	REQUIRE(distortos_Mutex_tryLock(&mutex) == EBUSY);
}

TEST_CASE("Testing distortos_Mutex_tryLockFor()", "[tryLockFor]")
{
	constexpr int64_t randomDuration {0x44f439ca83c455c4};

	distortos::FromCApiMock fromCApiMock;
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_Mutex mutex;

	REQUIRE(distortos_Mutex_tryLockFor(nullptr, randomDuration) == EINVAL);

	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	const auto duration = distortos::TickClock::duration{randomDuration};
	REQUIRE_CALL(mutexMock, tryLockFor(duration)).RETURN(ETIMEDOUT);
	REQUIRE(distortos_Mutex_tryLockFor(&mutex, randomDuration) == ETIMEDOUT);
}

TEST_CASE("Testing distortos_Mutex_tryLockUntil()", "[tryLockUntil]")
{
	constexpr int64_t randomTimePoint {0x5d9094bbfca9ab1a};

	distortos::FromCApiMock fromCApiMock;
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_Mutex mutex;

	REQUIRE(distortos_Mutex_tryLockUntil(nullptr, randomTimePoint) == EINVAL);

	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	const auto timePoint = distortos::TickClock::time_point{distortos::TickClock::duration{randomTimePoint}};
	REQUIRE_CALL(mutexMock, tryLockUntil(timePoint)).RETURN(ETIMEDOUT);
	REQUIRE(distortos_Mutex_tryLockUntil(&mutex, randomTimePoint) == ETIMEDOUT);
}

TEST_CASE("Testing distortos_Mutex_unlock()", "[unlock]")
{
	distortos::FromCApiMock fromCApiMock;
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_Mutex mutex;

	REQUIRE(distortos_Mutex_unlock(nullptr) == EINVAL);

	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	REQUIRE_CALL(mutexMock, unlock()).RETURN(EPERM);
	REQUIRE(distortos_Mutex_unlock(&mutex) == EPERM);
}
