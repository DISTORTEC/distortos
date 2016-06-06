/**
 * \file
 * \brief __assert_func() system call implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/assertHook.h"

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

	distortos::architecture::InterruptMaskingLock interruptMaskingLock;
	abort();
}

}	// extern "C"
