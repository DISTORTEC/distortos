/**
 * \file
 * \brief Definitions of clocks for STM32F7
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

/// HSI clock frequency, Hz
constexpr uint32_t hsiFrequency {16000000};

/// minimum allowed value for VCO input frequency, Hz
constexpr uint32_t minVcoInFrequency {1000000};

/// maximum allowed value for VCO input frequency, Hz
constexpr uint32_t maxVcoInFrequency {2000000};

/// minimum allowed value for VCO output frequency, Hz
constexpr uint32_t minVcoOutFrequency {100000000};

/// maximum allowed value for VCO output frequency, Hz
constexpr uint32_t maxVcoOutFrequency {432000000};

/// maximum allowed values for PLL output frequency, Hz
/// [...][0] - over-drive disabled
/// [...][1] - over-drive enabled
/// [0][...] - voltage scale 1
/// [1][...] - voltage scale 2
/// [2][...] - voltage scale 3
constexpr uint32_t maxPllOutFrequencies[3][2]
{
		{180000000, 216000000},
		{168000000, 180000000},
		{144000000, 144000000},
};

/// maximum allowed value for PLL "Q" output frequency, Hz
constexpr uint32_t maxPllqOutFrequency {48000000};

/// maximum allowed APB1 (low speed) frequencies, Hz
/// [0] - over-drive disabled
/// [1] - over-drive enabled
constexpr uint32_t maxApb1Frequencies[2] {45000000, 54000000};

/// maximum allowed APB2 (high speed) frequencies, Hz
/// [0] - over-drive disabled
/// [1] - over-drive enabled
constexpr uint32_t maxApb2Frequencies[2] {90000000, 108000000};

#ifdef CONFIG_CHIP_STM32F7_STANDARD_CLOCK_CONFIGURATION_ENABLE

#if defined(CONFIG_CHIP_STM32F7_PWR_OVER_DRIVE_ENABLE)

static_assert(CONFIG_CHIP_STM32F7_PWR_VOLTAGE_SCALE_MODE == 1 || CONFIG_CHIP_STM32F7_PWR_VOLTAGE_SCALE_MODE == 2,
		"Over-drive mode requires voltage scale 1 mode or voltage scale 2 mode!");
static_assert(CONFIG_CHIP_STM32F7_VDD_MV >= 2100,
		"Over-drive mode must not be enabled when supply voltage is below 2.1 V!");

/// over-drive index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t overDriveIndex {1};

#else	// !defined(CONFIG_CHIP_STM32F7_PWR_OVER_DRIVE_ENABLE)

/// over-drive index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t overDriveIndex {0};

#endif	// !defined(CONFIG_CHIP_STM32F7_PWR_OVER_DRIVE_ENABLE)

/// voltage scale index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t voltageScaleIndex {CONFIG_CHIP_STM32F7_PWR_VOLTAGE_SCALE_MODE - 1};

/// maximum allowed value for PLL output frequency, Hz
constexpr uint32_t maxPllOutFrequency {maxPllOutFrequencies[voltageScaleIndex][overDriveIndex]};

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {maxApb1Frequencies[overDriveIndex]};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {maxApb2Frequencies[overDriveIndex]};

#ifdef CONFIG_CHIP_STM32F7_RCC_PLLS_ENABLE

/// input frequency of main and audio PLLs, Hz
#if defined(CONFIG_CHIP_STM32F7_RCC_PLLSRC_HSI)
constexpr uint32_t pllInFrequency {hsiFrequency};
#elif defined(CONFIG_CHIP_STM32F7_RCC_PLLSRC_HSE)
constexpr uint32_t pllInFrequency {CONFIG_CHIP_STM32F7_RCC_HSE_FREQUENCY};
#endif

/// VCO input frequency of main and audio PLLs, Hz
constexpr uint32_t vcoInFrequency {pllInFrequency / CONFIG_CHIP_STM32F7_RCC_PLLM};

static_assert(minVcoInFrequency <= vcoInFrequency && vcoInFrequency <= maxVcoInFrequency,
		"Invalid VCO input frequency!");

#ifdef CONFIG_CHIP_STM32F7_RCC_PLL_ENABLE

/// VCO output frequency of main PLL, Hz
constexpr uint32_t vcoOutFrequency {vcoInFrequency * CONFIG_CHIP_STM32F7_RCC_PLLN};

static_assert(minVcoOutFrequency <= vcoOutFrequency && vcoOutFrequency <= maxVcoOutFrequency,
		"Invalid VCO output frequency!");

/// main PLL output frequency, Hz
constexpr uint32_t pllOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F7_RCC_PLLP};

static_assert(pllOutFrequency <= maxPllOutFrequency, "Invalid PLL output frequency!");

/// main PLL "Q" output frequency, Hz
constexpr uint32_t pllqOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F7_RCC_PLLQ};

static_assert(pllqOutFrequency <= maxPllqOutFrequency, "Invalid PLL \"/Q\" output frequency!");

#if defined(CONFIG_CHIP_STM32F76) || defined(CONFIG_CHIP_STM32F77)

/// main PLL "R" output frequency, Hz
constexpr uint32_t pllrOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F7_RCC_PLLR};

#endif	// defined(CONFIG_CHIP_STM32F76) || defined(CONFIG_CHIP_STM32F77)

#endif	// def CONFIG_CHIP_STM32F7_RCC_PLL_ENABLE

#endif	// def CONFIG_CHIP_STM32F7_RCC_PLLS_ENABLE

/// SYSCLK frequency, Hz
#if defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_HSI)
constexpr uint32_t sysclkFrequency {hsiFrequency};
#elif defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F7_RCC_HSE_FREQUENCY};
#elif defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllOutFrequency};
#endif	// defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_PLL)

#else	// !def CONFIG_CHIP_STM32F7_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F7_RCC_SYSCLK_FREQUENCY};

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {maxApb1Frequencies[1]};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {maxApb2Frequencies[1]};

#endif	// !def CONFIG_CHIP_STM32F7_STANDARD_CLOCK_CONFIGURATION_ENABLE

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
