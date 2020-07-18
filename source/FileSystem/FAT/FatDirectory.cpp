/**
 * \file
 * \brief FatDirectory class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "FatDirectory.hpp"

#include "ufatErrorToErrorCode.hpp"

#include "distortos/FileSystem/FatFileSystem.hpp"

#include "distortos/assert.h"

#include <mutex>

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

FatDirectory::~FatDirectory()
{
	assert(opened_ == false);
}

int FatDirectory::close()
{
	const std::lock_guard<FatDirectory> lockGuard {*this};

	assert(opened_ == true);

	opened_ = {};
	return {};
}

std::pair<int, off_t> FatDirectory::getPosition()
{
	const std::lock_guard<FatDirectory> lockGuard {*this};

	assert(opened_ == true);

	return {{}, directory_.cur_pos};
}

void FatDirectory::lock()
{
	fileSystem_.lock();
}

int FatDirectory::open(const char* const path)
{
	assert(opened_ == false);
	assert(path != nullptr);

	ufat_open_root(&fileSystem_.fileSystem_, &directory_);
	ufat_dirent directoryEntry {};
	{
		const auto ret = ufat_dir_find_path(&directory_, path, &directoryEntry, {});
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
		if (ret > 0)
			return ENOENT;
	}
	const auto ret = ufat_open_subdir(&fileSystem_.fileSystem_, &directory_, &directoryEntry);
	if (ret < 0)
		return ufatErrorToErrorCode(ret);

	opened_ = true;
	return {};
}

int FatDirectory::read(dirent& entry)
{
	const std::lock_guard<FatDirectory> lockGuard {*this};

	assert(opened_ == true);

	entry = {};
	ufat_dirent directoryEntry {};
	const auto ret = ufat_dir_read(&directory_, &directoryEntry, entry.d_name, sizeof(entry.d_name));
	if (ret < 0)
		return ufatErrorToErrorCode(ret);
	if (ret > 0)
		return ENOENT;

	return {};
}

int FatDirectory::rewind()
{
	const std::lock_guard<FatDirectory> lockGuard {*this};

	assert(opened_ == true);

	ufat_dir_rewind(&directory_);
	return {};
}

int FatDirectory::seek(const off_t position)
{
	const std::lock_guard<FatDirectory> lockGuard {*this};

	assert(opened_ == true);

	if (directory_.cur_pos == static_cast<decltype(directory_.cur_pos)>(position))
		return {};

	ufat_dir_rewind(&directory_);
	while (directory_.cur_pos != static_cast<decltype(directory_.cur_pos)>(position))
	{
		ufat_dirent directoryEntry {};
		const auto ret = ufat_dir_read(&directory_, &directoryEntry, {}, {});
		if (ret < 0)
			return ufatErrorToErrorCode(ret);
		if (ret > 0)
			return ENOENT;
	}

	return {};
}

void FatDirectory::unlock()
{
	fileSystem_.unlock();
}

}	// namespace distortos
