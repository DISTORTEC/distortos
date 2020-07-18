/**
 * \file
 * \brief telldir() implementation
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

#include <tuple>

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
 * \brief Returns current position in the directory.
 *
 * See [telldir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/telldir.html)
 *
 * \pre %Directory is opened.
 * \pre \a dir is valid.
 *
 * \param [in] dir is a pointer to directory that will be accessed, must be valid
 *
 * \return current position in the directory
 */

long telldir(DIR* const dir)
{
	assert(dir != nullptr);

	const auto directory = reinterpret_cast<Directory*>(dir);
	off_t position;
	std::tie(std::ignore, position) = directory->getPosition();
	return position;
}

}	// extern "C"

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
