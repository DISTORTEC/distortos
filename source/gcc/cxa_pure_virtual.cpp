/**
 * \file
 * \brief __cxa_pure_virtual() system call implementation
 *
 * \author Copyright (C) 2014-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/FATAL_ERROR.h"

#include <cstdlib>

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Replacement of original __cxa_pure_virtual().
 *
 * This function is called on pure virtual function call. It does not return.
 *
 * It just calls FATAL_ERROR().
 */

__attribute__ ((noreturn))
void __cxa_pure_virtual()
{
	FATAL_ERROR("Pure virtual function called!");
}

}	// extern "C"
