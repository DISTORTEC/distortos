/**
 * \file
 * \brief _lseek_r() system call implementation
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
#include <cstdio>

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
 * \brief Moves file offset.
 *
 * See [lseek()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/lseek.html)
 *
 * \param [in] fileDescriptor is a descriptor of file that will be accessed
 * \param [in] offset is the value of offset, bytes
 * \param [in] whence selects the mode of operation: `SEEK_SET` will set file offset to \a offset, `SEEK_CUR` will set
 * file offset to its current value plus \a offset, `SEEK_END` will set file offset to the size of the file plus
 * \a offset
 *
 * \return current file offset on success, -1 otherwise; error codes (via errno):
 * - EBADF - \a fileDescriptor is not a valid file descriptor;
 * - EINVAL - \a whence is not valid;
 * - error codes returned by File::seek();
 */

off_t _lseek_r(_reent*, const int fileDescriptor, const off_t offset, const int whence)
{
	if (whence != SEEK_CUR && whence != SEEK_END && whence != SEEK_SET)
	{
		errno = EINVAL;
		return -1;
	}

	if (fileDescriptor < 0 || static_cast<size_t>(fileDescriptor) >= fileDescriptions.size() ||
			fileDescriptions[fileDescriptor] == nullptr)
	{
		errno = EBADF;
		return -1;
	}

	int ret;
	off_t position;
	std::tie(ret, position) = fileDescriptions[fileDescriptor]->seek(whence == SEEK_SET ? File::Whence::beginning :
			whence == SEEK_CUR ? File::Whence::current : File::Whence::end, offset);
	if (ret != 0)
	{
		errno = ret;
		return -1;
	}

	return position;
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
