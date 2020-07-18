/**
 * \file
 * \brief getDeferredThreadDeleter() definition
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETDEFERREDTHREADDELETER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETDEFERREDTHREADDELETER_HPP_

#include "distortos/distortosConfiguration.h"

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

namespace distortos
{

namespace internal
{

class DeferredThreadDeleter;

/**
 * \return reference to main instance of DeferredThreadDeleter
 */

constexpr DeferredThreadDeleter& getDeferredThreadDeleter()
{
	extern DeferredThreadDeleter deferredThreadDeleterInstance;
	return deferredThreadDeleterInstance;
}

}	// namespace internal

}	// namespace distortos

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE

#endif	// INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETDEFERREDTHREADDELETER_HPP_
