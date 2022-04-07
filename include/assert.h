/**
 * \file
 * \brief assert() macro
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_ASSERT_H_
#define INCLUDE_ASSERT_H_

#ifndef DISTORTOS_UNIT_TEST

#include "distortos/internal/assertHandler.h"

#undef assert

#ifndef NDEBUG

#ifndef DISTORTOS_LIGHTWEIGHT_ASSERT

#ifndef ASSERT_FUNCTION

/**
 * \brief name of function in which an assertion failed
 *
 * Depending on the context it wraps:
 * - `__PRETTY_FUNCTION__` for C++ (assumes that GCC is used),
 * - `__func__` for C99,
 * - `__FUNCTION__` otherwise (assumes that GCC is used).
 */

#ifdef __cplusplus
#define ASSERT_FUNCTION __PRETTY_FUNCTION__
#elif __STDC_VERSION__ >= 199901L
#define ASSERT_FUNCTION __func__
#else
#define ASSERT_FUNCTION __FUNCTION__
#endif

#endif	/* !def ASSERT_FUNCTION */

/**
 * \brief assert() macro for "regular asserts enabled" configuration
 *
 * Does nothing is \a expression is true, otherwise calls assertHandler() with location of the failed assertion.
 *
 * \param [in] expression is the expression that is checked
 */

#define assert(expression) ((expression) ? (void)0 : assertHandler(__FILE__, __LINE__, ASSERT_FUNCTION, #expression))

#else	/* def DISTORTOS_LIGHTWEIGHT_ASSERT */

/**
 * \brief assert() macro for "lightweight asserts enabled" configuration
 *
 * Does nothing is \a expression is true, otherwise calls assertHandler().
 *
 * \param [in] expression is the expression that is checked
 */

#define assert(expression) ((expression) ? (void)0 : assertHandler())

#endif	/* def DISTORTOS_LIGHTWEIGHT_ASSERT */

#else	/* def NDEBUG */

/**
 * \brief assert() macro for "asserts disabled" configuration which does not produce "unused variable" warning
 */

#define assert(expression) (void)sizeof(expression)

#endif	/* def NDEBUG */

#if __STDC_VERSION__ >= 201112L && !defined(__cplusplus)
#undef static_assert
#define static_assert _Static_assert
#endif

#else	/* def DISTORTOS_UNIT_TEST */

#include_next <assert.h>

#endif	/* def DISTORTOS_UNIT_TEST */

#endif /* INCLUDE_ASSERT_H_ */
