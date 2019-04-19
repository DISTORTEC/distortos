/**
 * \file
 * \brief Definitions of clocks for STM32L4
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed values for HSE frequency, Hz
/// [0] - "HSE Clock Bypass" disabled
/// [1] - "HSE Clock Bypass" enabled
constexpr uint32_t minHseFrequencies[2] {4000000, 1};

/// maximum allowed values for HSE frequency, Hz
/// [...][0] - voltage scaling range 1
/// [...][1] - voltage scaling range 2
/// [0][...] - "HSE Clock Bypass" disabled
/// [1][...] - "HSE Clock Bypass" enabled
constexpr uint32_t maxHseFrequencies[2][2]
{
		{48000000, 48000000},
		{48000000, 26000000},
};

/// HSI16 clock frequency, Hz
constexpr uint32_t hsi16Frequency {16000000};

/// MSI frequencies, Hz
/// [0] - range 0
/// [1] - range 1
/// [2] - range 2
/// [3] - range 3
/// [4] - range 4
/// [5] - range 5
/// [6] - range 6
/// [7] - range 7
/// [8] - range 8
/// [9] - range 9
/// [10] - range 10
/// [11] - range 11
constexpr uint32_t msiFrequencies[12]
{
		100000,
		200000,
		400000,
		800000,
		1000000,
		2000000,
		4000000,
		8000000,
		16000000,
		24000000,
		32000000,
		48000000,
};

/// minimum allowed value for VCO input frequency, Hz
constexpr uint32_t minVcoInFrequency {4000000};

/// maximum allowed value for VCO input frequency, Hz
constexpr uint32_t maxVcoInFrequency {16000000};

/// minimum allowed value for VCO output frequency, Hz
constexpr uint32_t minVcoOutFrequency {64000000};

/// maximum allowed values for VCO output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxVcoOutFrequency[2] {344000000, 128000000};

/// minimum allowed value for PLL "P" output frequency, Hz
constexpr uint32_t minPllpOutFrequency {2064500};

/// maximum allowed values for PLL "P" output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxPllpOutFrequencies[2] {80000000, 26000000};

/// minimum allowed value for PLL "Q" output frequency, Hz
constexpr uint32_t minPllqOutFrequency {8000000};

/// maximum allowed values for PLL "Q" output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxPllqOutFrequencies[2] {80000000, 26000000};

/// minimum allowed value for PLL "R" output frequency, Hz
constexpr uint32_t minPllrOutFrequency {8000000};

/// maximum allowed values for PLL "R" output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxPllrOutFrequencies[2] {80000000, 26000000};

/// maximum allowed values for SYSCLK frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxSysclkFrequencies[2] {80000000, 26000000};

/// maximum allowed values for AHB frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxAhbFrequencies[2] {80000000, 26000000};

/// maximum allowed values for APB1 (low speed) frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxApb1Frequencies[2] {80000000, 26000000};

/// maximum allowed values for APB2 (high speed) frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxApb2Frequencies[2] {80000000, 26000000};

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// voltage scaling range index for arrays
constexpr uint8_t voltageScalingRangeIndex {DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE - 1};

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE

/// HSE frequency, Hz
constexpr uint32_t hseFrequency {DISTORTOS_CHIP_RCC_HSE_FREQUENCY};

/// HSE clock bypass
#ifdef DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
constexpr bool hseClockBypass {true};
#else	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
constexpr bool hseClockBypass {false};
#endif	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS

static_assert(minHseFrequencies[hseClockBypass] <= hseFrequency &&
		hseFrequency <= maxHseFrequencies[hseClockBypass][voltageScalingRangeIndex], "Invalid HSE frequency!");

#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_MSI_ENABLE

/// MSI frequency, Hz
constexpr uint32_t msiFrequency {msiFrequencies[DISTORTOS_CHIP_RCC_MSI_RANGE]};

#endif	// def DISTORTOS_CHIP_RCC_MSI_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

/// PLL input frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
constexpr uint32_t pllInFrequency {hseFrequency};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI16)
constexpr uint32_t pllInFrequency {hsi16Frequency};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_MSI)
constexpr uint32_t pllInFrequency {msiFrequency};
#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_MSI)

/// VCO input frequency, Hz
constexpr uint32_t vcoInFrequency {pllInFrequency / DISTORTOS_CHIP_RCC_PLLM};

static_assert(minVcoInFrequency <= vcoInFrequency && vcoInFrequency <= maxVcoInFrequency,
		"Invalid VCO input frequency!");

/// VCO output frequency, Hz
constexpr uint32_t vcoOutFrequency {vcoInFrequency * DISTORTOS_CHIP_RCC_PLLN};

static_assert(minVcoOutFrequency <= vcoOutFrequency && vcoOutFrequency <= maxVcoOutFrequency[voltageScalingRangeIndex],
		"Invalid VCO output frequency!");

/// PLL "P" output frequency, Hz
constexpr uint32_t pllpOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLP};

static_assert(minPllpOutFrequency <= pllpOutFrequency &&
		pllpOutFrequency <= maxPllpOutFrequencies[voltageScalingRangeIndex], "Invalid PLL \"P\" output frequency!");

/// PLL "Q" output frequency, Hz
constexpr uint32_t pllqOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLQ};

static_assert(minPllqOutFrequency <= pllqOutFrequency &&
		pllqOutFrequency <= maxPllqOutFrequencies[voltageScalingRangeIndex], "Invalid PLL \"Q\" output frequency!");

/// PLL "R" output frequency, Hz
constexpr uint32_t pllrOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLR};

static_assert(minPllrOutFrequency <= pllrOutFrequency &&
		pllrOutFrequency <= maxPllrOutFrequencies[voltageScalingRangeIndex], "Invalid PLL \"R\" output frequency!");

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

/// SYSCLK frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {hseFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI16)
constexpr uint32_t sysclkFrequency {hsi16Frequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_MSI)
constexpr uint32_t sysclkFrequency {msiFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllrOutFrequency};
#else
#error "All SYSCLK sources disabled!"
#endif

#else	// !def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// voltage scaling range index for arrays
constexpr uint8_t voltageScalingRangeIndex {0};

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY};

#endif	// !def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

static_assert(sysclkFrequency <= maxSysclkFrequencies[voltageScalingRangeIndex], "Invalid SYSCLK frequency!");

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / DISTORTOS_CHIP_RCC_HPRE};

static_assert(ahbFrequency <= maxAhbFrequencies[voltageScalingRangeIndex], "Invalid AHB frequency!");

/// APB1 frequency, Hz
constexpr uint32_t apb1Frequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE1};

static_assert(apb1Frequency <= maxApb1Frequencies[voltageScalingRangeIndex], "Invalid APB1 (low speed) frequency!");

/// APB2 frequency, Hz
constexpr uint32_t apb2Frequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE2};

static_assert(apb2Frequency <= maxApb2Frequencies[voltageScalingRangeIndex], "Invalid APB2 (high speed) frequency!");

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
