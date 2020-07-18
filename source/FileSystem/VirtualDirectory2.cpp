/**
 * \file
 * \brief VirtualDirectory2 class implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VirtualDirectory2.hpp"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/assert.h"

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

VirtualDirectory2::~VirtualDirectory2()
{
	assert(isOpened() == false);
}

int VirtualDirectory2::close()
{
	assert(isOpened() == true);

	const auto ret = directory_->close();
	directory_.reset();
	mountPointSharedPointer_ = {};
	return ret;
}

std::pair<int, off_t> VirtualDirectory2::getPosition()
{
	assert(isOpened() == true);
	return directory_->getPosition();
}

void VirtualDirectory2::lock()
{
	assert(isOpened() == true);
	directory_->lock();
}

int VirtualDirectory2::read(dirent& entry)
{
	assert(isOpened() == true);
	return directory_->read(entry);
}

int VirtualDirectory2::rewind()
{
	assert(isOpened() == true);
	return directory_->rewind();
}

int VirtualDirectory2::seek(const off_t position)
{
	assert(isOpened() == true);
	return directory_->seek(position);
}

void VirtualDirectory2::unlock()
{
	assert(isOpened() == true);
	directory_->unlock();
}

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
