/**
 * \file
 * \brief LittlefsFileSystem class implementation
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/FileSystem/littlefs/LittlefsFileSystem.hpp"

#include "littlefsErrorToErrorCode.hpp"

#include "distortos/devices/memory/MemoryTechnologyDevice.hpp"

#include "distortos/FileSystem/littlefs/LittlefsDirectory.hpp"
#include "distortos/FileSystem/littlefs/LittlefsFile.hpp"

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
 * \brief Callback for lfs_traverse() that counts block used by file system.
 *
 * Idea from https://github.com/ARMmbed/littlefs/issues/1#issuecomment-355116984
 *
 * \param [out] data is a pointer to size_t variable with number of used blocks
 *
 * \return always 0
 */

int countUsedBlocks(void* const data, lfs_block_t)
{
	assert(data != nullptr);
	auto& usedBlocks = *static_cast<size_t*>(data);
	++usedBlocks;
	return {};
}

/**
 * \brief Converts error code to littlefs error.
 *
 * \param [in] errorCode is an error code
 *
 * \return \a errorCode converted to littlefs error
 */

int errorCodeToLittlefsError(const int errorCode)
{
	if (errorCode == 0)
		return LFS_ERR_OK;
	if (errorCode == EIO)
		return LFS_ERR_IO;
	if (errorCode == EILSEQ)
		return LFS_ERR_CORRUPT;
	if (errorCode == ENOENT)
		return LFS_ERR_NOENT;
	if (errorCode == EEXIST)
		return LFS_ERR_EXIST;
	if (errorCode == ENOTDIR)
		return LFS_ERR_NOTDIR;
	if (errorCode == EISDIR)
		return LFS_ERR_ISDIR;
	if (errorCode == ENOTEMPTY)
		return LFS_ERR_NOTEMPTY;
	if (errorCode == EBADF)
		return LFS_ERR_BADF;
	if (errorCode == EINVAL)
		return LFS_ERR_INVAL;
	if (errorCode == ENOSPC)
		return LFS_ERR_NOSPC;
	if (errorCode == ENOMEM)
		return LFS_ERR_NOMEM;

	return -errorCode;
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::erase()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 * \param [in] block is the index of block that will be erased
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::erase();
 */

int littlefsMemoryTechnologyDeviceErase(const lfs_config* const configuration, const lfs_block_t block)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->erase(static_cast<uint64_t>(block) * configuration->block_size,
			configuration->block_size);
	return errorCodeToLittlefsError(ret);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::program()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 * \param [in] block is the index of block that will be programmed
 * \param [in] offset is the offset of area that will be programmed, bytes
 * \param [in] buffer is the buffer with data that will be programmed
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::program();
 */

int littlefsMemoryTechnologyDeviceProgram(const lfs_config* const configuration, const lfs_block_t block,
		const lfs_off_t offset, const void* const buffer, const lfs_size_t size)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->program(static_cast<uint64_t>(block) * configuration->block_size + offset,
			buffer, size);
	return errorCodeToLittlefsError(ret.first);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::read()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 * \param [in] block is the index of block that will be read
 * \param [in] offset is the offset of area that will be read, bytes
 * \param [out] buffer is the buffer into which the data will be read
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::read();
 */

int littlefsMemoryTechnologyDeviceRead(const lfs_config* const configuration, const lfs_block_t block,
		const lfs_off_t offset, void* const buffer, const lfs_size_t size)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->read(static_cast<uint64_t>(block) * configuration->block_size + offset,
			buffer, size);
	return errorCodeToLittlefsError(ret.first);
}

/**
 * \brief Wrapper for MemoryTechnologyDevice::synchronize()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by MemoryTechnologyDevice::synchronize();
 */

