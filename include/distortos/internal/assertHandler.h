/**
 * \file
 * \brief assertHandler() header
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_ASSERTHANDLER_H_
#define INCLUDE_DISTORTOS_INTERNAL_ASSERTHANDLER_H_

#ifndef NDEBUG

#include "distortos/distortosConfiguration.h"

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

#ifndef DISTORTOS_LIGHTWEIGHT_ASSERT

/**
 * \brief Handler of failed asserts, regular version.
 *
 * This function is called via failed assert() macro, which is used to supply some of the arguments. It calls weak
 * assertHook() (only if it is defined) and halts the system by calling abort() with masked interrupts.
 *
 * \param [in] file is a null-terminated string with the name of file in which assertion failed
 * \param [in] line is a line number at which assertion failed
 * \param [in] function is a null-terminated string with function in which assertion failed
 * \param [in] failedExpression is a null-terminated string with the failed assert expression
 */

void assertHandler(const char* file, int line, const char* function,
		const char* failedExpression) __attribute__ ((noreturn));

#else	/* def DISTORTOS_LIGHTWEIGHT_ASSERT */

/**
 * \brief Handler of failed asserts, lightweight version.
 *
 * This function is called via failed assert() macro. It calls weak assertHook() (only if it is defined) and halts the
 * system by masking interrupts and executing an infinite loop.
 */

void assertHandler() __attribute__ ((noreturn));

#endif	/* def DISTORTOS_LIGHTWEIGHT_ASSERT */

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* !def NDEBUG */

#endif /* INCLUDE_DISTORTOS_INTERNAL_ASSERTHANDLER_H_ */
