/**
 * \file
 * \brief Definitions of clocks for STM32F4
 *
 * \author Copyright (C) 2016-2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

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
#if defined(DISTORTOS_CHIP_STM32F401)
constexpr uint32_t minVcoOutFrequency {192000000};
#else	// !defined(DISTORTOS_CHIP_STM32F401)
constexpr uint32_t minVcoOutFrequency {100000000};
#endif	// !defined(DISTORTOS_CHIP_STM32F401)

/// maximum allowed value for VCO output frequency, Hz
constexpr uint32_t maxVcoOutFrequency {432000000};

/// maximum allowed values for PLL output frequency, Hz
/// [...][0] - over-drive disabled
/// [...][1] - over-drive enabled
/// [0][...] - voltage scale 1
/// [1][...] - voltage scale 2
/// [2][...] - voltage scale 3
#if defined(DISTORTOS_CHIP_STM32F401)
constexpr uint32_t maxPllOutFrequencies[3][2]
{
		{84000000, 84000000},
		{84000000, 84000000},
		{60000000, 60000000},
};
#elif defined(DISTORTOS_CHIP_STM32F405) || defined(DISTORTOS_CHIP_STM32F407) || defined(DISTORTOS_CHIP_STM32F415) || \
		defined(DISTORTOS_CHIP_STM32F417)
constexpr uint32_t maxPllOutFrequencies[3][2]
{
		{168000000, 168000000},
		{144000000, 144000000},
		{144000000, 144000000},
};
#elif defined(DISTORTOS_CHIP_STM32F410) || defined(DISTORTOS_CHIP_STM32F411) || defined(DISTORTOS_CHIP_STM32F412) || \
		defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
constexpr uint32_t maxPllOutFrequencies[3][2]
{
		{100000000, 100000000},
		{84000000, 84000000},
		{64000000, 64000000},
};
#else	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F405) &&
		// !defined(DISTORTOS_CHIP_STM32F407) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F415) &&
		// !defined(DISTORTOS_CHIP_STM32F417) && !defined(DISTORTOS_CHIP_STM32F423)
constexpr uint32_t maxPllOutFrequencies[3][2]
{
		{168000000, 180000000},
		{144000000, 144000000},
		{120000000, 120000000},
};
#endif	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F405) &&
		// !defined(DISTORTOS_CHIP_STM32F407) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F415) &&
		// !defined(DISTORTOS_CHIP_STM32F417) && !defined(DISTORTOS_CHIP_STM32F423)

/// maximum allowed value for PLL "Q" output frequency, Hz
constexpr uint32_t maxPllqOutFrequency {48000000};

/// maximum allowed APB1 (low speed) frequencies, Hz
/// [0] - over-drive disabled
/// [1] - over-drive enabled
#if defined(DISTORTOS_CHIP_STM32F401) || defined(DISTORTOS_CHIP_STM32F405) || defined(DISTORTOS_CHIP_STM32F407) || \
		defined(DISTORTOS_CHIP_STM32F415) || defined(DISTORTOS_CHIP_STM32F417)
constexpr uint32_t maxApb1Frequencies[2] {42000000, 42000000};
#elif defined(DISTORTOS_CHIP_STM32F410) || defined(DISTORTOS_CHIP_STM32F411) || defined(DISTORTOS_CHIP_STM32F412) || \
		defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
constexpr uint32_t maxApb1Frequencies[2] {50000000, 50000000};
#else	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F405) &&
		// !defined(DISTORTOS_CHIP_STM32F407) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F415) &&
		// !defined(DISTORTOS_CHIP_STM32F417) && !defined(DISTORTOS_CHIP_STM32F423)
constexpr uint32_t maxApb1Frequencies[2] {42000000, 45000000};
#endif	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F405) &&
		// !defined(DISTORTOS_CHIP_STM32F407) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F415) &&
		// !defined(DISTORTOS_CHIP_STM32F417) && !defined(DISTORTOS_CHIP_STM32F423)

/// maximum allowed APB2 (high speed) frequencies, Hz
/// [0] - over-drive disabled
/// [1] - over-drive enabled
#if defined(DISTORTOS_CHIP_STM32F401) || defined(DISTORTOS_CHIP_STM32F405) || defined(DISTORTOS_CHIP_STM32F407) || \
		defined(DISTORTOS_CHIP_STM32F415) || defined(DISTORTOS_CHIP_STM32F417)
constexpr uint32_t maxApb2Frequencies[2] {84000000, 84000000};
#elif defined(DISTORTOS_CHIP_STM32F410) || defined(DISTORTOS_CHIP_STM32F411) || defined(DISTORTOS_CHIP_STM32F412) || \
	defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
constexpr uint32_t maxApb2Frequencies[2] {100000000, 100000000};
#else	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F405) &&
		// !defined(DISTORTOS_CHIP_STM32F407) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F415) &&
		// !defined(DISTORTOS_CHIP_STM32F417) && !defined(DISTORTOS_CHIP_STM32F423)
constexpr uint32_t maxApb2Frequencies[2] {84000000, 90000000};
#endif	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F405) &&
		// !defined(DISTORTOS_CHIP_STM32F407) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F415) &&
		// !defined(DISTORTOS_CHIP_STM32F417) && !defined(DISTORTOS_CHIP_STM32F423)

/// maximum allowed value for SDIO adapter clock frequency, Hz
constexpr uint32_t maxSdioFrequency {48000000};

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

#if (defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) || defined(DISTORTOS_CHIP_STM32F43) || \
		defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || \
		defined(DISTORTOS_CHIP_STM32F479)) && defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

static_assert(DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE == 1, "Over-drive mode requires voltage scale 1 mode!");
static_assert(DISTORTOS_CHIP_VDD_MV >= 2100, "Over-drive mode must not be enabled when supply voltage is below 2.1 V!");

/// over-drive index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t overDriveIndex {1};

#else	// !(defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) ||
		// defined(DISTORTOS_CHIP_STM32F43) || defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) ||
		// defined(DISTORTOS_CHIP_STM32F479)) || !defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

/// over-drive index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t overDriveIndex {0};

#endif	// !(defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) ||
		// defined(DISTORTOS_CHIP_STM32F43) || defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) ||
		// defined(DISTORTOS_CHIP_STM32F479)) || !defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

/// voltage scale index for \a maxPllOutFrequencies array (maxPllOutFrequencies[voltageScaleIndex][overDriveIndex])
constexpr uint8_t voltageScaleIndex {DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE - 1};

/// maximum allowed value for PLL output frequency, Hz
constexpr uint32_t maxPllOutFrequency {maxPllOutFrequencies[voltageScaleIndex][overDriveIndex]};

/// maximum allowed APB1 (low speed) frequency, Hz
constexpr uint32_t maxApb1Frequency {maxApb1Frequencies[overDriveIndex]};

/// maximum allowed APB2 (high speed) frequency, Hz
constexpr uint32_t maxApb2Frequency {maxApb2Frequencies[overDriveIndex]};

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

/// PLL input frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI)
constexpr uint32_t pllInFrequency {hsiFrequency};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
constexpr uint32_t pllInFrequency {DISTORTOS_CHIP_RCC_HSE_FREQUENCY};
#endif

/// VCO input frequency, Hz
constexpr uint32_t vcoInFrequency {pllInFrequency / DISTORTOS_CHIP_RCC_PLLM};

static_assert(minVcoInFrequency <= vcoInFrequency && vcoInFrequency <= maxVcoInFrequency,
		"Invalid VCO input frequency!");

/// VCO output frequency, Hz
constexpr uint32_t vcoOutFrequency {vcoInFrequency * DISTORTOS_CHIP_RCC_PLLN};

static_assert(minVcoOutFrequency <= vcoOutFrequency && vcoOutFrequency <= maxVcoOutFrequency,
		"Invalid VCO output frequency!");

/// PLL output frequency, Hz
constexpr uint32_t pllOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLP};

static_assert(pllOutFrequency <= maxPllOutFrequency, "Invalid PLL output frequency!");

/// PLL "Q" output frequency, Hz
constexpr uint32_t pllqOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLQ};

static_assert(pllqOutFrequency <= maxPllqOutFrequency, "Invalid PLL \"/Q\" output frequency!");

#if defined(DISTORTOS_CHIP_STM32F412) || defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423) || \
		defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || defined(DISTORTOS_CHIP_STM32F479)

/// PLL "R" output frequency, Hz
constexpr uint32_t pllrOutFrequency {vcoOutFrequency / DISTORTOS_CHIP_RCC_PLLR};

#endif	// defined(DISTORTOS_CHIP_STM32F412) || defined(DISTORTOS_CHIP_STM32F413) ||
		// defined(DISTORTOS_CHIP_STM32F423) || defined(DISTORTOS_CHIP_STM32F446) ||
		// defined(DISTORTOS_CHIP_STM32F469) || defined(DISTORTOS_CHIP_STM32F479)

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

/// SYSCLK frequency, Hz
#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI)
constexpr uint32_t sysclkFrequency {hsiFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_HSE_FREQUENCY};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllOutFrequency};
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLLR)
constexpr uint32_t sysclkFrequency {pllrOutFrequency};
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLLR)

#if !defined(DISTORTOS_CHIP_STM32F410)

/// SDIO adapter clock frequency, Hz
constexpr uint32_t sdioclkFrequency {pllqOutFrequency};

static_assert(sdioclkFrequency <= maxSdioFrequency, "Invalid SDIO adapter clock frequency!");

#endif	// !defined(DISTORTOS_CHIP_STM32F410)

#else	// !def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {DISTORTOS_CHIP_RCC_SYSCLK_FREQUENCY};

#if DISTORTOS_CHIP_RCC_SDIOCLK_FREQUENCY != 0

/// SDIO adapter clock frequency, Hz
constexpr uint32_t sdioclkFrequency {DISTORTOS_CHIP_RCC_SDIOCLK_FREQUENCY};

#endif	// DISTORTOS_CHIP_RCC_SDIOCLK_FREQUENCY != 0

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

#endif	// SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
