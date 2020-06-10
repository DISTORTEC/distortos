/**
 * \file
 * \brief mount() declaration
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_MOUNT_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_MOUNT_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

namespace distortos
{

class FileSystem;

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Mounts file system at mount point with given name.
 *
 * Similar to [mount()](https://linux.die.net/man/2/mount)
 *
 * \warning This function must not be called from interrupt context!
 *
 * \pre \a path is valid.
 *
 * \param [in] fileSystem is a reference to file system that will be mounted
 * \param [in] path is the path of mount point at which \a fileSystem will be mounted, must be valid, must consist of
 * only one component, the single component must consist of only alphanumeric characters, length of the single component
 * must be less than or equal to \a internal::MountPoint::maxNameLength
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by VirtualFileSystem::mount();
 */

int mount(FileSystem& fileSystem, const char* path);

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_MOUNT_HPP_
