/**
 * \file
 * \brief _close_r() system call implementation
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

#include "distortos/Mutex.hpp"

#include <mutex>

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
 * \brief Closes file.
 *
 * See [close()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html)
 *
 * \note Even if error code is returned, the file must not be used.
 *
 * \param [in] fileDescriptor is a descriptor of file that will be closed
 *
 * \return 0 on success, -1 otherwise; error codes (via errno):
 * - EBADF - \a fileDescriptor is not an opened file descriptor;
 * - error codes returned by File::close();
 */

int _close_r(_reent*, const int fileDescriptor)
{
	if (fileDescriptor < 0 || static_cast<size_t>(fileDescriptor) >= fileDescriptions.size() ||
			fileDescriptions[fileDescriptor] == nullptr)
	{
		errno = EBADF;
		return -1;
	}

	auto& fileDescription = fileDescriptions[fileDescriptor];
	const auto ret = fileDescription->close();

	{
		const std::lock_guard<Mutex> lockGuard {fileDescriptionsMutex};

		fileDescription.reset();
		--fileDescriptionsCount;
	}

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
