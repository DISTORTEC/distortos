/**
 * \file
 * \brief MutexProtocol enum class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_MUTEXPROTOCOL_HPP_
#define INCLUDE_DISTORTOS_MUTEXPROTOCOL_HPP_

#include <cstdint>

namespace distortos
{

/// mutex protocols
enum class MutexProtocol : uint8_t
{
	/// no protocol, similar to PTHREAD_PRIO_NONE
	none,
	/// priority inheritance protocol, similar to PTHREAD_PRIO_INHERIT
	priorityInheritance,
	/// priority protection protocol (Immediate Ceiling Priority Protocol), similar to PTHREAD_PRIO_PROTECT
	priorityProtect,
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MUTEXPROTOCOL_HPP_
