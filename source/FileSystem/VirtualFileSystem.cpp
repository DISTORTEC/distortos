/**
 * \file
 * \brief VirtualFileSystem class implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/FileSystem/VirtualFileSystem.hpp"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "VirtualDirectory2.hpp"
#include "VirtualFile.hpp"

#include "distortos/FileSystem/FileSystem.hpp"

#include "estd/ContiguousRange.hpp"
#include "estd/ScopeGuard.hpp"

#include <mutex>

#include <cstring>

namespace distortos
{

namespace internal
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Splits path into a mount point name and the remaining suffix (with initial slashes skipped).
 *
 * \pre \a path is valid.
 *
 * \param [in] path is the path that will be split, must be valid
 *
 * \return pair with mount point name (as contiguous range) and remaining suffix
 */

std::pair<estd::ContiguousRange<const char>, const char*> splitPath(const char* const path)
{
	assert(path != nullptr);

	auto nameBegin = path;
	while (*nameBegin == '/')
		++nameBegin;

	auto suffixBegin = strchr(nameBegin, '/');
	if (suffixBegin == nullptr)
		return {{nameBegin, strlen(nameBegin)}, ""};	// path consists of just the mount point name

	const estd::ContiguousRange<const char> nameRange {nameBegin, suffixBegin};

	while (*suffixBegin == '/')
		++suffixBegin;

	return {nameRange, suffixBegin};
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int VirtualFileSystem::getFileStatus(const char* const path, struct stat& status)
{
	estd::ContiguousRange<const char> nameRange;
	const char* suffix;
	std::tie(nameRange, suffix) = splitPath(path);
	if (*suffix == '\0')	// there is just the mount point name, so no status can be returned anyway
		return ENOENT;

	const auto mountPointSharedPointer = getMountPointSharedPointer(nameRange.begin(), nameRange.size());
	if (mountPointSharedPointer == false)
		return ENOENT;

	return mountPointSharedPointer->getFileSystem().getFileStatus(suffix, status);
}

int VirtualFileSystem::getStatus(const char* const path, struct statvfs& status)
{
	estd::ContiguousRange<const char> nameRange;
	std::tie(nameRange, std::ignore) = splitPath(path);

	const auto mountPointSharedPointer = getMountPointSharedPointer(nameRange.begin(), nameRange.size());
	if (mountPointSharedPointer == false)
		return ENOENT;

	return mountPointSharedPointer->getFileSystem().getStatus(status);
}

int VirtualFileSystem::makeDirectory(const char* const path, const mode_t mode)
{
	estd::ContiguousRange<const char> nameRange;
	const char* suffix;
	std::tie(nameRange, suffix) = splitPath(path);
	if (*suffix == '\0')	// there is just the mount point name, so no directory can be created anyway
		return ENOENT;

	const auto mountPointSharedPointer = getMountPointSharedPointer(nameRange.begin(), nameRange.size());
	if (mountPointSharedPointer == false)
		return ENOENT;

	return mountPointSharedPointer->getFileSystem().makeDirectory(suffix, mode);
}

int VirtualFileSystem::mount(FileSystem& fileSystem, const char* const path)
{
	estd::ContiguousRange<const char> nameRange;
	const char* suffix;
	std::tie(nameRange, suffix) = splitPath(path);

	assert(*suffix == '\0');

	const auto name = nameRange.begin();
	const auto length = nameRange.size();
	assert(length <= MountPoint::maxNameLength);
	for (size_t i {}; i < length; ++i)
		assert(std::isalnum(name[i]) != 0);

	{
		const auto ret = fileSystem.mount();
		if (ret != 0)
			return ret;
	}

	auto unmountScopeGuard = estd::makeScopeGuard(
			[&fileSystem]()
			{
				fileSystem.unmount();
			});

	std::unique_ptr<MountPoint> mountPoint {new (std::nothrow) MountPoint{fileSystem, name, length}};
	if (mountPoint == nullptr)
		return ENOMEM;

	std::unique_ptr<MountPointListNode> node {new (std::nothrow) MountPointListNode{std::move(mountPoint)}};
	if (node == nullptr)
		return ENOMEM;

	{
		const std::lock_guard<Mutex> lockGuard {mutex_};
		mountPoints_.push_back(*node.release());
	}

	unmountScopeGuard.release();
	return {};
}

std::pair<int, std::unique_ptr<Directory>> VirtualFileSystem::openDirectory(const char* const path)
{
	estd::ContiguousRange<const char> nameRange;
	const char* suffix;
	std::tie(nameRange, suffix) = splitPath(path);

	auto mountPointSharedPointer = getMountPointSharedPointer(nameRange.begin(), nameRange.size());
	if (mountPointSharedPointer == false)
		return {ENOENT, std::unique_ptr<Directory>{}};

	std::unique_ptr<Directory> directory;

	{
		int ret;
		std::tie(ret, directory) = mountPointSharedPointer->getFileSystem().openDirectory(suffix);
		if (ret != 0)
			return {ret, std::unique_ptr<Directory>{}};
	}

	std::unique_ptr<Directory> virtualDirectory2 {new (std::nothrow) VirtualDirectory2{std::move(directory),
			std::move(mountPointSharedPointer)}};
	if (virtualDirectory2 == nullptr)
		return {ENOMEM, std::unique_ptr<Directory>{}};

	return {{}, std::move(virtualDirectory2)};
}

std::pair<int, std::unique_ptr<File>> VirtualFileSystem::openFile(const char* const path, const int flags)
{
	estd::ContiguousRange<const char> nameRange;
	const char* suffix;
	std::tie(nameRange, suffix) = splitPath(path);
	if (*suffix == '\0')	// there is just the mount point name, so no file can be opened anyway
		return {ENOENT, std::unique_ptr<File>{}};

	auto mountPointSharedPointer = getMountPointSharedPointer(nameRange.begin(), nameRange.size());
	if (mountPointSharedPointer == false)
		return {ENOENT, std::unique_ptr<File>{}};

	std::unique_ptr<File> file;

	{
		int ret;
		std::tie(ret, file) = mountPointSharedPointer->getFileSystem().openFile(suffix, flags);
		if (ret != 0)
			return {ret, std::unique_ptr<File>{}};
	}

	std::unique_ptr<File> virtualFile {new (std::nothrow) VirtualFile{std::move(file),
			std::move(mountPointSharedPointer)}};
	if (virtualFile == nullptr)
		return {ENOMEM, std::unique_ptr<File>{}};

	return {{}, std::move(virtualFile)};
}

int VirtualFileSystem::remove(const char* const path)
{
	estd::ContiguousRange<const char> nameRange;
	const char* suffix;
	std::tie(nameRange, suffix) = splitPath(path);
	if (*suffix == '\0')	// there is just the mount point name, so no file can be removed anyway
		return ENOENT;

	const auto mountPointSharedPointer = getMountPointSharedPointer(nameRange.begin(), nameRange.size());
	if (mountPointSharedPointer == false)
		return ENOENT;

	return mountPointSharedPointer->getFileSystem().remove(suffix);
}

int VirtualFileSystem::unmount(const char* const path, const bool detach)
{
	estd::ContiguousRange<const char> nameRange;
	const char* suffix;
	std::tie(nameRange, suffix) = splitPath(path);

	assert(*suffix == '\0');

	const auto name = nameRange.begin();
	const auto length = nameRange.size();

	std::unique_ptr<MountPointListNode> node;

	{
		const std::lock_guard<Mutex> lockGuard {mutex_};

		const auto iterator = std::find_if(mountPoints_.begin(), mountPoints_.end(),
				[name, length](const MountPointSharedPointer& entry) -> bool
				{
					const auto entryName = entry->getName();
					return strlen(entryName) == length && memcmp(entryName, name, length) == 0;
				});

		assert(iterator != mountPoints_.end());
		if (detach == false && iterator->get()->getReferenceCount() != 1)
			return EBUSY;

		mountPoints_.erase(iterator);
		node.reset(&*iterator);
	}

	return {};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

MountPointSharedPointer VirtualFileSystem::getMountPointSharedPointer(const char* const name, const size_t length)
{
	assert(name != nullptr);

	const std::lock_guard<Mutex> lockGuard {mutex_};

	const auto iterator = std::find_if(mountPoints_.begin(), mountPoints_.end(),
			[name, length](const MountPointSharedPointer& entry) -> bool
			{
				const auto entryName = entry->getName();
				return strlen(entryName) == length && memcmp(entryName, name, length) == 0;
			});

	if (iterator == mountPoints_.end())
		return {};

	return *iterator;
}

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
