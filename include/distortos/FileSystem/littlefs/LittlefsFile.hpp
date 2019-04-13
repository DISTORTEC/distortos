/**
 * \file
 * \brief LittlefsFile class header
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSFILE_HPP_
#define INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSFILE_HPP_

#include "distortos/FileSystem/File.hpp"

#include "distortos/distortosConfiguration.h"

#include "lfs.h"

#include <memory>

namespace distortos
{

class LittlefsFileSystem;

/**
 * LittlefsFile class is an [littlefs](https://github.com/ARMmbed/littlefs) file.
 *
 * \ingroup fileSystem
 */

class LittlefsFile : public File
{
	friend class LittlefsFileSystem;

public:

	/**
	 * \brief LittlefsFile's destructor
	 *
	 * \pre File is closed.
	 */

	~LittlefsFile() override;

	/**
	 * \brief Closes file.
	 *
	 * Similar to [close()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre File is opened.
	 *
	 * \post File is closed.
	 *
	 * \return 0 on success, error code otherwise
	 * - converted error codes returned by lfs_file_close();
	 */

	int close() override;

	/**
	 * \brief Returns current file offset.
	 *
	 * Similar to [ftello()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/ftell.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes; error codes:
	 * - EBADF - the file is not opened;
	 * - converted error codes returned by lfs_file_tell();
	 */

	std::pair<int, off_t> getPosition() override;

	/**
	 * \brief Returns size of file.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return pair with return code (0 on success, error code otherwise) and size of file, bytes; error codes:
	 * - EBADF - the file is not opened;
	 * - converted error codes returned by lfs_file_size();
	 */

	std::pair<int, off_t> getSize() override;

	/**
	 * \brief Returns status of file.
	 *
	 * Similar to [fstat()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fstat.html)
	 *
	 * `st_mode` and `st_size` fields are set in all cases. All other fields are zero-initialized.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return pair with return code (0 on success, error code otherwise) and status of file in `stat` struct; error
	 * codes:
	 * - error codes returned by getSize();
	 */

	std::pair<int, struct stat> getStatus() override;

	/**
	 * \brief Tells whether the file is a terminal.
	 *
	 * Similar to [isatty()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/isatty.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return pair with return code (0 on success, error code otherwise) and bool telling whether the file is a
	 * terminal (true) or not (false); error codes:
	 * - EBADF - the file is not opened;
	 */

	std::pair<int, bool> isATerminal() override;

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
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by LittlefsFileSystem::lock();
	 */

	int lock() override;

	/**
	 * \brief Reads data from file.
	 *
	 * Similar to [read()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [out] buffer is the buffer into which the data will be read
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - the file is not opened or opened only for writing;
	 * - EINVAL - \a buffer is not valid;
	 * - converted error codes returned by lfs_file_read();
	 */

	std::pair<int, size_t> read(void* buffer, size_t size) override;

	/**
	 * \brief Resets current file offset.
	 *
	 * Similar to [rewind()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewind.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the file is not opened;
	 * - converted error codes returned by lfs_file_rewind();
	 */

	int rewind() override;

	/**
	 * \brief Moves file offset.
	 *
	 * Similar to [lseek()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/lseek.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] whence selects the mode of operation: `Whence::beginning` will set file offset to \a offset,
	 * `Whence::current` will set file offset to its current value plus \a offset, `Whence::end` will set file offset to
	 * the size of the file plus \a offset
	 * \param [in] offset is the value of offset, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes; error codes:
	 * - EBADF - the file is not opened;
	 * - converted error codes returned by lfs_file_seek();
	 */

	std::pair<int, off_t> seek(Whence whence, off_t offset) override;

	/**
	 * \brief Synchronizes state of a file, ensuring all cached writes are finished.
	 *
	 * Similar to [fsync()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fsync.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the file is not opened;
	 * - converted error codes returned by lfs_file_sync();
	 */

	int synchronize() override;

	/**
	 * \brief Unlocks the file which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by LittlefsFileSystem::unlock();
	 */

	int unlock() override;

	/**
	 * \brief Writes data to file.
	 *
	 * Similar to [write()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] buffer is the buffer with data that will be written
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - the file is not opened or opened only for reading;
	 * - EINVAL - \a buffer is not valid;
	 * - converted error codes returned by lfs_file_write();
	 */

	std::pair<int, size_t> write(const void* buffer, size_t size) override;

private:

	/**
	 * \brief LittlefsFile's constructor
	 *
	 * \param [in] fileSystem is a reference to owner file system
	 */

	constexpr explicit LittlefsFile(LittlefsFileSystem& fileSystem) :
			buffer_{},
			configuration_{},
			file_{},
			fileSystem_{fileSystem},
			opened_{}
	{

	}

	/**
	 * \brief Opens file.
	 *
	 * \param [in] path is the path of file that will be opened
	 * \param [in] flags are file status flags, for list of available flags and valid combinations see
	 * [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a path and/or \a flags are not valid;
	 * - converted error codes returned by lfs_file_open();
	 */

	int open(const char* path, int flags);

	/// file buffer
	std::unique_ptr<uint8_t[]> buffer_;

	/// littlefs file configuration
	lfs_file_config configuration_;

	/// littlefs file
	lfs_file_t file_;

	/// reference to owner file system
	LittlefsFileSystem& fileSystem_;

	/// true if file is opened, false otherwise
	bool opened_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_FILESYSTEM_LITTLEFS_LITTLEFSFILE_HPP_
