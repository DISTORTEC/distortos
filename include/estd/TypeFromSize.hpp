/**
 * \file
 * \brief TypeFromSize type alias template header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_TYPEFROMSIZE_HPP_
#define ESTD_TYPEFROMSIZE_HPP_

#include <type_traits>

#include <cstddef>
#include <cstdint>

/**
 * \brief Produces a `static_assert()` where the expression is also used as the message.
 *
 * \param [in] __VA_ARGS__ is the expression that will be tested, which will also be used as the message
 */

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

namespace estd
{

namespace internal
{

/**
 * \brief Implementation of TypeFromSize
 *
 * \tparam size is the expected size of type, bytes
 */

template<size_t size>
struct TypeFromSize
{
	static_assert(size <= sizeof(uint64_t), "Max supported type is uint64_t!");

	/// one of fixed width types which is at least \a size bytes
	using Type = typename std::conditional<size <= sizeof(uint8_t), uint8_t,
			typename std::conditional<size <= sizeof(uint16_t), uint16_t,
			typename std::conditional<size <= sizeof(uint32_t), uint32_t, uint64_t>::type>::type>::type;
};

}

/**
 * \brief Selects fixed width type from requested byte size.
 *
 * \tparam size is the expected size of type, bytes
 */

template<size_t size>
using TypeFromSize = typename internal::TypeFromSize<size>::Type;

STATIC_ASSERT(std::is_same<TypeFromSize<0>, uint8_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<1>, uint8_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<2>, uint16_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<3>, uint32_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<4>, uint32_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<5>, uint64_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<6>, uint64_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<7>, uint64_t>::value == true);
STATIC_ASSERT(std::is_same<TypeFromSize<8>, uint64_t>::value == true);

}	// namespace estd

#endif	// ESTD_TYPEFROMSIZE_HPP_
