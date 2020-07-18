/**
 * \file
 * \brief Littlefs1FileSystem class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS1FILESYSTEM_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS1FILESYSTEM_HPP_

#include "distortos/FileSystem/FileSystem.hpp"

#include "distortos/Mutex.hpp"

#include "lfs1.h"

namespace distortos
{

namespace devices
{

class MemoryTechnologyDevice;

}	// namespace devices

/**
 * \brief Littlefs1FileSystem class is a [littlefs-v1](https://github.com/ARMmbed/littlefs) file system.
 *
 * \ingroup fileSystem
 */

class Littlefs1FileSystem : public FileSystem
{
	friend class Littlefs1Directory;
	friend class Littlefs1File;

public:

	/**
	 * \brief Littlefs1FileSystem's constructor
	 *
	 * \param [in] memoryTechnologyDevice is a reference to memory technology device on which the file system will be
	 * mounted
	 * \param [in] readBlockSize is the read block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] programBlockSize is the program block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] eraseBlockSize is the erase block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] blocksCount is the number of erase blocks used for file system, 0 to use max value of device,
	 * default - 0
	 * \param [in] lookahead is the number of blocks to lookahead during block allocation, default - 512
	 */

	constexpr explicit Littlefs1FileSystem(devices::MemoryTechnologyDevice& memoryTechnologyDevice,
			const size_t readBlockSize = {}, const size_t programBlockSize = {}, const size_t eraseBlockSize = {},
			const size_t blocksCount = {}, const size_t lookahead = 32 * 16) :
					configuration_{},
					fileSystem_{},
					mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
					memoryTechnologyDevice_{memoryTechnologyDevice},
					readBlockSize_{readBlockSize},
					programBlockSize_{programBlockSize},
					eraseBlockSize_{eraseBlockSize},
					blocksCount_{blocksCount},
					lookahead_{lookahead},
					mounted_{}
	{

	}

	/**
	 * \brief Littlefs1FileSystem's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is unmounted.
	 */

	~Littlefs1FileSystem() override;

	/**
	 * \brief Formats associated device with the file system.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is unmounted.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_format();
	 * - error codes returned by MemoryTechnologyDevice::open();
	 */

	int format() override;

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
	 * \pre %File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path to file for which status should be returned, must be valid
	 * \param [out] status is a reference to `stat` struct into which status of file will be written
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_stat();
	 */

	int getFileStatus(const char* path, struct stat& status) override;

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
	 * \pre %File system is mounted.
	 *
	 * \param [out] status is a reference to `statvfs` struct into which status of file system will be written
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_traverse();
	 */

	int getStatus(struct statvfs& status) override;

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
	 * \post %File system is locked.
	 */

	void lock() override;

	/**
	 * \brief Makes a directory.
	 *
	 * Similar to [mkdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mkdir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of the directory that will be created, must be valid
	 * \param [in] mode is the value of permission bits of the created directory
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_mkdir();
	 */

	int makeDirectory(const char* path, mode_t mode) override;

	/**
	 * \brief Mounts file system on associated device.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is unmounted.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_mount();
	 * - error codes returned by MemoryTechnologyDevice::open();
	 */

	int mount() override;

	/**
	 * \brief Opens directory.
	 *
	 * Similar to [opendir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of directory that will be opened, must be valid
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened directory;
	 * error codes:
	 * - ENOMEM - unable to allocate memory for directory;
	 * - error codes returned by Littlefs1Directory::open();
	 */

	std::pair<int, std::unique_ptr<Directory>> openDirectory(const char* path) override;

	/**
	 * \brief Opens file.
	 *
	 * Similar to [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is mounted.
	 * \pre \a path is valid.
	 * \pre \a flags are valid.
	 *
	 * \param [in] path is the path of file that will be opened, must be valid
	 * \param [in] flags are file status flags, must be valid, for list of available flags and valid combinations see
	 * [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \return pair with return code (0 on success, error code otherwise) and `std::unique_ptr` with opened file; error
	 * codes:
	 * - ENOMEM - unable to allocate memory for file;
	 * - error codes returned by Littlefs1File::open();
	 */

	std::pair<int, std::unique_ptr<File>> openFile(const char* path, int flags) override;

	/**
	 * \brief Removes file or directory.
	 *
	 * Similar to [remove()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/remove.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is mounted.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of file or directory that will be removed, must be valid
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_remove();
	 */

	int remove(const char* path) override;

	/**
	 * \brief Renames file or directory.
	 *
	 * Similar to [rename()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rename.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is mounted.
	 * \pre \a path and \a newPath are valid.
	 *
	 * \param [in] path is the path of file or directory that will be renamed, must be valid
	 * \param [in] newPath is the new path of file or directory, must be valid
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_rename();
	 */

	int rename(const char* path, const char* newPath) override;

	/**
	 * \brief Unlocks the file system which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the file system.
	 */

	void unlock() override;

	/**
	 * \brief Unmounts file system from associated device.
	 *
	 * \note Even if error code is returned, the file system must not be used (until it is successfully mounted again).
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is mounted.
	 *
	 * \post %File system is unmounted.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_unmount();
	 * - error codes returned by MemoryTechnologyDevice::close();
	 */

	int unmount() override;

private:

	/// configuration of littlefs-v1
	lfs1_config configuration_;

	/// littlefs-v1 file system
	lfs1_t fileSystem_;

	/// mutex for serializing access to the object
	distortos::Mutex mutex_;

	/// reference to associated memory technology device
	devices::MemoryTechnologyDevice& memoryTechnologyDevice_;

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

	/// tells whether the file system is currently mounted on associated memory technology device (true) or not (false)
	bool mounted_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS1FILESYSTEM_HPP_
