/**
 * \file
 * \brief _isatty_r() system call implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
 * \brief Checks whether a file is a terminal.
 *
 * See [isatty()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/isatty.html)
 *
 * \param [in] fileDescriptor is a descriptor of file that will be checked
 *
 * \return 1 if \a fileDescriptor refers to a terminal, 0 otherwise and sets errno to error code:
 * - EBADF - \a fileDescriptor is not a valid file descriptor;
 * - ENOTTY - \a fileDescriptor is not a terminal;
 * - error codes returned by File::isATerminal();
 */

int _isatty_r(_reent*, const int fileDescriptor)
{
	if (fileDescriptor < 0 || static_cast<size_t>(fileDescriptor) >= fileDescriptions.size() ||
			fileDescriptions[fileDescriptor] == nullptr)
	{
		errno = EBADF;
		return {};
	}

	int ret;
	bool isATerminal;
	std::tie(ret, isATerminal) = fileDescriptions[fileDescriptor]->isATerminal();
	if (ret != 0)
	{
		errno = ret;
		return {};
	}

	if (isATerminal == false)
	{
		errno = ENOTTY;
		return {};
	}

	return 1;
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
