/**
 * \file
 * \brief _open_r() system call implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/fileDescriptions.hpp"
#include "distortos/internal/FileSystem/rootFileSystem.hpp"
#include "distortos/internal/FileSystem/VirtualFileSystem.hpp"

#include "distortos/FileSystem/File.hpp"

#include "distortos/assert.h"

#include "estd/ScopeGuard.hpp"

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
 * \brief Opens file.
 *
 * See [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
 *
 * \pre \a path is valid.
 *
 * \param [in] path is the path of file that will be opened, must be valid
 * \param [in] flags are file status flags, must be valid, for list of available flags and valid combinations see
 * [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
 *
 * \return non-negative integer representing the file descriptor on success, -1 otherwise; error codes (via errno):
 * - ENFILE - maximum allowable number of files is currently open in the system;
 * - error codes returned by VirtualFileSystem::openFile();
 */

int _open_r(_reent*, const char* const path, const int flags, int)
{
	{
		const std::lock_guard<Mutex> lockGuard {fileDescriptionsMutex};
		assert(fileDescriptionsCount <= fileDescriptions.size());
		if (fileDescriptionsCount == fileDescriptions.size())
		{
			errno = ENFILE;
			return -1;
		}

		++fileDescriptionsCount;
	}

	auto fileDescriptionsCountScopeGuard = estd::makeScopeGuard(
			[]()
			{
				const std::lock_guard<Mutex> lockGuard {fileDescriptionsMutex};
				assert(fileDescriptionsCount != 0);
				--fileDescriptionsCount;
			});

	std::unique_ptr<File> file;

	{
		int ret;
		std::tie(ret, file) = rootFileSystem.openFile(path, flags);
		if (ret != 0)
		{
			errno = ret;
			return -1;
		}
	}

	decltype(fileDescriptions)::iterator iterator;

	{
		const std::lock_guard<Mutex> lockGuard {fileDescriptionsMutex};

		iterator = std::find(fileDescriptions.begin(), fileDescriptions.end(), nullptr);
		assert(iterator != fileDescriptions.end());

		*iterator = std::move(file);
	}

	fileDescriptionsCountScopeGuard.release();
	return iterator - fileDescriptions.begin();
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
