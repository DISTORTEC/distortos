/**
 * \file
 * \brief _stat_r() system call implementation
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

#include "distortos/assert.h"

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
 * \brief Returns status of file.
 *
 * See [stat()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/stat.html)
 *
 * \pre \a path is valid.
 * \pre \a status is valid.
 *
 * \param [in] path is the path to file for which status should be returned, must be valid
 * \param [out] status is a pointer to `stat` struct into which status of file will be written, must be valid
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by VirtualFileSystem::getFileStatus();
 */

int _stat_r(_reent*, const char* const path, struct stat* const status)
{
	assert(status != nullptr);

	const auto ret = rootFileSystem.getFileStatus(path, *status);
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