int littlefsMemoryTechnologyDeviceSynchronize(const lfs_config* const configuration)
{
	assert(configuration != nullptr);
	const auto memoryTechnologyDevice = static_cast<devices::MemoryTechnologyDevice*>(configuration->context);
	assert(memoryTechnologyDevice != nullptr);
	const auto ret = memoryTechnologyDevice->synchronize();
	return errorCodeToLittlefsError(ret);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

LittlefsFileSystem::~LittlefsFileSystem()
{
	unmount();
}

int LittlefsFileSystem::formatAndMount(devices::MemoryTechnologyDevice* const memoryTechnologyDevice)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	const auto oldMemoryTechnologyDevice = memoryTechnologyDevice_;
	const auto newMemoryTechnologyDevice =
			memoryTechnologyDevice != nullptr ? memoryTechnologyDevice : oldMemoryTechnologyDevice;
	if (newMemoryTechnologyDevice == nullptr)
		return EINVAL;

	if (memoryTechnologyDevice_ != nullptr)
	{
		const auto ret = unmount();
		if (ret != 0)
			return ret;
	}
	{
		const auto ret = format(*newMemoryTechnologyDevice, readBlockSize_, programBlockSize_, eraseBlockSize_,
				blocksCount_, lookahead_);
		if (ret != 0)
			return ret;
	}

	return mount(*newMemoryTechnologyDevice);
}

std::pair<int, struct stat> LittlefsFileSystem::getFileStatus(const char* const path)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return {EBADF, {}};

	lfs_info info;
	{
		const auto ret = lfs_stat(&fileSystem_, path, &info);
		if (ret != LFS_ERR_OK)
			return {littlefsErrorToErrorCode(ret), {}};
	}

	struct stat status {};
	status.st_mode = info.type == LFS_TYPE_DIR ? S_IFDIR : S_IFREG;
	if (info.type == LFS_TYPE_REG)
		status.st_size = info.size;
	return {{}, status};
}

std::pair<int, struct statvfs> LittlefsFileSystem::getStatus()
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return {EBADF, {}};

	size_t usedBlocks {};
	{
		const auto ret = lfs_traverse(&fileSystem_, countUsedBlocks, &usedBlocks);
		if (ret != LFS_ERR_OK)
			return {littlefsErrorToErrorCode(ret), {}};
	}

	struct statvfs status {};
	status.f_bsize = configuration_.block_size;
	status.f_frsize = status.f_bsize;
	status.f_blocks = configuration_.block_count;
	status.f_bfree = status.f_blocks - usedBlocks;
	status.f_bavail = status.f_bfree;
	status.f_namemax = LFS_NAME_MAX;
	return {{}, status};
}

int LittlefsFileSystem::lock()
{
	return mutex_.lock();
}

int LittlefsFileSystem::makeDirectory(const char* const path, mode_t)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return EBADF;

	const auto ret = lfs_mkdir(&fileSystem_, path);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsFileSystem::mount(devices::MemoryTechnologyDevice& memoryTechnologyDevice)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ != nullptr)
		return EBUSY;

	{
		const auto ret = memoryTechnologyDevice.open();
		if (ret != 0)
			return ret;
	}

	auto closeScopeGuard = estd::makeScopeGuard([&memoryTechnologyDevice]()
			{
				memoryTechnologyDevice.close();
			});

	configuration_ = {};
	fileSystem_ = {};
	configuration_.context = &memoryTechnologyDevice;
	configuration_.read = littlefsMemoryTechnologyDeviceRead;
	configuration_.prog = littlefsMemoryTechnologyDeviceProgram;
	configuration_.erase = littlefsMemoryTechnologyDeviceErase;
	configuration_.sync = littlefsMemoryTechnologyDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : memoryTechnologyDevice.getReadBlockSize();
	configuration_.prog_size =
			programBlockSize_ != 0 ? programBlockSize_ : memoryTechnologyDevice.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : memoryTechnologyDevice.getEraseBlockSize();
	configuration_.block_count =
			blocksCount_ != 0 ? blocksCount_ : (memoryTechnologyDevice.getSize() / configuration_.block_size);
	configuration_.lookahead = (std::max(lookahead_, 1u) + 31) / 32 * 32;

	{
		const auto ret = lfs_mount(&fileSystem_, &configuration_);
		if (ret != LFS_ERR_OK)
			return littlefsErrorToErrorCode(ret);
	}

	memoryTechnologyDevice_ = &memoryTechnologyDevice;
	closeScopeGuard.release();
	return 0;
}

