/**
 * \file
 * \brief AddressRange class header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_DEVICES_MEMORY_ADDRESSRANGE_HPP_
#define SOURCE_DEVICES_MEMORY_ADDRESSRANGE_HPP_

#include <algorithm>

#include <cstdint>

namespace distortos
{

namespace devices
{

/// AddressRange class is a contiguous range of addresses.
class AddressRange
{
public:

	/**
	 * \brief AddressRange's constructor
	 *
	 * \param [in] beginn is the starting address of the range
	 * \param [in] sizee is the size of range, bytes
	 */

	constexpr AddressRange(const uint64_t beginn, const uint64_t sizee) noexcept:
			begin_{beginn},
			size_{sizee}
	{

	}

	/**
	 * \brief Empty AddressRange's constructor
	 */

	constexpr AddressRange() noexcept:
			AddressRange{{}, {}}
	{

	}

	/**
	 * \brief AddressRange's bitwise AND assignment operator
	 *
	 * Makes this object an intersection of this and given ranges.
	 *
	 * \param [in] range is a reference to range which will be intersected with this object
	 *
	 * \return reference to current object
	 */

	AddressRange& operator&=(const AddressRange& range) noexcept
	{
		// one or both ranges are empty
		if (size() == 0 || range.size() == 0)
		{
			*this = {};
			return *this;
		}

		const auto newBegin = std::max(begin(), range.begin());
		const auto newEnd = std::min(end(), range.end());

		// no intersection
		if (newBegin >= newEnd)
		{
			*this = {};
			return *this;
		}

		begin_ = newBegin;
		size_ = newEnd - newBegin;
		return *this;
	}

	/**
	 * \brief AddressRange's bitwise OR assignment operator
	 *
	 * Makes this object an union of this and given ranges. Resulting union range is not empty only if source ranges are
	 * either intersecting or adjacent.
	 *
	 * \param [in] range is a reference to range which will be intersected with this object
	 *
	 * \return reference to current object
	 */

	AddressRange& operator|=(const AddressRange& range) noexcept
	{
		// both ranges are empty
		if (size() == 0 && range.size() == 0)
		{
			*this = {};
			return *this;
		}

		if (size() == 0)
		{
			*this = range;
			return *this;
		}

		if (range.size() == 0)
			return *this;

		const auto newBegin = std::min(begin(), range.begin());
		const auto newEnd = std::max(end(), range.end());
		const auto newSize = newEnd - newBegin;

		if (size() + range.size() < newSize)
		{
			*this = {};
			return *this;
		}

		begin_ = newBegin;
		size_ = newSize;
		return *this;
	}

	/**
	 * \return starting address of the range
	 */

	constexpr uint64_t begin() const noexcept
	{
		return begin_;
	}

	/**
	 * \return "one past the last" address of the range
	 */

	constexpr uint64_t end() const noexcept
	{
		return begin_ + size_;
	}

	/**
	 * \return size of range, bytes
	 */

	constexpr uint64_t size() const noexcept
	{
		return size_;
	}

private:

	/// starting address of the range
	uint64_t begin_;

	/// size of range, bytes
	uint64_t size_;
};

/**
 * \brief AddressRange's equality comparison operator
 *
 * \relates AddressRange
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return true if \a left and \b right are equal, false otherwise
 */

inline bool operator==(const AddressRange& left, const AddressRange& right) noexcept
{
	if (left.size() == 0 && right.size() == 0)
		return true;

	if (left.begin() != right.begin())
		return false;

	if (left.size() != right.size())
		return false;

	return true;
}

/**
 * \brief AddressRange's inequality comparison operator
 *
 * \relates AddressRange
 *
 * \param [in] left is the object on the left-hand side of comparison
 * \param [in] right is the object on the right-hand side of comparison
 *
 * \return true if \a left and \b right are not equal, false otherwise
 */

inline bool operator!=(const AddressRange& left, const AddressRange& right) noexcept
{
	return (left == right) == false;
}

/**
 * \brief AddressRange's bitwise AND operator
 *
 * Makes an intersection of given ranges.
 *
 * \relates AddressRange
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return intersection of \a left and \a right
 */

inline AddressRange operator&(AddressRange left, const AddressRange& right) noexcept
{
	left &= right;
	return left;
}

/**
 * \brief AddressRange's bitwise OR operator
 *
 * Makes an union of given ranges. Resulting union range is not empty only if source ranges are either intersecting or
 * adjacent.
 *
 * \relates AddressRange
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return union of \a left and \a right
 */

inline AddressRange operator|(AddressRange left, const AddressRange& right) noexcept
{
	left |= right;
	return left;
}

}	// namespace devices

}	// namespace distortos

#endif	// SOURCE_DEVICES_MEMORY_ADDRESSRANGE_HPP_
