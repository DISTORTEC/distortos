/**
 * \file
 * \brief readdir_r() implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/FileSystem/Directory.hpp"

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
 * \brief Reads next entry from directory.
 *
 * See [readdir_r()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html)
 *
 * \pre %Directory is opened.
 * \pre \a dir is valid.
 * \pre \a entry is valid.
 * \pre \a result is valid.
 *
 * \param [in] dir is a pointer to directory that will be read, must be valid
 * \param [in] entry is a pointer to `dirent` struct into which next entry from directory will be written, must be valid
 * \param [out] result is a pointer to the variable which will be written with address of \a entry on success, must be
 * valid
 *
 * \return 0 on success, error code otherwise:
 * - error codes (except ENOENT) returned by Directory::read();
 */

int readdir_r(DIR* const dir, dirent* const entry, dirent** const result)
{
	assert(dir != nullptr);
	assert(entry != nullptr);
	assert(result != nullptr);

	const auto directory = reinterpret_cast<Directory*>(dir);
	const auto ret = directory->read(*entry);
	if (ret != 0 && ret != ENOENT)
		return ret;

	*result = ret == 0 ? entry : nullptr;
	return {};
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
