/**
 * \file
 * \brief getDeferredThreadDeleter() definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/memory/getDeferredThreadDeleter.hpp"

#ifdef CONFIG_THREAD_DETACH_ENABLE

#include "distortos/internal/memory/DeferredThreadDeleter.hpp"

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// storage for main instance of DeferredThreadDeleter
std::aligned_storage<sizeof(DeferredThreadDeleter), alignof(DeferredThreadDeleter)>::type deferredThreadDeleterStorage;

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

DeferredThreadDeleter& getDeferredThreadDeleter()
{
	return reinterpret_cast<DeferredThreadDeleter&>(deferredThreadDeleterStorage);
}

}	// namespace internal

}	// namespace distortos

#endif	// def CONFIG_THREAD_DETACH_ENABLE
