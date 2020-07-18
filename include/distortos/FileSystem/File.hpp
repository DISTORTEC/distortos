/**
 * \file
 * \brief File class header
 *
 * \author Copyright (C) 2018-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_FILE_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_FILE_HPP_

#include <sys/stat.h>

#include <utility>

#include <cstddef>

namespace distortos
{

/**
 * \brief File class is an interface for a file.
 *
 * \ingroup fileSystem
 */

class File
{
public:

	/// seek() mode of operation
	enum class Whence
	{
		/// set file offset to provided argument, similar to `SEEK_SET`
		beginning,
		/// set file offset to its current value plus provided argument, similar to `SEEK_CUR`
		current,
		/// set file offset to the size of the file plus provided argument, similar to `SEEK_END`
		end,
	};

	/**
	 * \brief File's destructor
	 *
	 * \pre %File is closed.
	 */

	virtual ~File() = default;

	/**
	 * \brief Closes file.
	 *
	 * Similar to [close()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html)
	 *
	 * \note Even if error code is returned, the file must not be used.
	 *
	 * \pre %File is opened.
	 *
	 * \post %File is closed.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int close() = 0;

	/**
	 * \brief Returns current file offset.
	 *
	 * Similar to [ftello()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/ftell.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes
	 */

	virtual std::pair<int, off_t> getPosition() = 0;

	/**
	 * \brief Returns size of file.
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and size of file, bytes
	 */

	virtual std::pair<int, off_t> getSize() = 0;

	/**
	 * \brief Returns status of file.
	 *
	 * Similar to [fstat()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fstat.html)
	 *
	 * \pre %File is opened.
	 *
	 * \param [out] status is a reference to `stat` struct into which status of file will be written
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int getStatus(struct stat& status) = 0;

	/**
	 * \brief Tells whether the file is a terminal.
	 *
	 * Similar to [isatty()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/isatty.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and bool telling whether the file is a
	 * terminal (true) or not (false)
	 */

	virtual std::pair<int, bool> isATerminal() = 0;

	/**
	 * \brief Locks the file for exclusive use by current thread.
	 *
	 * When the object is locked, any call to any member function from other thread will be blocked until the object is
	 * unlocked. Locking is optional, but may be useful when more than one operation must be done atomically.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The number of recursive locks of file is less than 65535.
	 *
	 * \post %File is locked.
	 */

	virtual void lock() = 0;

	/**
	 * \brief Reads data from file.
	 *
	 * Similar to [read()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html)
	 *
	 * \pre \a buffer is valid.
	 *
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - file is not opened for reading;
	 */

	virtual std::pair<int, size_t> read(void* buffer, size_t size) = 0;

	/**
	 * \brief Resets current file offset.
	 *
	 * Similar to [rewind()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewind.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int rewind() = 0;

	/**
	 * \brief Moves file offset.
	 *
	 * Similar to [lseek()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/lseek.html)
	 *
	 * \pre %File is opened.
	 *
	 * \param [in] whence selects the mode of operation: `Whence::beginning` will set file offset to \a offset,
	 * `Whence::current` will set file offset to its current value plus \a offset, `Whence::end` will set file offset to
	 * the size of the file plus \a offset
	 * \param [in] offset is the value of offset, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes; error codes:
	 * - EINVAL - resulting file offset would be negative;
	 */

	virtual std::pair<int, off_t> seek(Whence whence, off_t offset) = 0;

	/**
	 * \brief Synchronizes state of a file, ensuring all cached writes are finished.
	 *
	 * Similar to [fsync()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fsync.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return 0 on success, error code otherwise
	 */

	virtual int synchronize() = 0;

	/**
	 * \brief Unlocks the file which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the file.
	 */

	virtual void unlock() = 0;

	/**
	 * \brief Writes data to file.
	 *
	 * Similar to [write()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html)
	 *
	 * \pre \a buffer is valid.
	 *
	 * \param [in] buffer is the buffer with data that will be written, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - file is not opened for writing;
	 * - ENOSPC - no space left on the device containing the file;
	 */

	virtual std::pair<int, size_t> write(const void* buffer, size_t size) = 0;

	File() = default;
	File(const File&) = delete;
	File& operator=(const File&) = delete;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_FILE_HPP_
