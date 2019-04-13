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
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// required buffer alignment of memory technology device
constexpr size_t alignment {CONFIG_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENT};

/// margin between platform's biggest alignment and required buffer alignment
constexpr size_t alignmentMargin {alignment > __BIGGEST_ALIGNMENT__ ? alignment - __BIGGEST_ALIGNMENT__ : 0};

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
	return errorCodeToLittlefsError(ret);
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
	return errorCodeToLittlefsError(ret);
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
	assert(mounted_ == false);
}

int LittlefsFileSystem::format()
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

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
	configuration_.read = littlefsMemoryTechnologyDeviceRead;
	configuration_.prog = littlefsMemoryTechnologyDeviceProgram;
	configuration_.erase = littlefsMemoryTechnologyDeviceErase;
	configuration_.sync = littlefsMemoryTechnologyDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : memoryTechnologyDevice_.getReadBlockSize();
	configuration_.prog_size =
			programBlockSize_ != 0 ? programBlockSize_ : memoryTechnologyDevice_.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : memoryTechnologyDevice_.getEraseBlockSize();
	configuration_.block_count =
			blocksCount_ != 0 ? blocksCount_ : (memoryTechnologyDevice_.getSize() / configuration_.block_size);
	configuration_.lookahead = (std::max(lookahead_, 1u) + 31) / 32 * 32;

	const size_t lookaheadBufferSize {configuration_.lookahead / CHAR_BIT};
	const std::unique_ptr<uint8_t[]> lookaheadBuffer {new (std::nothrow) uint8_t[lookaheadBufferSize]};
	if (lookaheadBuffer.get() == nullptr)
		return ENOMEM;

	const size_t programBufferSize {configuration_.prog_size + alignmentMargin};
	const std::unique_ptr<uint8_t[]> programBuffer {new (std::nothrow) uint8_t[programBufferSize]};
	if (programBuffer.get() == nullptr)
		return ENOMEM;

	const size_t readBufferSize {configuration_.read_size + alignmentMargin};
	const std::unique_ptr<uint8_t[]> readBuffer {new (std::nothrow) uint8_t[readBufferSize]};
	if (readBuffer.get() == nullptr)
		return ENOMEM;

	configuration_.lookahead_buffer = lookaheadBuffer.get();
	configuration_.prog_buffer =
			reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(programBuffer.get()) + alignment - 1) /
			alignment * alignment);
	configuration_.read_buffer =
			reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(readBuffer.get()) + alignment - 1) /
			alignment * alignment);

	const auto ret = lfs_format(&fileSystem_, &configuration_);
	return littlefsErrorToErrorCode(ret);
}

std::pair<int, struct stat> LittlefsFileSystem::getFileStatus(const char* const path)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	assert(mounted_ == true);

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

	if (mounted_ == false)
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

	if (mounted_ == false)
		return EBADF;

	const auto ret = lfs_mkdir(&fileSystem_, path);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsFileSystem::mount()
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (mounted_ == true)
		return EBUSY;

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
	configuration_.read = littlefsMemoryTechnologyDeviceRead;
	configuration_.prog = littlefsMemoryTechnologyDeviceProgram;
	configuration_.erase = littlefsMemoryTechnologyDeviceErase;
	configuration_.sync = littlefsMemoryTechnologyDeviceSynchronize;
	configuration_.read_size = readBlockSize_ != 0 ? readBlockSize_ : memoryTechnologyDevice_.getReadBlockSize();
	configuration_.prog_size =
			programBlockSize_ != 0 ? programBlockSize_ : memoryTechnologyDevice_.getProgramBlockSize();
	configuration_.block_size = eraseBlockSize_ != 0 ? eraseBlockSize_ : memoryTechnologyDevice_.getEraseBlockSize();
	configuration_.block_count =
			blocksCount_ != 0 ? blocksCount_ : (memoryTechnologyDevice_.getSize() / configuration_.block_size);
	configuration_.lookahead = (std::max(lookahead_, 1u) + 31) / 32 * 32;

	const size_t lookaheadBufferSize {configuration_.lookahead / CHAR_BIT};
	std::unique_ptr<uint8_t[]> lookaheadBuffer {new (std::nothrow) uint8_t[lookaheadBufferSize]};
	if (lookaheadBuffer.get() == nullptr)
		return ENOMEM;

	const size_t programBufferSize {configuration_.prog_size + alignmentMargin};
	std::unique_ptr<uint8_t[]> programBuffer {new (std::nothrow) uint8_t[programBufferSize]};
	if (programBuffer.get() == nullptr)
		return ENOMEM;

	const size_t readBufferSize {configuration_.read_size + alignmentMargin};
	std::unique_ptr<uint8_t[]> readBuffer {new (std::nothrow) uint8_t[readBufferSize]};
	if (readBuffer.get() == nullptr)
		return ENOMEM;

	configuration_.lookahead_buffer = lookaheadBuffer.get();
	configuration_.prog_buffer =
			reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(programBuffer.get()) + alignment - 1) /
			alignment * alignment);
	configuration_.read_buffer =
			reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(readBuffer.get()) + alignment - 1) /
			alignment * alignment);

	const auto ret = lfs_mount(&fileSystem_, &configuration_);
	if (ret != LFS_ERR_OK)
		return littlefsErrorToErrorCode(ret);

	mounted_ = true;
	lookaheadBuffer_ = std::move(lookaheadBuffer);
	programBuffer_ = std::move(programBuffer);
	readBuffer_ = std::move(readBuffer);
	closeScopeGuard.release();
	return 0;
}

std::pair<int, std::unique_ptr<Directory>> LittlefsFileSystem::openDirectory(const char* const path)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (mounted_ == false)
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

	if (mounted_ == false)
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

	if (mounted_ == false)
		return EBADF;

	const auto ret = lfs_remove(&fileSystem_, path);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsFileSystem::rename(const char* const path, const char* const newPath)
{
	const std::lock_guard<LittlefsFileSystem> lockGuard {*this};

	if (mounted_ == false)
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

	if (mounted_ == false)
		return EBADF;

	const auto unmountRet = lfs_unmount(&fileSystem_);
	const auto closeRet = memoryTechnologyDevice_.close();
	mounted_ = {};
	lookaheadBuffer_.reset();
	programBuffer_.reset();
	readBuffer_.reset();

	return unmountRet != LFS_ERR_OK ? littlefsErrorToErrorCode(unmountRet) : closeRet;
}

}	// namespace distortos
