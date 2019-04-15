/**
 * \file
 * \brief getDeferredThreadDeleter() definition
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/memory/getDeferredThreadDeleter.hpp"

#ifdef DISTORTOS_THREAD_DETACH_ENABLE

#include "distortos/internal/memory/DeferredThreadDeleter.hpp"

#if __GNUC_PREREQ(5, 1) != 1
// GCC 4.x doesn't fully support constexpr constructors
#error "GCC 5.1 is the minimum version supported by distortos"
#endif

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// main instance of DeferredThreadDeleter
DeferredThreadDeleter deferredThreadDeleterInstance;

}	// namespace internal

}	// namespace distortos

#endif	// def DISTORTOS_THREAD_DETACH_ENABLE
