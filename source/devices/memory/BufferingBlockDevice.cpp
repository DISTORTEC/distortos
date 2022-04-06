/**
 * \file
 * \brief BufferingBlockDevice class implementation
 *
 * \author Copyright (C) 2019-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/BufferingBlockDevice.hpp"

#include "AddressRange.hpp"

#ifndef DISTORTOS_UNIT_TEST

#include "distortos/distortosConfiguration.h"

#endif	// !def DISTORTOS_UNIT_TEST

#include <mutex>

#include <cassert>
#include <cstring>

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

BufferingBlockDevice::~BufferingBlockDevice()
{
	assert(openCount_ == 0);
}

int BufferingBlockDevice::close()
{
	const std::lock_guard<BufferingBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);

	int ret {};
	if (openCount_ == 1)	// last close?
	{
		const auto flushRet = flushWriteBuffer();
		// make sure both buffers are invalidated even if flushing fails
		readBufferValid_ = {};
		writeBufferValidSize_ = {};
		const auto closeRet = blockDevice_.close();
		ret = flushRet != 0 ? flushRet : closeRet;
	}

	--openCount_;
	return ret;
}

int BufferingBlockDevice::erase(const uint64_t address, const uint64_t size)
{
	const std::lock_guard<BufferingBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);

	const auto blockSize = blockDevice_.getBlockSize();
	assert(address % blockSize == 0 && size % blockSize == 0);
	assert(address + size <= blockDevice_.getSize());

	if (size == 0)
		return {};

	const AddressRange eraseRange {address, size};

	{
		const AddressRange readBufferRange {readBufferAddress_, readBufferSize_ * readBufferValid_};
		const auto intersection = eraseRange & readBufferRange;
		if (intersection.size() != 0)
			readBufferValid_ = {};
	}
	{
		const AddressRange writeBufferRange {writeBufferAddress_, writeBufferValidSize_};
		const auto intersection = eraseRange & writeBufferRange;
		if (intersection.size() != 0)
		{
			if (intersection == writeBufferRange)
			{
				// erase range completely overlaps write buffer - drop it
				writeBufferValidSize_ = {};
			}
			else if (intersection.begin() == writeBufferRange.begin())
			{
				// erase range overlaps front part of write buffer - resize it and shift its contents
				memmove(writeBuffer_, static_cast<const uint8_t*>(writeBuffer_) + intersection.size(),
						writeBufferValidSize_ - intersection.size());
				writeBufferAddress_ += intersection.size();
				writeBufferValidSize_ -= intersection.size();
			}
			else if (intersection.end() == writeBufferRange.end())
			{
				// erase range overlaps back part of write buffer - resize it
				writeBufferValidSize_ -= intersection.size();
			}
			else
			{
				// erase range overlaps middle part of write buffer - flush the front part, resize it and shift the back
				// part
				const auto ret = flushWriteBuffer(intersection.begin() - writeBufferRange.begin());
				if (ret != 0)
					return ret;

				memmove(writeBuffer_, static_cast<const uint8_t*>(writeBuffer_) + intersection.size(),
						writeBufferValidSize_ - intersection.size());
				writeBufferAddress_ += intersection.size();
				writeBufferValidSize_ -= intersection.size();
			}
		}
	}

	return blockDevice_.erase(address, size);
}

size_t BufferingBlockDevice::getBlockSize() const
{
	return blockDevice_.getBlockSize();
}

uint64_t BufferingBlockDevice::getSize() const
{
	return blockDevice_.getSize();
}

void BufferingBlockDevice::lock()
{
	blockDevice_.lock();
}

int BufferingBlockDevice::open()
{
	const std::lock_guard<BufferingBlockDevice> lockGuard {*this};

	assert(openCount_ < std::numeric_limits<decltype(openCount_)>::max());

	if (openCount_ == 0)	// first open?
	{
		const auto ret = blockDevice_.open();
		if (ret != 0)
			return ret;

		assert(reinterpret_cast<uintptr_t>(readBuffer_) % DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT == 0);
		assert(reinterpret_cast<uintptr_t>(writeBuffer_) % DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT == 0);
		const auto blockSize = blockDevice_.getBlockSize();
		assert(readBufferSize_ >= blockSize);
		assert(readBufferSize_ % blockSize == 0);
		assert(writeBufferSize_ >= blockSize);
		assert(writeBufferSize_ % blockSize == 0);
	}

	++openCount_;
	return {};
}

int BufferingBlockDevice::read(const uint64_t address, void* const buffer, const size_t size)
{
	const std::lock_guard<BufferingBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);
	assert(buffer != nullptr);

	const auto blockSize = blockDevice_.getBlockSize();
	const auto deviceSize = blockDevice_.getSize();
	assert(address % blockSize == 0 && size % blockSize == 0);
	assert(address + size <= deviceSize);

	if (size == 0)
		return {};

	const AddressRange readRange {address, size};
	const AddressRange readBufferRange {readBufferAddress_, readBufferSize_ * readBufferValid_};
	const auto intersection0 = readRange & readBufferRange;
	const auto frontSize0 = intersection0.size() != 0 ? intersection0.begin() - readRange.begin() : 0;
	const auto backSize0 = readRange.size() - intersection0.size() - frontSize0;
	const AddressRange chunks0[]
	{
			{intersection0},	// middle part
			{readRange.begin(), frontSize0},	// front part
			{readRange.begin() + frontSize0 + intersection0.size(), backSize0},	// back part
	};
	for (auto& chunk0 : chunks0)
	{
		if (chunk0.size() == 0)
			continue;

		const AddressRange writeBufferRange {writeBufferAddress_, writeBufferValidSize_};
		const auto intersection1 = chunk0 & writeBufferRange;
		const auto frontSize1 = intersection1.size() != 0 ? intersection1.begin() - chunk0.begin() : 0;
		const auto backSize1 = chunk0.size() - intersection1.size() - frontSize1;
		const AddressRange chunks1[]
		{
				{chunk0.begin(), frontSize1},	// front part
				{chunk0.begin() + frontSize1 + intersection1.size(), backSize1},	// back part
		};

		if (intersection1.size() != 0)
		{
			const auto sourceOffset = intersection1.begin() - writeBufferRange.begin();
			const auto destinationOffset = intersection1.begin() - readRange.begin();
			memcpy(static_cast<uint8_t*>(buffer) + destinationOffset,
					static_cast<const uint8_t*>(writeBuffer_) + sourceOffset, intersection1.size());
		}

		for (auto& chunk1 : chunks1)
		{
			if (chunk1.size() == 0)
				continue;

			const auto offset = chunk1.begin() - address;
			const auto ret = readImplementation(chunk1.begin(), static_cast<uint8_t*>(buffer) + offset, chunk1.size(),
					deviceSize);
			if (ret != 0)
				return ret;
		}
	}

	return {};
}

int BufferingBlockDevice::synchronize()
{
	const std::lock_guard<BufferingBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);

	const auto ret = flushWriteBuffer();
	if (ret != 0)
		return ret;

	return blockDevice_.synchronize();
}

void BufferingBlockDevice::unlock()
{
	blockDevice_.unlock();
}

int BufferingBlockDevice::write(uint64_t address, const void* buffer, size_t size)
{
	const std::lock_guard<BufferingBlockDevice> lockGuard {*this};

	assert(openCount_ != 0);
	assert(buffer != nullptr);

	const auto blockSize = blockDevice_.getBlockSize();
	assert(address % blockSize == 0 && size % blockSize == 0);
	assert(address + size <= blockDevice_.getSize());

	if (size == 0)
		return {};

	while (size > 0)
	{
		const AddressRange writeBufferRange {writeBufferAddress_, writeBufferValidSize_};

		if (writeBufferRange.size() != 0)
		{
			const AddressRange writeRange {address, size};
			const auto intersection = writeBufferRange & writeRange;
			const auto unionn = writeBufferRange | writeRange;

			if (unionn == writeRange)	// drop
			{
				// write range completely overlaps write buffer - drop it
				writeBufferValidSize_ = {};
			}
			else if (writeBufferRange.size() < unionn.size() && writeBufferRange.size() < writeBufferSize_)
			{
				// write buffer may be expanded to accomodate more data
				const auto newValidSize = std::min<decltype(unionn.size())>(unionn.size(), writeBufferSize_);
				const auto offset = std::min(writeBufferRange.begin() - unionn.begin(),
						newValidSize - writeBufferValidSize_);
				if (offset != 0)	// shift current contents if expanding the front part
				{
					memmove(static_cast<uint8_t*>(writeBuffer_) + offset, writeBuffer_, writeBufferRange.size());
					writeBufferAddress_ -= offset;
				}

				writeBufferValidSize_ = newValidSize;
			}
			else if (intersection.size() != 0)
			{
				// front or back part of write range overlaps write buffer
				assert(intersection.begin() == writeRange.begin() || intersection.end() == writeRange.end());
				const auto sourceOffset = intersection.begin() - writeRange.begin();
				const auto destinationOffset = intersection.begin() - writeBufferRange.begin();
				const auto chunk = intersection.size();
				memcpy(static_cast<uint8_t*>(writeBuffer_) + destinationOffset,
						static_cast<const uint8_t*>(buffer) + sourceOffset, chunk);
				size -= chunk;
				if (sourceOffset == 0)
				{
					address += chunk;
					buffer = static_cast<const uint8_t*>(buffer) + chunk;
				}
			}
			else
			{
				const auto ret = flushWriteBuffer();
				if (ret != 0)
					return ret;
			}
		}
		else
		{
			const auto chunk = std::min(size, writeBufferSize_);
			memcpy(writeBuffer_, buffer, chunk);
			writeBufferAddress_ = address;
			writeBufferValidSize_ = chunk;
			address += chunk;
			buffer = static_cast<const uint8_t*>(buffer) + chunk;
			size -= chunk;
		}
	}

	return {};
}

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int BufferingBlockDevice::flushWriteBuffer(const size_t size)
{
	const auto chunk = std::min(size, writeBufferValidSize_);
	if (chunk == 0)
		return {};

	const auto ret = blockDevice_.write(writeBufferAddress_, writeBuffer_, chunk);
	if (ret != 0)
		return ret;

	const AddressRange readBufferRange {readBufferAddress_, readBufferSize_ * readBufferValid_};
	const AddressRange writeBufferRange {writeBufferAddress_, chunk};
	const auto intersection = readBufferRange & writeBufferRange;
	if (intersection.size() != 0)
	{
		const auto sourceOffset = intersection.begin() - writeBufferRange.begin();
		const auto destinationOffset = intersection.begin() - readBufferRange.begin();
		memcpy(static_cast<uint8_t*>(readBuffer_) + destinationOffset,
				static_cast<const uint8_t*>(writeBuffer_) + sourceOffset, intersection.size());
	}

	writeBufferAddress_ += chunk;
	writeBufferValidSize_ -= chunk;
	if (writeBufferValidSize_ != 0)
		memmove(writeBuffer_, static_cast<const uint8_t*>(writeBuffer_) + chunk, writeBufferValidSize_);
	return {};
}

int BufferingBlockDevice::readImplementation(uint64_t address, void* buffer, size_t size, const uint64_t deviceSize)
{
	while (size > 0)
	{
		const AddressRange readRange {address, size};
		const AddressRange readBufferRange {readBufferAddress_, readBufferSize_ * readBufferValid_};
		const auto intersection = readRange & readBufferRange;

		if (intersection.size() != 0)
		{
			assert(intersection.begin() == readRange.begin());

			const auto sourceOffset = intersection.begin() - readBufferRange.begin();
			const auto chunk = intersection.size();
			memcpy(buffer, static_cast<uint8_t*>(readBuffer_) + sourceOffset, chunk);
			address += chunk;
			buffer = static_cast<uint8_t*>(buffer) + chunk;
			size -= chunk;
		}
		else
		{
			decltype(readBufferAddress_) newBufferAddress = address;
			if (newBufferAddress > deviceSize - readBufferSize_)
			{
				// clip the address so that the read does not extend beyond device size
				newBufferAddress = deviceSize - readBufferSize_;
			}
			if (readBufferValid_ == true && readBufferAddress_ > newBufferAddress &&
					readBufferAddress_ - newBufferAddress < readBufferSize_)
			{
				// if new read buffer would be below current read buffer, make sure that the new range doesn't overlap
				// the old one (unless we rached the beginning of the device, in which case this is not possible)
				if (readBufferAddress_ > readBufferSize_)
					newBufferAddress = readBufferAddress_ - readBufferSize_;
				else
					newBufferAddress = {};
			}

			readBufferValid_ = {};	// make sure to invalidate current read buffer

			const auto ret = blockDevice_.read(newBufferAddress, readBuffer_, readBufferSize_);
			if (ret != 0)
				return ret;

			readBufferAddress_ = newBufferAddress;
			readBufferValid_ = true;
		}
	}

	return {};
}

}	// namespace devices

}	// namespace distortos
