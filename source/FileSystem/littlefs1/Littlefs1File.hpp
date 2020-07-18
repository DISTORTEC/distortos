/**
 * \file
 * \brief Littlefs1File class header
 *
 * \author Copyright (C) 2018-2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFS1FILE_HPP_
#define SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFS1FILE_HPP_

#include "distortos/FileSystem/File.hpp"

#include "lfs1.h"

namespace distortos
{

class Littlefs1FileSystem;

/**
 * \brief Littlefs1File class is a [littlefs-v1](https://github.com/ARMmbed/littlefs) file.
 *
 * \ingroup fileSystem
 */

class Littlefs1File : public File
{
public:

	/**
	 * \brief Littlefs1File's constructor
	 *
	 * \param [in] fileSystem is a reference to owner file system
	 */

	constexpr explicit Littlefs1File(Littlefs1FileSystem& fileSystem) :
			file_{},
			fileSystem_{fileSystem},
			opened_{},
			readable_{},
			writable_{}
	{

	}

	/**
	 * \brief Littlefs1File's destructor
	 *
	 * \pre %File is closed.
	 */

	~Littlefs1File() override;

	/**
	 * \brief Closes file.
	 *
	 * Similar to [close()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/close.html)
	 *
	 * \note Even if error code is returned, the file must not be used.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \post %File is closed.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_file_close();
	 */

	int close() override;

	/**
	 * \brief Returns current file offset.
	 *
	 * Similar to [ftello()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/ftell.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes; error codes:
	 * - converted error codes returned by lfs1_file_tell();
	 */

	std::pair<int, off_t> getPosition() override;

	/**
	 * \brief Returns size of file.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and size of file, bytes; error codes:
	 * - converted error codes returned by lfs1_file_size();
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
	 * \pre %File is opened.
	 *
	 * \param [out] status is a reference to `stat` struct into which status of file will be written
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by getSize();
	 */

	int getStatus(struct stat& status) override;

	/**
	 * \brief Tells whether the file is a terminal.
	 *
	 * Similar to [isatty()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/isatty.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and bool telling whether the file is a
	 * terminal (true) or not (false)
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
	 * \pre The number of recursive locks of file is less than 65535.
	 *
	 * \post %File is locked.
	 */

	void lock() override;

	/**
	 * \brief Opens file.
	 *
	 * \pre %File is not opened.
	 * \pre \a path is valid.
	 * \pre \a flags are valid.
	 *
	 * \param [in] path is the path of file that will be opened, must be valid
	 * \param [in] flags are file status flags, must be valid, for list of available flags and valid combinations see
	 * [open()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html)
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_file_open();
	 */

	int open(const char* path, int flags);

	/**
	 * \brief Reads data from file.
	 *
	 * Similar to [read()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a buffer is valid.
	 *
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - file is not opened for reading;
	 * - converted error codes returned by lfs1_file_read();
	 */

	std::pair<int, size_t> read(void* buffer, size_t size) override;

	/**
	 * \brief Resets current file offset.
	 *
	 * Similar to [rewind()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewind.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_file_rewind();
	 */

	int rewind() override;

	/**
	 * \brief Moves file offset.
	 *
	 * Similar to [lseek()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/lseek.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \param [in] whence selects the mode of operation: `Whence::beginning` will set file offset to \a offset,
	 * `Whence::current` will set file offset to its current value plus \a offset, `Whence::end` will set file offset to
	 * the size of the file plus \a offset
	 * \param [in] offset is the value of offset, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes; error codes:
	 * - converted error codes returned by lfs1_file_seek();
	 */

	std::pair<int, off_t> seek(Whence whence, off_t offset) override;

	/**
	 * \brief Synchronizes state of a file, ensuring all cached writes are finished.
	 *
	 * Similar to [fsync()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fsync.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - converted error codes returned by lfs1_file_sync();
	 */

	int synchronize() override;

	/**
	 * \brief Unlocks the file which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the file.
	 */

	void unlock() override;

	/**
	 * \brief Writes data to file.
	 *
	 * Similar to [write()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre \a buffer is valid.
	 *
	 * \param [in] buffer is the buffer with data that will be written, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - file is not opened for writing;
	 * - converted error codes returned by lfs1_file_write();
	 */

	std::pair<int, size_t> write(const void* buffer, size_t size) override;

private:

	/// littlefs-v1 file
	lfs1_file_t file_;

	/// reference to owner file system
	Littlefs1FileSystem& fileSystem_;

	/// true if file is opened, false otherwise
	bool opened_;

	/// true if file is opened for reading, false otherwise
	bool readable_;

	/// true if file is opened for writing, false otherwise
	bool writable_;
};

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_LITTLEFS1_LITTLEFS1FILE_HPP_
