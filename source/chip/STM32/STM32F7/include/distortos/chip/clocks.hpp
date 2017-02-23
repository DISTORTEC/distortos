/**
 * \file
 * \brief Definitions of clocks for STM32F7
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {54000000};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {108000000};

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F7_RCC_SYSCLK_FREQUENCY};

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / CONFIG_CHIP_STM32F7_RCC_HPRE};

/// APB1 frequency, Hz
constexpr uint32_t apb1Frequency {ahbFrequency / CONFIG_CHIP_STM32F7_RCC_PPRE1};

static_assert(apb1Frequency <= maxApb1Frequency, "Invalid APB1 (low speed) frequency!");

/// APB2 frequency, Hz
constexpr uint32_t apb2Frequency {ahbFrequency / CONFIG_CHIP_STM32F7_RCC_PPRE2};

static_assert(apb2Frequency <= maxApb2Frequency, "Invalid APB2 (high speed) frequency!");

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
