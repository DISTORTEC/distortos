/**
 * \file
 * \brief Definitions of clocks for STM32F7
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
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

/// maximum allowed value for PLLI2S output frequency, Hz
constexpr uint32_t maxPlli2sOutFrequency {216000000};

/// maximum allowed value for PLLI2S "Q" output frequency, Hz
constexpr uint32_t maxPlli2sqOutFrequency {216000000};

/// maximum allowed value for PLLI2S "R" output frequency, Hz
constexpr uint32_t maxPlli2srOutFrequency {192000000};

/// maximum allowed value for PLLSAI output frequency, Hz
constexpr uint32_t maxPllsaiOutFrequency {48000000};

/// maximum allowed value for PLLSAI "Q" output frequency, Hz
constexpr uint32_t maxPllsaiqOutFrequency {216000000};

/// maximum allowed value for PLLSAI "R" output frequency, Hz
constexpr uint32_t maxPllsairOutFrequency {216000000};

/// maximum allowed value for SDMMC adapter clock frequency, Hz
constexpr uint32_t maxSdmmcFrequency {48000000};

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

#if defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

static_assert(DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE == 1 || DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE == 2,
		"Over-drive mode requires voltage scale 1 mode or voltage scale 2 mode!");
static_assert(DISTORTOS_CHIP_VDD_MV >= 2100, "Over-drive mode must not be enabled when supply voltage is below 2.1 V!");

/// over-drive index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t overDriveIndex {1};

#else	// !defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

/// over-drive index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t overDriveIndex {0};

#endif	// !defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

/// voltage scale index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t voltageScaleIndex {DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE - 1};

/// maximum allowed value for PLL output frequency, Hz
constexpr uint32_t maxPllOutFrequency {maxPllOutFrequencies[voltageScaleIndex][overDriveIndex]};

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {maxApb1Frequencies[overDriveIndex]};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {maxApb2Frequencies[overDriveIndex]};

#ifdef DISTORTOS_CHIP_RCC_PLLS_ENABLE

/// input frequency of main and audio PLLs, Hz
#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI)
constexpr uint32_t pllInFrequency {hsiFrequency};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
constexpr uint32_t pllInFrequency {DISTORTOS_CHIP_RCC_HSE_FREQUENCY};
#endif

/// VCO input frequency of main and audio PLLs, Hz
constexpr uint32_t vcoInFrequency {pllInFrequency / DISTORTOS_CHIP_RCC_PLLM};

static_assert(minVcoInFrequency <= vcoInFrequency && vcoInFrequency <= maxVcoInFrequency,
		"Invalid VCO input frequency!");

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

/// VCO output frequency of main PLL, Hz
constexpr uint32_t vcoOutFrequency {vcoInFrequency * DISTORTOS_CHIP_RCC_PLLN};

static_assert(minVcoOutFrequency <= vcoOutFrequency && vcoOutFrequency <= maxVcoOutFrequency,
		"Invalid VCO output frequency!");

/// main PLL output frequency, Hz
constexpr uint32_t pllOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLP};

static_assert(pllOutFrequency <= maxPllOutFrequency, "Invalid PLL output frequency!");

/// main PLL "Q" output frequency, Hz
constexpr uint32_t pllqOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLQ};

static_assert(pllqOutFrequency <= maxPllqOutFrequency, "Invalid PLL \"/Q\" output frequency!");

#if defined(DISTORTOS_CHIP_STM32F76) || defined(DISTORTOS_CHIP_STM32F77)

/// main PLL "R" output frequency, Hz
constexpr uint32_t pllrOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLR};

#endif	// defined(DISTORTOS_CHIP_STM32F76) || defined(DISTORTOS_CHIP_STM32F77)

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLLI2S_ENABLE

/// VCO output frequency of PLLI2S, Hz
constexpr uint32_t plli2sVcoOutFrequency {vcoInFrequency * DISTORTOS_CHIP_RCC_PLLI2SN};

static_assert(minVcoOutFrequency <= plli2sVcoOutFrequency && plli2sVcoOutFrequency <= maxVcoOutFrequency,
		"Invalid PLLI2S VCO output frequency!");

#ifdef DISTORTOS_CHIP_RCC_PLLI2SP

/// PLLI2S output frequency, Hz
constexpr uint32_t plli2sOutFrequency {plli2sVcoOutFrequency / DISTORTOS_CHIP_RCC_PLLI2SP};

static_assert(plli2sOutFrequency <= maxPlli2sOutFrequency, "Invalid PLLI2S output frequency!");

#endif	// def DISTORTOS_CHIP_RCC_PLLI2SP

/// PLLI2S "Q" output frequency, Hz
constexpr uint32_t plli2sqOutFrequency {plli2sVcoOutFrequency / DISTORTOS_CHIP_RCC_PLLI2SQ};

static_assert(plli2sqOutFrequency <= maxPlli2sqOutFrequency, "Invalid PLLI2S \"/Q\" output frequency!");

/// PLLI2S "R" output frequency, Hz
constexpr uint32_t plli2srOutFrequency {plli2sVcoOutFrequency / DISTORTOS_CHIP_RCC_PLLI2SR};

static_assert(plli2srOutFrequency <= maxPlli2srOutFrequency, "Invalid PLLI2S \"/R\" output frequency!");

#endif	// def DISTORTOS_CHIP_RCC_PLLI2S_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLLSAI_ENABLE

/// VCO output frequency of PLLSAI, Hz
constexpr uint32_t pllsaiVcoOutFrequency {vcoInFrequency * DISTORTOS_CHIP_RCC_PLLSAIN};

static_assert(minVcoOutFrequency <= pllsaiVcoOutFrequency && pllsaiVcoOutFrequency <= maxVcoOutFrequency,
		"Invalid PLLSAI VCO output frequency!");

/// PLLSAI output frequency, Hz
constexpr uint32_t pllsaiOutFrequency {pllsaiVcoOutFrequency / DISTORTOS_CHIP_RCC_PLLSAIP};

static_assert(pllsaiOutFrequency <= maxPllsaiOutFrequency, "Invalid PLLSAI output frequency!");

/// PLLSAI "Q" output frequency, Hz
constexpr uint32_t pllsaiqOutFrequency {pllsaiVcoOutFrequency / DISTORTOS_CHIP_RCC_PLLSAIQ};

static_assert(pllsaiqOutFrequency <= maxPllsaiqOutFrequency, "Invalid PLLSAI \"/Q\" output frequency!");

#ifdef DISTORTOS_CHIP_RCC_PLLSAIR

/// PLLSAI "R" output frequency, Hz
constexpr uint32_t pllsairOutFrequency {pllsaiVcoOutFrequency / DISTORTOS_CHIP_RCC_PLLSAIR};

static_assert(pllsairOutFrequency <= maxPllsairOutFrequency, "Invalid PLLSAI \"/R\" output frequency!");

#endif	// def DISTORTOS_CHIP_RCC_PLLSAIR

#endif	// def DISTORTOS_CHIP_RCC_PLLSAI_ENABLE

/// PLL48CLK frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_PLL48CLK_PLLQ)
constexpr uint32_t pll48clkFrequency {pllqOutFrequency};
#elif defined(DISTORTOS_CHIP_RCC_PLL48CLK_PLLSAIP)
constexpr uint32_t pll48clkFrequency {pllsaiOutFrequency};
#endif	// defined(DISTORTOS_CHIP_RCC_PLL48CLK_PLLSAIP)

#endif	// def DISTORTOS_CHIP_RCC_PLLS_ENABLE

/// SYSCLK frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI)
constexpr uint32_t sysclkFrequency {hsiFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_HSE_FREQUENCY};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllOutFrequency};
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)

#if defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_PLL48CLK) || defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_SYSCLK)

/// SDMMC1 adapter clock frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_PLL48CLK)
constexpr uint32_t sdmmc1clkFrequency {pll48clkFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_SYSCLK)
constexpr uint32_t sdmmc1clkFrequency {sysclkFrequency};
#endif	// defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_SYSCLK)

static_assert(sdmmc1clkFrequency <= maxSdmmcFrequency, "Invalid SDMMC1 adapter clock frequency!");

#endif	// defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_PLL48CLK) || defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_SYSCLK)

#if defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_PLL48CLK) || defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_SYSCLK)

/// SDMMC2 adapter clock frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_PLL48CLK)
constexpr uint32_t sdmmc2clkFrequency {pll48clkFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_SYSCLK)
constexpr uint32_t sdmmc2clkFrequency {sysclkFrequency};
#endif	// defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_SYSCLK)

static_assert(sdmmc2clkFrequency <= maxSdmmcFrequency, "Invalid SDMMC2 adapter clock frequency!");

#endif	// defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_PLL48CLK) || defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_SYSCLK)

#else	// !def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY};

#if DISTORTOS_CHIP_RCC_PLL48CLK_FREQUENCY != 0

/// PLL48CLK frequency, Hz
constexpr uint32_t pll48clkFrequency {DISTORTOS_CHIP_RCC_PLL48CLK_FREQUENCY};

#endif	// DISTORTOS_CHIP_RCC_PLL48CLK_FREQUENCY != 0

#if DISTORTOS_CHIP_RCC_SDMMC1CLK_FREQUENCY != 0

/// SDMMC1 adapter clock frequency, Hz
constexpr uint32_t sdmmc1clkFrequency {DISTORTOS_CHIP_RCC_SDMMC1CLK_FREQUENCY};

#endif	// DISTORTOS_CHIP_RCC_SDMMC1CLK_FREQUENCY != 0

#if DISTORTOS_CHIP_RCC_SDMMC2CLK_FREQUENCY != 0

/// SDMMC2 adapter clock frequency, Hz
constexpr uint32_t sdmmc2clkFrequency {DISTORTOS_CHIP_RCC_SDMMC2CLK_FREQUENCY};

#endif	// DISTORTOS_CHIP_RCC_SDMMC2CLK_FREQUENCY != 0

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {maxApb1Frequencies[1]};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {maxApb2Frequencies[1]};

#endif	// !def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / DISTORTOS_CHIP_RCC_HPRE};

/// APB1 frequency, Hz
constexpr uint32_t apb1Frequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE1};

static_assert(apb1Frequency <= maxApb1Frequency, "Invalid APB1 (low speed) frequency!");

/// APB2 frequency, Hz
constexpr uint32_t apb2Frequency {ahbFrequency / DISTORTOS_CHIP_RCC_PPRE2};

static_assert(apb2Frequency <= maxApb2Frequency, "Invalid APB2 (high speed) frequency!");

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
