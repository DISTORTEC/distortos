/**
 * \file
 * \brief DynamicCircularBuffer class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_DYNAMICCIRCULARBUFFER_HPP_
#define ESTD_DYNAMICCIRCULARBUFFER_HPP_

#include "estd/CircularBuffer.hpp"

namespace estd
{

/**
 * \brief DynamicCircularBuffer class is a variant of CircularBuffer that has dynamic storage for circular buffer's
 * contents.
 *
 * \tparam T is the type of data in circular buffer
 */

template<typename T>
class DynamicCircularBuffer : public CircularBuffer<T, std::default_delete>
{
public:

	/// Base is a type alias for the base class
	using Base = CircularBuffer<T, std::default_delete>;

	/// import Storage type from base class
	using typename Base::Storage;

	/**
	 * \brief DynamicCircularBuffer's constructor
	 *
	 * \param [in] capacity is the maximum number of elements in circular buffer, should be less than or equal to
	 * SIZE_MAX / 2
	 */

	explicit DynamicCircularBuffer(const size_t capacity) :
			Base{typename Base::StorageUniquePointer{new Storage[capacity]}, capacity}
	{

	}
};

}	// namespace estd

#endif	// ESTD_DYNAMICCIRCULARBUFFER_HPP_
