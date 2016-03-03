/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F1
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/STM32F1-FLASH.hpp"

#include "distortos/architecture/configureSysTick.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32F1_FLASH_STANDARD_CONFIGURATION_ENABLE

#ifndef CONFIG_CHIP_STM32F100

#ifdef CONFIG_CHIP_STM32F1_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(true);
#else	// !def CONFIG_CHIP_STM32F1_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(false);
#endif	// !def CONFIG_CHIP_STM32F1_FLASH_PREFETCH_ENABLE

#endif	// !def CONFIG_CHIP_STM32F100

#ifdef CONFIG_CHIP_STM32F1_FLASH_HALF_CYCLE_ACCESS_ENABLE
	configureFlashHalfCycleAccess(true);
#else	// !def CONFIG_CHIP_STM32F1_FLASH_HALF_CYCLE_ACCESS_ENABLE
	configureFlashHalfCycleAccess(false);
#endif	// !def CONFIG_CHIP_STM32F1_FLASH_HALF_CYCLE_ACCESS_ENABLE

#endif	// def CONFIG_CHIP_STM32F1_FLASH_STANDARD_CONFIGURATION_ENABLE

	constexpr uint32_t ahbFrequency {CONFIG_CHIP_STM32F1_RCC_AHB_FREQUENCY};

#if !defined(CONFIG_CHIP_STM32F100) && defined(CONFIG_CHIP_STM32F1_FLASH_STANDARD_CONFIGURATION_ENABLE)

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / 24000000};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#endif	// !defined(CONFIG_CHIP_STM32F100) && defined(CONFIG_CHIP_STM32F1_FLASH_STANDARD_CONFIGURATION_ENABLE)

	constexpr uint32_t period {ahbFrequency / CONFIG_TICK_FREQUENCY};
	constexpr uint32_t periodDividedBy8 {period / 8};
	constexpr bool divideBy8 {period > architecture::maxSysTickPeriod};
	// at least one of the periods must be valid
	static_assert(period <= architecture::maxSysTickPeriod || periodDividedBy8 <= architecture::maxSysTickPeriod,
			"Invalid SysTick configuration!");
	architecture::configureSysTick(divideBy8 == false ? period : periodDividedBy8, divideBy8);
}

}	// namespace chip

}	// namespace distortos
