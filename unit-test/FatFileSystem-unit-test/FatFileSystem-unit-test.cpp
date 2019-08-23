/**
 * \file
 * \brief FatFileSystem test cases
 *
 * This test checks whether FatFileSystem perform all operations properly and in correct order.
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "distortos/devices/memory/BlockDevice.hpp"

#include "distortos/FileSystem/FatFileSystem.hpp"

#include "estd/log2u.hpp"

#include <fcntl.h>

using trompeloeil::_;
using trompeloeil::ne;
using Whence = distortos::File::Whence;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class BlockDevice : public distortos::devices::BlockDevice
{
public:

	MAKE_MOCK0(close, int());
	MAKE_MOCK2(erase, int(uint64_t, uint64_t));
	MAKE_CONST_MOCK0(getBlockSize, size_t());
	MAKE_CONST_MOCK0(getSize, uint64_t());
	MAKE_MOCK0(lock, void());
	MAKE_MOCK0(open, int());
	MAKE_MOCK3(read, int(uint64_t, void*, size_t));
	MAKE_MOCK0(synchronize, int());
	MAKE_MOCK0(unlock, void());
	MAKE_MOCK3(write, int(uint64_t, const void*, size_t));
};

class UfatMock
{
public:

	UfatMock()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance == nullptr);
		instance = this;
	}

	virtual ~UfatMock()
	{
		auto& instance = getInstanceInternal();
		if (instance == this)
			instance = {};
	}

	MAKE_MOCK1(ufat_close, void(ufat*));
	MAKE_MOCK2(ufat_count_free_clusters, int(ufat*, ufat_cluster_t*));
	MAKE_MOCK3(ufat_dir_create, int(ufat_directory*, ufat_dirent*, const char*));
	MAKE_MOCK2(ufat_dir_delete, int(ufat*, ufat_dirent*));
	MAKE_MOCK4(ufat_dir_find_path, int(ufat_directory*, const char*, ufat_dirent*, const char**));
	MAKE_MOCK3(ufat_dir_mkfile, int(ufat_directory*, ufat_dirent*, const char*));
	MAKE_MOCK4(ufat_dir_read, int(ufat_directory*, ufat_dirent*, char*, int));
	MAKE_MOCK1(ufat_dir_rewind, void(ufat_directory*));
	MAKE_MOCK2(ufat_file_advance, int(ufat_file*, ufat_size_t));
	MAKE_MOCK3(ufat_file_read, int(ufat_file*, void*, ufat_size_t));
	MAKE_MOCK1(ufat_file_rewind, void(ufat_file*));
	MAKE_MOCK1(ufat_file_truncate, int(ufat_file*));
	MAKE_MOCK3(ufat_file_write, int(ufat_file*, const void*, ufat_size_t));
	MAKE_MOCK2(ufat_mkfs, int(ufat_device*, ufat_block_t));
	MAKE_MOCK3(ufat_move, int(ufat_dirent*, ufat_directory*, const char*));
	MAKE_MOCK3(ufat_open_file, int(ufat*, ufat_file*, const ufat_dirent*));
	MAKE_MOCK2(ufat_open_root, void(ufat*, ufat_directory*));
	MAKE_MOCK3(ufat_open_subdir, int(ufat*, ufat_directory*, const ufat_dirent*));
	MAKE_MOCK2(ufat_open, int(ufat*, const ufat_device*));
	MAKE_MOCK1(ufat_sync, int(ufat*));

	static UfatMock& getInstance()
	{
		const auto instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		return *instance;
	}

private:

	static UfatMock*& getInstanceInternal()
	{
		static UfatMock* instance;
		return instance;
	}
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr size_t defaultBlockSize {512};
constexpr size_t defaultBlocksCount {SIZE_MAX / defaultBlockSize};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

extern "C" void ufat_close(ufat* const fileSystem)
{
	return UfatMock::getInstance().ufat_close(fileSystem);
}

extern "C" int ufat_count_free_clusters(ufat* const fileSystem, ufat_cluster_t* const freeClusters)
{
	return UfatMock::getInstance().ufat_count_free_clusters(fileSystem, freeClusters);
}

extern "C" int ufat_dir_create(ufat_directory* const directory, ufat_dirent* const directoryEntry,
		const char* const name)
{
	return UfatMock::getInstance().ufat_dir_create(directory, directoryEntry, name);
}

extern "C" int ufat_dir_delete(ufat* const fileSystem, ufat_dirent* const directoryEntry)
{
	return UfatMock::getInstance().ufat_dir_delete(fileSystem, directoryEntry);
}

extern "C" int ufat_dir_find_path(ufat_directory* const directory, const char* const path,
		ufat_dirent* const directoryEntry, const char** const pathRemainder)
{
	return UfatMock::getInstance().ufat_dir_find_path(directory, path, directoryEntry, pathRemainder);
}

extern "C" int ufat_dir_mkfile(ufat_directory* const directory, ufat_dirent* const directoryEntry,
		const char* const name)
{
	return UfatMock::getInstance().ufat_dir_mkfile(directory, directoryEntry, name);
}

extern "C" int ufat_dir_read(ufat_directory* const directory, ufat_dirent* const directoryEntry, char* const nameBuffer,
		const int nameBufferSize)
{
	return UfatMock::getInstance().ufat_dir_read(directory, directoryEntry, nameBuffer, nameBufferSize);
}

extern "C" void ufat_dir_rewind(ufat_directory* const directory)
{
	return UfatMock::getInstance().ufat_dir_rewind(directory);
}

extern "C" int ufat_file_advance(ufat_file* const file, const ufat_size_t offset)
{
	return UfatMock::getInstance().ufat_file_advance(file, offset);
}

extern "C" int ufat_file_read(ufat_file* const file, void* const buffer, const ufat_size_t size)
{
	return UfatMock::getInstance().ufat_file_read(file, buffer, size);
}

extern "C" void ufat_file_rewind(ufat_file* const file)
{
	return UfatMock::getInstance().ufat_file_rewind(file);
}

extern "C" int ufat_file_truncate(ufat_file* const file)
{
	return UfatMock::getInstance().ufat_file_truncate(file);
}

extern "C" int ufat_file_write(ufat_file* const file, const void* const buffer, const ufat_size_t size)
{
	return UfatMock::getInstance().ufat_file_write(file, buffer, size);
}

extern "C" int ufat_mkfs(ufat_device* const device, const ufat_block_t blocksCount)
{
	return UfatMock::getInstance().ufat_mkfs(device, blocksCount);
}

extern "C" int ufat_move(ufat_dirent* const directoryEntry, ufat_directory* const directory, const char* const newName)
{
	return UfatMock::getInstance().ufat_move(directoryEntry, directory, newName);
}

extern "C" int ufat_open_file(ufat* const fileSystem, ufat_file* const file, const ufat_dirent* const directoryEntry)
{
	return UfatMock::getInstance().ufat_open_file(fileSystem, file, directoryEntry);
}

extern "C" void ufat_open_root(ufat* const fileSystem, ufat_directory* const directory)
{
	return UfatMock::getInstance().ufat_open_root(fileSystem, directory);
}

extern "C" int ufat_open_subdir(ufat* const fileSystem, ufat_directory* const directory,
		const ufat_dirent* const directoryEntry)
{
	return UfatMock::getInstance().ufat_open_subdir(fileSystem, directory, directoryEntry);
}

extern "C" int ufat_open(ufat* const fileSystem, const ufat_device* const device)
{
	return UfatMock::getInstance().ufat_open(fileSystem, device);
}

extern "C" int ufat_sync(ufat* const fileSystem)
{
	return UfatMock::getInstance().ufat_sync(fileSystem);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing format()", "[format]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	SECTION("Block device open error should propagate error code to caller")
	{
		distortos::FatFileSystem ffs {blockDeviceMock};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		constexpr int ret {0x12be8f55};
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.format() == ret);
	}
	SECTION("Using invalid block size should fail with EINVAL")
	{
		distortos::FatFileSystem ffs {blockDeviceMock};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize + 1);
		REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.format() == EINVAL);
	}

	static const size_t blockSizes[]
	{
			0,
			defaultBlockSize / 4,
	};
	static const size_t blocksCounts[]
	{
			0,
			0xd2da8ceb,
	};
	for (const auto blockSize : blockSizes)
	{
		const auto realBlockSize = blockSize != 0 ? blockSize : defaultBlockSize;
		for (const auto blocksCount : blocksCounts)
		{
			const auto realBlocksCount = blocksCount != 0 ? blocksCount : defaultBlocksCount;
			DYNAMIC_SECTION("Testing with " << realBlockSize << " bytes block size" <<
					(blockSize == 0 ? " (default)" : "") << " and " << realBlocksCount << " blocks" <<
					(blocksCount == 0 ? " (default)" : ""))
			{
				distortos::FatFileSystem ffs {blockDeviceMock, blockSize, blocksCount};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
				if (blockSize == 0)
					expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence)
							.RETURN(realBlockSize));
				if (blocksCount == 0)
					expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence)
							.RETURN(static_cast<uint64_t>(realBlockSize) * realBlocksCount));
				REQUIRE_CALL(ufatMock, ufat_mkfs(ne(nullptr), realBlocksCount)).IN_SEQUENCE(sequence)
						.RETURN(-UFAT_MAX_ERR);
				REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(ffs.format() == EINVAL);
			}
		}
	}
}

TEST_CASE("Testing mount() & unmount()", "[mount/unmount]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	SECTION("Block device open error should propagate error code to caller")
	{
		distortos::FatFileSystem ffs {blockDeviceMock};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		constexpr int ret {0x10ccbc5e};
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.mount() == ret);
	}
	SECTION("Using invalid block size should fail with EINVAL")
	{
		distortos::FatFileSystem ffs {blockDeviceMock};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize + 1);
		REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.mount() == EINVAL);
	}
	SECTION("ufat_open() error should propagate converted error code to caller")
	{
		distortos::FatFileSystem ffs {blockDeviceMock};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
		REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_IO);
		REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.mount() == EIO);
	}

	static const size_t blockSizes[]
	{
			0,
			defaultBlockSize / 4,
	};
	for (const auto blockSize : blockSizes)
	{
		const auto realBlockSize = blockSize != 0 ? blockSize : defaultBlockSize;
		DYNAMIC_SECTION("Testing with " << realBlockSize << " bytes block size" << (blockSize == 0 ? " (default)" : ""))
		{
			distortos::FatFileSystem ffs {blockDeviceMock, blockSize};

			ufat* ufatFileSystem {};

			REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
			if (blockSize == 0)
				expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence)
						.RETURN(realBlockSize));
			REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
					.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
			REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(ffs.mount() == 0);

			REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
			constexpr int ret {0x65b5900d};
			REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(ret);
			REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(ffs.unmount() == ret);
		}
	}
}

TEST_CASE("Testing getFileStatus()", "[getFileStatus]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};

	distortos::FatFileSystem ffs {blockDeviceMock};

	ufat* ufatFileSystem {};

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
	REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
			.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.mount() == 0);

	SECTION("ufat_dir_find_path() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};
		struct stat status {};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_BLOCK_SIZE);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.getFileStatus(path, status) == EINVAL);
	}
	SECTION("Using path to non-existing file/directory should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};
		struct stat status {};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.getFileStatus(path, status) == ENOENT);
	}
	SECTION("Testing with path to file")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};
		constexpr size_t fileSize {0x02bf76b3};
		struct stat status {};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).SIDE_EFFECT(_3->attributes = {}).SIDE_EFFECT(_3->file_size = fileSize).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.getFileStatus(path, status) == 0);
		REQUIRE(status.st_mode == S_IFREG);
		REQUIRE(status.st_size == fileSize);
	}
	SECTION("Testing with path to directory")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};
		struct stat status {};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).SIDE_EFFECT(_3->attributes = UFAT_ATTR_DIRECTORY).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.getFileStatus(path, status) == 0);
		REQUIRE(status.st_mode == S_IFDIR);
		REQUIRE(status.st_size == 0);
	}

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.unmount() == 0);
}

TEST_CASE("Testing getStatus()", "[getStatus]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};

	distortos::FatFileSystem ffs {blockDeviceMock};

	ufat* ufatFileSystem {};

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
	REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
			.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.mount() == 0);

	SECTION("ufat_count_free_clusters() error should propagate converted error code to caller")
	{
		struct statvfs status {};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_count_free_clusters(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.RETURN(-UFAT_ERR_INVALID_BPB);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.getStatus(status) == EILSEQ);
	}
	SECTION("Testing successful ufat_count_free_clusters()")
	{
		constexpr unsigned int blocksPerCluster {128};
		constexpr unsigned int clusters {0x71541c72};
		constexpr unsigned int freeClusters {0x26480c45};
		struct statvfs status {};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_count_free_clusters(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.SIDE_EFFECT(_1->bpb.log2_blocks_per_cluster = estd::log2u(blocksPerCluster))
				.SIDE_EFFECT(_1->bpb.num_clusters = clusters + 2).SIDE_EFFECT(*_2 = freeClusters).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.getStatus(status) == 0);
		REQUIRE(status.f_bsize == defaultBlockSize * blocksPerCluster);
		REQUIRE(status.f_frsize == status.f_bsize);
		REQUIRE(status.f_blocks == clusters);
		REQUIRE(status.f_bfree == freeClusters);
		REQUIRE(status.f_bavail == status.f_bfree);
		REQUIRE(status.f_namemax == UFAT_LFN_MAX_CHARS);
	}

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.unmount() == 0);
}

TEST_CASE("Testing makeDirectory()", "[makeDirectory]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};

	distortos::FatFileSystem ffs {blockDeviceMock};

	ufat* ufatFileSystem {};

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
	REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
			.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.mount() == 0);

	SECTION("ufat_dir_find_path() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_BLOCK_ALIGNMENT);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.makeDirectory(path, {}) == EFAULT);
	}
	SECTION("Using path to existing file/directory should fail with EEXIST")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.makeDirectory(path, {}) == EEXIST);
	}
	SECTION("Using path with non-existing components (except the last one) should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"existing-0/existing-1/non-existing-0/non-existing-1"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).SIDE_EFFECT(*_4 = strchr(strchr(path, '/') + 1, '/') + 1).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.makeDirectory(path, {}) == ENOENT);
	}
	SECTION("ufat_dir_create() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};
		const char** pathRemainder {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).LR_SIDE_EFFECT(pathRemainder = _4)
				.SIDE_EFFECT(*_4 = strrchr(path, '/') + 1).RETURN(1);
		REQUIRE_CALL(ufatMock, ufat_dir_create(_, _, _)).LR_WITH(_1 == ufatDirectory).LR_WITH(_2 == ufatDirectoryEntry)
				.LR_WITH(_3 == *pathRemainder).IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_INVALID_CLUSTER);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.makeDirectory(path, {}) == EFAULT);
	}

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.unmount() == 0);
}

TEST_CASE("Testing remove()", "[remove]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};

	distortos::FatFileSystem ffs {blockDeviceMock};

	ufat* ufatFileSystem {};

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
	REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
			.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.mount() == 0);

	SECTION("ufat_dir_find_path() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_NAME_TOO_LONG);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.remove(path) == ENAMETOOLONG);
	}
	SECTION("Using path to non-existing file/directory should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.remove(path) == ENOENT);
	}
	SECTION("ufat_dir_delete() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_dir_delete(ufatFileSystem, _)).LR_WITH(_2 == ufatDirectoryEntry)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_NOT_DIRECTORY);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.remove(path) == ENOTDIR);
	}

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.unmount() == 0);
}

TEST_CASE("Testing rename()", "[rename]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};

	distortos::FatFileSystem ffs {blockDeviceMock};

	ufat* ufatFileSystem {};

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
	REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
			.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.mount() == 0);

	SECTION("ufat_dir_find_path() error from first call should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};
		const char* const newPath {"new/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_NOT_FILE);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.rename(path, newPath) == EISDIR);
	}
	SECTION("Using path to non-existing file/directory should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};
		const char* const newPath {"new/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.rename(path, newPath) == ENOENT);
	}
	SECTION("ufat_dir_find_path() error from second call should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};

		const char* const path {"some/path"};
		const char* const newPath {"new/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, newPath, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.LR_WITH(_3 != ufatDirectoryEntry).IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_IMMUTABLE);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.rename(path, newPath) == EACCES);
	}
	SECTION("Using path to existing file/directory should fail with EEXIST")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};

		const char* const path {"some/path"};
		const char* const newPath {"new/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, newPath, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.LR_WITH(_3 != ufatDirectoryEntry).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.rename(path, newPath) == EEXIST);
	}
	SECTION("Using path with non-existing components (except the last one) should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};

		const char* const path {"some/path"};
		const char* const newPath {"existing-0/existing-1/non-existing-0/non-existing-1"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, newPath, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.LR_WITH(_3 != ufatDirectoryEntry).IN_SEQUENCE(sequence)
				.SIDE_EFFECT(*_4 = strchr(strchr(newPath, '/') + 1, '/') + 1).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.rename(path, newPath) == ENOENT);
	}
	SECTION("ufat_move() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};
		const char** newPathRemainder {};

		const char* const path {"some/path"};
		const char* const newPath {"new/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, newPath, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.LR_WITH(_3 != ufatDirectoryEntry).IN_SEQUENCE(sequence).LR_SIDE_EFFECT(newPathRemainder = _4)
				.SIDE_EFFECT(*_4 = strrchr(newPath, '/') + 1).RETURN(1);
		REQUIRE_CALL(ufatMock, ufat_move(_, _, _)).LR_WITH(_1 == ufatDirectoryEntry).LR_WITH(_2 == ufatDirectory)
				.LR_WITH(_3 == *newPathRemainder).IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_DIRECTORY_NOT_EMPTY);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(ffs.rename(path, newPath) == ENOTEMPTY);
	}

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.unmount() == 0);
}

TEST_CASE("Testing openDirectory()", "[openDirectory]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::FatFileSystem ffs {blockDeviceMock};

	ufat* ufatFileSystem {};

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
	REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
			.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.mount() == 0);

	SECTION("ufat_dir_find_path() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_ILLEGAL_NAME);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, directory] = ffs.openDirectory(path);
		REQUIRE(ret == EINVAL);
		REQUIRE(directory == nullptr);
	}
	SECTION("Using path to non-existing directory should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, directory] = ffs.openDirectory(path);
		REQUIRE(ret == ENOENT);
		REQUIRE(directory == nullptr);
	}
	SECTION("ufat_open_subdir() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_subdir(ufatFileSystem, _, _)).LR_WITH(_2 == ufatDirectory)
				.LR_WITH(_3 == ufatDirectoryEntry).IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_DIRECTORY_NOT_EMPTY);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, directory] = ffs.openDirectory(path);
		REQUIRE(ret == ENOTEMPTY);
		REQUIRE(directory == nullptr);
	}
	SECTION("Testing successful openDirectory()")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), nullptr)).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_subdir(ufatFileSystem, _, _)).LR_WITH(_2 == ufatDirectory)
				.LR_WITH(_3 == ufatDirectoryEntry).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [openRet, directory] = ffs.openDirectory(path);
		REQUIRE(openRet == 0);
		REQUIRE(directory != nullptr);

		SECTION("Testing getPosition()")
		{
			constexpr uint32_t expectedPosition {0x5e619529};
			ufatDirectory->cur_pos = expectedPosition;

			REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			const auto [ret, position] = directory->getPosition();
			REQUIRE(ret == 0);
			REQUIRE(position == expectedPosition);
		}
		SECTION("Testing read()")
		{
			SECTION("ufat_dir_read() error should propagate converted error code to caller")
			{
				dirent entry {};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock,
						ufat_dir_read(ufatDirectory, ne(nullptr), entry.d_name, static_cast<int>(sizeof(entry.d_name))))
						.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_FILE_EXISTS);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(directory->read(entry) == EEXIST);
			}
			SECTION("End-of-directory should be reported with ENOENT")
			{
				dirent entry {};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock,
						ufat_dir_read(ufatDirectory, ne(nullptr), entry.d_name, static_cast<int>(sizeof(entry.d_name))))
						.IN_SEQUENCE(sequence).RETURN(1);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(directory->read(entry) == ENOENT);
			}
			SECTION("Testing successful ufat_dir_read()")
			{
				dirent entry {};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock,
						ufat_dir_read(ufatDirectory, ne(nullptr), entry.d_name, static_cast<int>(sizeof(entry.d_name))))
						.IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(directory->read(entry) == 0);
			}
		}
		SECTION("Testing rewind()")
		{
			REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(ufatMock, ufat_dir_rewind(ufatDirectory)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(directory->rewind() == 0);
		}
		SECTION("Testing seek()")
		{
			SECTION("seek() to current position should perform no action")
			{
				constexpr uint32_t currentPosition {0x4d33409d};
				ufatDirectory->cur_pos = currentPosition;

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(directory->seek(currentPosition) == 0);
			}
			SECTION("ufat_dir_read() error should propagate converted error code to caller")
			{
				constexpr off_t position {0x0167af6f};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock, ufat_dir_rewind(ufatDirectory)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(ufatMock, ufat_dir_read(ufatDirectory, ne(nullptr), nullptr, 0)).IN_SEQUENCE(sequence)
						.RETURN(-UFAT_ERR_BAD_ENCODING);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(directory->seek(position) == EILSEQ);
			}
			SECTION("End-of-directory should be reported with ENOENT")
			{
				constexpr off_t position {0x10ac2096};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock, ufat_dir_rewind(ufatDirectory)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(ufatMock, ufat_dir_read(ufatDirectory, ne(nullptr), nullptr, 0)).IN_SEQUENCE(sequence)
						.RETURN(1);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(directory->seek(position) == ENOENT);
			}
			SECTION("Testing successful ufat_dir_read()")
			{
				constexpr off_t position {5};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock, ufat_dir_rewind(ufatDirectory)).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(_1->cur_pos = {});
				for (off_t currentPosition {}; currentPosition < position; ++currentPosition)
					expectations.emplace_back(NAMED_REQUIRE_CALL(ufatMock,
							ufat_dir_read(ufatDirectory, ne(nullptr), nullptr, 0)).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(++_1->cur_pos).RETURN(0));
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(directory->seek(position) == 0);
			}
		}

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(directory->close() == 0);
	}

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.unmount() == 0);
}

TEST_CASE("Testing openFile()", "[openFile]")
{
	BlockDevice blockDeviceMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	UfatMock ufatMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::FatFileSystem ffs {blockDeviceMock};

	ufat* ufatFileSystem {};

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(defaultBlockSize);
	REQUIRE_CALL(ufatMock, ufat_open(ne(nullptr), ne(nullptr))).IN_SEQUENCE(sequence)
			.LR_SIDE_EFFECT(ufatFileSystem = _1).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.mount() == 0);

	SECTION("ufat_dir_find_path() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_DIRECTORY_FULL);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, file] = ffs.openFile(path, O_RDWR);
		REQUIRE(ret == ENOSPC);
		REQUIRE(file == nullptr);
	}
	SECTION("Using path to existing file when O_EXCL is set should fail with EEXIST")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, file] = ffs.openFile(path, O_RDWR | O_CREAT | O_EXCL);
		REQUIRE(ret == EEXIST);
		REQUIRE(file == nullptr);
	}
	SECTION("Using path to non-existing file when O_CREAT is not set should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, file] = ffs.openFile(path, O_RDWR);
		REQUIRE(ret == ENOENT);
		REQUIRE(file == nullptr);
	}
	SECTION("Using path with non-existing components (except the last one) when O_CREAT is set should fail with ENOENT")
	{
		ufat_directory* ufatDirectory {};

		const char* const path {"existing-0/existing-1/non-existing-0/non-existing-1"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).SIDE_EFFECT(*_4 = strchr(strchr(path, '/') + 1, '/') + 1).RETURN(1);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, file] = ffs.openFile(path, O_RDWR | O_CREAT);
		REQUIRE(ret == ENOENT);
		REQUIRE(file == nullptr);
	}
	SECTION("ufat_dir_mkfile() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};
		const char** pathRemainder {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).LR_SIDE_EFFECT(pathRemainder = _4)
				.SIDE_EFFECT(*_4 = strrchr(path, '/') + 1).RETURN(1);
		REQUIRE_CALL(ufatMock, ufat_dir_mkfile(_, _, _)).LR_WITH(_1 == ufatDirectory).LR_WITH(_2 == ufatDirectoryEntry)
				.LR_WITH(_3 == *pathRemainder).IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_NO_CLUSTERS);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, file] = ffs.openFile(path, O_RDWR | O_CREAT);
		REQUIRE(ret == ENOSPC);
		REQUIRE(file == nullptr);
	}
	SECTION("ufat_open_file() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};
		const char** pathRemainder {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).LR_SIDE_EFFECT(pathRemainder = _4)
				.SIDE_EFFECT(*_4 = strrchr(path, '/') + 1).RETURN(1);
		REQUIRE_CALL(ufatMock, ufat_dir_mkfile(_, _, _)).LR_WITH(_1 == ufatDirectory).LR_WITH(_2 == ufatDirectoryEntry)
				.LR_WITH(_3 == *pathRemainder).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_file(ufatFileSystem, ne(nullptr), _)).LR_WITH(_3 == ufatDirectoryEntry)
				.IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_IO);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, file] = ffs.openFile(path, O_RDWR | O_CREAT);
		REQUIRE(ret == EIO);
		REQUIRE(file == nullptr);
	}
	SECTION("ufat_file_truncate() error should propagate converted error code to caller")
	{
		ufat_directory* ufatDirectory {};
		ufat_dirent* ufatDirectoryEntry {};
		ufat_file* ufatFile {};

		const char* const path {"some/path"};

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
				.LR_SIDE_EFFECT(ufatDirectory = _2);
		REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr))).LR_WITH(_1 == ufatDirectory)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_open_file(ufatFileSystem, ne(nullptr), _)).LR_WITH(_3 == ufatDirectoryEntry)
				.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatFile = _2).RETURN(0);
		REQUIRE_CALL(ufatMock, ufat_file_truncate(_)).LR_WITH(_1 == ufatFile).IN_SEQUENCE(sequence)
				.RETURN(-UFAT_ERR_BLOCK_SIZE);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		const auto [ret, file] = ffs.openFile(path, O_WRONLY | O_TRUNC);
		REQUIRE(ret == EINVAL);
		REQUIRE(file == nullptr);
	}
	SECTION("Testing successful open()")
	{
		static const std::tuple<bool, int, const char*> associations[]
		{
				// existing file, no additional flags
				{true, O_RDONLY, "O_RDONLY"},
				{true, O_WRONLY, "O_WRONLY"},
				{true, O_RDWR, "O_RDWR"},
				// existing file, with O_APPEND
				{true, O_RDONLY | O_APPEND, "O_RDONLY | O_APPEND"},
				{true, O_WRONLY | O_APPEND, "O_WRONLY | O_APPEND"},
				{true, O_RDWR | O_APPEND, "O_RDWR | O_APPEND"},
				// existing file, with O_CREAT
				{true, O_RDONLY | O_CREAT, "O_RDONLY | O_CREAT"},
				{true, O_WRONLY | O_CREAT, "O_WRONLY | O_CREAT"},
				{true, O_RDWR | O_CREAT, "O_RDWR | O_CREAT"},
				// existing file, with O_APPEND and O_CREAT
				{true, O_RDONLY | O_APPEND | O_CREAT, "O_RDONLY | O_APPEND | O_CREAT"},
				{true, O_WRONLY | O_APPEND | O_CREAT, "O_WRONLY | O_APPEND | O_CREAT"},
				{true, O_RDWR | O_APPEND | O_CREAT, "O_RDWR | O_APPEND | O_CREAT"},
				// existing file, with O_TRUNC
				{true, O_WRONLY | O_TRUNC, "O_WRONLY | O_TRUNC"},
				{true, O_RDWR | O_TRUNC, "O_RDWR | O_TRUNC"},
				// existing file, with O_APPEND and O_TRUNC
				{true, O_WRONLY | O_APPEND | O_TRUNC, "O_WRONLY | O_APPEND | O_TRUNC"},
				{true, O_RDWR | O_APPEND | O_TRUNC, "O_RDWR | O_APPEND | O_TRUNC"},
				// existing file, with O_CREAT and O_TRUNC
				{true, O_WRONLY | O_CREAT | O_TRUNC, "O_WRONLY | O_CREAT | O_TRUNC"},
				{true, O_RDWR | O_CREAT | O_TRUNC, "O_RDWR | O_CREAT | O_TRUNC"},
				// existing file, with O_APPEND, O_CREAT and O_TRUNC
				{true, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, "O_WRONLY | O_APPEND | O_CREAT | O_TRUNC"},
				{true, O_RDWR | O_APPEND | O_CREAT | O_TRUNC, "O_RDWR | O_APPEND | O_CREAT | O_TRUNC"},

				// non-existing file, with O_CREAT
				{false, O_RDONLY | O_CREAT, "O_RDONLY | O_CREAT"},
				{false, O_WRONLY | O_CREAT, "O_WRONLY | O_CREAT"},
				{false, O_RDWR | O_CREAT, "O_RDWR | O_CREAT"},
				// non-existing file, with O_APPEND and O_CREAT
				{false, O_RDONLY | O_APPEND | O_CREAT, "O_RDONLY | O_APPEND | O_CREAT"},
				{false, O_WRONLY | O_APPEND | O_CREAT, "O_WRONLY | O_APPEND | O_CREAT"},
				{false, O_RDWR | O_APPEND | O_CREAT, "O_RDWR | O_APPEND | O_CREAT"},
				// non-existing file, with O_CREAT and O_EXCL
				{false, O_RDONLY | O_CREAT | O_EXCL, "O_RDONLY | O_CREAT | O_EXCL"},
				{false, O_WRONLY | O_CREAT | O_EXCL, "O_WRONLY | O_CREAT | O_EXCL"},
				{false, O_RDWR | O_CREAT | O_EXCL, "O_RDWR | O_CREAT | O_EXCL"},
				// non-existing file, with O_APPEND, O_CREAT and O_EXCL
				{false, O_RDONLY | O_APPEND | O_CREAT | O_EXCL, "O_RDONLY | O_APPEND | O_CREAT | O_EXCL"},
				{false, O_WRONLY | O_APPEND | O_CREAT | O_EXCL, "O_WRONLY | O_APPEND | O_CREAT | O_EXCL"},
				{false, O_RDWR | O_APPEND | O_CREAT | O_EXCL, "O_RDWR | O_APPEND | O_CREAT | O_EXCL"},
				// non-existing file, with O_CREAT and O_TRUNC
				{false, O_WRONLY | O_CREAT | O_TRUNC, "O_WRONLY | O_CREAT | O_TRUNC"},
				{false, O_RDWR | O_CREAT | O_TRUNC, "O_RDWR | O_CREAT | O_TRUNC"},
				// non-existing file, with O_APPEND, O_CREAT and O_TRUNC
				{false, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, "O_WRONLY | O_APPEND | O_CREAT | O_TRUNC"},
				{false, O_RDWR | O_APPEND | O_CREAT | O_TRUNC, "O_RDWR | O_APPEND | O_CREAT | O_TRUNC"},
				// non-existing file, with O_CREAT, O_EXCL and O_TRUNC
				{false, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, "O_WRONLY | O_CREAT | O_EXCL | O_TRUNC"},
				{false, O_RDWR | O_CREAT | O_EXCL | O_TRUNC, "O_RDWR | O_CREAT | O_EXCL | O_TRUNC"},
				// non-existing file, with O_APPEND, O_CREAT, O_EXCL and O_TRUNC
				{false, O_WRONLY | O_APPEND | O_CREAT | O_EXCL, "O_WRONLY | O_APPEND | O_CREAT | O_EXCL | O_TRUNC"},
				{false, O_RDWR | O_APPEND | O_CREAT | O_EXCL | O_TRUNC,
						"O_RDWR | O_APPEND | O_CREAT | O_EXCL | O_TRUNC"},
		};
		for (const auto [existing, flags, description] : associations)
			DYNAMIC_SECTION("Testing successful open() of " << (existing == true ? "" : "non-") <<
					"existing file with \"" << description << "\" flags")
			{
				ufat_directory* ufatDirectory {};
				ufat_dirent* ufatDirectoryEntry {};
				const char** pathRemainder {};
				ufat_file* ufatFile {};

				const char* const path {"some/path"};

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock, ufat_open_root(ufatFileSystem, ne(nullptr))).IN_SEQUENCE(sequence)
						.LR_SIDE_EFFECT(ufatDirectory = _2);
				REQUIRE_CALL(ufatMock, ufat_dir_find_path(_, path, ne(nullptr), ne(nullptr)))
						.LR_WITH(_1 == ufatDirectory).IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatDirectoryEntry = _3)
						.LR_SIDE_EFFECT(pathRemainder = _4).SIDE_EFFECT(*_4 = strrchr(path, '/') + 1)
						.RETURN(existing == true ? 0 : 1);
				if (existing == false)
					expectations.emplace_back(NAMED_REQUIRE_CALL(ufatMock, ufat_dir_mkfile(_, _, _))
							.LR_WITH(_1 == ufatDirectory).LR_WITH(_2 == ufatDirectoryEntry)
							.LR_WITH(_3 == *pathRemainder).IN_SEQUENCE(sequence).RETURN(0));
				REQUIRE_CALL(ufatMock, ufat_open_file(ufatFileSystem, ne(nullptr), _)).LR_WITH(_3 == ufatDirectoryEntry)
						.IN_SEQUENCE(sequence).LR_SIDE_EFFECT(ufatFile = _2).RETURN(0);
				if ((flags & O_TRUNC) != 0)
					expectations.emplace_back(NAMED_REQUIRE_CALL(ufatMock, ufat_file_truncate(_))
							.LR_WITH(_1 == ufatFile).IN_SEQUENCE(sequence).RETURN(0));
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				const auto [openRet, file] = ffs.openFile(path, flags);
				REQUIRE(openRet == 0);
				REQUIRE(file != nullptr);

				SECTION("Testing getPosition()")
				{
					constexpr uint32_t expectedPosition {0x0a8df6ab};
					ufatFile->cur_pos = expectedPosition;

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					const auto [ret, position] = file->getPosition();
					REQUIRE(ret == 0);
					REQUIRE(position == expectedPosition);
				}
				SECTION("Testing getSize()")
				{
					constexpr uint32_t expectedSize {0x58bdc8a9};
					ufatFile->file_size = expectedSize;

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					const auto [ret, size] = file->getSize();
					REQUIRE(ret == 0);
					REQUIRE(size == expectedSize);
				}
				SECTION("Testing getStatus()")
				{
					constexpr uint32_t expectedSize {0x4cf77f5a};
					struct stat status {};
					ufatFile->file_size = expectedSize;

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE(file->getStatus(status) == 0);
					REQUIRE(status.st_mode == S_IFREG);
					REQUIRE(status.st_size == expectedSize);
				}
				SECTION("Testing isATerminal()")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					const auto [ret, isATerminal] = file->isATerminal();
					REQUIRE(ret == 0);
					REQUIRE(isATerminal == false);
				}
				SECTION("Testing rewind()")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(ufatMock, ufat_file_rewind(ufatFile)).IN_SEQUENCE(sequence);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE(file->rewind() == 0);
				}
				SECTION("Testing synchronize()")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(ufatMock, ufat_sync(ufatFileSystem)).IN_SEQUENCE(sequence)
							.RETURN(-UFAT_ERR_BLOCK_ALIGNMENT);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE(file->synchronize() == EFAULT);
				}

				constexpr int mask {O_RDONLY | O_WRONLY | O_RDWR};
				const auto readOnly = (flags & mask) == O_RDONLY;
				const auto writeOnly = (flags & mask) == O_WRONLY;
				const auto readWrite = (flags & mask) == O_RDWR;
				const auto readable = readOnly == true || readWrite == true;
				const auto writable = writeOnly == true || readWrite == true;
				const auto appendMode = (flags & O_APPEND) != 0;

				if (readable == true)
				{
					SECTION("ufat_file_read() error should propagate converted error code to caller")
					{
						uint8_t buffer[16] {};

						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(ufatMock, ufat_file_read(ufatFile, buffer, sizeof(buffer))).IN_SEQUENCE(sequence)
								.RETURN(-UFAT_ERR_INVALID_CLUSTER);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						const auto [ret, bytesRead] = file->read(buffer, sizeof(buffer));
						REQUIRE(ret == EFAULT);
						REQUIRE(bytesRead == 0);
					}
					SECTION("Testing successful read()")
					{
						uint8_t buffer[15] {};
						constexpr size_t expectedBytesRead {sizeof(buffer) / 2};

						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(ufatMock, ufat_file_read(ufatFile, buffer, sizeof(buffer))).IN_SEQUENCE(sequence)
								.RETURN(expectedBytesRead);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						const auto [ret, bytesRead] = file->read(buffer, sizeof(buffer));
						REQUIRE(ret == 0);
						REQUIRE(bytesRead == expectedBytesRead);
					}
				}

				if (writable == false)
				{
					SECTION("Writing read-only file should fail with EBADF")
					{
						const uint8_t buffer[123] {};

						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						const auto [ret, bytesWritten] = file->write(buffer, sizeof(buffer));
						REQUIRE(ret == EBADF);
						REQUIRE(bytesWritten == 0);
					}
				}
				else
				{
					if (appendMode == true)
					{
						SECTION("ufat_file_advance() error should propagate converted error code to caller")
						{
							const uint8_t buffer[125] {};
							constexpr uint32_t position {0x1cfc16d0};
							constexpr uint32_t size {0x2f35d42d};

							ufatFile->cur_pos = position;
							ufatFile->file_size = size;

							REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(ufatMock, ufat_file_advance(ufatFile, size - position)).IN_SEQUENCE(sequence)
									.RETURN(-UFAT_ERR_NAME_TOO_LONG);
							REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
							const auto [ret, bytesWritten] = file->write(buffer, sizeof(buffer));
							REQUIRE(ret == ENAMETOOLONG);
							REQUIRE(bytesWritten == 0);
						}
					}

					SECTION("ufat_file_write() error should propagate converted error code to caller")
					{
						const uint8_t buffer[120] {};
						constexpr uint32_t position {0x83af6cb4};
						constexpr uint32_t size {0xc7992345};

						ufatFile->cur_pos = position;
						ufatFile->file_size = size;

						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						if (appendMode == true)
							expectations.emplace_back(NAMED_REQUIRE_CALL(ufatMock,
									ufat_file_advance(ufatFile, size - position)).IN_SEQUENCE(sequence).RETURN(0));
						REQUIRE_CALL(ufatMock, ufat_file_write(ufatFile, buffer, sizeof(buffer))).IN_SEQUENCE(sequence)
								.RETURN(-UFAT_ERR_NOT_DIRECTORY);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						const auto [ret, bytesWritten] = file->write(buffer, sizeof(buffer));
						REQUIRE(ret == ENOTDIR);
						REQUIRE(bytesWritten == 0);
					}
					SECTION("Testing successful write()")
					{
						const uint8_t buffer[79] {};
						constexpr uint32_t position {0xa798095f};
						constexpr uint32_t size {0xf0bbcf3f};
						constexpr size_t expectedBytesWritten {sizeof(buffer) / 2};

						ufatFile->cur_pos = position;
						ufatFile->file_size = size;

						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						if (appendMode == true)
							expectations.emplace_back(NAMED_REQUIRE_CALL(ufatMock,
									ufat_file_advance(ufatFile, size - position)).IN_SEQUENCE(sequence).RETURN(0));
						REQUIRE_CALL(ufatMock, ufat_file_write(ufatFile, buffer, sizeof(buffer))).IN_SEQUENCE(sequence)
								.RETURN(expectedBytesWritten);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						const auto [ret, bytesWritten] = file->write(buffer, sizeof(buffer));
						REQUIRE(ret == 0);
						REQUIRE(bytesWritten == expectedBytesWritten);
					}
				}
				static const std::pair<Whence, const char*> whenceAssociations[]
				{
						{Whence::beginning, "Whence::beginning"},
						{Whence::current, "Whence::current"},
						{Whence::end, "Whence::end"},
				};
				for (const auto [whence, whenceDescription] : whenceAssociations)
					DYNAMIC_SECTION("Testing seek(" << whenceDescription << ", ...)")
					{
						constexpr uint32_t position {0x3fc06b6e};
						constexpr uint32_t size {0x7839d2b0};

						ufatFile->cur_pos = position;
						ufatFile->file_size = size;

						SECTION("seek() which ends at negative position should fail with EINVAL")
						{
							const auto offset = -(whence == Whence::beginning ? off_t{} :
									whence == Whence::current ? off_t{position} : off_t{size}) - 1;

							REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE(file->seek(whence, offset).first == EINVAL);
						}
						SECTION("seek() to current position should perform no action")
						{
							const auto offset = whence == Whence::beginning ? off_t{position} :
									whence == Whence::current ? off_t{} : -off_t{size - position};

							REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
							const auto [ret, newPosition] = file->seek(whence, offset);
							REQUIRE(ret == 0);
							REQUIRE(newPosition == position);
						}
						if (writable == false)
							SECTION("seek() past the end-of-file should fail with EBADF")
							{
								const auto offset = (whence == Whence::beginning ? off_t{size} :
										whence == Whence::current ? off_t{size - position} : off_t{}) + 1;

								REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
								REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
								REQUIRE(file->seek(whence, offset).first == EBADF);
							}
						SECTION("ufat_file_advance() error should propagate converted error code to caller")
						{
							const auto offset = whence == Whence::beginning ? off_t{1} :
									whence == Whence::current ? -off_t{position} + 1 : -off_t{size} + 1;

							REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(ufatMock, ufat_file_rewind(ufatFile)).IN_SEQUENCE(sequence);
							REQUIRE_CALL(ufatMock, ufat_file_advance(ufatFile, 1u)).IN_SEQUENCE(sequence)
									.RETURN(-UFAT_ERR_NOT_FILE);
							REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE(file->seek(whence, offset).first == EISDIR);
						}
						if (writable == true)
							SECTION("ufat_file_write() error should propagate converted error code to caller")
							{
								const auto offset = (whence == Whence::beginning ? off_t{size} :
										whence == Whence::current ? off_t{size - position} : off_t{}) + 1;

								REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
								REQUIRE_CALL(ufatMock, ufat_file_rewind(ufatFile)).IN_SEQUENCE(sequence);
								REQUIRE_CALL(ufatMock, ufat_file_advance(ufatFile, size))
										.IN_SEQUENCE(sequence).RETURN(0);
								REQUIRE_CALL(ufatMock, ufat_file_write(ufatFile, ne(nullptr), 1u))
										.WITH(*reinterpret_cast<const uint8_t*>(_2) == 0).IN_SEQUENCE(sequence)
										.RETURN(-UFAT_ERR_IMMUTABLE);
								REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
								REQUIRE(file->seek(whence, offset).first == EACCES);
							}
						SECTION("Testing successful seek()")
						{
							const auto offset = whence == Whence::beginning ? off_t{size} :
									whence == Whence::current ? off_t{size - position} : off_t{};
							const decltype(offset) pastTheEnd {5 * writable};

							REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(ufatMock, ufat_file_rewind(ufatFile)).IN_SEQUENCE(sequence);
							REQUIRE_CALL(ufatMock, ufat_file_advance(ufatFile, size)).IN_SEQUENCE(sequence).RETURN(0);
							for (off_t i {}; i < pastTheEnd; ++i)
								expectations.emplace_back(NAMED_REQUIRE_CALL(ufatMock,
										ufat_file_write(ufatFile, ne(nullptr), 1u))
										.WITH(*reinterpret_cast<const uint8_t*>(_2) == 0).IN_SEQUENCE(sequence)
										.RETURN(_3));
							REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
							const auto [ret, newPosition] = file->seek(whence, offset + pastTheEnd);
							REQUIRE(ret == 0);
							REQUIRE(newPosition == size + pastTheEnd);
						}
					}

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(ufatMock, ufat_sync(ufatFileSystem)).IN_SEQUENCE(sequence).RETURN(-UFAT_ERR_INVALID_BPB);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(file->close() == EILSEQ);
			}
	}

	REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(ufatMock, ufat_close(ufatFileSystem)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(ffs.unmount() == 0);
}
