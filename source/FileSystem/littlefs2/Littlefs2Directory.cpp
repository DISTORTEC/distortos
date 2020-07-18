/**
 * \file
 * \brief Littlefs2Directory class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Littlefs2Directory.hpp"

#include "littlefs2ErrorToErrorCode.hpp"

#include "distortos/FileSystem/Littlefs2FileSystem.hpp"

#include "distortos/assert.h"

#include <mutex>

#include <cstring>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

Littlefs2Directory::~Littlefs2Directory()
{
	assert(opened_ == false);
}

int Littlefs2Directory::close()
{
	const std::lock_guard<Littlefs2Directory> lockGuard {*this};

	assert(opened_ == true);

	opened_ = {};
	const auto ret = lfs2_dir_close(&fileSystem_.fileSystem_, &directory_);
	return littlefs2ErrorToErrorCode(ret);
}

std::pair<int, off_t> Littlefs2Directory::getPosition()
{
	const std::lock_guard<Littlefs2Directory> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs2_dir_tell(&fileSystem_.fileSystem_, &directory_);
	if (ret < 0)
		return {littlefs2ErrorToErrorCode(ret), {}};

	return {{}, ret};
}

void Littlefs2Directory::lock()
{
	fileSystem_.lock();
}

int Littlefs2Directory::open(const char* const path)
{
	assert(opened_ == false);
	assert(path != nullptr);

	const auto ret = lfs2_dir_open(&fileSystem_.fileSystem_, &directory_, path);
	if (ret < 0)
		return littlefs2ErrorToErrorCode(ret);

	opened_ = true;
	return {};
}

int Littlefs2Directory::read(dirent& entry)
{
	const std::lock_guard<Littlefs2Directory> lockGuard {*this};

	assert(opened_ == true);

	lfs2_info info;
	const auto ret = lfs2_dir_read(&fileSystem_.fileSystem_, &directory_, &info);
	if (ret < 0)
		return littlefs2ErrorToErrorCode(ret);

	if (ret == 0)
		return ENOENT;

	entry = {};
	static_assert(sizeof(entry.d_name) >= sizeof(info.name),
			"Size of entry.d_name must be greater than or equal to size of info.name to safely use strcpy()");
	strcpy(entry.d_name, info.name);
	return {};
}

int Littlefs2Directory::rewind()
{
	const std::lock_guard<Littlefs2Directory> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs2_dir_rewind(&fileSystem_.fileSystem_, &directory_);
	return littlefs2ErrorToErrorCode(ret);
}

int Littlefs2Directory::seek(const off_t position)
{
	const std::lock_guard<Littlefs2Directory> lockGuard {*this};

	assert(opened_ == true);

	const auto ret = lfs2_dir_seek(&fileSystem_.fileSystem_, &directory_, position);
	if (ret < 0)
		return littlefs2ErrorToErrorCode(ret);

	return ret;
}

void Littlefs2Directory::unlock()
{
	fileSystem_.unlock();
}

}	// namespace distortos
