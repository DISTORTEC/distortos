/**
 * \file
 * \brief Condition variable C-API test cases
 *
 * This test checks whether condition variable C-API functions properly call appropriate functions from
 * distortos::ConditionVariable class.
 *
 * \author Copyright (C) 2017-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/ConditionVariable.hpp"
#include "distortos/fromCApi.hpp"
#include "distortos/C-API/ConditionVariable.h"

using trompeloeil::_;

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing distortos_ConditionVariable_construct()", "[construct]")
{
	REQUIRE(distortos_ConditionVariable_construct(nullptr) == EINVAL);

	distortos::FromCApiMock fromCApiMock;
	distortos::ConditionVariable conditionVariableMock {distortos::ConditionVariable::UnitTestTag{}};
	std::aligned_storage<sizeof(distortos::ConditionVariable), alignof(distortos::ConditionVariable)>::type storage;

	distortos::ConditionVariable::getProxyInstance() = &conditionVariableMock;
	REQUIRE_CALL(conditionVariableMock, construct());
	REQUIRE(distortos_ConditionVariable_construct(reinterpret_cast<distortos_ConditionVariable*>(&storage)) == 0);
	distortos::ConditionVariable::getProxyInstance() = {};

	reinterpret_cast<distortos::ConditionVariable*>(&storage)->~ConditionVariable();
}

TEST_CASE("Testing distortos_ConditionVariable_destruct()", "[destruct]")
{
	distortos::FromCApiMock fromCApiMock;
	using Storage = std::aligned_storage<sizeof(trompeloeil::deathwatched<distortos::ConditionVariable>),
			alignof(trompeloeil::deathwatched<distortos::ConditionVariable>)>::type;
	Storage storage;
	auto& conditionVariableMock = *new (&storage) trompeloeil::deathwatched<distortos::ConditionVariable>
	{
			distortos::ConditionVariable::UnitTestTag{}
	};
	distortos_ConditionVariable conditionVariable;

	REQUIRE(distortos_ConditionVariable_destruct(nullptr) == EINVAL);

	{
		REQUIRE_CALL(fromCApiMock,
				getConditionVariable(_)).LR_WITH(&_1 == &conditionVariable).LR_RETURN(std::ref(conditionVariableMock));
		REQUIRE_DESTRUCTION(conditionVariableMock);
		REQUIRE(distortos_ConditionVariable_destruct(&conditionVariable) == 0);
	}
}

TEST_CASE("Testing distortos_ConditionVariable_notifyAll()", "[notifyAll]")
{
	distortos::FromCApiMock fromCApiMock;
	distortos::ConditionVariable conditionVariableMock {distortos::ConditionVariable::UnitTestTag{}};
	distortos_ConditionVariable conditionVariable;

	REQUIRE(distortos_ConditionVariable_notifyAll(nullptr) == EINVAL);

	REQUIRE_CALL(fromCApiMock,
			getConditionVariable(_)).LR_WITH(&_1 == &conditionVariable).LR_RETURN(std::ref(conditionVariableMock));
	REQUIRE_CALL(conditionVariableMock, notifyAll());
	REQUIRE(distortos_ConditionVariable_notifyAll(&conditionVariable) == 0);
}

TEST_CASE("Testing distortos_ConditionVariable_notifyOne()", "[notifyOne]")
{
	distortos::FromCApiMock fromCApiMock;
	distortos::ConditionVariable conditionVariableMock {distortos::ConditionVariable::UnitTestTag{}};
	distortos_ConditionVariable conditionVariable;

	REQUIRE(distortos_ConditionVariable_notifyOne(nullptr) == EINVAL);

	REQUIRE_CALL(fromCApiMock,
			getConditionVariable(_)).LR_WITH(&_1 == &conditionVariable).LR_RETURN(std::ref(conditionVariableMock));
	REQUIRE_CALL(conditionVariableMock, notifyOne());
	REQUIRE(distortos_ConditionVariable_notifyOne(&conditionVariable) == 0);
}

TEST_CASE("Testing distortos_ConditionVariable_wait()", "[wait]")
{
	distortos::FromCApiMock fromCApiMock;
	distortos::ConditionVariable conditionVariableMock {distortos::ConditionVariable::UnitTestTag{}};
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_ConditionVariable conditionVariable;
	distortos_Mutex mutex;

	REQUIRE(distortos_ConditionVariable_wait(nullptr, &mutex) == EINVAL);
	REQUIRE(distortos_ConditionVariable_wait(&conditionVariable, nullptr) == EINVAL);

	REQUIRE_CALL(fromCApiMock,
			getConditionVariable(_)).LR_WITH(&_1 == &conditionVariable).LR_RETURN(std::ref(conditionVariableMock));
	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	REQUIRE_CALL(conditionVariableMock, wait(_)).LR_WITH(&_1 == &mutexMock).RETURN(EPERM);
	REQUIRE(distortos_ConditionVariable_wait(&conditionVariable, &mutex) == EPERM);
}

TEST_CASE("Testing distortos_ConditionVariable_waitFor()", "[waitFor]")
{
	constexpr int64_t randomDuration {0x393d0330d0218f6a};

	distortos::FromCApiMock fromCApiMock;
	distortos::ConditionVariable conditionVariableMock {distortos::ConditionVariable::UnitTestTag{}};
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_ConditionVariable conditionVariable;
	distortos_Mutex mutex;

	REQUIRE(distortos_ConditionVariable_waitFor(nullptr, &mutex, randomDuration) == EINVAL);
	REQUIRE(distortos_ConditionVariable_waitFor(&conditionVariable, nullptr, randomDuration) == EINVAL);

	REQUIRE_CALL(fromCApiMock,
			getConditionVariable(_)).LR_WITH(&_1 == &conditionVariable).LR_RETURN(std::ref(conditionVariableMock));
	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	const auto duration = distortos::TickClock::duration{randomDuration};
	REQUIRE_CALL(conditionVariableMock, waitFor(_, duration)).LR_WITH(&_1 == &mutexMock).RETURN(ETIMEDOUT);
	REQUIRE(distortos_ConditionVariable_waitFor(&conditionVariable, &mutex, randomDuration) == ETIMEDOUT);
}

TEST_CASE("Testing distortos_ConditionVariable_waitUntil()", "[waitUntil]")
{
	constexpr int64_t randomTimePoint {0x080eca18e717cc1d};

	distortos::FromCApiMock fromCApiMock;
	distortos::ConditionVariable conditionVariableMock {distortos::ConditionVariable::UnitTestTag{}};
	distortos::Mutex mutexMock {distortos::Mutex::UnitTestTag{}};
	distortos_ConditionVariable conditionVariable;
	distortos_Mutex mutex;

	REQUIRE(distortos_ConditionVariable_waitUntil(nullptr, &mutex, randomTimePoint) == EINVAL);
	REQUIRE(distortos_ConditionVariable_waitUntil(&conditionVariable, nullptr, randomTimePoint) == EINVAL);

	REQUIRE_CALL(fromCApiMock,
			getConditionVariable(_)).LR_WITH(&_1 == &conditionVariable).LR_RETURN(std::ref(conditionVariableMock));
	REQUIRE_CALL(fromCApiMock, getMutex(_)).LR_WITH(&_1 == &mutex).LR_RETURN(std::ref(mutexMock));
	const auto timePoint = distortos::TickClock::time_point{distortos::TickClock::duration{randomTimePoint}};
	REQUIRE_CALL(conditionVariableMock, waitUntil(_, timePoint)).LR_WITH(&_1 == &mutexMock).RETURN(ETIMEDOUT);
	REQUIRE(distortos_ConditionVariable_waitUntil(&conditionVariable, &mutex, randomTimePoint) == ETIMEDOUT);
}
