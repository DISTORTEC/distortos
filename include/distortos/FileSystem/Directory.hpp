/**
 * \file
 * \brief Directory class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_DIRECTORY_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_DIRECTORY_HPP_

#include <dirent.h>

#include <utility>

namespace distortos
{

/**
 * Directory class is an interface for a directory.
 *
 * \ingroup fileSystem
 */

class Directory
{
public:

	/**
	 * \brief Directory's destructor
	 *
	 * \pre Directory is closed.
	 */

	virtual ~Directory() = default;

	/**
	 * \brief Closes directory.
	 *
	 * Similar to [closedir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html)
	 *
	 * \pre Directory is opened.
	 *
	 * \post Directory is closed.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int close() = 0;

	/**
	 * \brief Returns current position in the directory.
	 *
	 * Similar to [telldir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/telldir.html)
	 *
	 * \pre Directory is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current position in the directory
	 */

	virtual std::pair<int, off_t> getPosition() = 0;

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

	virtual void lock() = 0;

	/**
	 * \brief Reads next entry from directory.
	 *
	 * Similar to [readdir_r()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html)
	 *
	 * \return pair with return code (0 on success, error code otherwise) and next entry from directory; error codes:
	 * - EBADF - the directory is not opened;
	 * - ENOENT - current position in the directory is invalid (i.e. end of the directory reached);
	 */

	virtual std::pair<int, struct dirent> read() = 0;

	/**
	 * \brief Resets current position in the directory.
	 *
	 * Similar to [rewinddir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html)
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the directory is not opened;
	 */

	virtual int rewind() = 0;

	/**
	 * \brief Moves position in the directory.
	 *
	 * Similar to [seekdir()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/seekdir.html)
	 *
	 * \param [in] position is the value of position, must be a value previously returned by getPosition()!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the directory is not opened;
	 */

	virtual int seek(off_t position) = 0;

	/**
	 * \brief Unlocks the directory which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EPERM - current thread did not lock the directory;
	 */

	virtual int unlock() = 0;

	Directory() = default;
	Directory(const Directory&) = delete;
	Directory& operator=(const Directory&) = delete;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_DIRECTORY_HPP_
