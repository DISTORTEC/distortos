/**
 * \file
 * \brief LittlefsDirectory class implementation
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/FileSystem/littlefs/LittlefsDirectory.hpp"

#include "littlefsErrorToErrorCode.hpp"

#include "distortos/FileSystem/littlefs/LittlefsFileSystem.hpp"

#include "distortos/assert.h"

#include <mutex>

#include <cstring>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

LittlefsDirectory::~LittlefsDirectory()
{
	assert(opened_ == false);
}

int LittlefsDirectory::close()
{
	const std::lock_guard<LittlefsDirectory> lockGuard {*this};

	assert(opened_ == true);

	opened_ = {};
	const auto ret = lfs_dir_close(&fileSystem_.fileSystem_, &directory_);
	return littlefsErrorToErrorCode(ret);
}

std::pair<int, off_t> LittlefsDirectory::getPosition()
{
	const std::lock_guard<LittlefsDirectory> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs_dir_tell(&fileSystem_.fileSystem_, &directory_);
	if (ret < 0)
		return {littlefsErrorToErrorCode(ret), {}};

	return {{}, ret};
}

void LittlefsDirectory::lock()
{
	fileSystem_.lock();
}

std::pair<int, struct dirent> LittlefsDirectory::read()
{
	const std::lock_guard<LittlefsDirectory> lockGuard {*this};

	assert(opened_ == true);

	lfs_info info;
	const auto ret = lfs_dir_read(&fileSystem_.fileSystem_, &directory_, &info);
	if (ret < 0)
		return {littlefsErrorToErrorCode(ret), {}};

	if (ret == 0)
		return {ENOENT, {}};

	dirent entry {};
	static_assert(sizeof(entry.d_name) >= sizeof(info.name),
			"Size of entry.d_name must be greater than or equal to size of info.name to safely use strcpy()");
	strcpy(entry.d_name, info.name);
	return {{}, entry};
}

int LittlefsDirectory::rewind()
{
	const std::lock_guard<LittlefsDirectory> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs_dir_rewind(&fileSystem_.fileSystem_, &directory_);
	return littlefsErrorToErrorCode(ret);
}

int LittlefsDirectory::seek(const off_t position)
{
	const std::lock_guard<LittlefsDirectory> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs_dir_seek(&fileSystem_.fileSystem_, &directory_, position);
	if (ret < 0)
		return littlefsErrorToErrorCode(ret);

	return ret;
}

void LittlefsDirectory::unlock()
{
	fileSystem_.unlock();
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int LittlefsDirectory::open(const char* const path)
{
	assert(opened_ == false);
	assert(path != nullptr);

	const auto ret = lfs_dir_open(&fileSystem_.fileSystem_, &directory_, path);
	if (ret == LFS_ERR_OK)
		opened_ = true;
	return littlefsErrorToErrorCode(ret);
}

}	// namespace distortos
