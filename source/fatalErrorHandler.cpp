/**
 * \file
 * \brief fatalErrorHandler() implementation
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/fatalErrorHandler.h"

#include "distortos/fatalErrorHook.h"
#include "distortos/InterruptMaskingLock.hpp"

#include <cstdlib>

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifndef DISTORTOS_LIGHTWEIGHT_FATAL_ERROR

void fatalErrorHandler(const char* const file, const int line, const char* const function, const char* const message)
{
	if (fatalErrorHook != nullptr)
		fatalErrorHook(file, line, function, message);

	const distortos::InterruptMaskingLock interruptMaskingLock;
	abort();
}

#else	// def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR

void fatalErrorHandler()
{
	if (fatalErrorHook != nullptr)
		fatalErrorHook();

	const distortos::InterruptMaskingLock interruptMaskingLock;
	while (1);
}

#endif	// def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR

}	// extern "C"
