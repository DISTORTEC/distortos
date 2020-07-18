/**
 * \file
 * \brief Mutex C-API compile/link test
 *
 * The only purpose of this test is to ensure mutex C-API can be used from C code and that whole application can be
 * linked correctly. It just uses all types, macros and functions from the tested header.
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/C-API/Mutex.h"

#include <stddef.h>

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void compileLinkTest()
{
	{
		struct distortos_Mutex mutex = DISTORTOS_MUTEX_INITIALIZER(mutex, 0, 0, 0);
	}
	{
		DISTORTOS_MUTEX_CONSTRUCT_3(mutex, 0, 0, 0);
	}
	{
		DISTORTOS_MUTEX_CONSTRUCT_2PC(mutex, 0, 0);
	}
	{
		DISTORTOS_MUTEX_CONSTRUCT_2TP(mutex, 0, 0);
	}
	{
		DISTORTOS_MUTEX_CONSTRUCT_1P(mutex, 0);
	}
	{
		DISTORTOS_MUTEX_CONSTRUCT_1T(mutex, 0);
	}
	{
		DISTORTOS_MUTEX_CONSTRUCT(mutex);
	}

	distortos_Mutex_construct_3(NULL, 0, 0, 0);
	distortos_Mutex_construct_2pc(NULL, 0, 0);
	distortos_Mutex_construct_2tp(NULL, 0, 0);
	distortos_Mutex_construct_1p(NULL, 0);
	distortos_Mutex_construct_1t(NULL, 0);
	distortos_Mutex_construct(NULL);
	distortos_Mutex_destruct(NULL);
	distortos_Mutex_lock(NULL);
	distortos_Mutex_tryLock(NULL);
	distortos_Mutex_tryLockFor(NULL, 0);
	distortos_Mutex_tryLockUntil(NULL, 0);
	distortos_Mutex_unlock(NULL);
}
