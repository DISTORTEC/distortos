/**
 * \file
 * \brief seekdir() implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/FileSystem/Directory.hpp"

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
 * \brief Moves position in the directory.
 *
 * See [seekdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/seekdir.html)
 *
 * \pre %Directory is opened.
 * \pre \a dir is valid.
 *
 * \param [in] dir is a pointer to directory that will be accessed, must be valid
 * \param [in] position is the value of position, must be a value previously returned by telldir()!
 */

void seekdir(DIR* const dir, const long position)
{
	assert(dir != nullptr);

	const auto directory = reinterpret_cast<Directory*>(dir);
	directory->seek(position);
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
