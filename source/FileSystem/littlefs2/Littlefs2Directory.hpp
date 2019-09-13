/**
 * \file
 * \brief Littlefs2Directory class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_LITTLEFS2_LITTLEFS2DIRECTORY_HPP_
#define SOURCE_FILESYSTEM_LITTLEFS2_LITTLEFS2DIRECTORY_HPP_

#include "distortos/FileSystem/Directory.hpp"

#include "lfs2.h"

namespace distortos
{

class Littlefs2FileSystem;

/**
 * \brief Littlefs2Directory class is a [littlefs-v2](https://github.com/ARMmbed/littlefs) directory.
 *
 * \ingroup fileSystem
 */

class Littlefs2Directory : public Directory
{
public:

	/**
	 * \brief Littlefs2Directory's constructor
	 *
	 * \param [in] fileSystem is a reference to owner file system
	 */

	constexpr explicit Littlefs2Directory(Littlefs2FileSystem& fileSystem) :
			directory_{},
			fileSystem_{fileSystem},
			opened_{}
	{

	}

	/**
	 * \brief Littlefs2Directory's destructor
	 *
	 * \pre %Directory is closed.
	 */

	~Littlefs2Directory() override;

	/**
	 * \brief Closes directory.
	 *
	 * Similar to [closedir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html)
	 *
	 * \note Even if error code is returned, the directory must not be used.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \post %Directory is closed.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs2_dir_close();
	 */

	int close() override;

	/**
	 * \brief Returns current position in the directory.
	 *
	 * Similar to [telldir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/telldir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current position in the directory; error
	 * codes:
	 * - converted error codes returned by lfs2_dir_tell();
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
	 * \post %Directory is locked.
	 */

	void lock() override;

	/**
	 * \brief Opens directory.
	 *
	 * \pre %Directory is not opened.
	 * \pre \a path is valid.
	 *
	 * \param [in] path is the path of directory that will be opened, must be valid
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs2_dir_open();
	 */

	int open(const char* path);

	/**
	 * \brief Reads next entry from directory.
	 *
	 * Similar to [readdir_r()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html)
	 *
	 * `d_name` field is set in all cases. All other fields are zero-initialized.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \param [out] entry is a reference to `dirent` struct into which next entry from directory will be written
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOENT - current position in the directory is invalid (i.e. end of the directory reached);
	 * - converted error codes returned by lfs2_dir_read();
	 */

	int read(dirent& entry) override;

	/**
	 * \brief Resets current position in the directory.
	 *
	 * Similar to [rewinddir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs2_dir_rewind();
	 */

	int rewind() override;

	/**
	 * \brief Moves position in the directory.
	 *
	 * Similar to [seekdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/seekdir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \param [in] position is the value of position, must be a value previously returned by getPosition()!
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs2_dir_seek();
	 */

	int seek(off_t position) override;

	/**
	 * \brief Unlocks the directory which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the directory.
	 */

	void unlock() override;

private:

	/// littlefs-v2 directory
	lfs2_dir_t directory_;

	/// reference to owner file system
	Littlefs2FileSystem& fileSystem_;

	/// true if directory is opened, false otherwise
	bool opened_;
};

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_LITTLEFS2_LITTLEFS2DIRECTORY_HPP_
