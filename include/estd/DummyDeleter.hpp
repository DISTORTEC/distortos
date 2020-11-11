/**
 * \file
 * \brief DummyDeleter class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_DUMMYDELETER_HPP_
#define ESTD_DUMMYDELETER_HPP_

namespace estd
{

/**
 * \brief Dummy deleter, used when static storage is passed to `std::unique_ptr`.
 *
 * \tparam T is the type of data handled by deleter, must be trivially destructible
 */

template<typename T>
class DummyDeleter
{
	static_assert(std::is_trivially_destructible<T>::value == true,
			"estd::DummyDeleter cannot be used with types that are not trivially destructible!");

public:

	constexpr DummyDeleter() = default;

	/**
	 * \brief DummyDeleter's function call operator
	 *
	 * Does nothing.
	 */

	void operator()(T*) const
	{

	}
};

/**
 * \brief Dummy deleter, used when static storage is passed to `std::unique_ptr`.
 *
 * Specialization for arrays.
 *
 * \tparam T is the type of data handled by deleter, must be trivially destructible
 */

template<typename T>
class DummyDeleter<T[]>
{
	static_assert(std::is_trivially_destructible<T>::value == true,
			"estd::DummyDeleter cannot be used with types that are not trivially destructible!");

public:

	constexpr DummyDeleter() = default;

	/**
	 * \brief DummyDeleter's function call operator
	 *
	 * Does nothing.
	 */

	void operator()(T*) const
	{

	}
};

}	// namespace estd

#endif	// ESTD_DUMMYDELETER_HPP_
