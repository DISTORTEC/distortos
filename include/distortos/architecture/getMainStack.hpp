/**
 * \file
 * \brief getMainStack() declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_GETMAINSTACK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_GETMAINSTACK_HPP_

#include <utility>

#include <cstddef>

namespace distortos
{

namespace architecture
{

/**
 * \brief Gets the stack used to run main().
 *
 * \return beginning of stack and its size in bytes
 */

std::pair<void*, size_t> getMainStack();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_GETMAINSTACK_HPP_
