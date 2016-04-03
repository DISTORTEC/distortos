/**
 * \file
 * \brief Definitions of clocks for STM32F1
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

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

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

/// minimum allowed value for PLL2 and PLL3 input frequency, Hz
constexpr uint32_t minPll23InFrequency {3000000};

/// maximum allowed value for PLL2 and PLL3 input frequency, Hz
constexpr uint32_t maxPll23InFrequency {5000000};

/// minimum allowed value for PLL2 and PLL3 output frequency, Hz
constexpr uint32_t minPll23OutFrequency {40000000};

/// maximum allowed value for PLL2 and PLL3 output frequency, Hz
constexpr uint32_t maxPll23OutFrequency {74000000};

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

/// minimum allowed value for PLL input frequency, Hz
#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)
constexpr uint32_t minPllInFrequency {3000000};
#else	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
constexpr uint32_t minPllInFrequency {1000000};
#endif	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

/// maximum allowed value for PLL input frequency, Hz
#if defined(CONFIG_CHIP_STM32F100)
constexpr uint32_t maxPllInFrequency {24000000};
#elif defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)
constexpr uint32_t maxPllInFrequency {12000000};
#else	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
constexpr uint32_t maxPllInFrequency {25000000};
#endif	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

/// minimum allowed value for PLL output frequency, Hz
#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)
constexpr uint32_t minPllOutFrequency {18000000};
#else	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
constexpr uint32_t minPllOutFrequency {16000000};
#endif	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

/// maximum allowed value for PLL output frequency, Hz
#if defined(CONFIG_CHIP_STM32F100)
constexpr uint32_t maxPllOutFrequency {24000000};
#elif defined(CONFIG_CHIP_STM32F101)
constexpr uint32_t maxPllOutFrequency {36000000};
#elif defined(CONFIG_CHIP_STM32F102)
constexpr uint32_t maxPllOutFrequency {48000000};
#else	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F101) && !defined(CONFIG_CHIP_STM32F102)
constexpr uint32_t maxPllOutFrequency {72000000};
#endif	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F101) && !defined(CONFIG_CHIP_STM32F102)

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {36000000};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {72000000};

#ifdef CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

#if defined(CONFIG_CHIP_STM32F1_RCC_HSE_ENABLE) && \
		(defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)) && \
		defined(CONFIG_CHIP_STM32F1_RCC_PREDIV2_PLL2_PLL3_ENABLE)

/// PLL2 & PLL3 input frequency, Hz
constexpr uint32_t pll23InFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY / CONFIG_CHIP_STM32F1_RCC_PREDIV2};

static_assert(minPll23InFrequency <= pll23InFrequency && pll23InFrequency <= maxPll23InFrequency,
		"Invalid PLL2 and PLL3 input frequency!");

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL2_ENABLE

/// PLL2 output frequency, Hz
constexpr uint32_t pll2OutFrequency {pll23InFrequency * CONFIG_CHIP_STM32F1_RCC_PLL2MUL};

static_assert(minPll23OutFrequency <= pll2OutFrequency && pll2OutFrequency <= maxPll23OutFrequency,
		"Invalid PLL2 output frequency!");

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL2_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL3_ENABLE

/// PLL3 output frequency, Hz
constexpr uint32_t pll3OutFrequency {pll23InFrequency * CONFIG_CHIP_STM32F1_RCC_PLL3MUL};

static_assert(minPll23OutFrequency <= pll3OutFrequency && pll3OutFrequency <= maxPll23OutFrequency,
		"Invalid PLL3 output frequency!");

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL3_ENABLE

#endif	// defined(CONFIG_CHIP_STM32F1_RCC_HSE_ENABLE) &&
		// (defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)) &&
		// defined(CONFIG_CHIP_STM32F1_RCC_PREDIV2_PLL2_PLL3_ENABLE)

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_HSIDIV2)

/// PLL input frequency, Hz
constexpr uint32_t pllInFrequency {hsiFrequency / 2};

#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_PREDIV1)

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

/// PREDIV1 input frequency, Hz
#if defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_HSE)
constexpr uint32_t prediv1InFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)
constexpr uint32_t prediv1InFrequency {pll2OutFrequency};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)

#else	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

/// PREDIV1 input frequency, Hz
constexpr uint32_t prediv1InFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY};

#endif	// !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

/// PREDIV1 output frequency, Hz
constexpr uint32_t prediv1OutFrequency {prediv1InFrequency / CONFIG_CHIP_STM32F1_RCC_PREDIV1};

/// PLL input frequency, Hz
constexpr uint32_t pllInFrequency {prediv1OutFrequency};

#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_PREDIV1)

static_assert(minPllInFrequency <= pllInFrequency && pllInFrequency <= maxPllInFrequency,
		"Invalid PLL input frequency!");

/// PLL output frequency, Hz
constexpr uint32_t pllOutFrequency {pllInFrequency *
		CONFIG_CHIP_STM32F1_RCC_PLLMUL_NUMERATOR / CONFIG_CHIP_STM32F1_RCC_PLLMUL_DENOMINATOR};

static_assert(minPllOutFrequency <= pllOutFrequency && pllOutFrequency <= maxPllOutFrequency,
		"Invalid PLL output frequency!");

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL_ENABLE

/// SYSCLK frequency, Hz
#if defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSI)
constexpr uint32_t sysclkFrequency {hsiFrequency};
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F1_RCC_HSE_FREQUENCY};
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllOutFrequency};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)

#else	// !def CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F1_RCC_SYSCLK_FREQUENCY};

#endif	// !def CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / CONFIG_CHIP_STM32F1_RCC_HPRE};

/// APB1 frequency, Hz
constexpr uint32_t apb1Frequency {ahbFrequency / CONFIG_CHIP_STM32F1_RCC_PPRE1};

static_assert(apb1Frequency <= maxApb1Frequency, "Invalid APB1 (low speed) frequency!");

/// APB2 frequency, Hz
constexpr uint32_t apb2Frequency {ahbFrequency / CONFIG_CHIP_STM32F1_RCC_PPRE2};

static_assert(apb2Frequency <= maxApb2Frequency, "Invalid APB2 (high speed) frequency!");

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
