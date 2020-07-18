/**
 * \file
 * \brief Littlefs2FileSystem class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/FileSystem/Littlefs2FileSystem.hpp"

#include "Littlefs2Directory.hpp"
#include "littlefs2ErrorToErrorCode.hpp"
#include "Littlefs2File.hpp"

#include "distortos/devices/memory/MemoryTechnologyDevice.hpp"

#include "distortos/assert.h"

#include "estd/ScopeGuard.hpp"

#include <mutex>

namespace distortos
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Converts error code to littlefs-v2 error.
 *
 * \param [in] errorCode is an error code
 *
 * \return \a errorCode converted to littlefs-v2 error
 */

int errorCodeToLittlefs2Error(const int errorCode)
{
	if (errorCode == 0)
		return LFS2_ERR_OK;
	if (errorCode == EIO)
		return LFS2_ERR_IO;
	if (errorCode == EILSEQ)
		return LFS2_ERR_CORRUPT;
	if (errorCode == ENOENT)
		return LFS2_ERR_NOENT;
	if (errorCode == EEXIST)
		return LFS2_ERR_EXIST;
	if (errorCode == ENOTDIR)
		return LFS2_ERR_NOTDIR;
	if (errorCode == EISDIR)
		return LFS2_ERR_ISDIR;
	if (errorCode == ENOTEMPTY)
		return LFS2_ERR_NOTEMPTY;
	if (errorCode == EBADF)
		return LFS2_ERR_BADF;
	if (errorCode == EFBIG)
		return LFS2_ERR_FBIG;
	if (errorCode == EINVAL)
		return LFS2_ERR_INVAL;
	if (errorCode == ENOSPC)
		return LFS2_ERR_NOSPC;
	if (errorCode == ENOMEM)
		return LFS2_ERR_NOMEM;
	if (errorCode == ENOENT)
		return LFS2_ERR_NOATTR;
	if (errorCode == ENAMETOOLONG)
		return LFS2_ERR_NAMETOOLONG;

	return -errorCode;
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::erase()
 *
 * \param [in] configuration is a pointer to littlefs-v2 configuration
 * \param [in] block is the index of block that will be erased
 *
 * \return LFS2_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::erase();
 */

int littlefs2MemoryTechnologyDeviceErase(const lfs2_config* const configuration, const lfs2_block_t block)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->erase(static_cast<uint64_t>(block) * configuration->block_size,
			configuration->block_size);
	return errorCodeToLittlefs2Error(ret);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::program()
 *
 * \param [in] configuration is a pointer to littlefs-v2 configuration
 * \param [in] block is the index of block that will be programmed
 * \param [in] offset is the offset of area that will be programmed, bytes
 * \param [in] buffer is the buffer with data that will be programmed
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS2_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::program();
 */

int littlefs2MemoryTechnologyDeviceProgram(const lfs2_config* const configuration, const lfs2_block_t block,
		const lfs2_off_t offset, const void* const buffer, const lfs2_size_t size)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->program(static_cast<uint64_t>(block) * configuration->block_size + offset,
			buffer, size);
	return errorCodeToLittlefs2Error(ret);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::read()
 *
 * \param [in] configuration is a pointer to littlefs-v2 configuration
 * \param [in] block is the index of block that will be read
 * \param [in] offset is the offset of area that will be read, bytes
 * \param [out] buffer is the buffer into which the data will be read
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS2_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::read();
 */

int littlefs2MemoryTechnologyDeviceRead(const lfs2_config* const configuration, const lfs2_block_t block,
		const lfs2_off_t offset, void* const buffer, const lfs2_size_t size)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->read(static_cast<uint64_t>(block) * configuration->block_size + offset,
			buffer, size);
	return errorCodeToLittlefs2Error(ret);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::synchronize()
 *
 * \param [in] configuration is a pointer to littlefs-v2 configuration
 *
 * \return LFS2_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::synchronize();
 */

int littlefs2MemoryTechnologyDeviceSynchronize(const lfs2_config* const configuration)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->synchronize();
	return errorCodeToLittlefs2Error(ret);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Littlefs2FileSystem::~Littlefs2FileSystem()
{
	assert(mounted_ == false);
}

int Littlefs2FileSystem::format()
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == false);

	{
		const auto ret = memoryTechnologyDevice_.open();
		if (ret != 0)
			return ret;
	}

	const auto closeScopeGuard = estd::makeScopeGuard([this]()
			{
				memoryTechnologyDevice_.close();
			});

	configuration_ = {};
	fileSystem_ = {};
	configuration_.context = &memoryTechnologyDevice_;
	configuration_.read = littlefs2MemoryTechnologyDeviceRead;
	configuration_.prog = littlefs2MemoryTechnologyDeviceProgram;
	configuration_.erase = littlefs2MemoryTechnologyDeviceErase;
	configuration_.sync = littlefs2MemoryTechnologyDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : memoryTechnologyDevice_.getReadBlockSize();
	configuration_.prog_size =
			programBlockSize_ != 0 ? programBlockSize_ : memoryTechnologyDevice_.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : memoryTechnologyDevice_.getEraseBlockSize();
	configuration_.block_count =
			blocksCount_ != 0 ? blocksCount_ : (memoryTechnologyDevice_.getSize() / configuration_.block_size);
	configuration_.block_cycles = blockCycles_;
	configuration_.cache_size =
			cacheSize_ != 0 ? cacheSize_ : std::max(configuration_.read_size, configuration_.prog_size);
	configuration_.lookahead_size = (std::max(lookaheadSize_, 1u) + 63) / 64 * 64;
	configuration_.name_max = filenameLengthLimit_;
	configuration_.file_max = fileSizeLimit_;
	configuration_.attr_max = attributeSizeLimit_;

	const auto ret = lfs2_format(&fileSystem_, &configuration_);
	return littlefs2ErrorToErrorCode(ret);
}

