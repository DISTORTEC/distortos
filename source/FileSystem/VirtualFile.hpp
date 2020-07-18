/**
 * \file
 * \brief VirtualFile class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_VIRTUALFILE_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_VIRTUALFILE_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/MountPointSharedPointer.hpp"

#include "distortos/FileSystem/File.hpp"

namespace distortos
{

namespace internal
{

/// VirtualFile class is a wrapper for File and shared pointer to its mount point
class VirtualFile : public File
{
public:

	/**
	 * \brief VirtualFile's constructor
	 *
	 * \param [in] file is a rvalue reference to unique_ptr to file
	 * \param [in] mountPointSharedPointer is a rvalue reference to shared pointer to mount point
	 */

	VirtualFile(std::unique_ptr<File>&& file, MountPointSharedPointer&& mountPointSharedPointer) :
			file_{std::move(file)},
			mountPointSharedPointer_{std::move(mountPointSharedPointer)}
	{

	}

	/**
	 * \brief VirtualFile's destructor
	 *
	 * \pre %File is closed.
	 */

	~VirtualFile() override;

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
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by File::close();
	 */

	int close() override;

	/**
	 * \brief Returns current file offset.
	 *
	 * Similar to [ftello()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/ftell.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and current file offset, bytes; error codes:
	 * - error codes returned by File::getPosition();
	 */

	std::pair<int, off_t> getPosition() override;

	/**
	 * \brief Returns size of file.
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and size of file, bytes; error codes:
	 * - error codes returned by File::getSize();
	 */

	std::pair<int, off_t> getSize() override;

	/**
	 * \brief Returns status of file.
	 *
	 * Similar to [fstat()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fstat.html)
	 *
	 * \pre %File is opened.
	 *
	 * \param [out] status is a reference to `stat` struct into which status of file will be written
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by File::getStatus();
	 */

	int getStatus(struct stat& status) override;

	/**
	 * \brief Tells whether the file is a terminal.
	 *
	 * Similar to [isatty()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/isatty.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return pair with return code (0 on success, error code otherwise) and bool telling whether the file is a
	 * terminal (true) or not (false); error codes:
	 * - error codes returned by File::isATerminal();
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
	 * - error codes returned by File::read();
	 */

	std::pair<int, size_t> read(void* buffer, size_t size) override;

	/**
	 * \brief Resets current file offset.
	 *
	 * Similar to [rewind()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewind.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by File::rewind();
	 */

	int rewind() override;

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
	 * - error codes returned by File::seek();
	 */

	std::pair<int, off_t> seek(Whence whence, off_t offset) override;

	/**
	 * \brief Synchronizes state of a file, ensuring all cached writes are finished.
	 *
	 * Similar to [fsync()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/fsync.html)
	 *
	 * \pre %File is opened.
	 *
	 * \return 0 on success, error code otherwise:
	 * - error codes returned by File::synchronize();
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
	 * \pre \a buffer is valid.
	 *
	 * \param [in] buffer is the buffer with data that will be written, must be valid
	 * \param [in] size is the size of \a buffer, bytes
	 *
	 * \return pair with return code (0 on success, error code otherwise) and number of written bytes (valid even when
	 * error code is returned); error codes:
	 * - error codes returned by File::write();
	 */

	std::pair<int, size_t> write(const void* buffer, size_t size) override;

private:

	/**
	 * \return true if file is opened, false otherwise
	 */

	bool isOpened() const
	{
		return file_ != nullptr && mountPointSharedPointer_ == true;
	}

	/// unique_ptr to file
	std::unique_ptr<File> file_;

	/// shared pointer to mount point
	MountPointSharedPointer mountPointSharedPointer_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_VIRTUALFILE_HPP_
