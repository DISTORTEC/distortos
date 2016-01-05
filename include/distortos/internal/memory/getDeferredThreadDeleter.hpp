/**
 * \file
 * \brief getDeferredThreadDeleter() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETDEFERREDTHREADDELETER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETDEFERREDTHREADDELETER_HPP_

#include "distortos/distortosConfiguration.h"

#ifdef CONFIG_THREAD_DETACH_ENABLE

namespace distortos
{

namespace internal
{

class DeferredThreadDeleter;

/**
 * \return reference to main instance of DeferredThreadDeleter
 */

DeferredThreadDeleter& getDeferredThreadDeleter();

}	// namespace internal

}	// namespace distortos

#endif	// def CONFIG_THREAD_DETACH_ENABLE

#endif	// INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETDEFERREDTHREADDELETER_HPP_
