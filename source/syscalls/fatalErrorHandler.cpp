/**
 * \file
 * \brief fatalErrorHandler() implementation
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/fatalErrorHandler.h"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include "distortos/fatalErrorHook.h"

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

	distortos::architecture::InterruptMaskingLock interruptMaskingLock;
	abort();
}

}	// extern "C"
