/**
 * \file
 * \brief fatalErrorHook() header
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FATALERRORHOOK_H_
#define INCLUDE_DISTORTOS_FATALERRORHOOK_H_

#include "distortos/distortosConfiguration.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

#ifndef DISTORTOS_LIGHTWEIGHT_FATAL_ERROR

/**
 * \brief Hook function called when fatal error is detected, regular version.
 *
 * This function is called at the beginning of fatalErrorHandler(). It may be used to put the system into a safe state,
 * log the failure, reset the device, etc. After this function returns, the system is halted by calling abort() with
 * masked interrupts.
 *
 * \note Use of this function is optional - it may be left undefined, in which case it will not be called.
 *
 * \param [in] file is a null-terminated string with the name of file in which fatal error was detected
 * \param [in] line is a line number at which fatal error was detected
 * \param [in] function is a null-terminated string with function in which fatal error was detected
 * \param [in] message is a null-terminated string with the message of fatal error
 */

void fatalErrorHook(const char* file, int line, const char* function, const char* message) __attribute__ ((weak));

#else	/* def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR */

/**
 * \brief Hook function called when fatal error is detected, lightweight version.
 *
 * This function is called at the beginning of fatalErrorHandler(). It may be used to put the system into a safe state,
 * log the failure, reset the device, etc. After this function returns, interrupts are masked and the system is halted
 * with an inifinite loop.
 *
 * \note Use of this function is optional - it may be left undefined, in which case it will not be called.
 */

void fatalErrorHook() __attribute__ ((weak));

#endif	/* def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR */

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif /* INCLUDE_DISTORTOS_FATALERRORHOOK_H_ */
