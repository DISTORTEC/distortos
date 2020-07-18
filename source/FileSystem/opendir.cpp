/**
 * \file
 * \brief opendir() implementation
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

#include "distortos/FileSystem/Directory.hpp"

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
 * \brief Opens directory.
 *
 * See [opendir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html)
 *
 * \pre \a path is valid.
 *
 * \param [in] path is the path of directory that will be opened, must be valid
 *
 * \return pointer to opened directory on success, nullptr otherwise; error codes (via errno):
 * - error codes returned by VirtualFileSystem::openDirectory();
 */

DIR* opendir(const char* const path)
{
	int ret;
	std::unique_ptr<Directory> directory;
	std::tie(ret, directory) = rootFileSystem.openDirectory(path);
	if (ret != 0)
	{
		errno = ret;
		return {};
	}

	return reinterpret_cast<DIR*>(directory.release());
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
