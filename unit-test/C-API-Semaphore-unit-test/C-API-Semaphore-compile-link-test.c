/**
 * \file
 * \brief Semaphore C-API compile/link test
 *
 * The only purpose of this test is to ensure semaphore C-API can be used from C code and that whole application can be
 * linked correctly. It just uses all types, macros and functions from the tested header.
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/C-API/Semaphore.h"

#include <stddef.h>

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void compileLinkTest()
{
	{
		struct distortos_Semaphore semaphore = DISTORTOS_SEMAPHORE_INITIALIZER(semaphore, 0, 0);
	}
	{
		DISTORTOS_SEMAPHORE_CONSTRUCT_1(semaphore, 0, 0);
	}
	{
		DISTORTOS_SEMAPHORE_CONSTRUCT(semaphore, 0);
	}

	distortos_Semaphore_construct_1(NULL, 0, 0);
	distortos_Semaphore_construct(NULL, 0);
	distortos_Semaphore_destruct(NULL);
	distortos_Semaphore_getMaxValue(NULL, NULL);
	distortos_Semaphore_getValue(NULL, NULL);
	distortos_Semaphore_post(NULL);
	distortos_Semaphore_tryWait(NULL);
	distortos_Semaphore_tryWaitFor(NULL, 0);
	distortos_Semaphore_tryWaitUntil(NULL, 0);
	distortos_Semaphore_wait(NULL);
}
