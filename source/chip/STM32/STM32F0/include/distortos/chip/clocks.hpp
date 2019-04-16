/**
 * \file
 * \brief Definitions of clocks for STM32F0
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

#if defined(DISTORTOS_CHIP_STM32F04) || defined(DISTORTOS_CHIP_STM32F071) || defined(DISTORTOS_CHIP_STM32F072) || \
	defined(DISTORTOS_CHIP_STM32F078) || defined(DISTORTOS_CHIP_STM32F09)

/// HSI48 clock frequency, Hz
constexpr uint32_t hsi48Frequency {48000000};

#endif	// defined(DISTORTOS_CHIP_STM32F04) || defined(DISTORTOS_CHIP_STM32F071) || defined(DISTORTOS_CHIP_STM32F072) ||
		// defined(DISTORTOS_CHIP_STM32F078) || defined(DISTORTOS_CHIP_STM32F09)

/// minimum allowed value for PLL input frequency, Hz
constexpr uint32_t minPllInFrequency {1000000};

/// maximum allowed value for PLL input frequency, Hz
constexpr uint32_t maxPllInFrequency {24000000};

/// minimum allowed value for PLL output frequency, Hz
constexpr uint32_t minPllOutFrequency {16000000};

/// maximum allowed value for PLL output frequency, Hz
constexpr uint32_t maxPllOutFrequency {48000000};

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

/// PLL input frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSIDIV2)
constexpr uint32_t pllInFrequency {hsiFrequency / 2};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSIPREDIV)
constexpr uint32_t pllInFrequency {hsiFrequency / DISTORTOS_CHIP_RCC_PREDIV};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSEPREDIV)
constexpr uint32_t pllInFrequency {DISTORTOS_CHIP_RCC_HSE_FREQUENCY / DISTORTOS_CHIP_RCC_PREDIV};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI48PREDIV)
constexpr uint32_t pllInFrequency {hsi48Frequency / DISTORTOS_CHIP_RCC_PREDIV};
#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI48PREDIV)

static_assert(minPllInFrequency <= pllInFrequency && pllInFrequency <= maxPllInFrequency,
		"Invalid PLL input frequency!");

/// PLL output frequency, Hz
constexpr uint32_t pllOutFrequency {pllInFrequency * DISTORTOS_CHIP_RCC_PLLMUL};

static_assert(minPllOutFrequency <= pllOutFrequency && pllOutFrequency <= maxPllOutFrequency,
		"Invalid PLL output frequency!");

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

/// SYSCLK frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI)
constexpr uint32_t sysclkFrequency {hsiFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_HSE_FREQUENCY};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllOutFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI48)
constexpr uint32_t sysclkFrequency {hsi48Frequency};
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI48)

#else	// !def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY};

#endif	// !def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / DISTORTOS_CHIP_RCC_HPRE};

/// APB frequency, Hz
constexpr uint32_t apbFrequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
