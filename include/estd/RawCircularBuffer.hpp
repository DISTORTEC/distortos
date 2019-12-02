/**
 * \file
 * \brief RawCircularBuffer class header
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_RAWCIRCULARBUFFER_HPP_
#define ESTD_RAWCIRCULARBUFFER_HPP_

#include <utility>

#include <cstddef>
#include <cstdint>

namespace estd
{

/**
 * \brief Thread-safe, lock-free raw circular buffer for single-producer and single-consumer
 *
 * Distinction between empty and full buffer is possible because most significant bits of read and write positions are
 * used as single-bit counters of wrap-arounds. This limits the size of buffer to SIZE_MAX / 2, but allows full
 * utilization of storage (no free slot is needed).
 */

class RawCircularBuffer
{
public:

	/**
	 * \brief RawCircularBuffer's constructor
	 *
	 * \param [in] buffer is a buffer for data
	 * \param [in] size is the size of \a buffer, bytes, must be less than or equal to SIZE_MAX / 2
	 */

	constexpr RawCircularBuffer(void* const buffer, const size_t size) :
			buffer_{static_cast<uint8_t*>(buffer)},
			size_{size & sizeMask_},
			readPosition_{},
			writePosition_{}
	{

	}

	/**
	 * \brief RawCircularBuffer's constructor, read-only variant
	 *
	 * \param [in] buffer is a read-only buffer with data
	 * \param [in] size is the size of \a buffer, bytes, must be less than or equal to SIZE_MAX / 2
	 */

	constexpr RawCircularBuffer(const void* const buffer, const size_t size) :
			buffer_{static_cast<uint8_t*>(const_cast<void*>(buffer))},
			size_{(size & sizeMask_) | readOnlyMask_},
			readPosition_{},
			writePosition_{}
	{

	}

	/**
	 * \brief Clears raw circular buffer
	 */

	void clear()
	{
		readPosition_ = {};
		writePosition_ = {};
	}

	/**
	 * \return total capacity of raw circular buffer, bytes
	 */

	size_t getCapacity() const
	{
		return size_ & sizeMask_;
	}

	/**
	 * \return first contiguous block (as a pair with pointer and size) available for reading
	 */

	std::pair<const void*, size_t> getReadBlock() const
	{
		const auto readPosition = readPosition_ ;
		const auto writePosition = writePosition_;
		if (isEmpty(readPosition, writePosition) == true)
			return {{}, {}};
		return getBlock(readPosition, writePosition);
	}

	/**
	 * \return total amount of valid data in raw circular buffer, bytes
	 */

	size_t getSize() const
	{
		const auto readPosition = readPosition_ ;
		const auto writePosition = writePosition_;
		if (isEmpty(readPosition, writePosition) == true)
			return 0;
		const auto capacity = getCapacity();
		if (isFull(readPosition, writePosition) == true)
			return capacity;
		return (capacity - (readPosition & positionMask_) + (writePosition & positionMask_)) % capacity;
	}

	/**
	 * \return first contiguous block (as a pair with pointer and size) available for writing
	 */

	std::pair<void*, size_t> getWriteBlock() const
	{
		if (isReadOnly() == true)
			return {{}, {}};

		const auto readPosition = readPosition_ ;
		const auto writePosition = writePosition_;
		if (isFull(readPosition, writePosition) == true)
			return {{}, {}};
		return getBlock(writePosition, readPosition);
	}

	/**
	 * \brief Increases read position by given value.
	 *
	 * \param [in] value is the value which will be added to read position, must be less than or equal to the value from
	 * last call to getReadBlock()
	 */

	void increaseReadPosition(const size_t value)
	{
		readPosition_ = increasePosition(readPosition_, value);
	}

	/**
	 * \brief Increases write position by given value.
	 *
	 * \param [in] value is the value which will be added to write position, must be less than or equal to the value
	 * from last call to getWriteBlock()
	 */

