/**
 * \file
 * \brief AligningBlockDevice class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/AligningBlockDevice.hpp"

#include "distortos/assert.h"

#ifndef DISTORTOS_UNIT_TEST

#include "distortos/distortosConfiguration.h"

#endif	// !def DISTORTOS_UNIT_TEST

#include <mutex>

#include <cstring>

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

AligningBlockDevice::~AligningBlockDevice()
{
	assert(openCount_ == 0);
}

int AligningBlockDevice::close()
{
	const std::lock_guard<AligningBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);

	int ret {};
	if (openCount_ == 1)	// last close?
		ret = blockDevice_.close();

	--openCount_;
	return ret;
}

int AligningBlockDevice::erase(const uint64_t address, const uint64_t size)
{
	return blockDevice_.erase(address, size);
}

size_t AligningBlockDevice::getBlockSize() const
{
	return blockDevice_.getBlockSize();
}

uint64_t AligningBlockDevice::getSize() const
{
	return blockDevice_.getSize();
}

void AligningBlockDevice::lock()
{
	blockDevice_.lock();
}

int AligningBlockDevice::open()
{
	const std::lock_guard<AligningBlockDevice> lockGuard {*this};

	assert(openCount_ < std::numeric_limits<decltype(openCount_)>::max());

	if (openCount_ == 0)	// first open?
	{
		const auto ret = blockDevice_.open();
		if (ret != 0)
			return ret;

		assert(reinterpret_cast<uintptr_t>(buffer_) % DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT == 0);
		const auto blockSize = blockDevice_.getBlockSize();
		assert(bufferSize_ >= blockSize);
		assert(bufferSize_ % blockSize == 0);
	}

	++openCount_;
	return 0;
}

int AligningBlockDevice::read(const uint64_t address, void* const buffer, const size_t size)
{
	const std::lock_guard<AligningBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);
	assert(buffer != nullptr);

	const auto blockSize = blockDevice_.getBlockSize();
	assert(address % blockSize == 0 && size % blockSize == 0);
	assert(address + size <= blockDevice_.getSize());

	if (size == 0)
		return {};

	if (reinterpret_cast<uintptr_t>(buffer) % DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT == 0)	// buffer already aligned?
		return blockDevice_.read(address, buffer, size);

	size_t bytesRead {};
	while (bytesRead < size)
	{
		const auto chunk = std::min(size - bytesRead, bufferSize_);
		const auto ret = blockDevice_.read(address + bytesRead, buffer_, chunk);
		if (ret != 0)
			return ret;

		memcpy(static_cast<uint8_t*>(buffer) + bytesRead, buffer_, chunk);
		bytesRead += chunk;
	}

	return {};
}

int AligningBlockDevice::synchronize()
{
	return blockDevice_.synchronize();
}

void AligningBlockDevice::unlock()
{
	blockDevice_.unlock();
}

int AligningBlockDevice::write(const uint64_t address, const void* const buffer, const size_t size)
{
	const std::lock_guard<AligningBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);
	assert(buffer != nullptr);

	const auto blockSize = blockDevice_.getBlockSize();
	assert(address % blockSize == 0 && size % blockSize == 0);
	assert(address + size <= blockDevice_.getSize());

	if (size == 0)
		return {};

	if (reinterpret_cast<uintptr_t>(buffer) % DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT == 0)	// buffer already aligned?
		return blockDevice_.write(address, buffer, size);

	size_t bytesWritten {};
	while (bytesWritten < size)
	{
		const auto chunk = std::min(size - bytesWritten, bufferSize_);
		memcpy(buffer_, static_cast<const uint8_t*>(buffer) + bytesWritten, chunk);
		const auto ret = blockDevice_.write(address + bytesWritten, buffer_, chunk);
		if (ret != 0)
			return ret;

		bytesWritten += chunk;
	}

	return {};
}

}	// namespace devices

}	// namespace distortos
