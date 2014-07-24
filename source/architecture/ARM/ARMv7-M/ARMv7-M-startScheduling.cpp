/**
 * \file
 * \brief startScheduling() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-07-24
 */

#include "distortos/architecture.hpp"

namespace distortos
{

namespace architecture
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void startScheduling()
{
	asm volatile
	(
			"	svc		0		\n"	// request first context switch
	);

	__builtin_unreachable();
}

}	// namespace architecture

}	// namespace distortos
