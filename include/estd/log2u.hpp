/**
 * \file
 * \brief log2u() header
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_LOG2U_HPP_
#define ESTD_LOG2U_HPP_

#include <climits>

/**
 * \brief Produces a `static_assert()` where the expression is also used as the message.
 *
 * \param [in] __VA_ARGS__ is the expression that will be tested, which will also be used as the message
 */

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

namespace estd
{

/**
 * \brief log2()-like constexpr function for `unsigned int`.
 *
 * Returns 0 when passed 0.
 *
 * \param [in] value is the value for which the log2() will be calculated
 *
 * \return integer part of log2() of \a value
 */

constexpr int log2u(const unsigned int value)
{
	return value != 0 ? (sizeof(value) * CHAR_BIT - 1) - __builtin_clz(value) : 0;
}

STATIC_ASSERT(log2u(0) == 0);
STATIC_ASSERT(log2u(1) == 0);
STATIC_ASSERT(log2u(2) == 1);
STATIC_ASSERT(log2u(4) == 2);
STATIC_ASSERT(log2u(8) == 3);
STATIC_ASSERT(log2u(15) == 3);
STATIC_ASSERT(log2u(16) == 4);
STATIC_ASSERT(log2u(17) == 4);
STATIC_ASSERT(log2u(127) == 6);
STATIC_ASSERT(log2u(128) == 7);
STATIC_ASSERT(log2u(129) == 7);
STATIC_ASSERT(log2u(UINT_MAX) == sizeof(UINT_MAX) * CHAR_BIT - 1);

}	// namespace estd

#endif	// ESTD_LOG2U_HPP_
