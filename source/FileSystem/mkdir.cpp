/**
 * \file
 * \brief mkdir() implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
 * \brief Makes a directory.
 *
 * See [mkdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mkdir.html)
 *
 * \pre \a path is valid.
 *
 * \param [in] path is the path of the directory that will be created, must be valid
 * \param [in] mode is the value of permission bits of the created directory
 *
 * \return 0 on success, -1 otherwise; error codes (via errno):
 * - error codes returned by VirtualFileSystem::makeDirectory();
 */

int mkdir(const char* const path, const mode_t mode)
{
	const auto ret = rootFileSystem.makeDirectory(path, mode);
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
