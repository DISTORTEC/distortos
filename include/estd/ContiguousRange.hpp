/**
 * \file
 * \brief ContiguousRange template class header
 *
 * \author Copyright (C) 2014-2018 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_CONTIGUOUSRANGE_HPP_
#define ESTD_CONTIGUOUSRANGE_HPP_

#include <array>

#include <cstddef>

namespace estd
{

/**
 * \brief ContiguousRange template class is a pair of iterators to contiguous sequence of elements in memory.
 *
 * \tparam T is the type of data in the range
 */

template<typename T>
class ContiguousRange
{
public:

	/// value_type type
	using value_type = T;

	/// pointer type
	using pointer = value_type*;

	/// const_pointer type
	using const_pointer = const value_type*;

	/// reference type
	using reference = value_type&;

	/// const_reference type
	using const_reference = const value_type&;

	/// iterator type
	using iterator = value_type*;

	/// const_iterator type
	using const_iterator = const value_type*;

	/// size_type type
	using size_type = std::size_t;

	/// difference_type type
	using difference_type = std::ptrdiff_t;

	/// reverse_iterator type
	using reverse_iterator = std::reverse_iterator<iterator>;

	/// const_reverse_iterator type
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	/**
	 * \brief ContiguousRange's constructor
	 *
	 * \param [in] beginn is an iterator to first element in the range
	 * \param [in] endd is an iterator to "one past the last" element in the range
	 */

	constexpr ContiguousRange(const iterator beginn, const iterator endd) noexcept :
			begin_{beginn},
			end_{endd}
	{

	}

	/**
	 * \brief Empty ContiguousRange's constructor
	 */

	constexpr ContiguousRange() noexcept :
			ContiguousRange{nullptr, nullptr}
	{

	}

	/**
	 * \brief ContiguousRange's constructor using iterator and size
	 *
	 * \param [in] beginn is an iterator to first element in the range
	 * \param [in] sizee is the number of elements in the range
	 */

	constexpr ContiguousRange(const iterator beginn, size_t sizee) noexcept :
			begin_{beginn},
			end_{beginn + sizee}
	{

	}

	/**
	 * \brief ContiguousRange's constructor using C-style array
	 *
	 * \tparam N is the number of elements in the array
	 *
	 * \param [in] array is the array used to initialize the range
	 */

	template<size_t N>
	constexpr explicit ContiguousRange(T (& array)[N]) noexcept :
			ContiguousRange{array, array + N}
	{

	}

	/**
	 * \brief ContiguousRange's constructor using std::array
	 *
	 * \tparam N is the number of elements in the array
	 *
	 * \param [in] array is the array used to initialize the range
	 */

	template<size_t N>
	constexpr explicit ContiguousRange(std::array<T, N>& array) noexcept :
			ContiguousRange{array.begin(), array.end()}
	{

	}

	/**
	 * \brief ContiguousRange's constructor using const std::array
	 *
	 * \tparam N is the number of elements in the array
	 *
	 * \param [in] array is the const array used to initialize the range
	 */

	template<size_t N>
	constexpr explicit ContiguousRange(const std::array<typename std::remove_const<T>::type, N>& array) noexcept :
			ContiguousRange{array.begin(), array.end()}
	{

	}

	/**
	 * \brief ContiguousRange's constructor using const std::array with const type
	 *
	 * This constructor is enabled only if \a T is \a const.
	 *
	 * \tparam N is the number of elements in the array
	 *
	 * \param [in] array is the array used to initialize the range
	 */

	template<size_t N, typename TT = T, typename = typename std::enable_if<std::is_const<TT>::value == true>::type>
	constexpr explicit ContiguousRange(const std::array<T, N>& array) noexcept :
			ContiguousRange{array.begin(), array.end()}
	{

	}

	/**
	 * \brief ContiguousRange's constructor using single value
	 *
	 * \param [in] value is a reference to variable used to initialize the range
	 */

	constexpr explicit ContiguousRange(T& value) noexcept :
			ContiguousRange{&value, &value + 1}
	{

	}

	/**
	 * \brief ContiguousRange's converting constructor
	 *
	 * This constructor is enabled only if \a T is \a const. It can be used to convert non-const range to const range.
	 *
	 * \param [in] other is a reference to source of conversion
	 */

	template<typename TT = T, typename = typename std::enable_if<std::is_const<TT>::value == true>::type>
	constexpr explicit ContiguousRange(const ContiguousRange<typename std::remove_const<TT>::type>& other) noexcept :
			ContiguousRange{other.begin(), other.end()}
	{

	}

	/**
	 * \brief ContiguousRange's subscript operator
	 *
	 * \param [in] i is the index of element that will be accessed
	 *
	 * \return reference to element at given index
	 */

	reference operator[](const size_type i) const noexcept
	{
		return begin_[i];
	}

	/**
	 * \return iterator to first element in the range
	 */

	constexpr iterator begin() const noexcept
	{
		return begin_;
	}

	/**
	 * \return const_iterator to first element in the range
	 */

	constexpr const_iterator cbegin() const noexcept
	{
		return begin();
	}

	/**
	 * \return const_iterator to "one past the last" element in the range
	 */

	constexpr const_iterator cend() const noexcept
	{
		return end();
	}

	/**
	 * \return const_reverse_iterator to first element in the reversed range (last element of the non-reversed range)
	 */

	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return rbegin();
	}

	/**
	 * \return const_reverse_iterator to "one past the last" element in the reversed range ("one before the first"
	 * element of the non-reversed range)
	 */

	constexpr const_reverse_iterator crend() const noexcept
	{
		return rend();
	}

	/**
	 * \return iterator to "one past the last" element in the range
	 */

	constexpr iterator end() const noexcept
	{
		return end_;
	}

	/**
	 * \return reverse_iterator to first element in the reversed range (last element of the non-reversed range)
	 */

	constexpr reverse_iterator rbegin() const noexcept
	{
		return reverse_iterator{end()};
	}

	/**
	 * \return reverse_iterator to "one past the last" element in the reversed range ("one before the first" element of
	 * the non-reversed range)
	 */

	constexpr reverse_iterator rend() const noexcept
	{
		return reverse_iterator{begin()};
	}

	/**
	 * \return number of elements in the range
	 */

	constexpr size_type size() const noexcept
	{
		return end_ - begin_;
	}

private:

	/// iterator to first element in the range
	iterator begin_;

	/// iterator to "one past the last" element in the range
	iterator end_;
};

}	// namespace estd

#endif	// ESTD_CONTIGUOUSRANGE_HPP_
