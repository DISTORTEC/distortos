/**
 * \file
 * \brief Semaphore C-API test cases
 *
 * This test checks whether semaphore objects instantiated with C-API macros and functions are binary identical to
 * constructed distortos::Semaphore objects.
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/Semaphore.hpp"
#include "distortos/C-API/Semaphore.h"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

void testCommon(distortos_Semaphore& semaphore, const unsigned int value, const unsigned int maxValue = UINT_MAX)
{
	unsigned int readValue;
	REQUIRE(distortos_Semaphore_getValue(&semaphore, &readValue) == 0);
	REQUIRE(readValue == std::min(value, maxValue));
	unsigned int readMaxValue;
	REQUIRE(distortos_Semaphore_getMaxValue(&semaphore, &readMaxValue) == 0);
	REQUIRE(readMaxValue == maxValue);
	distortos_Semaphore constructed;
	memcpy(&constructed, &semaphore, sizeof(semaphore));
	REQUIRE(distortos_Semaphore_destruct(&semaphore) == 0);
	struct distortos_Semaphore destructed;
	memcpy(&destructed, &semaphore, sizeof(semaphore));

	memset(&semaphore, 0, sizeof(semaphore));

	const auto realSemaphore = new (&semaphore) distortos::Semaphore {value, maxValue};
	REQUIRE(realSemaphore->getValue() == std::min(value, maxValue));
	REQUIRE(realSemaphore->getMaxValue() == maxValue);
	REQUIRE(memcmp(&constructed, &semaphore, sizeof(semaphore)) == 0);
	realSemaphore->~Semaphore();
	REQUIRE(memcmp(&destructed, &semaphore, sizeof(semaphore)) == 0);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing DISTORTOS_SEMAPHORE_INITIALIZER()", "[initializer]")
{
	constexpr unsigned int lowRandomValue {0x2c380555};
	constexpr unsigned int highRandomValue {0x8d87de63};

	{
		distortos_Semaphore semaphore = DISTORTOS_SEMAPHORE_INITIALIZER(semaphore, lowRandomValue, highRandomValue);
		testCommon(semaphore, lowRandomValue, highRandomValue);
	}
	{
		distortos_Semaphore semaphore = DISTORTOS_SEMAPHORE_INITIALIZER(semaphore, highRandomValue, lowRandomValue);
		testCommon(semaphore, highRandomValue, lowRandomValue);
	}
}

TEST_CASE("Testing DISTORTOS_SEMAPHORE_CONSTRUCT_1()", "[construct]")
{
	constexpr unsigned int lowRandomValue {0x0dc449b6};
	constexpr unsigned int highRandomValue {0x726030cd};

	{
		DISTORTOS_SEMAPHORE_CONSTRUCT_1(semaphore, lowRandomValue, highRandomValue);
		testCommon(semaphore, lowRandomValue, highRandomValue);
	}
	{
		DISTORTOS_SEMAPHORE_CONSTRUCT_1(semaphore, highRandomValue, lowRandomValue);
		testCommon(semaphore, highRandomValue, lowRandomValue);
	}
}

TEST_CASE("Testing DISTORTOS_SEMAPHORE_CONSTRUCT()", "[construct]")
{
	constexpr unsigned int randomValue {0x5d0051dc};

	{
		DISTORTOS_SEMAPHORE_CONSTRUCT(semaphore, randomValue);
		testCommon(semaphore, randomValue);
	}
}

TEST_CASE("Testing distortos_Semaphore_construct_1()", "[construct]")
{
	constexpr unsigned int lowRandomValue {0x7a78032b};
	constexpr unsigned int highRandomValue {0xb24e4367};

	{
		distortos_Semaphore semaphore {};
		REQUIRE(distortos_Semaphore_construct_1(&semaphore, lowRandomValue, highRandomValue) == 0);
		testCommon(semaphore, lowRandomValue, highRandomValue);
	}
	{
		distortos_Semaphore semaphore {};
		REQUIRE(distortos_Semaphore_construct_1(&semaphore, highRandomValue, lowRandomValue) == 0);
		testCommon(semaphore, highRandomValue, lowRandomValue);
	}
}

TEST_CASE("Testing distortos_Semaphore_construct()", "[construct]")
{
	constexpr unsigned int randomValue {0xb86c251b};

	{
		distortos_Semaphore semaphore {};
		REQUIRE(distortos_Semaphore_construct(&semaphore, randomValue) == 0);
		testCommon(semaphore, randomValue);
	}
}
