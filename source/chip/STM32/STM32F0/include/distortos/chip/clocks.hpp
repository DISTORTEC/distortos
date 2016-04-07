/**
 * \file
 * \brief Definitions of clocks for STM32F0
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// HSI clock frequency, Hz
constexpr uint32_t hsiFrequency {8000000};

#if defined(CONFIG_CHIP_STM32F04) || defined(CONFIG_CHIP_STM32F071) || defined(CONFIG_CHIP_STM32F072) || \
	defined(CONFIG_CHIP_STM32F078) || defined(CONFIG_CHIP_STM32F09)

/// HSI48 clock frequency, Hz
constexpr uint32_t hsi48Frequency {48000000};

#endif	// defined(CONFIG_CHIP_STM32F04) || defined(CONFIG_CHIP_STM32F071) || defined(CONFIG_CHIP_STM32F072) ||
		// defined(CONFIG_CHIP_STM32F078) || defined(CONFIG_CHIP_STM32F09)

/// minimum allowed value for PLL input frequency, Hz
constexpr uint32_t minPllInFrequency {1000000};

/// maximum allowed value for PLL input frequency, Hz
constexpr uint32_t maxPllInFrequency {24000000};

/// minimum allowed value for PLL output frequency, Hz
constexpr uint32_t minPllOutFrequency {16000000};

/// maximum allowed value for PLL output frequency, Hz
constexpr uint32_t maxPllOutFrequency {48000000};

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F0_RCC_SYSCLK_FREQUENCY};

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / CONFIG_CHIP_STM32F0_RCC_HPRE};

/// APB frequency, Hz
constexpr uint32_t apbFrequency {ahbFrequency / CONFIG_CHIP_STM32F0_RCC_PPRE};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
