/**
 * \file
 * \brief storageDeleter() definition
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-20
 */

#ifndef INCLUDE_DISTORTOS_MEMORY_STORAGEDELETER_HPP_
#define INCLUDE_DISTORTOS_MEMORY_STORAGEDELETER_HPP_

namespace distortos
{

namespace memory
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Templated deleter that can be used with StorageUniquePointer and dynamic storage allocated with new T[].
 *
 * \param T is the real type of allocated storage.
 *
 * \param [in] storage is a pointer to storage that will be deleted
 */

template<typename T>
void storageDeleter(void* const storage)
{
	delete[] reinterpret_cast<T*>(storage);
}

}	// namespace memory

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MEMORY_STORAGEDELETER_HPP_
