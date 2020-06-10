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

struct stat;

namespace distortos
{

class Directory;
class File;

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
	 * \brief Returns status of file.
	 *
	 * Similar to [stat()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/stat.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path to file for which status should be returned, must be valid
	 * \param [out] status is a reference to `stat` struct into which status of file will be written
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOENT - no such file or directory;
	 * - error codes returned by FileSystem::getFileStatus();
	 */

	int getFileStatus(const char* path, struct stat& status);

	/**
	 * \brief Makes a directory.
	 *
	 * Similar to [mkdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mkdir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of the directory that will be created, must be valid
	 * \param [in] mode is the value of permission bits of the created directory
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOENT - prefix component of \a path does not name an existing directory;
	 * - error codes returned by FileSystem::makeDirectory();
	 */

	int makeDirectory(const char* path, mode_t mode);

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
	 * \brief Opens directory.
	 *
	 * Similar to [opendir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of directory that will be opened, must be valid
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened directory;
	 * error codes:
	 * - ENOENT - \a path does not name an existing directory;
	 * - ENOMEM - unable to allocate memory for directory;
	 * - error codes returned by FileSystem::openDirectory();
	 */

	std::pair<int, std::unique_ptr<Directory>> openDirectory(const char* path);

	/**
	 * \brief Opens file.
	 *
	 * Similar to [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a path is valid.
	 * \pre \a flags are valid.
	 *
	 * \param [in] path is the path of file that will be opened, must be valid
	 * \param [in] flags are file status flags, must be valid, for list of available flags and valid combinations see
	 * [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened file; error
	 * codes:
	 * - ENOENT - `O_CREAT` is not set and \a path does not name an existing file or `O_CREAT` is set and prefix
	 * component of \a path does not name an existing directory;
	 * - ENOMEM - unable to allocate memory for file;
	 * - error codes returned by FileSystem::openFile();
	 */

	std::pair<int, std::unique_ptr<File>> openFile(const char* path, int flags);

	/**
	 * \brief Removes file or directory.
	 *
	 * Similar to [remove()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/remove.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of file or directory that will be removed, must be valid
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOENT - \a path does not name an existing file or directory;
	 * - error codes returned by FileSystem::remove();
	 */

	int remove(const char* path);

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

	/**
	 * \brief Gets shared pointer to mount point associated with provided name.
	 *
	 * \pre \a name is valid.
	 *
 	 * \param [in] name is the name of requested mount point, must be valid
	 * \param [in] length is the length of \a name
	 *
	 * \return shared pointer to mount point associated with \a nameRange, the object may be empty if no mount point was
	 * found
	 */

	MountPointSharedPointer getMountPointSharedPointer(const char* name, size_t length);

	/// list of mount points
	estd::IntrusiveList<MountPointListNode, &MountPointListNode::node> mountPoints_;

	/// mutex for serializing access to the object
	distortos::Mutex mutex_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_VIRTUALFILESYSTEM_HPP_
