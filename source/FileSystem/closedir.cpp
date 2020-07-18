/**
 * \file
 * \brief closedir() implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/FileSystem/Directory.hpp"

#include "distortos/assert.h"

#include <memory>

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
 * \brief Closes directory.
 *
 * See [closedir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html)
 *
 * \note Even if error code is returned, the directory must not be used.
 *
 * \pre %Directory is opened.
 * \pre \a dir is valid.
 *
 * \post %Directory is closed.
 *
 * \param [in] dir is a pointer to directory that will be closed, must be valid
 *
 * \return 0 on success, -1 otherwise; error codes (via errno):
 * - error codes returned by Directory::close();
 */

int closedir(DIR* const dir)
{
	assert(dir != nullptr);

	std::unique_ptr<Directory> directory {reinterpret_cast<Directory*>(dir)};
	const auto ret = directory->close();
	directory.reset();

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
