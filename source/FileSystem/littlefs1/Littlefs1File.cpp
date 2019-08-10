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

#include "distortos/FileSystem/littlefs1/Littlefs1FileSystem.hpp"

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

std::pair<int, size_t> Littlefs1File::read(void* const buffer, const size_t size)
{
	const std::lock_guard<Littlefs1File> lockGuard {*this};

	assert(opened_ == true);
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
	assert(buffer != nullptr);

	const auto ret = lfs1_file_write(&fileSystem_.fileSystem_, &file_, buffer, size);
	if (ret < 0)
		return {littlefs1ErrorToErrorCode(ret), {}};

	return {{}, ret};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int Littlefs1File::open(const char* const path, const int flags)
{
	assert(opened_ == false);
	assert(path != nullptr);

	int convertedFlags;
	{
		constexpr int mask {O_RDONLY | O_WRONLY | O_RDWR};
		const auto readOnly = (flags & mask) == O_RDONLY;
		const auto writeOnly = (flags & mask) == O_WRONLY;
		const auto readWrite = (flags & mask) == O_RDWR;
		assert(readOnly == true || writeOnly == true || readWrite == true);
		if (readOnly == true)
			convertedFlags = LFS1_O_RDONLY;
		else if (writeOnly == true)
			convertedFlags = LFS1_O_WRONLY;
		else	// if (readWrite == true)
			convertedFlags = LFS1_O_RDWR;
	}

	if ((flags & O_CREAT) != 0)
		convertedFlags |= LFS1_O_CREAT;
	if ((flags & O_EXCL) != 0)
		convertedFlags |= LFS1_O_EXCL;
	if ((flags & O_TRUNC) != 0)
		convertedFlags |= LFS1_O_TRUNC;
	if ((flags & O_APPEND) != 0)
		convertedFlags |= LFS1_O_APPEND;

	constexpr size_t alignment {DISTORTOS_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENT};
	constexpr size_t alignmentMargin {alignment > __BIGGEST_ALIGNMENT__ ? alignment - __BIGGEST_ALIGNMENT__ : 0};
	const size_t bufferSize {fileSystem_.fileSystem_.cfg->prog_size + alignmentMargin};
	std::unique_ptr<uint8_t[]> buffer {new (std::nothrow) uint8_t[bufferSize]};
	if (buffer.get() == nullptr)
		return ENOMEM;

	// configuration_ is zeroed neither before use nor after failure - it is assumed that open() can be used only once
	// and only on a newly constructed object

	configuration_.buffer = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(buffer.get()) + alignment - 1) /
			alignment * alignment);

	const auto ret = lfs1_file_opencfg(&fileSystem_.fileSystem_, &file_, path, convertedFlags, &configuration_);
	if (ret != LFS1_ERR_OK)
		return littlefs1ErrorToErrorCode(ret);

	buffer_ = std::move(buffer);
	opened_ = true;
	return {};
}

}	// namespace distortos
