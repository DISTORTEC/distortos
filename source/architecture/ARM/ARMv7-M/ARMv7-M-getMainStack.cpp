/**
 * \file
 * \brief getMainStack() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-10-27
 */

#include "distortos/architecture/architecture.hpp"

namespace distortos
{

namespace architecture
{

extern "C"
{

/// beginning of process stack - imported from linker script
extern char __process_stack_start[];

/// size of process stack, bytes - imported from linker script
extern char __process_stack_size[];

}

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<void*, size_t> getMainStack()
{
	return {__process_stack_start, reinterpret_cast<size_t>(__process_stack_size)};
}

}	// namespace architecture

}	// namespace distortos
