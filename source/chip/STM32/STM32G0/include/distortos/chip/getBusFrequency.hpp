/**
 * \file
 * \brief getBusFrequency() definition for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_GETBUSFREQUENCY_HPP_
#define SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_GETBUSFREQUENCY_HPP_

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/**
 * \brief Returns frequency of the bus to which given peripheral is connected.
 *
 * \param [in] peripheralBase is the base address of peripheral
 *
 * \return peripheral's bus frequency, Hz
 */

constexpr uint32_t getBusFrequency(const uintptr_t peripheralBase)
{
	return peripheralBase >= AHBPERIPH_BASE ? ahbFrequency : apbFrequency;
}

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_GETBUSFREQUENCY_HPP_
