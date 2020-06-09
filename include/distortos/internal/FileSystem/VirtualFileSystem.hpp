/**
 * \file
 * \brief VirtualFileSystem class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_VIRTUALFILESYSTEM_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_VIRTUALFILESYSTEM_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/MountPointSharedPointer.hpp"

namespace distortos
{

namespace internal
{

/// VirtualFileSystem class is a top-level file system used to access other mounted file systems
class VirtualFileSystem
{
public:

	/**
	 * \brief VirtualFileSystem's constructor
	 */

	constexpr VirtualFileSystem() :
			mountPoints_{},
			mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance}
	{

	}

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
	 * \param [in] path is the path of mount point at which \a fileSystem will be mounted, must be valid, must consist
	 * of only one component, must consist of only alphanumeric characters, its length must be less than or equal to
	 * \a MountPoint::maxNameLength
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOMEM - unable to allocate memory for mount point;
	 * - error codes returned by FileSystem::mount();
	 */

	int mount(FileSystem& fileSystem, const char* path);

	/**
	 * \brief Unmounts file system from mount point with given name.
	 *
	 * Similar to [umount2()](https://linux.die.net/man/2/umount)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of mount point from which file system will be unmounted, must be valid, must consist
	 * of only one component, must be currently mounted
	 * \param [in] detach selects whether a regular (false) or lazy (true) unmount should be performed; see description
	 * of `MNT_DETACH` flag for [umount2()](https://linux.die.net/man/2/umount) for details
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBUSY - file system cannot be unmounted because it is busy;
	 */

	int unmount(const char* path, bool detach);

private:

	/// single node on intrusive list of mount points
	class MountPointListNode : public MountPointSharedPointer
	{
	public:

		/// import all constructors from base class
		using MountPointSharedPointer::MountPointSharedPointer;

		/// node for intrusive list
		estd::IntrusiveListNode node;
	};

	/// list of mount points
	estd::IntrusiveList<MountPointListNode, &MountPointListNode::node> mountPoints_;

	/// mutex for serializing access to the object
	distortos::Mutex mutex_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_VIRTUALFILESYSTEM_HPP_
