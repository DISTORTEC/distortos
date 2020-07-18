/**
 * \file
 * \brief unmount() declaration
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_UNMOUNT_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_UNMOUNT_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Unmounts file system from mount point with given name.
 *
 * Similar to [umount2()](https://linux.die.net/man/2/umount)
 *
 * \warning This function must not be called from interrupt context!
 *
 * \pre \a path is valid.
 *
 * \param [in] path is the path of mount point from which file system will be unmounted, must be valid, must consist of
 * only one component, must be currently mounted
 * \param [in] detach selects whether a regular (false) or lazy (true) unmount should be performed; see description of
 * `MNT_DETACH` flag for [umount2()](https://linux.die.net/man/2/umount) for details
 *
 * \return 0 on success, error code otherwise:
 * - error codes returned by VirtualFileSystem::unmount();
 */

int unmount(const char* path, bool detach);

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_UNMOUNT_HPP_
