/**
 * \file
 * \brief FatFile class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_FILESYSTEM_FAT_FATFILE_HPP_
#define SOURCE_FILESYSTEM_FAT_FATFILE_HPP_

#include "distortos/FileSystem/File.hpp"

#include "ufat.h"

namespace distortos
{

class FatFileSystem;

/**
 * FatFile class is a [FAT](https://en.wikipedia.org/wiki/File_Allocation_Table) file.
 *
 * \ingroup fileSystem
 */

class FatFile : public File
{
public:

	/**
	 * \brief FatFile's constructor
	 *
	 * \param [in] fileSystem is a reference to owner file system
	 */

	constexpr explicit FatFile(FatFileSystem& fileSystem) :
			file_{},
			fileSystem_{fileSystem},
			appendMode_{},
			opened_{},
			readable_{},
			writable_{}
	{

	}

	/**
	 * \brief FatFile's destructor
	 *
	 * \pre %File is closed.
	 */

	~FatFile() override;

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
	 * \return 0 on success, error code otherwise
	 * - converted error codes returned by ufat_sync();
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
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes
	 */

	std::pair<int, off_t> getPosition() override;

	/**
	 * \brief Returns size of file.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and size of file, bytes
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
	 * \return 0 on success, error code otherwise
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
	 * \pre The number of recursive locks of device is less than 65535.
	 *
	 * \post Device is locked.
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
	 * - EEXIST - `O_CREAT` and `O_EXCL` are set, and file named by \a path exists;
	 * - ENOENT - `O_CREAT` is not set and \a path does not name an existing file or `O_CREAT` is set and prefix
	 * component of \a path does not name an existing directory;
	 * - converted error codes returned by ufat_dir_find_path();
	 * - converted error codes returned by ufat_dir_mkfile();
	 * - converted error codes returned by ufat_file_truncate();
	 * - converted error codes returned by ufat_open_file();
	 */

	int open(const char* path, int flags);

	/**
	 * \brief Reads data from file.
	 *
	 * Similar to [read()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 * \pre \a buffer is valid.
	 *
	 * \param [out] buffer is the buffer into which the data will be read, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of read bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - file is not opened for reading;
	 * - converted error codes returned by ufat_file_read();
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
	 * \return 0 on success, error code otherwise
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
	 * - EBADF - resulting file offset would be past the end of file and file is not opened for writing;
	 * - EINVAL - resulting file offset would be negative;
	 * - converted error codes returned by ufat_file_advance();
	 * - converted error codes returned by ufat_file_write();
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
	 * - converted error codes returned by ufat_sync();
	 */

	int synchronize() override;

	/**
	 * \brief Unlocks the file which was previously locked by current thread.
	 *
	 * \note Locks are recursive.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre This function is called by the thread that locked the device.
	 */

	void unlock() override;

	/**
	 * \brief Writes data to file.
	 *
	 * Similar to [write()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html)
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre %File is opened.
	 * \pre \a buffer is valid.
	 *
	 * \param [in] buffer is the buffer with data that will be written, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - EBADF - file is not opened for writing;
	 * - converted error codes returned by ufat_file_advance();
	 * - converted error codes returned by ufat_file_write();
	 */

	std::pair<int, size_t> write(const void* buffer, size_t size) override;

private:

	/// uFAT file
	ufat_file file_;

	/// reference to owner file system
	FatFileSystem& fileSystem_;

	/// true if file is opened in append mode, false otherwise
	bool appendMode_;

	/// true if file is opened, false otherwise
	bool opened_;

	/// true if file is opened for reading, false otherwise
	bool readable_;

	/// true if file is opened for writing, false otherwise
	bool writable_;
};

}	// namespace distortos

#endif	// SOURCE_FILESYSTEM_FAT_FATFILE_HPP_
