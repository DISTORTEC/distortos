/**
 * \file
 * \brief CHECK_FUNCTION_CONTEXT() macro
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_CHECK_FUNCTION_CONTEXT_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_CHECK_FUNCTION_CONTEXT_HPP_

#include "distortos/distortosConfiguration.h"

#ifdef DISTORTOS_CHECK_FUNCTION_CONTEXT_ENABLE

#include "distortos/architecture/isInInterruptContext.hpp"

#include "distortos/FATAL_ERROR.h"

/**
 * \brief Macro used to check whether the function is executed from thread context.
 *
 * If the function using this macro is executed from interrupt context, FATAL_ERROR() is called.
 */

#define CHECK_FUNCTION_CONTEXT()	(distortos::architecture::isInInterruptContext() == false ? static_cast<void>(0) : \
		FATAL_ERROR("This function cannot be used from interrupt context!"))

#else	/* !def DISTORTOS_CHECK_FUNCTION_CONTEXT_ENABLE */

#define CHECK_FUNCTION_CONTEXT()

#endif	/* !def DISTORTOS_CHECK_FUNCTION_CONTEXT_ENABLE */

#endif /* INCLUDE_DISTORTOS_INTERNAL_CHECK_FUNCTION_CONTEXT_HPP_ */
