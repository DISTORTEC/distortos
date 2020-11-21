/**
 * \file
 * \brief CircularBuffer class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_CIRCULARBUFFER_HPP_
#define ESTD_CIRCULARBUFFER_HPP_

#include <memory>

namespace estd
{

/**
 * \brief Thread-safe, lock-free circular buffer for single-producer and single-consumer
 *
 * Distinction between empty and full buffer is possible because most significant bits of read and write positions are
 * used as single-bit counters of wrap-arounds. This limits the capacity of buffer to SIZE_MAX / 2 elements, but allows
 * full utilization of storage (no free slot is needed).
 *
 * \tparam T is the type of data in circular buffer
 * \tparam Deleter is the templated deleter which will be used for disposing of storage when circular buffer is
 * destructed, should be either estd::DummyDeleter for static storage or `std::default_delete` for dynamic storage
 */

template<typename T, template<typename> class Deleter>
class CircularBuffer
{
public:

	/// type of uninitialized storage for data
	using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	/// unique_ptr (with deleter) to Storage[]
	using StorageUniquePointer = std::unique_ptr<Storage[], Deleter<Storage[]>>;

	/// type of data in circular buffer
	using ValueType = T;

	/**
	 * \brief CircularBuffer's constructor
	 *
	 * \param [in] storageUniquePointer is a rvalue reference to StorageUniquePointer with storage for circular buffer
	 * elements (sufficiently large for \a capacity elements, each sizeof(T) bytes long) and appropriate deleter
	 * \param [in] capacity is the number of elements in storage array, should be less than or equal to SIZE_MAX / 2
	 */

	constexpr CircularBuffer(StorageUniquePointer&& storageUniquePointer, const size_t capacity) :
			storageUniquePointer_{std::move(storageUniquePointer)},
			capacity_{capacity & positionMask_},
			readPosition_{},
			writePosition_{}
	{

	}

	/**
	 * \brief CircularBuffer's destructor
	 *
	 * Destructs all elements remaining in the circular buffer.
	 */

	~CircularBuffer()
	{
		while (isEmpty() == false)
			pop();
	}

	/**
	 * \brief Emplaces the element in circular buffer.
	 *
	 * \pre Circular buffer is not full.
	 *
	 * \tparam Args are types of arguments for constructor of T
	 *
	 * \param [in] args are arguments for constructor of T
	 */

	template<typename... Args>
	void emplace(Args&&... args)
	{
		const auto writePosition = writePosition_;
		const auto storage = getStorage(writePosition);
		new (storage) T{std::forward<Args>(args)...};
		writePosition_ = incrementPosition(writePosition);
	}

	/**
	 * \pre Circular buffer is not empty.
	 *
	 * \return const reference to first element on the list
	 */

	const T& front() const
	{
		return *reinterpret_cast<T*>(getStorage(readPosition_));
	}

	/**
	 * \return maximum number of elements in circular buffer
	 */

	size_t getCapacity() const
	{
		return capacity_;
	}

	/**
	 * \return total amount of valid elements in circular buffer
	 */

	size_t getSize() const
	{
		const auto readPosition = readPosition_ ;
		const auto writePosition = writePosition_;
		if (isEmpty(readPosition, writePosition) == true)
			return {};
		const auto capacity = getCapacity();
		if (isFull(readPosition, writePosition) == true)
			return capacity;
		return (capacity - (readPosition & positionMask_) + (writePosition & positionMask_)) % capacity;
	}

	/**
	 * \return true if circular buffer is empty, false otherwise
	 */

	bool isEmpty() const
	{
		return isEmpty(readPosition_, writePosition_);
	}

	/**
	 * \return true if circular buffer is full, false otherwise
	 */

	bool isFull() const
	{
		return isFull(readPosition_, writePosition_);
	}

	/**
	 * \brief Pops the oldest (first) element from circular buffer.
	 *
	 * \pre Circular buffer is not empty.
	 */

