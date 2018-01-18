/**
 * \file
 * \brief distortosPreinitArray object definition
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/startScheduling.hpp"

#include "distortos/board/lowLevelInitialization.hpp"

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for optional board::lowLevelInitialization().
 *
 * Calls board::lowLevelInitialization() if it is defined, otherwise does nothing.
 */

void boardLowLevelInitializationWrapper()
{
	if (board::lowLevelInitialization != nullptr)
		board::lowLevelInitialization();
}

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// pointer to function taking no arguments and returning no value
using FunctionPointer = void(*)();

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array of pointers to functions executed before constructors for global and static objects from __libc_init_array()
const FunctionPointer distortosPreinitArray[] __attribute__ ((section(".preinit_array"), used))
{
		boardLowLevelInitializationWrapper,
		architecture::startScheduling,
};

}	// namespace

}	// namespace internal

}	// namespace distortos
