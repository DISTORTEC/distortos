/**
 * \file
 * \brief LittlefsFileSystem class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSFILESYSTEM_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSFILESYSTEM_HPP_

#include "distortos/FileSystem/FileSystem.hpp"

#include "distortos/Mutex.hpp"

#include "lfs.h"

namespace distortos
{

/**
 * LittlefsFileSystem class is an [littlefs](https://github.com/ARMmbed/littlefs) file system.
 *
 * \ingroup fileSystem
 */

class LittlefsFileSystem : public FileSystem
{
	friend class LittlefsDirectory;
	friend class LittlefsFile;

public:

	/**
	 * \brief LittlefsFileSystem's constructor
	 *
	 * \param [in] readBlockSize is the read block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] programBlockSize is the program block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] eraseBlockSize is the erase block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] blocksCount is the number of erase blocks used for file system, 0 to use max value of device,
	 * default - 0
	 * \param [in] lookahead is the number of blocks to lookahead during block allocation, default - 512
	 */

	constexpr explicit LittlefsFileSystem(const size_t readBlockSize = {}, const size_t programBlockSize = {},
			const size_t eraseBlockSize = {}, const size_t blocksCount = {}, const size_t lookahead = 32 * 16) :
					configuration_{},
					fileSystem_{},
					mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
					memoryTechnologyDevice_{},
					readBlockSize_{readBlockSize},
					programBlockSize_{programBlockSize},
					eraseBlockSize_{eraseBlockSize},
					blocksCount_{blocksCount},
					lookahead_{lookahead}
	{

	}

	/**
	 * \brief LittlefsFileSystem's destructor
	 *
	 * Unmounts the file system.
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~LittlefsFileSystem() override;

	/**
	 * \brief Unmounts current file system (if any is mounted), formats device with the file system and mounts it.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] memoryTechnologyDevice is a pointer to memory technology device which will be formatted with the file
	 * system and be mounted, nullptr to use currently mounted memory technology device
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - no file system is currently mounted and \a memoryTechnologyDevice is nullptr;
	 * - error codes returned by format();
	 * - error codes returned by mount();
	 * - error codes returned by unmount();
	 */

	int formatAndMount(devices::MemoryTechnologyDevice* memoryTechnologyDevice) override;

	/**
	 * \brief Returns status of file.
	 *
	 * Similar to [stat()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/stat.html)
	 *
	 * `st_mode` field is set in all cases, `st_size` field is set only for regular files. All other fields are
	 * zero-initialized.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] path is the path to file for which status should be returned
	 *
	 * \return pair with return code (0 on success, error code otherwise) and status of file in `stat` struct; error
	 * codes:
	 * - EBADF - no file system mounted;
	 * - converted error codes returned by lfs_stat();
	 */

	std::pair<int, struct stat> getFileStatus(const char* path) override;

	/**
	 * \brief Returns status of file system.
	 *
	 * Similar to [statvfs()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/statvfs.html)
	 *
	 * `f_bsize`, `f_frsize`, `f_blocks`, `f_bfree`, `f_bavail` and `f_namemax` fields are set in all cases. All other
	 * fields are zero-initialized.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return pair with return code (0 on success, error code otherwise) and status of file system in `statvfs` struct;
	 * error codes:
	 * - EBADF - no file system mounted;
	 * - converted error codes returned by lfs_traverse();
	 */

	std::pair<int, struct statvfs> getStatus() override;

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
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::lock();
	 */

	int lock() override;

	/**
	 * \brief Makes a directory.
	 *
	 * Similar to [mkdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mkdir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] path is the path of the directory that will be created
	 * \param [in] mode is the value of permission bits of the created directory
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - no file system mounted;
	 * - converted error codes returned by lfs_mkdir();
	 */

	int makeDirectory(const char* path, mode_t mode) override;

	/**
	 * \brief Mounts file system on provided device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] memoryTechnologyDevice is a reference to memory technology device on which the file system will be
	 * mounted
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBUSY - file system is already mounted;
	 * - converted error codes returned by lfs_mount();
	 * - error codes returned by MemoryTechnologyDevice::open();
	 */

	int mount(devices::MemoryTechnologyDevice& memoryTechnologyDevice) override;

	/**
	 * \brief Opens directory.
	 *
	 * Similar to [opendir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] path is the path of directory that will be opened
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened directory;
	 * error codes:
	 * - EBADF - no file system mounted;
	 * - ENOMEM - unable to allocate memory for directory;
	 * - error codes returned by LittlefsDirectory::open();
	 */

	std::pair<int, std::unique_ptr<Directory>> openDirectory(const char* path) override;

	/**
	 * \brief Opens file.
	 *
	 * Similar to [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] path is the path of file that will be opened
	 * \param [in] flags are file status flags, for list of available flags and valid combinations see
	 * [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened file; error
	 * codes:
	 * - EBADF - no file system mounted;
	 * - ENOMEM - unable to allocate memory for file;
	 * - error codes returned by LittlefsFile::open();
	 */

	std::pair<int, std::unique_ptr<File>> openFile(const char* path, int flags) override;

	/**
	 * \brief Removes file or directory.
	 *
	 * Similar to [remove()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/remove.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] path is the path of file or directory that will be removed
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - no file system mounted;
	 * - converted error codes returned by lfs_remove();
	 */

	int remove(const char* path) override;

	/**
	 * \brief Renames file or directory.
	 *
	 * Similar to [rename()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rename.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] path is the path of file or directory that will be renamed
	 * \param [in] newPath is the new path of file or directory
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - no file system mounted;
	 * - converted error codes returned by lfs_rename();
	 */

	int rename(const char* path, const char* newPath) override;

	/**
	 * \brief Unlocks the file system which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Mutex::lock();
	 */

	int unlock() override;

	/**
	 * \brief Unmounts file system from associated device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - no file system mounted;
	 * - converted error codes returned by lfs_unmount();
	 * - error codes returned by MemoryTechnologyDevice::close();
	 */

	int unmount() override;

	/**
	 * \brief Formats device with the file system.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] memoryTechnologyDevice is a reference to memory technology device which will be formatted
	 * \param [in] readBlockSize is the read block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] programBlockSize is the program block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] eraseBlockSize is the erase block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] blocksCount is the number of erase blocks used for file system, 0 to use max value of device,
	 * default - 0
	 * \param [in] lookahead is the number of blocks to lookahead during block allocation, default - 512
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs_format();
	 * - error codes returned by MemoryTechnologyDevice::open();
	 */

	static int format(devices::MemoryTechnologyDevice& memoryTechnologyDevice, size_t readBlockSize = {},
			size_t programBlockSize = {}, size_t eraseBlockSize = {}, size_t blocksCount = {},
			size_t lookahead = 32 * 16);

private:

	/// configuration of littlefs
	lfs_config configuration_;

	/// littlefs file system
	lfs_t fileSystem_;

	/// mutex for serializing access to the object
	distortos::Mutex mutex_;

	/// pointer to memory technology device on which file system is currently mounted, nullptr if file system is not
	/// mounted
	devices::MemoryTechnologyDevice* memoryTechnologyDevice_;

	/// read block size, bytes, 0 to use default value of device
	size_t readBlockSize_;

	/// program block size, bytes, 0 to use default value of device
	size_t programBlockSize_;

	/// erase block size, bytes, 0 to use default value of device
	size_t eraseBlockSize_;

	/// number of erase blocks used for file system, 0 to use max value of device
	size_t blocksCount_;

	/// number of blocks to lookahead during block allocation
	size_t lookahead_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSFILESYSTEM_HPP_
