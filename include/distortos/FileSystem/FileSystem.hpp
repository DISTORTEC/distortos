/**
 * \file
 * \brief FileSystem class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_FILESYSTEM_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_FILESYSTEM_HPP_

#include "distortos/FileSystem/Directory.hpp"
#include "distortos/FileSystem/File.hpp"

#include <sys/statvfs.h>

#include <memory>

namespace distortos
{

class Directory;
class File;

/**
 * FileSystem class is an interface for a file system.
 *
 * \ingroup fileSystem
 */

class FileSystem
{
public:

	/**
	 * \brief FileSystem's destructor
	 *
	 * \pre File system is unmounted.
	 */

	virtual ~FileSystem() = default;

	/**
	 * \brief Formats associated device with the file system.
	 *
	 * \pre File system is unmounted.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int format() = 0;

	/**
	 * \brief Returns status of file.
	 *
	 * Similar to [stat()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/stat.html)
	 *
	 * \pre File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path to file for which status should be returned, must be valid
	 *
	 * \return pair with return code (0 on success, error code otherwise) and status of file in `stat` struct; error
	 * codes:
	 * - ENAMETOOLONG - length of component of \a path and/or length of \a path are longer than allowed maximum;
	 * - ENOENT - no such file or directory;
	 * - ENOTDIR - component of \a path names an existing file where directory was expected;
	 */

	virtual std::pair<int, struct stat> getFileStatus(const char* path) = 0;

	/**
	 * \brief Returns status of file system.
	 *
	 * Similar to [statvfs()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/statvfs.html)
	 *
	 * \pre File system is mounted.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and status of file system in `statvfs` struct
	 */

	virtual std::pair<int, struct statvfs> getStatus() = 0;

	/**
	 * \brief Locks the file system for exclusive use by current thread.
	 *
	 * When the object is locked, any call to any member function from other thread will be blocked until the object is
	 * unlocked. Locking is optional, but may be useful when more than one operation must be done atomically.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of recursive locks of file system is less than 65535.
	 *
	 * \post File system is locked.
	 */

	virtual void lock() = 0;

	/**
	 * \brief Makes a directory.
	 *
	 * Similar to [mkdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mkdir.html)
	 *
	 * \pre File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of the directory that will be created, must be valid
	 * \param [in] mode is the value of permission bits of the created directory
	 *
	 * \return 0 on success, error code otherwise:
	 * - EEXIST - named file exists;
	 * - ENAMETOOLONG - length of component of \a path and/or length of \a path are longer than allowed maximum;
	 * - ENOENT - prefix component of \a path does not name an existing directory;
	 * - ENOSPC - no space left on the device containing the file system;
	 * - ENOTDIR - component of \a path names an existing file where directory was expected;
	 * - EROFS - read-only file system;
	 */

	virtual int makeDirectory(const char* path, mode_t mode) = 0;

	/**
	 * \brief Mounts file system on associated device.
	 *
	 * \pre File system is unmounted.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EILSEQ - device does not contain valid file system;
	 * - ENOMEM - unable to allocate memory for file system;
	 */

	virtual int mount() = 0;

	/**
	 * \brief Opens directory.
	 *
	 * Similar to [opendir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html)
	 *
	 * \pre File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of directory that will be opened, must be valid
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened directory;
	 * error codes:
	 * - ENAMETOOLONG - length of component of \a path and/or length of \a path are longer than allowed maximum;
	 * - ENOENT - \a path does not name an existing directory;
	 * - ENOMEM - unable to allocate memory for directory;
	 * - ENOTDIR - component of \a path names an existing file where directory was expected;
	 */

	virtual std::pair<int, std::unique_ptr<Directory>> openDirectory(const char* path) = 0;

	/**
	 * \brief Opens file.
	 *
	 * Similar to [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \pre File system is mounted.
	 * \pre \a path is valid.
	 * \pre \a flags are valid.
	 *
	 * \param [in] path is the path of file that will be opened, must be valid
	 * \param [in] flags are file status flags, must be valid, for list of available flags and valid combinations see
	 * [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened file; error
	 * codes:
	 * - EEXIST - `O_CREAT` and `O_EXCL` are set, and file named by \a path exists;
	 * - EISDIR - file named by \a path is a directory;
	 * - ENAMETOOLONG - length of component of \a path and/or length of \a path are longer than allowed maximum;
	 * - ENOENT - `O_CREAT` is not set and \a path does not name an existing file or `O_CREAT` is set and prefix
	 * component of \a path does not name an existing directory;
	 * - ENOMEM - unable to allocate memory for file;
	 * - ENOSPC - `O_CREAT` is set, file named by \a path does not exist and no space is left on the device containing
	 * the file system;	 *
	 * - ENOTDIR - component of \a path names an existing file where directory was expected;
	 * - EROFS - either `O_WRONLY`, `O_RDWR`, `O_CREAT` (and file named by \a path does not exist), or `O_TRUNC` are set
	 * and file system is read-only;
	 */

	virtual std::pair<int, std::unique_ptr<File>> openFile(const char* path, int flags) = 0;

	/**
	 * \brief Removes file or directory.
	 *
	 * Similar to [remove()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/remove.html)
	 *
	 * \pre File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of file or directory that will be removed, must be valid
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENAMETOOLONG - length of component of \a path and/or length of \a path are longer than allowed maximum;
	 * - ENOENT - \a path does not name an existing file or directory;
	 * - ENOTDIR - component of \a path names an existing file where directory was expected;
	 * - ENOTEMPTY - \a path names a directory which is not empty;
	 * - EROFS - read-only file system;
	 */

	virtual int remove(const char* path) = 0;

	/**
	 * \brief Renames file or directory.
	 *
	 * Similar to [rename()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rename.html)
	 *
	 * \pre File system is mounted.
	 * \pre \a path and \a newPath are valid.
	 *
	 * \param [in] path is the path of file or directory that will be renamed, must be valid
	 * \param [in] newPath is the new path of file or directory, must be valid
	 *
	 * \return 0 on success, error code otherwise:
	 * - EEXIST - file or directory named by \a newPath exists;
	 * - EISDIR - \a newPath points to a directory while \a path points to a file;
	 * - ENAMETOOLONG - length of component of \a path and/or length of \a path and/or length of component of \a newPath
	 * and/or length of \a newPath are longer than allowed maximum;
	 * - ENOENT - \a path does not name an existing file or directory or prefix component of \a newPath does not name an
	 * existing directory;
	 * - ENOSPC - no space left on the device containing the file system;
	 * - ENOTDIR - component of \a path and/or \a newPath name an existing file where directory was expected;
	 * - EROFS - read-only file system;
	 */

	virtual int rename(const char* path, const char* newPath) = 0;

	/**
	 * \brief Unlocks the file system which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the file system.
	 */

	virtual void unlock() = 0;

	/**
	 * \brief Unmounts file system from associated device.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - no file system mounted;
	 * - EBUSY - file system is busy;
	 */

	virtual int unmount() = 0;

	FileSystem() = default;
	FileSystem(const FileSystem&) = delete;
	FileSystem& operator=(const FileSystem&) = delete;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_FILESYSTEM_HPP_
