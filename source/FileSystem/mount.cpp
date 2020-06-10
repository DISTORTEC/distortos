/**
 * \file
 * \brief mount() definition
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/FileSystem/mount.hpp"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/rootFileSystem.hpp"
#include "distortos/internal/FileSystem/VirtualFileSystem.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int mount(FileSystem& fileSystem, const char* const path)
{
	return internal::rootFileSystem.mount(fileSystem, path);
}

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
