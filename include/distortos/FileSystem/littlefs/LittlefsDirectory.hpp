/**
 * \file
 * \brief LittlefsDirectory class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSDIRECTORY_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSDIRECTORY_HPP_

#include "distortos/FileSystem/Directory.hpp"

#include "lfs.h"

namespace distortos
{

class LittlefsFileSystem;

/**
 * LittlefsDirectory class is an [littlefs](https://github.com/ARMmbed/littlefs) directory.
 *
 * \ingroup fileSystem
 */

class LittlefsDirectory : public Directory
{
	friend class LittlefsFileSystem;

public:

	/**
	 * \brief LittlefsDirectory's destructor
	 *
	 * \pre Directory is closed.
	 */

	~LittlefsDirectory() override;

	/**
	 * \brief Closes directory.
	 *
	 * Similar to [closedir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Directory is opened.
	 *
	 * \post Directory is closed.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs_dir_close();
	 */

	int close() override;

	/**
	 * \brief Returns current position in the directory.
	 *
	 * Similar to [telldir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/telldir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Directory is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current position in the directory; error
	 * codes:
	 * - converted error codes returned by lfs_dir_tell();
	 */

	std::pair<int, off_t> getPosition() override;

	/**
	 * \brief Locks the directory for exclusive use by current thread.
	 *
	 * When the object is locked, any call to any member function from other thread will be blocked until the object is
	 * unlocked. Locking is optional, but may be useful when more than one operation must be done atomically.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of recursive locks of directory is less than 65535.
	 *
	 * \post Directory is locked.
	 */

	void lock() override;

	/**
	 * \brief Reads next entry from directory.
	 *
	 * Similar to [readdir_r()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html)
	 *
	 * `d_name` field is set in all cases. All other fields are zero-initialized.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre Directory is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and next entry from directory; error codes:
	 * - ENOENT - current position in the directory is invalid (i.e. end of the directory reached);
	 * - converted error codes returned by lfs_dir_read();
	 */

	std::pair<int, struct dirent> read() override;

	/**
	 * \brief Resets current position in the directory.
	 *
	 * Similar to [rewinddir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the directory is not opened;
	 * - converted error codes returned by lfs_dir_rewind();
	 */

	int rewind() override;

	/**
	 * \brief Moves position in the directory.
	 *
	 * Similar to [seekdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/seekdir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] position is the value of position, must be a value previously returned by getPosition()!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the directory is not opened;
	 * - converted error codes returned by lfs_dir_seek();
	 */

	int seek(off_t position) override;

	/**
	 * \brief Unlocks the directory which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by LittlefsFileSystem::unlock();
	 */

	int unlock() override;

private:

	/**
	 * \brief LittlefsDirectory's constructor
	 *
	 * \param [in] fileSystem is a reference to owner file system
	 */

	constexpr explicit LittlefsDirectory(LittlefsFileSystem& fileSystem) :
			directory_{},
			fileSystem_{fileSystem},
			opened_{}
	{

	}

	/**
	 * \brief Opens directory.
	 *
	 * \param [in] path is the path of directory that will be opened
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \path is not valid;
	 * - converted error codes returned by lfs_dir_open();
	 */

	int open(const char* path);

	/// littlefs directory
	lfs_dir_t directory_;

	/// reference to owner file system
	LittlefsFileSystem& fileSystem_;

	/// true if directory is opened, false otherwise
	bool opened_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSDIRECTORY_HPP_
