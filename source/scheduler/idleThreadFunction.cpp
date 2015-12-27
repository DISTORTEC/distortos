/**
 * \file
 * \brief idleThreadFunction() definition
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-27
 */

#include "distortos/internal/scheduler/idleThreadFunction.hpp"

#include "distortos/internal/memory/DeferredThreadDeleter.hpp"
#include "distortos/internal/memory/getDeferredThreadDeleter.hpp"

#include <cstdint>

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void idleThreadFunction()
{
	volatile uint64_t i {};

	while (1)
	{
		++i;

#ifdef CONFIG_THREAD_DETACH_ENABLE

		getDeferredThreadDeleter().tryCleanup();	/// \todo error handling?

#endif	// def CONFIG_THREAD_DETACH_ENABLE
	}
}

}	// namespace internal

}	// namespace distortos
