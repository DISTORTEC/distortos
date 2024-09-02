/**
 * \file
 * \brief getMainStack() implementation for ARMv6-M and ARMv7-M
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/getMainStack.hpp"

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
