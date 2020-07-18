/**
 * \file
 * \brief Low-level chip initializer for STM32F1
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32F1-FLASH.hpp"
#include "distortos/chip/STM32F1-RCC.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

namespace distortos
{

namespace chip
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Low-level chip initializer for STM32F1
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void chipLowLevelInitializer()
{
#ifndef DISTORTOS_CHIP_STM32F100

#ifdef DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(true);
#else	// !def DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(false);
#endif	// !def DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE

#endif	// !def DISTORTOS_CHIP_STM32F100

#ifdef DISTORTOS_CHIP_FLASH_HALF_CYCLE_ACCESS_ENABLE
	configureFlashHalfCycleAccess(true);
#else	// !def DISTORTOS_CHIP_FLASH_HALF_CYCLE_ACCESS_ENABLE
	configureFlashHalfCycleAccess(false);
#endif	// !def DISTORTOS_CHIP_FLASH_HALF_CYCLE_ACCESS_ENABLE

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS

#if (defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)) && \
		defined(DISTORTOS_CHIP_RCC_PREDIV2_PLL2_PLL3_ENABLE)

	configurePrediv2(DISTORTOS_CHIP_RCC_PREDIV2);

#ifdef DISTORTOS_CHIP_RCC_PLL2_ENABLE

	enablePll2(DISTORTOS_CHIP_RCC_PLL2MUL);

#endif	// def DISTORTOS_CHIP_RCC_PLL2_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL3_ENABLE

	enablePll3(DISTORTOS_CHIP_RCC_PLL3MUL);

#endif	// def DISTORTOS_CHIP_RCC_PLL3_ENABLE

#endif	// (defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)) &&
		// defined(DISTORTOS_CHIP_RCC_PREDIV2_PLL2_PLL3_ENABLE)

#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSIDIV2)

	constexpr bool pllClockSourcePrediv1 {};

#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_PREDIV1)

#if defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

#if defined(DISTORTOS_CHIP_RCC_PREDIV1SRC_HSE)
	configurePrediv1ClockSource(false);
#elif defined(DISTORTOS_CHIP_RCC_PREDIV1SRC_PLL2)
	configurePrediv1ClockSource(true);
#endif	// defined(DISTORTOS_CHIP_RCC_PREDIV1SRC_PLL2)

#endif	// defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

	configurePrediv1(DISTORTOS_CHIP_RCC_PREDIV1);

	constexpr bool pllClockSourcePrediv1 {true};

#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_PREDIV1)

#ifdef DISTORTOS_CHIP_RCC_PLLMUL6_5
	enablePll(pllClockSourcePrediv1, pllmul6_5);
#else	// !def DISTORTOS_CHIP_RCC_PLLMUL6_5
	enablePll(pllClockSourcePrediv1,
			DISTORTOS_CHIP_RCC_PLLMUL_NUMERATOR / DISTORTOS_CHIP_RCC_PLLMUL_DENOMINATOR);
#endif	// !def DISTORTOS_CHIP_RCC_PLLMUL6_5

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

	configureAhbClockDivider(DISTORTOS_CHIP_RCC_HPRE);
	configureApbClockDivider(false, DISTORTOS_CHIP_RCC_PPRE1);
	configureApbClockDivider(true, DISTORTOS_CHIP_RCC_PPRE2);

#ifndef DISTORTOS_CHIP_STM32F100

	constexpr uint8_t flashLatency {(sysclkFrequency - 1) / 24000000};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#endif	// !def DISTORTOS_CHIP_STM32F100

#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI)
	switchSystemClock(SystemClockSource::hsi);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)

#endif	// def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->APB2ENR |=
#if defined(DISTORTOS_CHIP_GPIOA_ENABLE)
			RCC_APB2ENR_IOPAEN |
#endif	// def DISTORTOS_CHIP_GPIOA_ENABLE
#if defined(DISTORTOS_CHIP_GPIOB_ENABLE)
			RCC_APB2ENR_IOPBEN |
#endif	// def DISTORTOS_CHIP_GPIOB_ENABLE
#if defined(DISTORTOS_CHIP_GPIOC_ENABLE)
			RCC_APB2ENR_IOPCEN |
#endif	// def DISTORTOS_CHIP_GPIOC_ENABLE
#if defined(DISTORTOS_CHIP_GPIOD_ENABLE)
			RCC_APB2ENR_IOPDEN |
#endif	// def DISTORTOS_CHIP_GPIOD_ENABLE
#if defined(DISTORTOS_CHIP_GPIOE_ENABLE)
			RCC_APB2ENR_IOPEEN |
#endif	// def DISTORTOS_CHIP_GPIOE_ENABLE
#if defined(DISTORTOS_CHIP_GPIOF_ENABLE)
			RCC_APB2ENR_IOPFEN |
#endif	// def DISTORTOS_CHIP_GPIOF_ENABLE
#if defined(DISTORTOS_CHIP_GPIOG_ENABLE)
			RCC_APB2ENR_IOPGEN |
#endif	// def DISTORTOS_CHIP_GPIOG_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
