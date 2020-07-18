/**
 * \file
 * \brief Condition variable C-API test cases
 *
 * This test checks whether condition variable objects instantiated with C-API macros and functions are binary identical
 * to constructed distortos::ConditionVariable objects.
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/ConditionVariable.hpp"
#include "distortos/C-API/ConditionVariable.h"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

void testCommon(distortos_ConditionVariable& conditionVariable)
{
	distortos_ConditionVariable constructed;
	memcpy(&constructed, &conditionVariable, sizeof(conditionVariable));
	REQUIRE(distortos_ConditionVariable_destruct(&conditionVariable) == 0);
	struct distortos_ConditionVariable destructed;
	memcpy(&destructed, &conditionVariable, sizeof(conditionVariable));

	memset(&conditionVariable, 0, sizeof(conditionVariable));

	const auto realConditionVariable = new (&conditionVariable) distortos::ConditionVariable;
	REQUIRE(memcmp(&constructed, &conditionVariable, sizeof(conditionVariable)) == 0);
	realConditionVariable->~ConditionVariable();
	REQUIRE(memcmp(&destructed, &conditionVariable, sizeof(conditionVariable)) == 0);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing DISTORTOS_CONDITIONVARIABLE_INITIALIZER()", "[initializer]")
{
	distortos_ConditionVariable conditionVariable = DISTORTOS_CONDITIONVARIABLE_INITIALIZER(conditionVariable);
	testCommon(conditionVariable);
}

TEST_CASE("Testing DISTORTOS_CONDITIONVARIABLE_CONSTRUCT()", "[construct]")
{
	DISTORTOS_CONDITIONVARIABLE_CONSTRUCT(conditionVariable);
	testCommon(conditionVariable);
}

TEST_CASE("Testing distortos_ConditionVariable_construct()", "[construct]")
{
	distortos_ConditionVariable conditionVariable {};
	REQUIRE(distortos_ConditionVariable_construct(&conditionVariable) == 0);
	testCommon(conditionVariable);
}
