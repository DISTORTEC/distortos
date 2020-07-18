/**
 * \file
 * \brief VirtualDirectory2 class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_LITTLEFS2_VIRTUALDIRECTORY2_HPP_
#define SOURCE_FILESYSTEM_LITTLEFS2_VIRTUALDIRECTORY2_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/MountPointSharedPointer.hpp"

#include "distortos/FileSystem/Directory.hpp"

namespace distortos
{

namespace internal
{

/// VirtualDirectory2 class is a wrapper for Directory and shared pointer to its mount point
class VirtualDirectory2 : public Directory
{
public:

	/**
	 * \brief VirtualDirectory2's constructor
	 *
	 * \param [in] directory is a rvalue reference to unique_ptr to directory
	 * \param [in] mountPointSharedPointer is a rvalue reference to shared pointer to mount point
	 */

	VirtualDirectory2(std::unique_ptr<Directory>&& directory, MountPointSharedPointer&& mountPointSharedPointer) :
			directory_{std::move(directory)},
			mountPointSharedPointer_{std::move(mountPointSharedPointer)}
	{

	}

	/**
	 * \brief VirtualDirectory2's destructor
	 *
	 * \pre %Directory is closed.
	 */

	~VirtualDirectory2() override;

	/**
	 * \brief Closes directory.
	 *
	 * Similar to [closedir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html)
	 *
	 * \note Even if error code is returned, the directory must not be used.
	 *
	 * \pre %Directory is opened.
	 *
	 * \post %Directory is closed.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Directory::close();
	 */

	int close() override;

	/**
	 * \brief Returns current position in the directory.
	 *
	 * Similar to [telldir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/telldir.html)
	 *
	 * \pre %Directory is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current position in the directory; error
	 * codes:
	 * - error codes returned by Directory::getPosition();
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
	 * \brief Reads next entry from directory.
	 *
	 * Similar to [readdir_r()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html)
	 *
	 * \pre %Directory is opened.
	 *
	 * \param [out] entry is a reference to `dirent` struct into which next entry from directory will be written
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Directory::read();
	 */

	int read(dirent& entry) override;

	/**
	 * \brief Resets current position in the directory.
	 *
	 * Similar to [rewinddir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html)
	 *
	 * \pre %Directory is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Directory::rewind();
	 */

	int rewind() override;

	/**
	 * \brief Moves position in the directory.
	 *
	 * Similar to [seekdir()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/seekdir.html)
	 *
	 * \pre %Directory is opened.
	 *
	 * \param [in] position is the value of position, must be a value previously returned by getPosition()!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by Directory::seek();
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

	/**
	 * \return true if directory is opened, false otherwise
	 */

	bool isOpened() const
	{
		return directory_ != nullptr && mountPointSharedPointer_ == true;
	}

	/// unique_ptr to directory
	std::unique_ptr<Directory> directory_;

	/// shared pointer to mount point
	MountPointSharedPointer mountPointSharedPointer_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// SOURCE_FILESYSTEM_LITTLEFS2_VIRTUALDIRECTORY2_HPP_
