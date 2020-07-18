/**
 * \file
 * \brief fatalErrorHandler() implementation
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

void fatalErrorHandler(const char* const file, const int line, const char* const function, const char* const message)
{
	if (fatalErrorHook != nullptr)
		fatalErrorHook(file, line, function, message);

	const distortos::InterruptMaskingLock interruptMaskingLock;
	abort();
}

}	// extern "C"
