/**
 * \file
 * \brief _fstat_r() system call implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/fileDescriptions.hpp"

#include "distortos/FileSystem/File.hpp"

#include "distortos/assert.h"

#include <cerrno>

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
 * See [fstat()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/fstat.html)
 *
 * \pre \a status is valid.
 *
 * \param [in] fileDescriptor is a descriptor of file that will be accessed
 * \param [out] status is a pointer to `stat` struct into which status of file will be written, must be valid
 *
 * \return 0 on success, -1 otherwise; error codes (via errno):
 * - EBADF - \a fileDescriptor is not a valid file descriptor;
 * - error codes returned by File::getStatus();
 */

int _fstat_r(_reent*, const int fileDescriptor, struct stat* const status)
{
	if (fileDescriptor < 0 || static_cast<size_t>(fileDescriptor) >= fileDescriptions.size() ||
			fileDescriptions[fileDescriptor] == nullptr)
	{
		errno = EBADF;
		return -1;
	}

	assert(status != nullptr);
	const auto ret = fileDescriptions[fileDescriptor]->getStatus(*status);
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
