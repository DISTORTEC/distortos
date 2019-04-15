/**
 * \file
 * \brief Definitions of clocks for STM32L0
 *
 * \author Copyright (C) 2017-2019 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// MSI frequencies, Hz
/// [0] - range 0
/// [1] - range 1
/// [2] - range 2
/// [3] - range 3
/// [4] - range 4
/// [5] - range 5
/// [6] - range 6
constexpr uint32_t msiFrequencies[7]
{
		65536,
		131072,
		262144,
		524288,
		1048000,
		2097000,
		4194000,
};

/// HSI16 clock frequency, Hz
constexpr uint32_t hsi16Frequency {16000000};

/// minimum allowed value for PLL VCO input frequency, Hz
constexpr uint32_t minPllInFrequency {2000000};

/// maximum allowed value for PLL VCO input frequency, Hz
constexpr uint32_t maxPllInFrequency {24000000};

/// minimum allowed value for PLL output frequency, Hz
constexpr uint32_t minPllOutFrequency {1500000};

/// maximum allowed values for PLL output frequency, Hz
/// [0] - voltage scale 1
/// [1] - voltage scale 2
/// [2] - voltage scale 3
constexpr uint32_t maxPllOutFrequencies[3]
{
		960000000,
		480000000,
		240000000,
};

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {32000000};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {32000000};

#ifdef DISTORTOS_CHIP_STM32L0_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// voltage scale index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex])
constexpr uint8_t voltageScaleIndex {DISTORTOS_CHIP_STM32L0_PWR_VOLTAGE_SCALE_MODE - 1};

/// maximum allowed value for PLL output frequency, Hz
constexpr uint32_t maxPllOutFrequency {maxPllOutFrequencies[voltageScaleIndex]};

#if defined(DISTORTOS_CHIP_STM32L0_RCC_MSI_ENABLE)
/// voltage scale index for \a msiFrequencies array (maxPllOutFrequencies[msiRangeIndex])
constexpr uint8_t msiRangeIndex {DISTORTOS_CHIP_STM32L0_RCC_MSIRANGE};

/// allowed value for MSI frequency, Hz
constexpr uint32_t msiFrequency {msiFrequencies[msiRangeIndex]};

#endif	// defined(DISTORTOS_CHIP_STM32L0_RCC_MSI_ENABLE)

#ifdef DISTORTOS_CHIP_STM32L0_RCC_PLL_ENABLE

/// PLL input frequency, Hz
#if defined(DISTORTOS_CHIP_STM32L0_RCC_PLLSRC_HSI16)
constexpr uint32_t pllInFrequency {hsi16Frequency};
#elif defined(DISTORTOS_CHIP_STM32L0_RCC_PLLSRC_HSE)
constexpr uint32_t pllInFrequency {DISTORTOS_CHIP_STM32L0_RCC_HSE_FREQUENCY};
#endif	// defined(DISTORTOS_CHIP_STM32L0_RCC_PLLSRC_HSE)

static_assert(minPllInFrequency <= pllInFrequency && pllInFrequency <= maxPllInFrequency,
		"Invalid PLL input frequency!");

/// PLL output frequency, Hz
constexpr uint32_t pllOutFrequency {(pllInFrequency * DISTORTOS_CHIP_STM32L0_RCC_PLLMUL) /
		DISTORTOS_CHIP_STM32L0_RCC_PLLDIV};

static_assert(minPllOutFrequency <= pllOutFrequency && pllOutFrequency <= maxPllOutFrequency,
		"Invalid PLL output frequency!");

#endif	// def DISTORTOS_CHIP_STM32L0_RCC_PLL_ENABLE

/// SYSCLK frequency, Hz
#if defined(DISTORTOS_CHIP_STM32L0_RCC_SYSCLK_MSI)
constexpr uint32_t sysclkFrequency {msiFrequency};
#elif defined(DISTORTOS_CHIP_STM32L0_RCC_SYSCLK_HSI16)
constexpr uint32_t sysclkFrequency {hsi16Frequency};
#elif defined(DISTORTOS_CHIP_STM32L0_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_STM32L0_RCC_HSE_FREQUENCY};
#elif defined(DISTORTOS_CHIP_STM32L0_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllOutFrequency};
#else
#error "All SYSCLK sources disabled!"
#endif	// defined(DISTORTOS_CHIP_STM32L0_RCC_SYSCLK_PLL)

#else	// !def DISTORTOS_CHIP_STM32L0_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_STM32L0_RCC_SYSCLK_FREQUENCY};

#endif	// !def DISTORTOS_CHIP_STM32L0_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / DISTORTOS_CHIP_STM32L0_RCC_HPRE};

/// APB1 frequency, Hz
constexpr uint32_t apb1Frequency {ahbFrequency / DISTORTOS_CHIP_STM32L0_RCC_PPRE1};

static_assert(apb1Frequency <= maxApb1Frequency, "Invalid APB1 (low speed) frequency!");

/// APB2 frequency, Hz
constexpr uint32_t apb2Frequency {ahbFrequency / DISTORTOS_CHIP_STM32L0_RCC_PPRE2};

static_assert(apb2Frequency <= maxApb2Frequency, "Invalid APB2 (high speed) frequency!");

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
