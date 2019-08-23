/**
 * \file
 * \brief Littlefs1File class implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Littlefs1File.hpp"

#include "littlefs1ErrorToErrorCode.hpp"

#include "distortos/FileSystem/Littlefs1FileSystem.hpp"

#include "distortos/assert.h"

#include <mutex>

#include <fcntl.h>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Littlefs1File::~Littlefs1File()
{
	assert(opened_ == false);
}

int Littlefs1File::close()
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);

	opened_ = {};
	const auto ret = lfs1_file_close(&fileSystem_.fileSystem_, &file_);
	return littlefs1ErrorToErrorCode(ret);
}

std::pair<int, off_t> Littlefs1File::getPosition()
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs1_file_tell(&fileSystem_.fileSystem_, &file_);
	if (ret < 0)
		return {littlefs1ErrorToErrorCode(ret), {}};

	return {{}, ret};
}

std::pair<int, off_t> Littlefs1File::getSize()
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs1_file_size(&fileSystem_.fileSystem_, &file_);
	if (ret < 0)
		return {littlefs1ErrorToErrorCode(ret), {}};

	return {{}, ret};
}

int Littlefs1File::getStatus(struct stat& status)
{
	const auto ret = getSize();
	if (ret.first != 0)
		return ret.first;

	status = {};
	status.st_mode = S_IFREG;
	status.st_size = ret.second;
	return {};
}

std::pair<int, bool> Littlefs1File::isATerminal()
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);

	return {{}, {}};
}

void Littlefs1File::lock()
{
	fileSystem_.lock();
}

int Littlefs1File::open(const char* const path, const int flags)
{
	assert(opened_ == false);
	assert(path != nullptr);

	constexpr int mask {O_RDONLY | O_WRONLY | O_RDWR};
	const auto readOnly = (flags & mask) == O_RDONLY;
	const auto writeOnly = (flags & mask) == O_WRONLY;
	const auto readWrite = (flags & mask) == O_RDWR;
	assert(readOnly == true || writeOnly == true || readWrite == true);
	readable_ = readOnly == true || readWrite == true;
	writable_ = writeOnly == true || readWrite == true;
	int convertedFlags {readOnly == true ? LFS1_O_RDONLY : writeOnly == true ? LFS1_O_WRONLY : LFS1_O_RDWR};

	if ((flags & O_CREAT) != 0)
		convertedFlags |= LFS1_O_CREAT;
	if ((flags & O_EXCL) != 0)
	{
		assert((flags & O_CREAT) != 0);	// result is undefined if O_EXCL is set and O_CREAT is not set
		convertedFlags |= LFS1_O_EXCL;
	}
	if ((flags & O_TRUNC) != 0)
	{
		assert(writable_ == true);	// result is undefined if O_TRUNC is set and O_WRONLY or O_RDWR are not set
		convertedFlags |= LFS1_O_TRUNC;
	}
	if ((flags & O_APPEND) != 0)
		convertedFlags |= LFS1_O_APPEND;

	const auto ret = lfs1_file_open(&fileSystem_.fileSystem_, &file_, path, convertedFlags);
	if (ret != LFS1_ERR_OK)
		return littlefs1ErrorToErrorCode(ret);

	opened_ = true;
	return {};
}

std::pair<int, size_t> Littlefs1File::read(void* const buffer, const size_t size)
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);
	assert(readable_ == true);
	assert(buffer != nullptr);

	const auto ret = lfs1_file_read(&fileSystem_.fileSystem_, &file_, buffer, size);
	if (ret < 0)
		return {littlefs1ErrorToErrorCode(ret), {}};

	return {{}, ret};
}

int Littlefs1File::rewind()
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs1_file_rewind(&fileSystem_.fileSystem_, &file_);
	return littlefs1ErrorToErrorCode(ret);
}

std::pair<int, off_t> Littlefs1File::seek(const Whence whence, const off_t offset)
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs1_file_seek(&fileSystem_.fileSystem_, &file_, offset,
			whence == Whence::beginning ? LFS1_SEEK_SET : whence == Whence::current ? LFS1_SEEK_CUR : LFS1_SEEK_END);
	if (ret < 0)
		return {littlefs1ErrorToErrorCode(ret), {}};

	return {{}, ret};

}

int Littlefs1File::synchronize()
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs1_file_sync(&fileSystem_.fileSystem_, &file_);
	return littlefs1ErrorToErrorCode(ret);
}

void Littlefs1File::unlock()
{
	fileSystem_.unlock();
}

std::pair<int, size_t> Littlefs1File::write(const void* const buffer, const size_t size)
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);
	assert(writable_ == true);
	assert(buffer != nullptr);

	const auto ret = lfs1_file_write(&fileSystem_.fileSystem_, &file_, buffer, size);
	if (ret < 0)
		return {littlefs1ErrorToErrorCode(ret), {}};

	return {{}, ret};
}

}	// namespace distortos
