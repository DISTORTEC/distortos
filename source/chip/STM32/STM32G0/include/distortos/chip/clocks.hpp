/**
 * \file
 * \brief Definitions of clocks for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

#if !defined(DISTORTOS_CHIP_STM32G030) && !defined(DISTORTOS_CHIP_STM32G050) && !defined(DISTORTOS_CHIP_STM32G070)

#define STM32G0_RCC_HAS_PLLQ

#endif	// !defined(DISTORTOS_CHIP_STM32G030) && !defined(DISTORTOS_CHIP_STM32G050) &&
		// !defined(DISTORTOS_CHIP_STM32G070)

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

/// minimum allowed values for LSE frequency, Hz
/// [0] - "LSE Clock Bypass" disabled
/// [1] - "LSE Clock Bypass" enabled
constexpr uint32_t minLseFrequencies[2] {32768, 1};

/// maximum allowed values for LSE frequency, Hz
/// [0] - "LSE Clock Bypass" disabled
/// [1] - "LSE Clock Bypass" enabled
constexpr uint32_t maxLseFrequencies[2] {32768, 1000000};

/// LSI clock frequency, Hz
constexpr uint32_t lsiFrequency {32000};

/// minimum allowed value for VCO input frequency, Hz
constexpr uint32_t minVcoInFrequency {2660000};

/// maximum allowed value for VCO input frequency, Hz
constexpr uint32_t maxVcoInFrequency {16000000};

/// minimum allowed value for VCO output frequency, Hz
constexpr uint32_t minVcoOutFrequency {96000000};

/// maximum allowed values for VCO output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxVcoOutFrequency[2] {344000000, 128000000};

/// minimum allowed value for PLL "P" output frequency, Hz
constexpr uint32_t minPllpOutFrequency {3090000};

/// maximum allowed values for PLL "P" output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxPllpOutFrequencies[2] {122000000, 40000000};

#ifdef STM32G0_RCC_HAS_PLLQ

/// minimum allowed value for PLL "Q" output frequency, Hz
constexpr uint32_t minPllqOutFrequency {12000000};

/// maximum allowed values for PLL "Q" output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxPllqOutFrequencies[2] {128000000, 33000000};

#endif	// def STM32G0_RCC_HAS_PLLQ

/// minimum allowed value for PLL "R" output frequency, Hz
constexpr uint32_t minPllrOutFrequency {12000000};

/// maximum allowed values for PLL "R" output frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxPllrOutFrequencies[2] {64000000, 16000000};

/// maximum allowed values for SYSCLK frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxSysclkFrequencies[2] {64000000, 16000000};

/// maximum allowed values for AHB frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxAhbFrequencies[2] {64000000, 16000000};

/// maximum allowed values for APB frequency, Hz
/// [0] - voltage scaling range 1
/// [1] - voltage scaling range 2
constexpr uint32_t maxApbFrequencies[2] {64000000, 16000000};

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

#ifdef DISTORTOS_CHIP_RCC_HSI16_ENABLE

/// HSISYS frequency, Hz
constexpr uint32_t hsisysFrequency {hsi16Frequency / DISTORTOS_CHIP_RCC_HSIDIV};

#endif	// def DISTORTOS_CHIP_RCC_HSI16_ENABLE

#ifdef DISTORTOS_CHIP_RCC_LSE_ENABLE

/// LSE frequency, Hz
constexpr uint32_t lseFrequency {DISTORTOS_CHIP_RCC_LSE_FREQUENCY};

/// LSE clock bypass
#ifdef DISTORTOS_CHIP_RCC_LSE_CLOCK_BYPASS
constexpr bool lseClockBypass {true};
#else	// !def DISTORTOS_CHIP_RCC_LSE_CLOCK_BYPASS
constexpr bool lseClockBypass {false};
#endif	// !def DISTORTOS_CHIP_RCC_LSE_CLOCK_BYPASS

static_assert(minLseFrequencies[lseClockBypass] <= lseFrequency && lseFrequency <= maxLseFrequencies[lseClockBypass],
		"Invalid LSE frequency!");

#endif	// def DISTORTOS_CHIP_RCC_LSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

/// PLL input frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
constexpr uint32_t pllInFrequency {hseFrequency};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI16)
constexpr uint32_t pllInFrequency {hsi16Frequency};
#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI16)

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

#ifdef STM32G0_RCC_HAS_PLLQ

/// PLL "Q" output frequency, Hz
constexpr uint32_t pllqOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLQ};

static_assert(minPllqOutFrequency <= pllqOutFrequency &&
		pllqOutFrequency <= maxPllqOutFrequencies[voltageScalingRangeIndex], "Invalid PLL \"Q\" output frequency!");

#endif	// def STM32G0_RCC_HAS_PLLQ

/// PLL "R" output frequency, Hz
constexpr uint32_t pllrOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLR};

static_assert(minPllrOutFrequency <= pllrOutFrequency &&
		pllrOutFrequency <= maxPllrOutFrequencies[voltageScalingRangeIndex], "Invalid PLL \"R\" output frequency!");

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

/// SYSCLK frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {hseFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSISYS)
constexpr uint32_t sysclkFrequency {hsisysFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_LSE)
constexpr uint32_t sysclkFrequency {lseFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_LSI)
constexpr uint32_t sysclkFrequency {lsiFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLLR)
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

/// APB frequency, Hz
constexpr uint32_t apbFrequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE};

static_assert(apbFrequency <= maxApbFrequencies[voltageScalingRangeIndex], "Invalid APB frequency!");

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
