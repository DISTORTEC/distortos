/**
 * \file
 * \brief idleThreadFunction() definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-14
 */

#include "distortos/scheduler/idleThreadFunction.hpp"

#include <cstdint>

namespace distortos
{

namespace scheduler
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
	}
}

}	// namespace scheduler

}	// namespace distortos
