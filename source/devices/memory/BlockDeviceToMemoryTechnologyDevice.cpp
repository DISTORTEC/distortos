/**
 * \file
 * \brief BlockDeviceToMemoryTechnologyDevice class implementation
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/BlockDeviceToMemoryTechnologyDevice.hpp"

#include "distortos/devices/memory/BlockDevice.hpp"

#include "distortos/assert.h"

#ifndef DISTORTOS_UNIT_TEST

#include "distortos/distortosConfiguration.h"

#endif	// !def DISTORTOS_UNIT_TEST

#include <mutex>

namespace distortos
{

namespace devices
{

#ifndef DISTORTOS_UNIT_TEST

static_assert(CONFIG_BLOCKDEVICE_BUFFER_ALIGNMENT <= CONFIG_MEMORYTECHNOLOGYDEVICE_BUFFER_ALIGNMENT,
		"Buffer alignment for BlockDevice is greater than for MemoryTechnologyDevice!");

#endif	// !def DISTORTOS_UNIT_TEST

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

BlockDeviceToMemoryTechnologyDevice::~BlockDeviceToMemoryTechnologyDevice()
{
	const std::lock_guard<BlockDeviceToMemoryTechnologyDevice> lockGuard {*this};

	assert(openCount_ == 0);
}

int BlockDeviceToMemoryTechnologyDevice::close()
{
	const std::lock_guard<BlockDeviceToMemoryTechnologyDevice> lockGuard {*this};

	assert(openCount_ != 0);

	if (openCount_ == 1)	// last close?
	{
		if (pendingEraseSize_ != 0)
		{
			const auto ret = blockDevice_.erase(pendingEraseAddress_, pendingEraseSize_);
			if (ret != 0)
				return ret;

			pendingEraseAddress_ = {};
			pendingEraseSize_ = {};
		}

		const auto ret = blockDevice_.close();
		if (ret != 0)
			return ret;
	}

	--openCount_;
	return 0;
}

int BlockDeviceToMemoryTechnologyDevice::erase(const uint64_t address, const uint64_t size)
{
	const std::lock_guard<BlockDeviceToMemoryTechnologyDevice> lockGuard {*this};

	assert(openCount_ != 0);

	if (size == 0)
		return {};

	const auto blockSize = blockDevice_.getBlockSize();
	if (address % blockSize != 0 || size % blockSize != 0)
		return EINVAL;

	if (address + size > blockDevice_.getSize())
		return ENOSPC;

	if (pendingEraseSize_ != 0)
	{
		const auto mergedAddress = std::min(address, pendingEraseAddress_);
		const auto mergedSize = std::max(address + size, pendingEraseAddress_ + pendingEraseSize_) - mergedAddress;
		if (mergedSize <= size + pendingEraseSize_)	// ranges can be merged if they overlap or are adjacent
		{
			pendingEraseAddress_ = mergedAddress;
			pendingEraseSize_ = mergedSize;
			return {};
		}

		// ranges cannot be merged so flush the pending erase
		const auto ret = blockDevice_.erase(pendingEraseAddress_, pendingEraseSize_);
		if (ret != 0)
			return ret;
	}

	pendingEraseAddress_ = address;
	pendingEraseSize_ = size;
	return {};
}

size_t BlockDeviceToMemoryTechnologyDevice::getEraseBlockSize() const
{
	return blockDevice_.getBlockSize();
}

size_t BlockDeviceToMemoryTechnologyDevice::getProgramBlockSize() const
{
	return blockDevice_.getBlockSize();
}

size_t BlockDeviceToMemoryTechnologyDevice::getReadBlockSize() const
{
	return blockDevice_.getBlockSize();
}

uint64_t BlockDeviceToMemoryTechnologyDevice::getSize() const
{
	return blockDevice_.getSize();
}

void BlockDeviceToMemoryTechnologyDevice::lock()
{
	const auto ret = blockDevice_.lock();
	assert(ret == 0);
}

int BlockDeviceToMemoryTechnologyDevice::open()
{
	const std::lock_guard<BlockDeviceToMemoryTechnologyDevice> lockGuard {*this};

	if (openCount_ == std::numeric_limits<decltype(openCount_)>::max())	// device is already opened too many times?
		return EMFILE;

	if (openCount_ == 0)	// first open?
	{
		const auto ret = blockDevice_.open();
		if (ret != 0)
			return ret;
	}

	++openCount_;
	return 0;
}

int BlockDeviceToMemoryTechnologyDevice::program(const uint64_t address, const void* const buffer, const size_t size)
{
	const std::lock_guard<BlockDeviceToMemoryTechnologyDevice> lockGuard {*this};

	assert(openCount_ != 0);

	if (size == 0)
		return {};

	if (buffer == nullptr)
		return EINVAL;

	const auto blockSize = blockDevice_.getBlockSize();
	if (address % blockSize != 0 || size % blockSize != 0)
		return EINVAL;

	if (address + size > blockDevice_.getSize())
		return ENOSPC;

	if (pendingEraseSize_ != 0 && address > pendingEraseAddress_ &&
			address + size < pendingEraseAddress_ + pendingEraseSize_)
	{
		// Data should be written in the middle of the pending erase range, so the erase range would have to be split in
		// two separate parts, which is not possible. Flush part of the pending erase which is below the written data,
		// based on assumption that the part above is more likely to be written soon.

		const auto eraseSize = address - pendingEraseAddress_;
		const auto ret = blockDevice_.erase(pendingEraseAddress_, eraseSize);
		if (ret != 0)
			return ret;

		pendingEraseAddress_ += eraseSize;
		pendingEraseSize_ -= eraseSize;
	}

	const auto ret = blockDevice_.write(address, buffer, size);
	if (ret != 0)
		return ret;

	const auto overlapBegin = std::max(address, pendingEraseAddress_);
	const auto overlapEnd = std::min(address + size, pendingEraseAddress_ + pendingEraseSize_);
	if (overlapBegin < overlapEnd)
	{
		if (pendingEraseAddress_ == overlapBegin)
			pendingEraseAddress_ = overlapEnd;
		pendingEraseSize_ -= overlapEnd - overlapBegin;
	}

	return {};
}

int BlockDeviceToMemoryTechnologyDevice::read(const uint64_t address, void* const buffer, const size_t size)
{
	const std::lock_guard<BlockDeviceToMemoryTechnologyDevice> lockGuard {*this};

	assert(openCount_ != 0);

	if (size == 0)
		return {};

	if (buffer == nullptr)
		return EINVAL;

	const auto blockSize = blockDevice_.getBlockSize();
	if (address % blockSize != 0 || size % blockSize != 0)
		return EINVAL;

	if (address + size > blockDevice_.getSize())
		return ENOSPC;

	const auto overlapBegin = std::max(address, pendingEraseAddress_);
	const auto overlapEnd = std::min(address + size, pendingEraseAddress_ + pendingEraseSize_);
	if (overlapBegin < overlapEnd)
	{
		uint64_t eraseAddress;
		uint64_t eraseSize;
		if (overlapBegin == pendingEraseAddress_ || overlapEnd == pendingEraseAddress_ + pendingEraseSize_)
		{
			// Overlapping range is adjacent to the beginning or end of pending erase range. Just erase the
			// overlapping range.

			eraseAddress = overlapBegin;
			eraseSize = overlapEnd - overlapBegin;
		}
		else
		{
			// Data will be read from the middle of the pending erase range, so the erase range would have to be split
			// in two separate parts, which is not possible. Flush part of the pending erase which is below the read
			// data as well as the overlapping range. The choice is made only based on consistency with program()
			// behaviour.

			eraseAddress = pendingEraseAddress_;
			eraseSize = overlapEnd - pendingEraseAddress_;
		}

		const auto ret = blockDevice_.erase(eraseAddress, eraseSize);
		if (ret != 0)
			return ret;

		if (pendingEraseAddress_ == eraseAddress)	// adjacent to the beginning of pending erase range?
			pendingEraseAddress_ += eraseSize;
		pendingEraseSize_ -= eraseSize;
	}

	return blockDevice_.read(address, buffer, size);
}

int BlockDeviceToMemoryTechnologyDevice::synchronize()
{
	const std::lock_guard<BlockDeviceToMemoryTechnologyDevice> lockGuard {*this};

	if (pendingEraseSize_ != 0)
	{
		const auto ret = blockDevice_.erase(pendingEraseAddress_, pendingEraseSize_);
		if (ret != 0)
			return ret;

		pendingEraseAddress_ = {};
		pendingEraseSize_ = {};
	}

	return blockDevice_.synchronize();
}

void BlockDeviceToMemoryTechnologyDevice::unlock()
{
	const auto ret = blockDevice_.unlock();
	assert(ret == 0);
}

}	// namespace devices

}	// namespace distortos
