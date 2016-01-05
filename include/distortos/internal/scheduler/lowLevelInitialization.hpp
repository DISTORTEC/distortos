/**
 * \file
 * \brief internal::lowLevelInitialization() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_LOWLEVELINITIALIZATION_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_LOWLEVELINITIALIZATION_HPP_

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Low level system initialization
 *
 * 1. Initializes main instance of system's Scheduler;
 * 2. Initializes main thread with its group;
 * 3. Starts idle thread;
 * 4. Initializes main instance of Mutex used for malloc() and free() locking;
 * 5. Initializes main instance of DeferredThreadDeleter (only if CONFIG_THREAD_DETACH_ENABLE option is enabled);
 *
 * This function is called before constructors for global and static objects from __libc_init_array() via address in
 * distortosPreinitArray[].
 */

void lowLevelInitialization();

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SCHEDULER_LOWLEVELINITIALIZATION_HPP_
