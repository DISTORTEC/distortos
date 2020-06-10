/**
 * \file
 * \brief _read_r() system call implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/fileDescriptions.hpp"

#include "distortos/FileSystem/File.hpp"

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
 * \brief Reads data from file.
 *
 * See [read()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html)
 *
 * \param [in] fileDescriptor is a descriptor of file that will be read
 * \param [out] buffer is the buffer into which the data will be read, must be valid
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return non-negative number of bytes actually read on success, -1 otherwise; error codes (via errno):
 * - EBADF - \a fileDescriptor is not a valid file descriptor opened for reading;
 * - error codes returned by File::read();
 */

ssize_t _read_r(_reent*, const int fileDescriptor, void* const buffer, const size_t size)
{
	if (fileDescriptor < 0 || static_cast<size_t>(fileDescriptor) >= fileDescriptions.size() ||
			fileDescriptions[fileDescriptor] == nullptr)
	{
		errno = EBADF;
		return -1;
	}

	int ret;
	size_t bytesRead;
	std::tie(ret, bytesRead) = fileDescriptions[fileDescriptor]->read(buffer, size);
	if (bytesRead == 0 && ret != 0)
	{
		errno = ret;
		return -1;
	}

	return bytesRead;
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
