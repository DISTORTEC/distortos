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

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/STM32F1-FLASH.hpp"
#include "distortos/chip/STM32F1-RCC.hpp"

#include "distortos/architecture/ARMv6-M-ARMv7-M-configureSysTick.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
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

#ifdef CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS
	constexpr bool hseClockBypass {true};
#else	// !def CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS
	constexpr bool hseClockBypass {};
#endif	// !def CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS
	enableHse(hseClockBypass);

#if (defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)) && \
		defined(CONFIG_CHIP_STM32F1_RCC_PREDIV2_PLL2_PLL3_ENABLE)

	configurePrediv2(CONFIG_CHIP_STM32F1_RCC_PREDIV2);

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL2_ENABLE

	enablePll2(CONFIG_CHIP_STM32F1_RCC_PLL2MUL);

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL2_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL3_ENABLE

	enablePll3(CONFIG_CHIP_STM32F1_RCC_PLL3MUL);

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL3_ENABLE

#endif	// (defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)) &&
		// defined(CONFIG_CHIP_STM32F1_RCC_PREDIV2_PLL2_PLL3_ENABLE)

#endif	// def CONFIG_CHIP_STM32F1_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F1_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_HSIDIV2)

	constexpr bool pllClockSourcePrediv1 {};

#elif defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_PREDIV1)

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

#if defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_HSE)
	constexpr bool prediv1ClockSourcePll2 {};
#elif defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)
	constexpr bool prediv1ClockSourcePll2 {true};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)

	configurePrediv1ClockSource(prediv1ClockSourcePll2);

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

	configurePrediv1(CONFIG_CHIP_STM32F1_RCC_PREDIV1);

	constexpr bool pllClockSourcePrediv1 {true};

#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_PREDIV1)

#ifdef CONFIG_CHIP_STM32F1_RCC_PLLMUL6_5
	constexpr uint8_t pllmul {pllmul6_5};
#else	// !def CONFIG_CHIP_STM32F1_RCC_PLLMUL6_5
	constexpr uint8_t pllmul {CONFIG_CHIP_STM32F1_RCC_PLLMUL_NUMERATOR / CONFIG_CHIP_STM32F1_RCC_PLLMUL_DENOMINATOR};
#endif	// !def CONFIG_CHIP_STM32F1_RCC_PLLMUL6_5

	enablePll(pllClockSourcePrediv1, pllmul);

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL_ENABLE

	configureAhbClockDivider(CONFIG_CHIP_STM32F1_RCC_HPRE);
	configureApbClockDivider(false, CONFIG_CHIP_STM32F1_RCC_PPRE1);
	configureApbClockDivider(true, CONFIG_CHIP_STM32F1_RCC_PPRE2);

#ifndef CONFIG_CHIP_STM32F100

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / 24000000};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#endif	// !def CONFIG_CHIP_STM32F100

#if defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSI)
	constexpr SystemClockSource systemClockSource {SystemClockSource::hsi};
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSE)
	constexpr SystemClockSource systemClockSource {SystemClockSource::hse};
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)
	constexpr SystemClockSource systemClockSource {SystemClockSource::pll};
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)

	switchSystemClock(systemClockSource);

#endif	// def CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

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
