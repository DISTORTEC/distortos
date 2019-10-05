/**
 * \file
 * \brief FatFile class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "FatFile.hpp"

#include "ufatErrorToErrorCode.hpp"

#include "distortos/devices/memory/BlockDevice.hpp"

#include "distortos/FileSystem/FatFileSystem.hpp"

#include "distortos/assert.h"

#include <mutex>

#include <fcntl.h>

#include <cstring>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

FatFile::~FatFile()
{
	assert(opened_ == false);
}

int FatFile::close()
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	opened_ = {};

	if (dirty_ == false)
		return {};

	const auto ret0 = ufat_sync(&fileSystem_.fileSystem_);
	const auto ret1 = fileSystem_.device_.blockDevice.synchronize();
	return ret0 < 0 ? ufatErrorToErrorCode(ret0) : ret1;
}

std::pair<int, off_t> FatFile::getPosition()
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	return {{}, position_};
}

std::pair<int, off_t> FatFile::getSize()
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	return {{}, file_.file_size};
}

int FatFile::getStatus(struct stat& status)
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	status = {};
	status.st_mode = S_IFREG;
	status.st_size = file_.file_size;
	return {};
}

std::pair<int, bool> FatFile::isATerminal()
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	return {{}, {}};
}

void FatFile::lock()
{
	fileSystem_.lock();
}

int FatFile::open(const char* const path, const int flags)
{
	assert(opened_ == false);
	assert(path != nullptr);

	ufat_directory directory {};
	ufat_dirent directoryEntry {};
	const char* pathRemainder {};
	bool exists {};

	{
		ufat_open_root(&fileSystem_.fileSystem_, &directory);
		const auto ret = ufat_dir_find_path(&directory, path, &directoryEntry, &pathRemainder);
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
		exists = ret == 0;
	}

	constexpr int mask {O_RDONLY | O_WRONLY | O_RDWR};
	const auto readOnly = (flags & mask) == O_RDONLY;
	const auto writeOnly = (flags & mask) == O_WRONLY;
	const auto readWrite = (flags & mask) == O_RDWR;
	assert(readOnly == true || writeOnly == true || readWrite == true);
	readable_ = readOnly == true || readWrite == true;
	writable_ = writeOnly == true || readWrite == true;

	const auto create = (flags & O_CREAT) != 0;
	const auto exclusive = (flags & O_EXCL) != 0;
	assert(exclusive == false || create == true);	// result is undefined if O_EXCL is set and O_CREAT is not set
	appendMode_ = (flags & O_APPEND) != 0;

	if (exists == true && exclusive == true)
		return EEXIST;
	if (exists == false && create == false)
		return ENOENT;

	if (exists == false && create == true)
	{
		if (strpbrk(pathRemainder, "/\\") != nullptr)
			return ENOENT;
		const auto ret = ufat_dir_mkfile(&directory, &directoryEntry, pathRemainder);
		dirty_ = true;
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
	}
	{
		const auto ret = ufat_open_file(&fileSystem_.fileSystem_, &file_, &directoryEntry);
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
	}
	if ((flags & O_TRUNC) != 0)
	{
		assert(writable_ == true);	// result is undefined if O_TRUNC is set and O_WRONLY or O_RDWR are not set
		const auto ret = ufat_file_truncate(&file_);
		dirty_ = true;
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
	}

	opened_ = true;
	return {};
}

std::pair<int, size_t> FatFile::read(void* const buffer, const size_t size)
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);
	assert(readable_ == true);
	assert(buffer != nullptr);

	const auto ret = ufat_file_read(&file_, buffer, size);
	position_ = file_.cur_pos;
	if (ret < 0)
		return {ufatErrorToErrorCode(ret), {}};

	return {{}, ret};
}

int FatFile::rewind()
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	ufat_file_rewind(&file_);
	position_ = file_.cur_pos;
	return {};
}

std::pair<int, off_t> FatFile::seek(const Whence whence, const off_t offset)
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	const auto currentPosition = position_;
	decltype(currentPosition) size = file_.file_size;
	const auto basePosition = whence == Whence::beginning ? decltype(currentPosition){} :
			whence == Whence::current ? currentPosition : size;
	if (offset < 0 && static_cast<std::decay<decltype(basePosition)>::type>(-offset) > basePosition)
		return {EINVAL, {}};
	decltype(basePosition) newPosition = basePosition + offset;
	if (newPosition == currentPosition)
		return {{}, currentPosition};

	if (newPosition < currentPosition)
	{
		ufat_file_rewind(&file_);
		position_ = file_.cur_pos;
	}

	const auto ret = ufat_file_advance(&file_, std::min(newPosition, size) - position_);
	position_ = file_.cur_pos;
	if (ret < 0)
		return {ufatErrorToErrorCode(ret), {}};

	position_ = newPosition;
	return {{}, position_};
}

int FatFile::synchronize()
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);

	if (dirty_ == false)
		return {};

	{
		const auto ret = ufat_sync(&fileSystem_.fileSystem_);
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
	}
	{
		const auto ret = fileSystem_.device_.blockDevice.synchronize();
		if (ret != 0)
			return ret;
	}

	dirty_ = {};
	return {};
}

void FatFile::unlock()
{
	fileSystem_.unlock();
}

std::pair<int, size_t> FatFile::write(const void* const buffer, const size_t size)
{
	const std::lock_guard<FatFile> lockGuard {*this};

	assert(opened_ == true);
	assert(writable_ == true);
	assert(buffer != nullptr);

	if (appendMode_ == true)
	{
		const auto ret = ufat_file_advance(&file_, file_.file_size - file_.cur_pos);
		position_ = file_.cur_pos;
		if (ret < 0)
			return {ufatErrorToErrorCode(ret), {}};
	}

	assert(static_cast<decltype(file_.cur_pos)>(position_) >= file_.cur_pos);
	const size_t fillSize = position_ - file_.cur_pos;
	for (size_t i {}; i < fillSize; ++i)
	{
		const uint8_t zero {};
		const auto ret = ufat_file_write(&file_, &zero, sizeof(zero));
		dirty_ = true;
		position_ = file_.cur_pos;
		if (ret < 0)
			return {ufatErrorToErrorCode(ret), {}};
		assert(ret == sizeof(zero));
	}

	const auto ret = ufat_file_write(&file_, buffer, size);
	dirty_ = true;
	position_ = file_.cur_pos;
	if (ret < 0)
		return {ufatErrorToErrorCode(ret), {}};

	return {{}, ret};
}

}	// namespace distortos
