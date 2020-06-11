/**
 * \file
 * \brief statvfs() implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/rootFileSystem.hpp"
#include "distortos/internal/FileSystem/VirtualFileSystem.hpp"

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

int statvfs(const char* const path, struct statvfs* const status)
{
	assert(status != nullptr);

	const auto ret = rootFileSystem.getStatus(path, *status);
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
