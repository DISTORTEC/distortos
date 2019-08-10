/**
 * \file
 * \brief FatFileSystem class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/FileSystem/FatFileSystem.hpp"

#include "FatDirectory.hpp"
#include "ufatErrorToErrorCode.hpp"

#include "distortos/devices/memory/BlockDevice.hpp"

#include "distortos/assert.h"

#include "estd/log2u.hpp"
#include "estd/ScopeGuard.hpp"

#include <mutex>

#include <cstring>

namespace distortos
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Wrapper for BlockDevice::read()
 *
 * \param [in] device is a pointer to uFAT device struct
 * \param [in] block is the index of block that will be read
 * \param [in] blocksCount is the number of blocks to read
 * \param [out] buffer is the buffer into which the data will be read
 *
 * \return 0 on success, -1 otherwise
 */

int ufatBlockDeviceRead(const ufat_device* const device, const ufat_block_t block, const ufat_block_t blocksCount,
		void* const buffer)
{
	assert(device != nullptr);
	auto& blockDevice = reinterpret_cast<const FatFileSystem::UfatDevice*>(device)->blockDevice;
	const auto blockSize = 1u << device->log2_block_size;
	const auto ret = blockDevice.read(static_cast<uint64_t>(block) * blockSize, buffer, blocksCount * blockSize);
	return ret == 0 ? 0 : -1;
}

/**
 * \brief Wrapper for BlockDevice::write()
 *
 * \param [in] device is a pointer to uFAT device struct
 * \param [in] block is the index of block that will be written
 * \param [in] blocksCount is the number of blocks to write
 * \param [in] buffer is the buffer with data that will be written
 *
 * \return 0 on success, -1 otherwise
 */

int ufatBlockDeviceWrite(const ufat_device* const device, const ufat_block_t block, const ufat_block_t blocksCount,
		const void* const buffer)
{
	assert(device != nullptr);
	auto& blockDevice = reinterpret_cast<const FatFileSystem::UfatDevice*>(device)->blockDevice;
	const auto blockSize = 1u << device->log2_block_size;
	const auto ret = blockDevice.write(static_cast<uint64_t>(block) * blockSize, buffer, blocksCount * blockSize);
	return ret == 0 ? 0 : -1;
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

FatFileSystem::~FatFileSystem()
{
	assert(mounted_ == false);
}

int FatFileSystem::format()
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == false);

	{
		const auto ret = device_.blockDevice.open();
		if (ret != 0)
			return ret;
	}

	const auto closeScopeGuard = estd::makeScopeGuard([this]()
			{
				device_.blockDevice.close();
			});

	const auto blockSize = blockSize_ != 0 ? blockSize_ : device_.blockDevice.getBlockSize();
	const auto blockSizeLog2 = estd::log2u(blockSize);
	if (1u << blockSizeLog2 != blockSize)
		return EINVAL;

	device_.device = {};
	device_.device.log2_block_size = blockSizeLog2;
	device_.device.read = ufatBlockDeviceRead;
	device_.device.write = ufatBlockDeviceWrite;

	const auto blocksCount = blocksCount_ != 0 ? blocksCount_ : device_.blockDevice.getSize() / blockSize;

	const auto ret = ufat_mkfs(&device_.device, blocksCount);
	return ufatErrorToErrorCode(ret);
}

int FatFileSystem::getFileStatus(const char* const path, struct stat& status)
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	ufat_directory directory {};
	ufat_dirent directoryEntry {};
	ufat_open_root(&fileSystem_, &directory);
	const auto ret = ufat_dir_find_path(&directory, path, &directoryEntry, {});
	if (ret < 0)
		return ufatErrorToErrorCode(ret);
	if (ret != 0)
		return ENOENT;

	status = {};
	status.st_mode = (directoryEntry.attributes & UFAT_ATTR_DIRECTORY) != 0 ? S_IFDIR : S_IFREG;
	if (status.st_mode == S_IFREG)
		status.st_size = directoryEntry.file_size;

	/// \todo parse create/modify/access date and time

	return {};
}

int FatFileSystem::getStatus(struct statvfs& status)
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == true);

	ufat_cluster_t freeClusters;
	const auto ret = ufat_count_free_clusters(&fileSystem_, &freeClusters);
	if (ret < 0)
		return ufatErrorToErrorCode(ret);

	status = {};
	status.f_bsize = (1 << device_.device.log2_block_size) * (1u << fileSystem_.bpb.log2_blocks_per_cluster);
	status.f_frsize = status.f_bsize;
	status.f_blocks = fileSystem_.bpb.num_clusters - 2;
	status.f_bfree = freeClusters;
	status.f_bavail = status.f_bfree;
	status.f_namemax = UFAT_LFN_MAX_CHARS;
	return {};
}