std::pair<int, std::unique_ptr<Directory>> LittlefsFileSystem::openDirectory(const char* const path)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return {EBADF, std::unique_ptr<LittlefsDirectory>{}};

	std::unique_ptr<LittlefsDirectory> directory {new (std::nothrow) LittlefsDirectory{*this}};
	if (directory == nullptr)
		return {ENOMEM, std::unique_ptr<LittlefsDirectory>{}};

	{
		const auto ret = directory->open(path);
		if (ret != 0)
			return {ret, std::unique_ptr<LittlefsDirectory>{}};
	}

	return {{}, std::move(directory)};
}

std::pair<int, std::unique_ptr<File>> LittlefsFileSystem::openFile(const char* const path, const int flags)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return {EBADF, std::unique_ptr<LittlefsFile>{}};

	std::unique_ptr<LittlefsFile> file {new (std::nothrow) LittlefsFile{*this}};
	if (file == nullptr)
		return {ENOMEM, std::unique_ptr<LittlefsFile>{}};

	{
		const auto ret = file->open(path, flags);
		if (ret != 0)
			return {ret, std::unique_ptr<LittlefsFile>{}};
	}

	return {{}, std::move(file)};
}

int LittlefsFileSystem::remove(const char* const path)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return EBADF;

	const auto ret = lfs_remove(&fileSystem_, path);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsFileSystem::rename(const char* const path, const char* const newPath)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return EBADF;

	const auto ret = lfs_rename(&fileSystem_, path, newPath);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsFileSystem::unlock()
{
	return mutex_.unlock();
}

int LittlefsFileSystem::unmount()
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (memoryTechnologyDevice_ == nullptr)
		return EBADF;

	const auto unmountRet = lfs_unmount(&fileSystem_);
	const auto closeRet = memoryTechnologyDevice_->close();
	memoryTechnologyDevice_ = {};

	return unmountRet != LFS_ERR_OK ? littlefsErrorToErrorCode(unmountRet) : closeRet;
}

/*---------------------------------------------------------------------------------------------------------------------+
| public static functions
+---------------------------------------------------------------------------------------------------------------------*/

int LittlefsFileSystem::format(devices::MemoryTechnologyDevice& memoryTechnologyDevice, const size_t readBlockSize,
		const size_t programBlockSize, const size_t eraseBlockSize, const size_t blocksCount, const size_t lookahead)
{
	{
		const auto ret = memoryTechnologyDevice.open();
		if (ret != 0)
			return ret;
	}

	const auto closeScopeGuard = estd::makeScopeGuard([&memoryTechnologyDevice]()
			{
				memoryTechnologyDevice.close();
			});

	lfs_config configuration {};
	lfs_t fileSystem {};
	configuration.context = &memoryTechnologyDevice;
	configuration.read = littlefsMemoryTechnologyDeviceRead;
	configuration.prog = littlefsMemoryTechnologyDeviceProgram;
	configuration.erase = littlefsMemoryTechnologyDeviceErase;
	configuration.sync = littlefsMemoryTechnologyDeviceSynchronize;
	configuration.read_size = readBlockSize != 0 ? readBlockSize : memoryTechnologyDevice.getReadBlockSize();
	configuration.prog_size = programBlockSize != 0 ? programBlockSize : memoryTechnologyDevice.getProgramBlockSize();
	configuration.block_size = eraseBlockSize != 0 ? eraseBlockSize : memoryTechnologyDevice.getEraseBlockSize();
	configuration.block_count =
			blocksCount != 0 ? blocksCount : (memoryTechnologyDevice.getSize() / configuration.block_size);
	configuration.lookahead = (std::max(lookahead, 1u) + 31) / 32 * 32;

	const auto ret = lfs_format(&fileSystem, &configuration);
	return littlefsErrorToErrorCode(ret);
}

}	// namespace distortos
