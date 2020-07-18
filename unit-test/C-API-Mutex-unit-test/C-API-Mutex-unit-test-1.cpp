/**
 * \file
 * \brief Mutex C-API test cases
 *
 * This test checks whether mutex objects instantiated with C-API macros and functions are binary identical to
 * constructed distortos::Mutex objects.
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/Mutex.hpp"
#include "distortos/C-API/Mutex.h"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// invalid value for mutex protocol
constexpr uint8_t invalidProtocol {UINT8_MAX};

/// invalid value for mutex type
constexpr uint8_t invalidType {UINT8_MAX};

/// associates C-API mutex protocol with appropriate distortos::Mutex::Protocol, with invalid value
const std::pair<uint8_t, distortos::Mutex::Protocol> protocolAssociations0[]
{
		{distortos_Mutex_Protocol_none, distortos::Mutex::Protocol::none},
		{distortos_Mutex_Protocol_priorityInheritance, distortos::Mutex::Protocol::priorityInheritance},
		{distortos_Mutex_Protocol_priorityProtect, distortos::Mutex::Protocol::priorityProtect},
		{invalidProtocol, distortos::Mutex::Protocol::none},
};

/// associates C-API mutex protocol with appropriate distortos::Mutex::Protocol, without invalid value
const std::pair<uint8_t, distortos::Mutex::Protocol> protocolAssociations1[]
{
		{distortos_Mutex_Protocol_none, distortos::Mutex::Protocol::none},
		{distortos_Mutex_Protocol_priorityInheritance, distortos::Mutex::Protocol::priorityInheritance},
		{distortos_Mutex_Protocol_priorityProtect, distortos::Mutex::Protocol::priorityProtect},
};

/// associates C-API mutex type with appropriate distortos::Mutex::Type, with invalid value
const std::pair<uint8_t, distortos::Mutex::Type> typeAssociations0[]
{
		{distortos_Mutex_Type_normal, distortos::Mutex::Type::normal},
		{distortos_Mutex_Type_errorChecking, distortos::Mutex::Type::errorChecking},
		{distortos_Mutex_Type_recursive, distortos::Mutex::Type::recursive},
		{invalidType, distortos::Mutex::Type::normal},
};

/// associates C-API mutex type with appropriate distortos::Mutex::Type, without invalid value
const std::pair<uint8_t, distortos::Mutex::Type> typeAssociations1[]
{
		{distortos_Mutex_Type_normal, distortos::Mutex::Type::normal},
		{distortos_Mutex_Type_errorChecking, distortos::Mutex::Type::errorChecking},
		{distortos_Mutex_Type_recursive, distortos::Mutex::Type::recursive},
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

void testCommon(distortos_Mutex& mutex, const distortos::Mutex::Type type = distortos::Mutex::Type::normal,
		const distortos::Mutex::Protocol protocol = distortos::Mutex::Protocol::none,
		const uint8_t priorityCeiling = {})
{
	distortos_Mutex constructed;
	memcpy(&constructed, &mutex, sizeof(mutex));
	REQUIRE(distortos_Mutex_destruct(&mutex) == 0);
	struct distortos_Mutex destructed;
	memcpy(&destructed, &mutex, sizeof(mutex));

	memset(&mutex, 0, sizeof(mutex));

	const auto realMutex = new (&mutex) distortos::Mutex {type, protocol, priorityCeiling};
	REQUIRE(memcmp(&constructed, &mutex, sizeof(mutex)) == 0);
	realMutex->~Mutex();
	REQUIRE(memcmp(&destructed, &mutex, sizeof(mutex)) == 0);
}

void testCommon(distortos_Mutex& mutex, const distortos::Mutex::Protocol protocol, const uint8_t priorityCeiling = {})
{
	testCommon(mutex, distortos::Mutex::Type::normal, protocol, priorityCeiling);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing DISTORTOS_MUTEX_INITIALIZER()", "[initializer]")
{
	constexpr uint8_t randomValue {0x46};

	for (auto& typeAssociation : typeAssociations0)
		for (auto& protocolAssociation : protocolAssociations0)
		{
			distortos_Mutex mutex = DISTORTOS_MUTEX_INITIALIZER(mutex, typeAssociation.first, protocolAssociation.first,
					randomValue);
			testCommon(mutex, typeAssociation.second, protocolAssociation.second, randomValue);
		}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_3()", "[construct]")
{
	constexpr uint8_t randomValue {0x80};

	for (auto& typeAssociation : typeAssociations0)
		for (auto& protocolAssociation : protocolAssociations0)
		{
			DISTORTOS_MUTEX_CONSTRUCT_3(mutex, typeAssociation.first, protocolAssociation.first, randomValue);
			testCommon(mutex, typeAssociation.second, protocolAssociation.second, randomValue);
		}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_2PC()", "[construct]")
{
	constexpr uint8_t randomValue {0x1c};

	for (auto& protocolAssociation : protocolAssociations0)
	{
		DISTORTOS_MUTEX_CONSTRUCT_2PC(mutex, protocolAssociation.first, randomValue);
		testCommon(mutex, protocolAssociation.second, randomValue);
	}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_2TP()", "[construct]")
{
	for (auto& typeAssociation : typeAssociations0)
		for (auto& protocolAssociation : protocolAssociations0)
		{
			DISTORTOS_MUTEX_CONSTRUCT_2TP(mutex, typeAssociation.first, protocolAssociation.first);
			testCommon(mutex, typeAssociation.second, protocolAssociation.second);
		}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_1P()", "[construct]")
{
	for (auto& protocolAssociation : protocolAssociations0)
	{
		DISTORTOS_MUTEX_CONSTRUCT_1P(mutex, protocolAssociation.first);
		testCommon(mutex, protocolAssociation.second);
	}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT_1T()", "[construct]")
{
	for (auto& typeAssociation : typeAssociations0)
	{
		DISTORTOS_MUTEX_CONSTRUCT_1T(mutex, typeAssociation.first);
		testCommon(mutex, typeAssociation.second);
	}
}

TEST_CASE("Testing DISTORTOS_MUTEX_CONSTRUCT()", "[construct]")
{
	DISTORTOS_MUTEX_CONSTRUCT(mutex);
	testCommon(mutex);
}

TEST_CASE("Testing distortos_Mutex_construct_3()", "[construct]")
{
	constexpr uint8_t randomValue {0xc4};

	for (auto& typeAssociation : typeAssociations1)
		for (auto& protocolAssociation : protocolAssociations1)
		{
			distortos_Mutex mutex {};
			REQUIRE(distortos_Mutex_construct_3(&mutex, typeAssociation.first, protocolAssociation.first,
					randomValue) == 0);
			testCommon(mutex, typeAssociation.second, protocolAssociation.second, randomValue);
		}
}

TEST_CASE("Testing distortos_Mutex_construct_2pc()", "[construct]")
{
	constexpr uint8_t randomValue {0x0d};

	for (auto& protocolAssociation : protocolAssociations1)
	{
		distortos_Mutex mutex {};
		REQUIRE(distortos_Mutex_construct_2pc(&mutex, protocolAssociation.first, randomValue) == 0);
		testCommon(mutex, protocolAssociation.second, randomValue);
	}
}

TEST_CASE("Testing distortos_Mutex_construct_2tp()", "[construct]")
{
	for (auto& typeAssociation : typeAssociations1)
		for (auto& protocolAssociation : protocolAssociations1)
		{
			distortos_Mutex mutex {};
			REQUIRE(distortos_Mutex_construct_2tp(&mutex, typeAssociation.first, protocolAssociation.first) == 0);
			testCommon(mutex, typeAssociation.second, protocolAssociation.second);
		}
}

TEST_CASE("Testing distortos_Mutex_construct_1p()", "[construct]")
{
	for (auto& protocolAssociation : protocolAssociations1)
	{
		distortos_Mutex mutex {};
		REQUIRE(distortos_Mutex_construct_1p(&mutex, protocolAssociation.first) == 0);
		testCommon(mutex, protocolAssociation.second);
	}
}

TEST_CASE("Testing distortos_Mutex_construct_1t()", "[construct]")
{
	for (auto& typeAssociation : typeAssociations1)
	{
		distortos_Mutex mutex {};
		REQUIRE(distortos_Mutex_construct_1t(&mutex, typeAssociation.first) == 0);
		testCommon(mutex, typeAssociation.second);
	}
}

TEST_CASE("Testing distortos_Mutex_construct()", "[construct]")
{
	distortos_Mutex mutex {};
	REQUIRE(distortos_Mutex_construct(&mutex) == 0);
	testCommon(mutex);
}
