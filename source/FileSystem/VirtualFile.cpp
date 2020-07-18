/**
 * \file
 * \brief VirtualFile class implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VirtualFile.hpp"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/assert.h"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

VirtualFile::~VirtualFile()
{
	assert(isOpened() == false);
}

int VirtualFile::close()
{
	assert(isOpened() == true);

	const auto ret = file_->close();
	file_.reset();
	mountPointSharedPointer_ = {};
	return ret;
}

std::pair<int, off_t> VirtualFile::getPosition()
{
	assert(isOpened() == true);
	return file_->getPosition();
}

std::pair<int, off_t> VirtualFile::getSize()
{
	assert(isOpened() == true);
	return file_->getSize();
}

int VirtualFile::getStatus(struct stat& status)
{
	assert(isOpened() == true);
	return file_->getStatus(status);
}

std::pair<int, bool> VirtualFile::isATerminal()
{
	assert(isOpened() == true);
	return file_->isATerminal();
}

void VirtualFile::lock()
{
	assert(isOpened() == true);
	file_->lock();
}

std::pair<int, size_t> VirtualFile::read(void* const buffer, const size_t size)
{
	assert(isOpened() == true);
	return file_->read(buffer, size);
}

int VirtualFile::rewind()
{
	assert(isOpened() == true);
	return file_->rewind();
}

std::pair<int, off_t> VirtualFile::seek(const Whence whence, const off_t offset)
{
	assert(isOpened() == true);
	return file_->seek(whence, offset);
}

int VirtualFile::synchronize()
{
	assert(isOpened() == true);
	return file_->synchronize();
}

void VirtualFile::unlock()
{
	assert(isOpened() == true);
	file_->unlock();
}

std::pair<int, size_t> VirtualFile::write(const void* const buffer, const size_t size)
{
	assert(isOpened() == true);
	return file_->write(buffer, size);
}

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
