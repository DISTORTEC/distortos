/**
 * \file
 * \brief StaticCircularBuffer class header
 *
 * \author Copyright (C) 2020-2022 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_STATICCIRCULARBUFFER_HPP_
#define ESTD_STATICCIRCULARBUFFER_HPP_

#include "estd/CircularBuffer.hpp"
#include "estd/DummyDeleter.hpp"

#include <array>

namespace estd
{

/**
 * \brief StaticCircularBuffer class is a variant of CircularBuffer that has automatic storage for circular buffer's
 * contents.
 *
 * \tparam T is the type of data in circular buffer
 * \tparam Capacity is the maximum number of elements in circular buffer, must be less than or equal to SIZE_MAX / 2
 */

template<typename T, size_t Capacity>
class StaticCircularBuffer : public CircularBuffer<T, DummyDeleter>
{
	static_assert(Capacity <= SIZE_MAX / 2,
			"Capacity of estd::StaticCircularBuffer must be less than or equal to SIZE_MAX / 2");

public:

	/// Base is a type alias for the base class
	using Base = CircularBuffer<T, DummyDeleter>;

	/// import Storage type from base class
	using typename Base::Storage;

	/**
	 * \brief StaticCircularBuffer's constructor
	 */

	constexpr StaticCircularBuffer() :
			Base{typename Base::StorageUniquePointer{storage_.data()}, storage_.size()}
	{

	}

private:

	/// storage for circular buffer's contents
	std::array<Storage, Capacity> storage_;
};

}	// namespace estd

#endif	// ESTD_STATICCIRCULARBUFFER_HPP_