	void pop()
	{
		const auto readPosition = readPosition_ ;
		reinterpret_cast<T*>(getStorage(readPosition))->~T();
		readPosition_ = incrementPosition(readPosition);
	}

	/**
	 * \brief Pushes the element to circular buffer.
	 *
	 * \pre Circular buffer is not full.
	 *
	 * \param [in] value is a reference to object that will be pushed, value in circular buffer's storage is
	 * copy-constructed
	 */

	void push(const T& value)
	{
		const auto writePosition = writePosition_;
		const auto storage = getStorage(writePosition);
		new (storage) T{value};
		writePosition_ = incrementPosition(writePosition);
	}

	/**
	 * \brief Pushes the element to circular buffer.
	 *
	 * \pre Circular buffer is not full.
	 *
	 * \param [in] value is a rvalue reference to object that will be pushed, value in circular buffer's storage is
	 * move-constructed
	 */

	void push(T&& value)
	{
		const auto writePosition = writePosition_;
		const auto storage = getStorage(writePosition);
		new (storage) T{std::move(value)};
		writePosition_ = incrementPosition(writePosition);
	}

	CircularBuffer(const CircularBuffer&) = delete;
	CircularBuffer(CircularBuffer&&) = default;
	const CircularBuffer& operator=(const CircularBuffer&) = delete;
	CircularBuffer& operator=(CircularBuffer&&) = delete;

private:

	/**
	 * \brief Gets pointer to storage at \a position.
	 *
	 * \param [in] position is the selected position
	 *
	 * \return pointer to storage at \a position
	 */

	Storage* getStorage(const size_t position) const
	{
		const auto maskedPosition = position & positionMask_;
		return &storageUniquePointer_[maskedPosition];
	}

	/**
	 * \brief Increments given position by 1.
	 *
	 * \param [in] position is the position that will be incremented
	 *
	 * \return \a position incremented by 1
	 */

	size_t incrementPosition(const size_t position)
	{
		const auto maskedPosition = position & positionMask_;
		const auto msb = position & msbMask_;
		// in case of wrap-around MSB is inverted and position is 0
		return maskedPosition + 1 != getCapacity() ? msb | (maskedPosition + 1) : msb ^ msbMask_;
	}

	/**
	 * \brief Tests for empty circular buffer.
	 *
	 * The buffer is empty if read and write positions are equal, including their MSBs.
	 *
	 * \param [in] readPosition is the value of \a readPosition_
	 * \param [in] writePosition is the value of \a writePosition_
	 *
	 * \return true if circular buffer is empty, false otherwise
	 */

	constexpr static bool isEmpty(const size_t readPosition, const size_t writePosition)
	{
		return readPosition == writePosition;
	}

	/**
	 * \brief Tests for full circular buffer.
	 *
	 * The buffer is full if masked read and write positions are equal, but their MSBs are different.
	 *
	 * \param [in] readPosition is the value of \a readPosition_
	 * \param [in] writePosition is the value of \a writePosition_
	 *
	 * \return true if circular buffer is full, false otherwise
	 */

	constexpr static bool isFull(const size_t readPosition, const size_t writePosition)
	{
		return (readPosition ^ writePosition) == msbMask_;
	}

	/// bitmask used to extract position from \a readPosition_ or \a writePosition_
	constexpr static size_t positionMask_ {SIZE_MAX >> 1};

	/// bitmask used to extract MSB from \a readPosition_ or \a writePosition_
	constexpr static size_t msbMask_ {~positionMask_};

	/// storage for circular buffer elements
	const StorageUniquePointer storageUniquePointer_;

	/// capacity of an array pointed by \a storageUniquePointer_
	size_t capacity_;

	/// current read position
	volatile size_t readPosition_;

	/// current write position
	volatile size_t writePosition_;
};

}	// namespace estd

#endif	// ESTD_CIRCULARBUFFER_HPP_
