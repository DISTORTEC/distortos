/**
 * \file
 * \brief getMallocMutex() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETMALLOCMUTEX_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETMALLOCMUTEX_HPP_

namespace distortos
{

class Mutex;

namespace internal
{

/**
 * \return reference to main instance of Mutex used for malloc() and free() locking
 */

Mutex& getMallocMutex();

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_MEMORY_GETMALLOCMUTEX_HPP_
