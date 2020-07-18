/**
 * \file
 * \brief dummyDeleter() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_MEMORY_DUMMYDELETER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_MEMORY_DUMMYDELETER_HPP_

#include <type_traits>

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief A "no-op" dummy deleter that can be used with std::unique_ptr and automatic storage that is trivially
 * destructible.
 *
 * \tparam T is the real type of storage, must be trivially destructible
 * \tparam U is the type of \a storage pointer
 *
 * \param [in] storage is a pointer to storage
 */

template<typename T, typename U>
void dummyDeleter(U*)
{
	static_assert(std::is_trivially_destructible<T>::value == true,
			"internal::dummyDeleter() cannot be used with types that are not trivially destructible!");
}

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_MEMORY_DUMMYDELETER_HPP_
