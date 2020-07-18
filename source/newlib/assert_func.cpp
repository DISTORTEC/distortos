/**
 * \file
 * \brief __assert_func() system call implementation
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/assertHook.h"
#include "distortos/InterruptMaskingLock.hpp"

#include <cassert>
#include <cstdlib>

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void __assert_func(const char* const file, const int line, const char* const function,
		const char* const failedExpression)
{
	if (assertHook != nullptr)
		assertHook(file, line, function, failedExpression);

	const distortos::InterruptMaskingLock interruptMaskingLock;
	abort();
}

}	// extern "C"
