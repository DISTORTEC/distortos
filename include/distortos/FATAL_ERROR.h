/**
 * \file
 * \brief FATAL_ERROR() macro
 *
 * \author Copyright (C) 2016-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FATAL_ERROR_H_
#define INCLUDE_DISTORTOS_FATAL_ERROR_H_

#include "distortos/internal/fatalErrorHandler.h"

#undef FATAL_ERROR

#ifndef DISTORTOS_LIGHTWEIGHT_FATAL_ERROR

#ifndef FATAL_ERROR_FUNCTION

/**
 * \brief name of function in which fatal error was detected
 *
 * Depending on the context it wraps:
 * - `__PRETTY_FUNCTION__` for C++ (assumes that GCC is used),
 * - `__func__` for C99,
 * - `__FUNCTION__` otherwise (assumes that GCC is used).
 */

#ifdef __cplusplus
#define FATAL_ERROR_FUNCTION	__PRETTY_FUNCTION__
#elif __STDC_VERSION__ >= 199901L
#define FATAL_ERROR_FUNCTION	__func__
#else
#define FATAL_ERROR_FUNCTION	__FUNCTION__
#endif

#endif	/* !def FATAL_ERROR_FUNCTION */

/**
 * \brief Regular version of macro used for handling fatal errors - wrapper for fatalErrorHandler()
 *
 * \param [in] message is the message of fatal error
 */

#define FATAL_ERROR(message)	fatalErrorHandler(__FILE__, __LINE__, FATAL_ERROR_FUNCTION, (message))

#else	/* def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR */

/**
 * \brief Lightweight version of macro used for handling fatal errors - wrapper for fatalErrorHandler()
 *
 * \param [in] message is the message of fatal error (ignored)
 */

#define FATAL_ERROR(message)	fatalErrorHandler()

#endif	/* def DISTORTOS_LIGHTWEIGHT_FATAL_ERROR */

#endif /* INCLUDE_DISTORTOS_FATAL_ERROR_H_ */
