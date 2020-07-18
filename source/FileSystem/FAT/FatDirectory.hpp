/**
 * \file
 * \brief FatDirectory class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_FAT_FATDIRECTORY_HPP_
#define SOURCE_FILESYSTEM_FAT_FATDIRECTORY_HPP_

#include "distortos/FileSystem/Directory.hpp"

#include "ufat.h"

namespace distortos
{

class FatFileSystem;

/**
 * \brief FatDirectory class is a [FAT](https://en.wikipedia.org/wiki/File_Allocation_Table) directory.
 *
 * \ingroup fileSystem
 */

class FatDirectory : public Directory
{
public:

	/**
	 * \brief FatDirectory's constructor
	 *
	 * \param [in] fileSystem is a reference to owner file system
	 */

	constexpr explicit FatDirectory(FatFileSystem& fileSystem) :
			directory_{},
			fileSystem_{fileSystem},
			opened_{}
	{

	}

	/**
	 * \brief FatDirectory's destructor
	 *
	 * \pre %Directory is closed.
	 */

	~FatDirectory() override;

	/**
	 * \brief Closes directory.
	 *
	 * Similar to [closedir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html)
	 *
	 * \note Even if error code is returned, the directory must not be used.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \post %Directory is closed.
	 *
	 * \return 0 on success, error code otherwise
	 */

	int close() override;

	/**
	 * \brief Returns current position in the directory.
	 *
	 * Similar to [telldir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/telldir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current position in the directory
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
	 * - ENOENT - \a path does not name an existing directory;
	 * - ENOTDIR - component of \a path names an existing file where directory was expected;
	 * - converted error codes returned by ufat_dir_find_path();
	 */

	int open(const char* path);

	/**
	 * \brief Reads next entry from directory.
	 *
	 * Similar to [readdir_r()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html)
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
	 * - converted error codes returned by ufat_dir_read();
	 */

	int read(dirent& entry) override;

	/**
	 * \brief Resets current position in the directory.
	 *
	 * Similar to [rewinddir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \return 0 on success, error code otherwise
	 */

	int rewind() override;

	/**
	 * \brief Moves position in the directory.
	 *
	 * Similar to [seekdir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/seekdir.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %Directory is opened.
	 *
	 * \param [in] position is the value of position, must be a value previously returned by getPosition()!
	 *
	 * \return 0 on success, error code otherwise:
	 * - ENOENT - \a position is invalid (i.e. end of the directory reached);
	 * - converted error codes returned by ufat_dir_read();
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

	/// uFAT directory
	ufat_directory directory_;

	/// reference to owner file system
	FatFileSystem& fileSystem_;

	/// true if directory is opened, false otherwise
	bool opened_;
};

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_FAT_FATDIRECTORY_HPP_
