/**
 * \file
 * \brief _unlink_r() system call implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/rootFileSystem.hpp"
#include "distortos/internal/FileSystem/VirtualFileSystem.hpp"

namespace distortos
{

namespace internal
{

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Removes directory entry.
 *
 * See [unlink()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/unlink.html)
 *
 * \pre \a path is valid.
 *
 * \param [in] path is the path of entry that will be removed, must be valid
 *
 * \return 0 on success, -1 otherwise; error codes (via errno):
 * - error codes returned by VirtualFileSystem::remove();
 */

int _unlink_r(_reent*, const char* const path)
{
	const auto ret = rootFileSystem.remove(path);
	if (ret != 0)
	{
		errno = ret;
		return -1;
	}

	return {};
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
