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
#elif defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411)
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
#elif defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411)
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
#elif defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411)
constexpr uint32_t maxApb2Frequency {100000000};
#else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)
constexpr uint32_t maxApb2Frequency {90000000};
#endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) &&
		// !defined(CONFIG_CHIP_STM32F415) && !defined(CONFIG_CHIP_STM32F417) && !defined(CONFIG_CHIP_STM32F410) &&
		// !defined(CONFIG_CHIP_STM32F411)

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
