/**
 * \file
 * \brief Definitions of clocks for STM32F4
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <array>

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
#if defined(CONFIG_CHIP_STM32F401)
constexpr uint32_t minVcoOutFrequency {192000000};
#else	// !defined(CONFIG_CHIP_STM32F401)
constexpr uint32_t minVcoOutFrequency {100000000};
#endif	// !defined(CONFIG_CHIP_STM32F401)

/// maximum allowed value for VCO output frequency, Hz
constexpr uint32_t maxVcoOutFrequency {432000000};

/// maximum allowed value for PLL output frequency, Hz
/// [0] - in overdrive mode with voltage scale 1
/// [1] - with voltage scale 1
/// [2] - with voltage scale 2
/// [3] - with voltage scale 3
#if defined(CONFIG_CHIP_STM32F401)
constexpr std::array<uint32_t, 4> maxPllOutFrequency {0, 0, 84000000, 60000000};
#elif defined(CONFIG_CHIP_STM32F405) || defined(CONFIG_CHIP_STM32F407) || defined(CONFIG_CHIP_STM32F415) || \
		defined(CONFIG_CHIP_STM32F417)
constexpr std::array<uint32_t, 4> maxPllOutFrequency {0, 168000000, 144000000, 0};
#elif defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411) || defined(CONFIG_CHIP_STM32F412)
constexpr std::array<uint32_t, 4> maxPllOutFrequency {0, 100000000, 84000000, 64000000};
#else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)
constexpr std::array<uint32_t, 4> maxPllOutFrequency {180000000, 168000000, 144000000, 120000000};
#endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)

/// maximum allowed value for PLL "Q" output frequency, Hz
constexpr uint32_t maxPllqOutFrequency {48000000};

/// maximum allowed APB1 (low speed) frequency, Hz
#if defined(CONFIG_CHIP_STM32F401) || defined(CONFIG_CHIP_STM32F405) || defined(CONFIG_CHIP_STM32F407) || \
		defined(CONFIG_CHIP_STM32F415) || defined(CONFIG_CHIP_STM32F417)
constexpr uint32_t maxApb1Frequency {42000000};
#elif defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411) || defined(CONFIG_CHIP_STM32F412)
constexpr uint32_t maxApb1Frequency {50000000};
#else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)
constexpr uint32_t maxApb1Frequency {45000000};
#endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)

/// maximum allowed APB2 (high speed) frequency, Hz
#if defined(CONFIG_CHIP_STM32F401) || defined(CONFIG_CHIP_STM32F405) || defined(CONFIG_CHIP_STM32F407) || \
		defined(CONFIG_CHIP_STM32F415) || defined(CONFIG_CHIP_STM32F417)
constexpr uint32_t maxApb2Frequency {84000000};
#elif defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411) || defined(CONFIG_CHIP_STM32F412)
constexpr uint32_t maxApb2Frequency {100000000};
#else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)
constexpr uint32_t maxApb2Frequency {90000000};
#endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)

#ifdef CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

#if (defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) || \
		defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) && \
		defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

static_assert(CONFIG_CHIP_STM32F4_PWR_VOLTAGE_SCALE_MODE == 1, "Over-drive mode requires voltage scale 1 mode!");
static_assert(CONFIG_CHIP_STM32F4_VDD_MV >= 2100,
		"Over-drive mode must not be enabled when supply voltage is below 2.1V!");

/// voltage scale index for \a maxPllOutFrequency array
constexpr uint8_t voltageScaleIndex {0};

#else	// !(defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) ||
		// defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) ||
		// !defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

/// voltage scale index for \a maxPllOutFrequency array
constexpr uint8_t voltageScaleIndex {CONFIG_CHIP_STM32F4_PWR_VOLTAGE_SCALE_MODE};

#endif	// !(defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) ||
		// defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) ||
		// !defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

#ifdef CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

/// PLL input frequency, Hz
#if defined(CONFIG_CHIP_STM32F4_RCC_PLLSRC_HSI)
constexpr uint32_t pllInFrequency {hsiFrequency};
#elif defined(CONFIG_CHIP_STM32F4_RCC_PLLSRC_HSE)
constexpr uint32_t pllInFrequency {CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY};
#endif

/// VCO input frequency, Hz
constexpr uint32_t vcoInFrequency {pllInFrequency / CONFIG_CHIP_STM32F4_RCC_PLLM};

static_assert(minVcoInFrequency <= vcoInFrequency && vcoInFrequency <= maxVcoInFrequency,
		"Invalid VCO input frequency!");

/// VCO output frequency, Hz
constexpr uint32_t vcoOutFrequency {vcoInFrequency * CONFIG_CHIP_STM32F4_RCC_PLLN};

static_assert(minVcoOutFrequency <= vcoOutFrequency && vcoOutFrequency <= maxVcoOutFrequency,
		"Invalid VCO output frequency!");

/// PLL output frequency, Hz
constexpr uint32_t pllOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F4_RCC_PLLP};

static_assert(pllOutFrequency <= maxPllOutFrequency[voltageScaleIndex], "Invalid PLL output frequency!");

/// PLL "Q" output frequency, Hz
constexpr uint32_t pllqOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F4_RCC_PLLQ};

static_assert(pllqOutFrequency <= maxPllqOutFrequency, "Invalid PLL \"/Q\" output frequency!");

#if defined(CONFIG_CHIP_STM32F412) || defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || \
		defined(CONFIG_CHIP_STM32F479)

/// PLL "R" output frequency, Hz
constexpr uint32_t pllrOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F4_RCC_PLLR};

#endif	// defined(CONFIG_CHIP_STM32F412) || defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) ||
		// defined(CONFIG_CHIP_STM32F479)

#endif	// def CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

/// SYSCLK frequency, Hz
#if defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSI)
constexpr uint32_t sysclkFrequency {hsiFrequency};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSE)
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLL)
constexpr uint32_t sysclkFrequency {pllOutFrequency};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)
constexpr uint32_t sysclkFrequency {pllrOutFrequency};
#endif	// defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)

#else	// !def CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// SYSCLK frequency, Hz
constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F4_RCC_SYSCLK_FREQUENCY};

#endif	// !def CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {sysclkFrequency / CONFIG_CHIP_STM32F4_RCC_HPRE};

/// APB1 frequency, Hz
constexpr uint32_t apb1Frequency {ahbFrequency / CONFIG_CHIP_STM32F4_RCC_PPRE1};

static_assert(apb1Frequency <= maxApb1Frequency, "Invalid APB1 (low speed) frequency!");

/// APB2 frequency, Hz
constexpr uint32_t apb2Frequency {ahbFrequency / CONFIG_CHIP_STM32F4_RCC_PPRE2};

static_assert(apb2Frequency <= maxApb2Frequency, "Invalid APB2 (high speed) frequency!");

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
