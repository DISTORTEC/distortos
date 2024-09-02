/**
 * \file
 * \brief Definitions of clocks for STM32U5
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32U5_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32U5_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY};

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / DISTORTOS_CHIP_RCC_HPRE};

/// APB1 frequency, Hz
constexpr uint32_t apb1Frequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE1};

/// APB2 frequency, Hz
constexpr uint32_t apb2Frequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE2};

/// APB3 frequency, Hz
constexpr uint32_t apb3Frequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE3};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32U5_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