int Littlefs2FileSystem::getFileStatus(const char* const path, struct stat& status)
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	lfs2_info info;
	const auto ret = lfs2_stat(&fileSystem_, path, &info);
	if (ret != LFS2_ERR_OK)
		littlefs2ErrorToErrorCode(ret);

	status = {};
	status.st_mode = info.type == LFS2_TYPE_DIR ? S_IFDIR : S_IFREG;
	if (info.type == LFS2_TYPE_REG)
		status.st_size = info.size;
	return {};
}

int Littlefs2FileSystem::getStatus(struct statvfs& status)
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	const auto ret = lfs2_fs_size(&fileSystem_);
	if (ret < 0)
		return littlefs2ErrorToErrorCode(ret);

	status = {};
	status.f_bsize = configuration_.block_size;
	status.f_frsize = status.f_bsize;
	status.f_blocks = configuration_.block_count;
	status.f_bfree = status.f_blocks - ret;
	status.f_bavail = status.f_bfree;
	status.f_namemax = configuration_.name_max != 0 ? configuration_.name_max : LFS2_NAME_MAX;
	return {};
}

void Littlefs2FileSystem::lock()
{
	const auto ret = mutex_.lock();
	assert(ret == 0);
}

int Littlefs2FileSystem::makeDirectory(const char* const path, mode_t)
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	const auto ret = lfs2_mkdir(&fileSystem_, path);
	return littlefs2ErrorToErrorCode(ret);
}

int Littlefs2FileSystem::mount()
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == false);

	{
		const auto ret = memoryTechnologyDevice_.open();
		if (ret != 0)
			return ret;
	}

	auto closeScopeGuard = estd::makeScopeGuard([this]()
			{
				memoryTechnologyDevice_.close();
			});

	configuration_ = {};
	fileSystem_ = {};
	configuration_.context = &memoryTechnologyDevice_;
	configuration_.read = littlefs2MemoryTechnologyDeviceRead;
	configuration_.prog = littlefs2MemoryTechnologyDeviceProgram;
	configuration_.erase = littlefs2MemoryTechnologyDeviceErase;
	configuration_.sync = littlefs2MemoryTechnologyDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : memoryTechnologyDevice_.getReadBlockSize();
	configuration_.prog_size =
			programBlockSize_ != 0 ? programBlockSize_ : memoryTechnologyDevice_.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : memoryTechnologyDevice_.getEraseBlockSize();
	configuration_.block_count =
			blocksCount_ != 0 ? blocksCount_ : (memoryTechnologyDevice_.getSize() / configuration_.block_size);
	configuration_.block_cycles = blockCycles_;
	configuration_.cache_size =
			cacheSize_ != 0 ? cacheSize_ : std::max(configuration_.read_size, configuration_.prog_size);
	configuration_.lookahead_size = (std::max(lookaheadSize_, 1u) + 63) / 64 * 64;
	configuration_.name_max = filenameLengthLimit_;
	configuration_.file_max = fileSizeLimit_;
	configuration_.attr_max = attributeSizeLimit_;

	const auto ret = lfs2_mount(&fileSystem_, &configuration_);
	if (ret != LFS2_ERR_OK)
		return littlefs2ErrorToErrorCode(ret);

	mounted_ = true;
	closeScopeGuard.release();
	return 0;
}

std::pair<int, std::unique_ptr<Directory>> Littlefs2FileSystem::openDirectory(const char* const path)
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	std::unique_ptr<Littlefs2Directory> directory {new (std::nothrow) Littlefs2Directory{*this}};
	if (directory == nullptr)
		return {ENOMEM, std::unique_ptr<Littlefs2Directory>{}};

	const auto ret = directory->open(path);
	if (ret != 0)
		return {ret, std::unique_ptr<Littlefs2Directory>{}};

	return {{}, std::move(directory)};
}

std::pair<int, std::unique_ptr<File>> Littlefs2FileSystem::openFile(const char* const path, const int flags)
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	std::unique_ptr<Littlefs2File> file {new (std::nothrow) Littlefs2File{*this}};
	if (file == nullptr)
		return {ENOMEM, std::unique_ptr<Littlefs2File>{}};

	const auto ret = file->open(path, flags);
	if (ret != 0)
		return {ret, std::unique_ptr<Littlefs2File>{}};

	return {{}, std::move(file)};
}

int Littlefs2FileSystem::remove(const char* const path)
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	const auto ret = lfs2_remove(&fileSystem_, path);
	return littlefs2ErrorToErrorCode(ret);
}

int Littlefs2FileSystem::rename(const char* const path, const char* const newPath)
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);
	assert(newPath != nullptr);

	const auto ret = lfs2_rename(&fileSystem_, path, newPath);
	return littlefs2ErrorToErrorCode(ret);
}

void Littlefs2FileSystem::unlock()
{
	const auto ret = mutex_.unlock();
	assert(ret == 0);
}

int Littlefs2FileSystem::unmount()
{
	const std::lock_guard<Littlefs2FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	const auto unmountRet = lfs2_unmount(&fileSystem_);
	const auto closeRet = memoryTechnologyDevice_.close();
	mounted_ = {};

	return unmountRet != LFS2_ERR_OK ? littlefs2ErrorToErrorCode(unmountRet) : closeRet;
}

}	// namespace distortos
