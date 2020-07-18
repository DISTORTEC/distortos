/**
 * \file
 * \brief Littlefs1FileSystem class implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/FileSystem/Littlefs1FileSystem.hpp"

#include "Littlefs1Directory.hpp"
#include "littlefs1ErrorToErrorCode.hpp"
#include "Littlefs1File.hpp"

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
 * \brief Callback for lfs1_traverse() that counts block used by file system.
 *
 * Idea from https://github.com/ARMmbed/littlefs1/issues/1#issuecomment-355116984
 *
 * \param [out] data is a pointer to size_t variable with number of used blocks
 *
 * \return always 0
 */

int countUsedBlocks(void* const data, lfs1_block_t)
{
	assert(data != nullptr);
	auto& usedBlocks = *static_cast<size_t*>(data);
	++usedBlocks;
	return {};
}

/**
 * \brief Converts error code to littlefs-v1 error.
 *
 * \param [in] errorCode is an error code
 *
 * \return \a errorCode converted to littlefs-v1 error
 */

int errorCodeToLittlefs1Error(const int errorCode)
{
	if (errorCode == 0)
		return LFS1_ERR_OK;
	if (errorCode == EIO)
		return LFS1_ERR_IO;
	if (errorCode == EILSEQ)
		return LFS1_ERR_CORRUPT;
	if (errorCode == ENOENT)
		return LFS1_ERR_NOENT;
	if (errorCode == EEXIST)
		return LFS1_ERR_EXIST;
	if (errorCode == ENOTDIR)
		return LFS1_ERR_NOTDIR;
	if (errorCode == EISDIR)
		return LFS1_ERR_ISDIR;
	if (errorCode == ENOTEMPTY)
		return LFS1_ERR_NOTEMPTY;
	if (errorCode == EBADF)
		return LFS1_ERR_BADF;
	if (errorCode == EINVAL)
		return LFS1_ERR_INVAL;
	if (errorCode == ENOSPC)
		return LFS1_ERR_NOSPC;
	if (errorCode == ENOMEM)
		return LFS1_ERR_NOMEM;

	return -errorCode;
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::erase()
 *
 * \param [in] configuration is a pointer to littlefs-v1 configuration
 * \param [in] block is the index of block that will be erased
 *
 * \return LFS1_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::erase();
 */

int littlefs1MemoryTechnologyDeviceErase(const lfs1_config* const configuration, const lfs1_block_t block)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->erase(static_cast<uint64_t>(block) * configuration->block_size,
			configuration->block_size);
	return errorCodeToLittlefs1Error(ret);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::program()
 *
 * \param [in] configuration is a pointer to littlefs-v1 configuration
 * \param [in] block is the index of block that will be programmed
 * \param [in] offset is the offset of area that will be programmed, bytes
 * \param [in] buffer is the buffer with data that will be programmed
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS1_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::program();
 */

int littlefs1MemoryTechnologyDeviceProgram(const lfs1_config* const configuration, const lfs1_block_t block,
		const lfs1_off_t offset, const void* const buffer, const lfs1_size_t size)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->program(static_cast<uint64_t>(block) * configuration->block_size + offset,
			buffer, size);
	return errorCodeToLittlefs1Error(ret);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::read()
 *
 * \param [in] configuration is a pointer to littlefs-v1 configuration
 * \param [in] block is the index of block that will be read
 * \param [in] offset is the offset of area that will be read, bytes
 * \param [out] buffer is the buffer into which the data will be read
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS1_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::read();
 */

int littlefs1MemoryTechnologyDeviceRead(const lfs1_config* const configuration, const lfs1_block_t block,
		const lfs1_off_t offset, void* const buffer, const lfs1_size_t size)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->read(static_cast<uint64_t>(block) * configuration->block_size + offset,
			buffer, size);
	return errorCodeToLittlefs1Error(ret);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::synchronize()
 *
 * \param [in] configuration is a pointer to littlefs-v1 configuration
 *
 * \return LFS1_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::synchronize();
 */

int littlefs1MemoryTechnologyDeviceSynchronize(const lfs1_config* const configuration)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->synchronize();
	return errorCodeToLittlefs1Error(ret);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Littlefs1FileSystem::~Littlefs1FileSystem()
{
	assert(mounted_ == false);
}

int Littlefs1FileSystem::format()
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

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
	configuration_.read = littlefs1MemoryTechnologyDeviceRead;
	configuration_.prog = littlefs1MemoryTechnologyDeviceProgram;
	configuration_.erase = littlefs1MemoryTechnologyDeviceErase;
	configuration_.sync = littlefs1MemoryTechnologyDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : memoryTechnologyDevice_.getReadBlockSize();
	configuration_.prog_size =
			programBlockSize_ != 0 ? programBlockSize_ : memoryTechnologyDevice_.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : memoryTechnologyDevice_.getEraseBlockSize();
	configuration_.block_count =
			blocksCount_ != 0 ? blocksCount_ : (memoryTechnologyDevice_.getSize() / configuration_.block_size);
	configuration_.lookahead = (std::max(lookahead_, 1u) + 31) / 32 * 32;

	const auto ret = lfs1_format(&fileSystem_, &configuration_);
	return littlefs1ErrorToErrorCode(ret);
}

