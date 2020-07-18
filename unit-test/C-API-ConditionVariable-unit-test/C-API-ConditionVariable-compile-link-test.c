/**
 * \file
 * \brief Condition variable C-API compile/link test
 *
 * The only purpose of this test is to ensure condition variable C-API can be used from C code and that whole
 * application can be linked correctly. It just uses all types, macros and functions from the tested header.
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/C-API/ConditionVariable.h"

#include <stddef.h>

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void compileLinkTest()
{
	{
		struct distortos_ConditionVariable conditionVariable =
				DISTORTOS_CONDITIONVARIABLE_INITIALIZER(conditionVariable);
	}
	{
		DISTORTOS_CONDITIONVARIABLE_CONSTRUCT(conditionVariable);
	}

	distortos_ConditionVariable_construct(NULL);
	distortos_ConditionVariable_destruct(NULL);
	distortos_ConditionVariable_notifyAll(NULL);
	distortos_ConditionVariable_notifyOne(NULL);
	distortos_ConditionVariable_wait(NULL, NULL);
	distortos_ConditionVariable_waitFor(NULL, NULL, 0);
	distortos_ConditionVariable_waitUntil(NULL, NULL, 0);
}