	void increaseWritePosition(const size_t value)
	{
		writePosition_ = increasePosition(writePosition_, value);
	}

	/**
	 * \return true if raw circular buffer is empty, false otherwise
	 */

	bool isEmpty() const
	{
		return isEmpty(readPosition_, writePosition_);
	}

	/**
	 * \return true if raw circular buffer is full, false otherwise
	 */

	bool isFull() const
	{
		return isFull(readPosition_, writePosition_);
	}

	/**
	 * \return true if raw circular buffer is read-only, false otherwise
	 */

	bool isReadOnly() const
	{
		return (size_ & readOnlyMask_) != 0;
	}

private:

	/**
	 * \brief Gets first contiguous block between \a position1 and \a position2.
	 *
	 * This function does not treat empty or full buffer in any special way.
	 *
	 * \param [in] begin is the beginning position
	 * \param [in] end is the ending position
	 *
	 * \return first contiguous block (as a pair with pointer and size) starting at \a begin and not crossing \a end or
	 * end of buffer
	 */

	std::pair<void*, size_t> getBlock(const size_t begin, const size_t end) const
	{
		const auto maskedBegin = begin & positionMask_;
		const auto maskedEnd = end & positionMask_;
		return {buffer_ + maskedBegin, (maskedEnd > maskedBegin ? maskedEnd : getCapacity()) - maskedBegin};
	}

	/**
	 * \brief Increases given position by given value.
	 *
	 * \param [in] position is the position that will be incremented
	 * \param [in] value is the value which will be added to \a position, must be less than or equal to the value from
	 * last call to getReadBlock() / getWriteBlock()
	 *
	 * \return \a position incremented by \a value
	 */

	size_t increasePosition(const size_t position, const size_t value)
	{
		const auto maskedPosition = position & positionMask_;
		const auto msb = position & msbMask_;
		// in case of wrap-around MSB is inverted and position is 0
		return maskedPosition + value != getCapacity() ? msb | (maskedPosition + value) : msb ^ msbMask_;
	}

	/**
	 * \brief Tests for empty raw circular buffer.
	 *
	 * The buffer is empty if read and write positions are equal, including their MSBs.
	 *
	 * \param [in] readPosition is the value of \a readPosition_
	 * \param [in] writePosition is the value of \a writePosition_
	 *
	 * \return true if raw circular buffer is empty, false otherwise
	 */

	constexpr static bool isEmpty(const size_t readPosition, const size_t writePosition)
	{
		return readPosition == writePosition;
	}

	/**
	 * \brief Tests for full raw circular buffer.
	 *
	 * The buffer is full if masked read and write positions are equal, but their MSBs are different.
	 *
	 * \param [in] readPosition is the value of \a readPosition_
	 * \param [in] writePosition is the value of \a writePosition_
	 *
	 * \return true if raw circular buffer is full, false otherwise
	 */

	constexpr static bool isFull(const size_t readPosition, const size_t writePosition)
	{
		return (readPosition ^ writePosition) == msbMask_;
	}

	/// bitmask used to extract position from \a readPosition_ or \a writePosition_
	constexpr static size_t positionMask_ {SIZE_MAX >> 1};

	/// bitmask used to extract MSB from \a readPosition_ or \a writePosition_
	constexpr static size_t msbMask_ {~positionMask_};

	/// bitmask used to extract size from \a size_
	constexpr static size_t sizeMask_ {SIZE_MAX >> 1};

	/// bitmask used to extract "read-only" flag from \a size_
	constexpr static size_t readOnlyMask_ {~sizeMask_};

	/// pointer to beginning of buffer
	uint8_t* buffer_;

	/// size of \a buffer_, bytes
	size_t size_;

	/// current read position
	volatile size_t readPosition_;

	/// current write position
	volatile size_t writePosition_;
};

}	// namespace estd

#endif	// ESTD_RAWCIRCULARBUFFER_HPP_