void FatFileSystem::lock()
{
	const auto ret = mutex_.lock();
	assert(ret == 0);
}

int FatFileSystem::makeDirectory(const char* const path, mode_t)
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	ufat_directory directory {};
	ufat_dirent directoryEntry {};
	const char* pathRemainder {};
	{
		ufat_open_root(&fileSystem_, &directory);
		const auto ret = ufat_dir_find_path(&directory, path, &directoryEntry, &pathRemainder);
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
		if (ret == 0)
			return EEXIST;
	}
	if (strpbrk(pathRemainder, "/\\") != nullptr)
		return ENOENT;

	directoryEntry = {};
	const auto ret = ufat_dir_create(&directory, &directoryEntry, pathRemainder);
	return ufatErrorToErrorCode(ret);
}

int FatFileSystem::mount()
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == false);

	{
		const auto ret = device_.blockDevice.open();
		if (ret != 0)
			return ret;
	}

	auto closeScopeGuard = estd::makeScopeGuard([this]()
			{
				device_.blockDevice.close();
			});

	const auto blockSize = blockSize_ != 0 ? blockSize_ : device_.blockDevice.getBlockSize();
	const auto blockSizeLog2 = estd::log2u(blockSize);
	if (1u << blockSizeLog2 != blockSize)
		return EINVAL;

	device_.device = {};
	device_.device.log2_block_size = blockSizeLog2;
	device_.device.read = ufatBlockDeviceRead;
	device_.device.write = ufatBlockDeviceWrite;

	fileSystem_ = {};
	const auto ret = ufat_open(&fileSystem_, &device_.device);
	if (ret < 0)
		return ufatErrorToErrorCode(ret);

	mounted_ = true;
	closeScopeGuard.release();
	return {};
}

std::pair<int, std::unique_ptr<Directory>> FatFileSystem::openDirectory(const char* const path)
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == true);

	std::unique_ptr<FatDirectory> directory {new (std::nothrow) FatDirectory{*this}};
	if (directory == nullptr)
		return {ENOMEM, std::unique_ptr<FatDirectory>{}};

	const auto ret = directory->open(path);
	if (ret != 0)
		return {ret, std::unique_ptr<FatDirectory>{}};

	return {{}, std::move(directory)};
}

int FatFileSystem::remove(const char* const path)
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);

	ufat_dirent directoryEntry {};
	{
		ufat_directory directory {};
		ufat_open_root(&fileSystem_, &directory);
		const auto ret = ufat_dir_find_path(&directory, path, &directoryEntry, {});
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
		if (ret != 0)
			return ENOENT;
	}

	const auto ret = ufat_dir_delete(&fileSystem_, &directoryEntry);
	return ufatErrorToErrorCode(ret);
}

int FatFileSystem::rename(const char* const path, const char* const newPath)
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == true);
	assert(path != nullptr);
	assert(newPath != nullptr);

	ufat_dirent directoryEntry {};
	{
		ufat_directory directory {};
		ufat_open_root(&fileSystem_, &directory);
		const auto ret = ufat_dir_find_path(&directory, path, &directoryEntry, {});
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
		if (ret != 0)
			return ENOENT;
	}

	ufat_directory newDirectory {};
	const char* newPathRemainder {};
	{
		ufat_open_root(&fileSystem_, &newDirectory);
		ufat_dirent newDirectoryEntry {};
		const auto ret = ufat_dir_find_path(&newDirectory, newPath, &newDirectoryEntry, &newPathRemainder);
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
		if (ret == 0)
			return EEXIST;
	}
	if (strpbrk(newPathRemainder, "/\\") != nullptr)
		return ENOENT;

	const auto ret = ufat_move(&directoryEntry, &newDirectory, newPathRemainder);
	return ufatErrorToErrorCode(ret);
}

void FatFileSystem::unlock()
{
	const auto ret = mutex_.unlock();
	assert(ret == 0);
}

int FatFileSystem::unmount()
{
	const std::lock_guard<FatFileSystem> lockGuard {*this};

	assert(mounted_ == true);

	ufat_close(&fileSystem_);
	const auto ret = device_.blockDevice.close();
	mounted_ = {};

	return ret;
}

}	// namespace distortos
