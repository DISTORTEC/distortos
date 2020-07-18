/**
 * \file
 * \brief MountPoint class implementation
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/internal/FileSystem/MountPoint.hpp"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/FileSystem/FileSystem.hpp"

#include "distortos/assert.h"

#include <mutex>

#include <cstring>

namespace distortos
{

namespace internal
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

MountPoint::MountPoint(FileSystem& fileSystem, const char* const name, const size_t length) :
		mutex_{Mutex::Protocol::priorityInheritance},
		fileSystem_{fileSystem},
		name_{},
		referenceCount_{}
{
	assert(length <= maxNameLength);
	memcpy(name_, name, length);
}

MountPoint::~MountPoint()
{
	assert(referenceCount_ == 0);
}

void MountPoint::decrementReferenceCount()
{
	{
		const std::lock_guard<Mutex> lockGuard {mutex_};

		assert(referenceCount_ != 0);
		--referenceCount_;

		if (referenceCount_ != 0)
			return;
	}

	// referenceCount_ == 0

	fileSystem_.unmount();
	delete this;
}

void MountPoint::incrementReferenceCount()
{
	const std::lock_guard<Mutex> lockGuard {mutex_};

	assert(referenceCount_ != std::numeric_limits<decltype(referenceCount_)>::max());
	++referenceCount_;
}

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1
