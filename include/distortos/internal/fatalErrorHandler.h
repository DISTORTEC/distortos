/**
 * \file
 * \brief fatalErrorHandler() header
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_FATALERRORHANDLER_H_
#define INCLUDE_DISTORTOS_INTERNAL_FATALERRORHANDLER_H_

#include "distortos/distortosConfiguration.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

#ifndef DISTORTOS_LIGHTWEIGHT_FATAL_ERROR

/**
 * \brief Handler of fatal errors, regular version.
 *
 * This function is called via FATAL_ERROR() macro, which is used to supply some of the arguments. It calls weak
 * fatalErrorHook() (only if it is defined) and halts the system by calling abort() with masked interrupts.
 *
 * \param [in] file is a null-terminated string with the name of file in which fatal error was detected
 * \param [in] line is a line number at which fatal error was detected
 * \param [in] function is a null-terminated string with function in which fatal error was detected
 * \param [in] message is a null-terminated string with the message of fatal error
 */

void fatalErrorHandler(const char* file, int line, const char* function,
		const char* message) __attribute__ ((noreturn));

#else	/* def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR */

/**
 * \brief Handler of fatal errors, lightweight version.
 *
 * This function is called via FATAL_ERROR() macro. It calls weak fatalErrorHook() (only if it is defined) and halts the
 * system by masking interrupts and executing an infinite loop.
 */

void fatalErrorHandler() __attribute__ ((noreturn));

#endif	/* def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR */

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif /* INCLUDE_DISTORTOS_INTERNAL_FATALERRORHANDLER_H_ */
