/**
 * \file
 * \brief Mock of STM32 SPIv2 functions
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SPIV2_HPP_DISTORTOS_CHIP_STM32_SPIV2_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SPIV2_HPP_DISTORTOS_CHIP_STM32_SPIV2_HPP_

#include "distortos/chip/STM32-SPIv1-SPIv2.hpp"

namespace distortos
{

namespace chip
{

constexpr static uint8_t minSpiWordLength {4};
constexpr static uint8_t maxSpiWordLength {16};

}	// namespace chip

}	// namespace distortos

#endif	// UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SPIV2_HPP_DISTORTOS_CHIP_STM32_SPIV2_HPP_
