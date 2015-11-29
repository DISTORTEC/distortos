/**
 * \file
 * \brief __malloc_lock() system call implementation
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-29
 */

#include "distortos/internal/memory/getMallocMutex.hpp"

#include "distortos/Mutex.hpp"

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Recursively locks Mutex used for malloc() and free() locking.
 */

void __malloc_lock()
{
	distortos::internal::getMallocMutex().lock();
}

}	// extern "C"
