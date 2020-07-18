/**
 * \file
 * \brief assert() macro
 *
 * \author Copyright (C) 2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ASSERT_H_
#define INCLUDE_DISTORTOS_ASSERT_H_

#include <assert.h>

#ifdef NDEBUG

#undef assert

/**
 * \brief Replacement of newlib's assert() macro for "asserts disabled" configuration which does not produce "unused
 * variable" warning
 */

#define assert(expression)		(void)sizeof(expression)

#endif	/* def NDEBUG */

#endif /* INCLUDE_DISTORTOS_ASSERT_H_ */
