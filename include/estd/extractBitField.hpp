/**
 * \file
 * \brief extractBitField() header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_EXTRACTBITFIELD_HPP_
#define ESTD_EXTRACTBITFIELD_HPP_

#include "estd/TypeFromSize.hpp"

#include <array>

#include <climits>

namespace estd
{

/**
 * \brief Extracts a bit field from array of raw data.
 *
 * \tparam index is the index of starting bit
 * \tparam size is the size of bit field to extract, bits
 * \tparam reverse selects whether \a index 0 is MSB (false) or LSB (true), default - false
 * \tparam Ret is the type of returned value, default - fixed width type with at least \a size bits
 * \tparam T is the type of elements of \a array, default - deduced from argument
 * \tparam arraySize is the number of elements in \a array, default - deduced from argument
 *
 * \param [in] array is a reference to array with raw data from which the bit field will be extracted
 *
 * \return bit field extracted from \a array
 */

template<size_t index, size_t size, bool reverse = {}, typename Ret = TypeFromSize<(size + CHAR_BIT - 1) / CHAR_BIT>,
		typename T, size_t arraySize>
inline static Ret extractBitField(const std::array<T, arraySize>& array)
{
	static_assert(size <= sizeof(Ret) * CHAR_BIT, "`Ret` is too small to fit `size` bits!");
	static_assert(index + size <= sizeof(array) * CHAR_BIT, "Bit field is out of `array` bounds!");

	constexpr auto divider = sizeof(T) * CHAR_BIT;
	constexpr auto begin = index / divider;
	constexpr auto offset = index % divider;
	constexpr auto end = (index + size + divider - 1) / divider;

	Ret value {};
	for (auto i = begin; i < end; ++i)
	{
		using Element = typename std::conditional<sizeof(Ret) >= sizeof(T), Ret, T>::type;
		const Element element = reverse == false ? array[i] : array.rbegin()[i];
		const auto shift = static_cast<int>((i - begin) * divider) - static_cast<int>(offset);
		if (shift >= 0)
			value |= element << shift;
		else
			value |= element >> -shift;
	}

	value &= (Ret{1} << size) - 1;
	return value;
}

}	// namespace estd

#endif	// ESTD_EXTRACTBITFIELD_HPP_
