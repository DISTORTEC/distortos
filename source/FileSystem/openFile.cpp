/**
 * \file
 * \brief openFile() definition
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define _GNU_SOURCE	// for fopencookie()

#include "distortos/FileSystem/openFile.hpp"

#include "distortos/FileSystem/FileSystem.hpp"

#include "distortos/assert.h"

#include "estd/ScopeGuard.hpp"

#include <cerrno>

extern "C"
{

// newlib's local function which converts mode to flags
int __sflags(_reent* reent, const char* mode, int* flags);

}	// extern "C"

namespace distortos
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for File::close() which can be used with fopencookie()
 *
 * \param [in] cookie is a cookie which was passed to fopencookie(), must be File!
 *
 * \return 0 on success, -1 otherwise; writes error code to errno:
 * - error codes returned by File::close();
 */

int fileClose(void* const cookie)
{
	const auto file = static_cast<File*>(cookie);
	assert(file != nullptr);

	const auto ret = file->close();
	delete file;

	if (ret != 0)
	{
		errno = ret;
		return -1;
	}

	return {};
}

/**
 * \brief Wrapper for File::read() which can be used with fopencookie()
 *
 * \param [in] cookie is a cookie which was passed to fopencookie(), must be File!
 * \param [out] buffer is the buffer into which the data will be read
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return 0 on success, -1 otherwise; writes error code to errno:
 * - error codes returned by File::read();
 */

ssize_t fileRead(void* const cookie, char* const buffer, const size_t size)
{
	const auto file = static_cast<File*>(cookie);
	assert(file != nullptr);

	const auto ret = file->read(buffer, size);
	if (ret.first != 0)
	{
		errno = ret.first;
		return -1;
	}

	return ret.second;
}

/**
 * \brief Wrapper for File::seek() which can be used with fopencookie()
 *
 * \param [in] cookie is a cookie which was passed to fopencookie(), must be File!
 * \param [in,out] offset is a pointer the value of offset, bytes
 * \param [in] whence selects the mode of operation: `SEEK_SET` will set file offset to \a offset, `SEEK_CUR` will set
 * file offset to its current value plus \a offset, `SEEK_END` will set file offset to the size of the file plus
 * \a offset
 *
 * \return 0 on success, -1 otherwise; writes error code to errno:
 * - EINVAL - \a whence is not valid;
 * - error codes returned by File::seek();
 */

int fileSeek(void* const cookie, off_t* const offset, const int whence)
{
	const auto file = static_cast<File*>(cookie);
	assert(file != nullptr);

	if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END)
	{
		errno = EINVAL;
		return -1;
	}

	const auto ret = file->seek(whence == SEEK_SET ? File::Whence::beginning :
			whence == SEEK_CUR ? File::Whence::current : File::Whence::end, *offset);
	if (ret.first != 0)
	{
		errno = ret.first;
		return -1;
	}

	*offset = ret.second;
	return ret.second;
}

/**
 * \brief Wrapper for File::write() which can be used with fopencookie()
 *
 * \param [in] cookie is a cookie which was passed to fopencookie(), must be File!
 * \param [in] buffer is the buffer with data that will be written
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return 0 on success, -1 otherwise; writes error code to errno:
 * - error codes returned by File::write();
 */

ssize_t fileWrite(void* const cookie, const char* const buffer, const size_t size)
{
	const auto file = static_cast<File*>(cookie);
	assert(file != nullptr);

	const auto ret = file->write(buffer, size);
	if (ret.first != 0)
	{
		errno = ret.first;
		return -1;
	}

	return ret.second;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, FILE*> openFile(FileSystem& fileSystem, const char* const path, const char* const mode)
{
	assert(mode != nullptr);

	int flags;
	{
		const auto ret = __sflags(_REENT, mode, &flags);
		assert(ret != 0);
	}

	std::unique_ptr<File> file;
	{
		int ret;
		std::tie(ret, file) = fileSystem.openFile(path, flags);
		if (ret != 0)
			return {ret, {}};
	}

	assert(file != nullptr);

	auto closeScopeGuard = estd::makeScopeGuard([&file]()
			{
				file->close();
			});

	const auto ret = fopencookie(file.get(), mode, {fileRead, fileWrite, fileSeek, fileClose});
	if (ret == nullptr)
		return {errno, {}};

	closeScopeGuard.release();
	file.release();
	return {{}, ret};
}

}	// namespace distortos
