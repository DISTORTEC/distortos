/**
 * \file
 * \brief StorageUniquePointer type alias header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-20
 */

#ifndef INCLUDE_DISTORTOS_MEMORY_STORAGEUNIQUEPOINTER_HPP_
#define INCLUDE_DISTORTOS_MEMORY_STORAGEUNIQUEPOINTER_HPP_

#include <memory>

namespace distortos
{

namespace memory
{

/// unique_ptr (with deleter) to storage
using StorageUniquePointer = std::unique_ptr<void, void(&)(void*)>;

}	// namespace memory

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MEMORY_STORAGEUNIQUEPOINTER_HPP_
