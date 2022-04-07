/**
 * \file
 * \brief assertHandler() implementation
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef NDEBUG

#include "distortos/internal/assertHandler.h"

#include "distortos/assertHook.h"
#include "distortos/InterruptMaskingLock.hpp"

#include <cassert>
#include <cstdlib>

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifndef DISTORTOS_LIGHTWEIGHT_ASSERT

void assertHandler(const char* const file, const int line, const char* const function,
		const char* const failedExpression)
{
	if (assertHook != nullptr)
		assertHook(file, line, function, failedExpression);

	const distortos::InterruptMaskingLock interruptMaskingLock;
	abort();
}

#else	// def DISTORTOS_LIGHTWEIGHT_ASSERT

void assertHandler()
{
	if (assertHook != nullptr)
		assertHook();

	const distortos::InterruptMaskingLock interruptMaskingLock;
	while (1);
}

#endif	// def DISTORTOS_LIGHTWEIGHT_ASSERT

}	// extern "C"

#endif	// !def NDEBUG
