/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F1
 *
 * \author Copyright (C) 2016-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32F1-FLASH.hpp"
#include "distortos/chip/STM32F1-RCC.hpp"

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
	enableHse(true);
#else	// !def CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def CONFIG_CHIP_STM32F1_RCC_HSE_CLOCK_BYPASS

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
	configurePrediv1ClockSource(false);
#elif defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)
	configurePrediv1ClockSource(true);
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PREDIV1SRC_PLL2)

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

	configurePrediv1(CONFIG_CHIP_STM32F1_RCC_PREDIV1);

	constexpr bool pllClockSourcePrediv1 {true};

#endif	// defined(CONFIG_CHIP_STM32F1_RCC_PLLSRC_PREDIV1)

#ifdef CONFIG_CHIP_STM32F1_RCC_PLLMUL6_5
	enablePll(pllClockSourcePrediv1, pllmul6_5);
#else	// !def CONFIG_CHIP_STM32F1_RCC_PLLMUL6_5
	enablePll(pllClockSourcePrediv1,
			CONFIG_CHIP_STM32F1_RCC_PLLMUL_NUMERATOR / CONFIG_CHIP_STM32F1_RCC_PLLMUL_DENOMINATOR);
#endif	// !def CONFIG_CHIP_STM32F1_RCC_PLLMUL6_5

#endif	// def CONFIG_CHIP_STM32F1_RCC_PLL_ENABLE

	configureAhbClockDivider(CONFIG_CHIP_STM32F1_RCC_HPRE);
	configureApbClockDivider(false, CONFIG_CHIP_STM32F1_RCC_PPRE1);
	configureApbClockDivider(true, CONFIG_CHIP_STM32F1_RCC_PPRE2);

#ifndef CONFIG_CHIP_STM32F100

	constexpr uint8_t flashLatency {(sysclkFrequency - 1) / 24000000};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#endif	// !def CONFIG_CHIP_STM32F100

#if defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSI)
	switchSystemClock(SystemClockSource::hsi);
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#endif	// defined(CONFIG_CHIP_STM32F1_RCC_SYSCLK_PLL)

#endif	// def CONFIG_CHIP_STM32F1_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->APB2ENR |=
#if defined(CONFIG_CHIP_STM32_GPIOV1_GPIOA_ENABLE)
			RCC_APB2ENR_IOPAEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV1_GPIOA_ENABLE
#if defined(CONFIG_CHIP_STM32_GPIOV1_GPIOB_ENABLE)
			RCC_APB2ENR_IOPBEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV1_GPIOB_ENABLE
#if defined(CONFIG_CHIP_STM32_GPIOV1_GPIOC_ENABLE)
			RCC_APB2ENR_IOPCEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV1_GPIOC_ENABLE
#if defined(CONFIG_CHIP_STM32_GPIOV1_GPIOD_ENABLE)
			RCC_APB2ENR_IOPDEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV1_GPIOD_ENABLE
#if defined(CONFIG_CHIP_STM32_GPIOV1_GPIOE_ENABLE)
			RCC_APB2ENR_IOPEEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV1_GPIOE_ENABLE
#if defined(CONFIG_CHIP_STM32_GPIOV1_GPIOF_ENABLE)
			RCC_APB2ENR_IOPFEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV1_GPIOF_ENABLE
#if defined(CONFIG_CHIP_STM32_GPIOV1_GPIOG_ENABLE)
			RCC_APB2ENR_IOPGEN |
#endif	// def CONFIG_CHIP_STM32_GPIOV1_GPIOG_ENABLE
			0;
}

}	// namespace chip

}	// namespace distortos
