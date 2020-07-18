/**
 * \file
 * \brief MutexType enum class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_MUTEXTYPE_HPP_
#define INCLUDE_DISTORTOS_MUTEXTYPE_HPP_

#include <cstdint>

namespace distortos
{

/// type of mutex
enum class MutexType : uint8_t
{
	/// normal mutex, similar to PTHREAD_MUTEX_NORMAL
	normal,
	/// mutex with additional error checking, similar to PTHREAD_MUTEX_ERRORCHECK
	errorChecking,
	/// recursive mutex, similar to PTHREAD_MUTEX_RECURSIVE
	recursive,
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MUTEXTYPE_HPP_