int Littlefs1FileSystem::getFileStatus(const char* const path, struct stat& status)
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	lfs1_info info;
	const auto ret = lfs1_stat(&fileSystem_, path, &info);
	if (ret != LFS1_ERR_OK)
		littlefs1ErrorToErrorCode(ret);

	status = {};
	status.st_mode = info.type == LFS1_TYPE_DIR ? S_IFDIR : S_IFREG;
	if (info.type == LFS1_TYPE_REG)
		status.st_size = info.size;
	return {};
}

int Littlefs1FileSystem::getStatus(struct statvfs& status)
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	size_t usedBlocks {};
	const auto ret = lfs1_traverse(&fileSystem_, countUsedBlocks, &usedBlocks);
	if (ret != LFS1_ERR_OK)
		return littlefs1ErrorToErrorCode(ret);

	status = {};
	status.f_bsize = configuration_.block_size;
	status.f_frsize = status.f_bsize;
	status.f_blocks = configuration_.block_count;
	status.f_bfree = status.f_blocks - usedBlocks;
	status.f_bavail = status.f_bfree;
	status.f_namemax = LFS1_NAME_MAX;
	return {};
}

void Littlefs1FileSystem::lock()
{
	const auto ret = mutex_.lock();
	assert(ret == 0);
}

int Littlefs1FileSystem::makeDirectory(const char* const path, mode_t)
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	const auto ret = lfs1_mkdir(&fileSystem_, path);
	return littlefs1ErrorToErrorCode(ret);
}

int Littlefs1FileSystem::mount()
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

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
	configuration_.read = littlefs1MemoryTechnologyDeviceRead;
	configuration_.prog = littlefs1MemoryTechnologyDeviceProgram;
	configuration_.erase = littlefs1MemoryTechnologyDeviceErase;
	configuration_.sync = littlefs1MemoryTechnologyDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : memoryTechnologyDevice_.getReadBlockSize();
	configuration_.prog_size =
			programBlockSize_ != 0 ? programBlockSize_ : memoryTechnologyDevice_.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : memoryTechnologyDevice_.getEraseBlockSize();
	configuration_.block_count =
			blocksCount_ != 0 ? blocksCount_ : (memoryTechnologyDevice_.getSize() / configuration_.block_size);
	configuration_.lookahead = (std::max(lookahead_, 1u) + 31) / 32 * 32;

	const auto ret = lfs1_mount(&fileSystem_, &configuration_);
	if (ret != LFS1_ERR_OK)
		return littlefs1ErrorToErrorCode(ret);

	mounted_ = true;
	closeScopeGuard.release();
	return 0;
}

std::pair<int, std::unique_ptr<Directory>> Littlefs1FileSystem::openDirectory(const char* const path)
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	std::unique_ptr<Littlefs1Directory> directory {new (std::nothrow) Littlefs1Directory{*this}};
	if (directory == nullptr)
		return {ENOMEM, std::unique_ptr<Littlefs1Directory>{}};

	const auto ret = directory->open(path);
	if (ret != 0)
		return {ret, std::unique_ptr<Littlefs1Directory>{}};

	return {{}, std::move(directory)};
}

std::pair<int, std::unique_ptr<File>> Littlefs1FileSystem::openFile(const char* const path, const int flags)
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	std::unique_ptr<Littlefs1File> file {new (std::nothrow) Littlefs1File{*this}};
	if (file == nullptr)
		return {ENOMEM, std::unique_ptr<Littlefs1File>{}};

	const auto ret = file->open(path, flags);
	if (ret != 0)
		return {ret, std::unique_ptr<Littlefs1File>{}};

	return {{}, std::move(file)};
}

int Littlefs1FileSystem::remove(const char* const path)
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	const auto ret = lfs1_remove(&fileSystem_, path);
	return littlefs1ErrorToErrorCode(ret);
}

int Littlefs1FileSystem::rename(const char* const path, const char* const newPath)
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);
	assert(newPath != nullptr);

	const auto ret = lfs1_rename(&fileSystem_, path, newPath);
	return littlefs1ErrorToErrorCode(ret);
}

void Littlefs1FileSystem::unlock()
{
	const auto ret = mutex_.unlock();
	assert(ret == 0);
}

int Littlefs1FileSystem::unmount()
{
	const std::lock_guard<Littlefs1FileSystem> lockGuard {*this};

	assert(mounted_ == true);

	const auto unmountRet = lfs1_unmount(&fileSystem_);
	const auto closeRet = memoryTechnologyDevice_.close();
	mounted_ = {};

	return unmountRet != LFS1_ERR_OK ? littlefs1ErrorToErrorCode(unmountRet) : closeRet;
}

}	// namespace distortos
