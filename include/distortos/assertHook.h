/**
 * \file
 * \brief assertHook() header
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ASSERTHOOK_H_
#define INCLUDE_DISTORTOS_ASSERTHOOK_H_

#ifndef NDEBUG

#include "distortos/distortosConfiguration.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

#ifndef DISTORTOS_LIGHTWEIGHT_ASSERT

/**
 * \brief Hook function called on failed assertion, regular version.
 *
 * This function is called at the beginning of assertHandler() when an assertion fails. It may be used to put the system
 * into a safe state, log the failure, reset the device, etc. After this function returns, the system is halted
 * by calling abort() with masked interrupts.
 *
 * \note Use of this function is optional - it may be left undefined, in which case it will not be called.
 *
 * \param [in] file is a null-terminated string with the name of file in which assertion failed
 * \param [in] line is a line number at which assertion failed
 * \param [in] function is a null-terminated string with function in which assertion failed
 * \param [in] failedExpression is a null-terminated string with the failed assert expression
 */

void assertHook(const char* file, int line, const char* function, const char* failedExpression) __attribute__ ((weak));

#else	/* def DISTORTOS_LIGHTWEIGHT_ASSERT */

/**
 * \brief Hook function called on failed assertion, lightweight version.
 *
 * This function is called at the beginning of assertHandler() when an assertion fails. It may be used to put the system
 * into a safe state, log the failure, reset the device, etc. After this function returns, interrupts are masked and the
 * system is halted with an inifinite loop.
 *
 * \note Use of this function is optional - it may be left undefined, in which case it will not be called.
 */

void assertHook() __attribute__ ((weak));

#endif	/* def DISTORTOS_LIGHTWEIGHT_ASSERT */

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* !def NDEBUG */

#endif	/* INCLUDE_DISTORTOS_ASSERTHOOK_H_ */
