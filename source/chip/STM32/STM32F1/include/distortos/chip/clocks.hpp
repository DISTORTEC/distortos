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

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
