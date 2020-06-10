/**
 * \file
 * \brief statvfs() implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
 * \brief Returns status of file system.
 *
 * See [statvfs()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/statvfs.html)
 *
 * \pre \a path is valid.
 *
 * \param [in] path is a path to file or directory contained on file system which should be accessed
 * \param [out] status is a reference to `statvfs` struct into which status of file system will be written
 *
 * \return 0 on success, -1 otherwise; error codes (via errno):
 * - error codes returned by VirtualFileSystem::getStatus();
 */

int statvfs(const char* const path, struct statvfs* const status)
{
	assert(status != nullptr);

	const auto ret = rootFileSystem.getStatus(path, *status);
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
