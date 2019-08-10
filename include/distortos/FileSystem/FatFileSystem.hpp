/**
 * \file
 * \brief FatFileSystem class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_FATFILESYSTEM_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_FATFILESYSTEM_HPP_

#include "distortos/FileSystem/FileSystem.hpp"

#include "distortos/Mutex.hpp"

#include "ufat.h"

namespace distortos
{

namespace devices
{

class BlockDevice;

}	// namespace devices

/**
 * FatFileSystem class is a [FAT](https://en.wikipedia.org/wiki/File_Allocation_Table) file system.
 *
 * \ingroup fileSystem
 */

class FatFileSystem : public FileSystem
{
	friend class FatDirectory;
	friend class FatFile;

public:

	/// UfatDevice binds ufat_device struct with a reference to block device
	struct UfatDevice
	{
		/// uFAT device
		ufat_device device;

		/// reference to associated block device
		devices::BlockDevice& blockDevice;
	};

	/**
	 * \brief FatFileSystem's constructor
	 *
	 * \param [in] blockDevice is a reference to block device on which the file system will be mounted
	 * \param [in] blockSize is the block size, bytes, 0 to use default value of device, default - 0
	 * \param [in] blocksCount is the number of blocks used when formating the device with the file system, 0 to use max
	 * value of device, default - 0
	 */

	constexpr explicit FatFileSystem(devices::BlockDevice& blockDevice, const size_t blockSize = {},
			const size_t blocksCount = {}) :
				device_{{}, blockDevice},
				fileSystem_{},
				mutex_{Mutex::Type::recursive, Mutex::Protocol::priorityInheritance},
				blocksCount_{blocksCount},
				blockSize_{blockSize},
				mounted_{}
	{

	}

	/**
	 * \brief FatFileSystem's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is unmounted.
	 */

	~FatFileSystem() override;

	/**
	 * \brief Formats associated device with the file system.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File system is unmounted.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - block size is not a power of 2;
	 * - converted error codes returned by ufat_mkfs();
	 * - error codes returned by BlockDevice::open();
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
	 * - ENOENT - no such file or directory;
	 * - converted error codes returned by ufat_dir_find_path();
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
	 * - converted error codes returned by ufat_count_free_clusters();
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
	 * - EEXIST - named file exists;
	 * - ENOENT - prefix component of \a path does not name an existing directory;
	 * - converted error codes returned by ufat_dir_create();
	 * - converted error codes returned by ufat_dir_find_path();
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
	 * - EINVAL - block size is not a power of 2;
	 * - converted error codes returned by ufat_open();
	 * - error codes returned by BlockDevice::open();
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
	 * - error codes returned by FatDirectory::open();
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
	 * - error codes returned by FatFile::open();
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
	 * - ENOENT - \a path does not name an existing file or directory;
	 * - converted error codes returned by ufat_dir_delete();
	 * - converted error codes returned by ufat_dir_find_path();
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
	 * - EEXIST - file or directory named by \a newPath exists;
	 * - ENOENT - \a path does not name an existing file or directory or prefix component of \a newPath does not name an
	 * existing directory;
	 * - converted error codes returned by ufat_dir_find_path();
	 * - converted error codes returned by ufat_move();
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
	 * - error codes returned by BlockDevice::close();
	 */

	int unmount() override;

private:

	/// ufat_device struct bound with a reference to block device
	UfatDevice device_;

	/// uFAT file system
	ufat fileSystem_;

	/// mutex for serializing access to the object
	distortos::Mutex mutex_;

	/// number of blocks used when formating the device with the file system, 0 to use max value of device
	size_t blocksCount_;

	/// block size, bytes, 0 to use default value of device
	size_t blockSize_;

	/// tells whether the file system is currently mounted on associated block device (true) or not (false)
	bool mounted_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_FATFILESYSTEM_HPP_
