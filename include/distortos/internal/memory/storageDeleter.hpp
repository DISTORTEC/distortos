/**
 * \file
 * \brief storageDeleter() definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_MEMORY_STORAGEDELETER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_MEMORY_STORAGEDELETER_HPP_

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Templated deleter that can be used with std::unique_ptr and dynamic storage allocated with new T[].
 *
 * \tparam T is the real type of allocated storage
 * \tparam U is the type of \a storage pointer
 *
 * \param [in] storage is a pointer to storage that will be deleted
 */

template<typename T, typename U>
void storageDeleter(U* const storage)
{
	delete[] reinterpret_cast<T*>(storage);
}

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_MEMORY_STORAGEDELETER_HPP_
