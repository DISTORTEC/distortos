/**
 * \file
 * \brief LittlefsFileSystem class implementation
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/FileSystem/littlefs/LittlefsFileSystem.hpp"

#include "littlefsErrorToErrorCode.hpp"

#include "distortos/devices/memory/BlockDevice.hpp"

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
 * \brief Wrapper for BlockDevice::erase()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 * \param [in] block is the index of block that will be erased
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by BlockDevice::erase();
 */

int littlefsBlockDeviceErase(const lfs_config* const configuration, const lfs_block_t block)
{
	assert(configuration != nullptr);
	const auto blockDevice = static_cast<devices::BlockDevice*>(configuration->context);
	assert(blockDevice != nullptr);
	const auto ret = blockDevice->erase(static_cast<uint64_t>(block) * configuration->block_size,
			configuration->block_size);
	return errorCodeToLittlefsError(ret);
}

/**
 * \brief Wrapper for BlockDevice::program()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 * \param [in] block is the index of block that will be programmed
 * \param [in] offset is the offset of area that will be programmed, bytes
 * \param [in] buffer is the buffer with data that will be programmed
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by BlockDevice::program();
 */

int littlefsBlockDeviceProgram(const lfs_config* const configuration, const lfs_block_t block, const lfs_off_t offset,
		const void* const buffer, const lfs_size_t size)
{
	assert(configuration != nullptr);
	const auto blockDevice = static_cast<devices::BlockDevice*>(configuration->context);
	assert(blockDevice != nullptr);
	const auto ret = blockDevice->program(static_cast<uint64_t>(block) * configuration->block_size + offset, buffer,
			size);
	return errorCodeToLittlefsError(ret.first);
}

/**
 * \brief Wrapper for BlockDevice::read()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 * \param [in] block is the index of block that will be read
 * \param [in] offset is the offset of area that will be read, bytes
 * \param [out] buffer is the buffer into which the data will be read
 * \param [in] size is the size of \a buffer, bytes
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by BlockDevice::read();
 */

int littlefsBlockDeviceRead(const lfs_config* const configuration, const lfs_block_t block, const lfs_off_t offset,
		void* const buffer, const lfs_size_t size)
{
	assert(configuration != nullptr);
	const auto blockDevice = static_cast<devices::BlockDevice*>(configuration->context);
	assert(blockDevice != nullptr);
	const auto ret = blockDevice->read(static_cast<uint64_t>(block) * configuration->block_size + offset, buffer, size);
	return errorCodeToLittlefsError(ret.first);
}

/**
 * \brief Wrapper for BlockDevice::synchronize()
 *
 * \param [in] configuration is a pointer to littlefs configuration
 *
 * \return LFS_ERR_OK on success, error code otherwise:
 * - converted error codes returned by BlockDevice::synchronize();
 */

int littlefsBlockDeviceSynchronize(const lfs_config* const configuration)
{
	assert(configuration != nullptr);
	const auto blockDevice = static_cast<devices::BlockDevice*>(configuration->context);
	assert(blockDevice != nullptr);
	const auto ret = blockDevice->synchronize();
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

int LittlefsFileSystem::formatAndMount(devices::BlockDevice* const blockDevice)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	const auto oldBlockDevice = blockDevice_;
	const auto newBlockDevice = blockDevice != nullptr ? blockDevice : oldBlockDevice;
	if (newBlockDevice == nullptr)
		return EINVAL;

	if (blockDevice_ != nullptr)
	{
		const auto ret = unmount();
		if (ret != 0)
			return ret;
	}
	{
		const auto ret = format(*newBlockDevice, readBlockSize_, programBlockSize_, eraseBlockSize_, blocksCount_,
				lookahead_);
		if (ret != 0)
			return ret;
	}

	return mount(*newBlockDevice);
}

std::pair<int, struct stat> LittlefsFileSystem::getFileStatus(const char* const path)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (blockDevice_ == nullptr)
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

	if (blockDevice_ == nullptr)
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

	if (blockDevice_ == nullptr)
		return EBADF;

	const auto ret = lfs_mkdir(&fileSystem_, path);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsFileSystem::mount(devices::BlockDevice& blockDevice)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (blockDevice_ != nullptr)
		return EBUSY;

	{
		const auto ret = blockDevice.open();
		if (ret != 0)
			return ret;
	}

	auto closeScopeGuard = estd::makeScopeGuard([&blockDevice]()
			{
				blockDevice.close();
			});

	configuration_ = {};
	fileSystem_ = {};
	configuration_.context = &blockDevice;
	configuration_.read = littlefsBlockDeviceRead;
	configuration_.prog = littlefsBlockDeviceProgram;
	configuration_.erase = littlefsBlockDeviceErase;
	configuration_.sync = littlefsBlockDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : blockDevice.getReadBlockSize();
	configuration_.prog_size = programBlockSize_ != 0 ? programBlockSize_ : blockDevice.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : blockDevice.getEraseBlockSize();
	configuration_.block_count = blocksCount_ != 0 ? blocksCount_ : (blockDevice.getSize() / configuration_.block_size);
	configuration_.lookahead = (std::max(lookahead_, 1u) + 31) / 32 * 32;

	{
		const auto ret = lfs_mount(&fileSystem_, &configuration_);
		if (ret != LFS_ERR_OK)
			return littlefsErrorToErrorCode(ret);
	}

	blockDevice_ = &blockDevice;
	closeScopeGuard.release();
	return 0;
}

int LittlefsFileSystem::remove(const char* const path)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (blockDevice_ == nullptr)
		return EBADF;

	const auto ret = lfs_remove(&fileSystem_, path);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsFileSystem::rename(const char* const path, const char* const newPath)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (blockDevice_ == nullptr)
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

	if (blockDevice_ == nullptr)
		return EBADF;

	const auto unmountRet = lfs_unmount(&fileSystem_);
	const auto closeRet = blockDevice_->close();
	blockDevice_ = {};

	return unmountRet != LFS_ERR_OK ? littlefsErrorToErrorCode(unmountRet) : closeRet;
}

/*---------------------------------------------------------------------------------------------------------------------+
| public static functions
+---------------------------------------------------------------------------------------------------------------------*/

int LittlefsFileSystem::format(devices::BlockDevice& blockDevice, const size_t readBlockSize,
		const size_t programBlockSize, const size_t eraseBlockSize, const size_t blocksCount, const size_t lookahead)
{
	{
		const auto ret = blockDevice.open();
		if (ret != 0)
			return ret;
	}

	const auto closeScopeGuard = estd::makeScopeGuard([&blockDevice]()
			{
				blockDevice.close();
			});

	lfs_config configuration {};
	lfs_t fileSystem {};
	configuration.context = &blockDevice;
	configuration.read = littlefsBlockDeviceRead;
	configuration.prog = littlefsBlockDeviceProgram;
	configuration.erase = littlefsBlockDeviceErase;
	configuration.sync = littlefsBlockDeviceSynchronize;
	configuration.read_size = readBlockSize != 0 ? readBlockSize : blockDevice.getReadBlockSize();
	configuration.prog_size = programBlockSize != 0 ? programBlockSize : blockDevice.getProgramBlockSize();
	configuration.block_size = eraseBlockSize != 0 ? eraseBlockSize : blockDevice.getEraseBlockSize();
	configuration.block_count = blocksCount != 0 ? blocksCount : (blockDevice.getSize() / configuration.block_size);
	configuration.lookahead = (std::max(lookahead, 1u) + 31) / 32 * 32;

	const auto ret = lfs_format(&fileSystem, &configuration);
	return littlefsErrorToErrorCode(ret);
}

}	// namespace distortos
