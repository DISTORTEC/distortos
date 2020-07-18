/**
 * \file
 * \brief Low-level chip initializer for STM32F0
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32F0-FLASH.hpp"
#include "distortos/chip/STM32F0-RCC.hpp"

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
 * \brief Low-level chip initializer for STM32F0
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void chipLowLevelInitializer()
{
#ifdef DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(true);
#else	// !def DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(false);
#endif	// !def DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS

#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSI48_ENABLE
	enableHsi48();
#endif	// def DISTORTOS_CHIP_RCC_HSI48_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSIPREDIV) || defined(DISTORTOS_CHIP_RCC_PLLSRC_HSEPREDIV) || \
		defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI48PREDIV)
	configurePrediv(DISTORTOS_CHIP_RCC_PREDIV);
#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_HSIPREDIV) || defined(DISTORTOS_CHIP_RCC_PLLSRC_HSEPREDIV) ||
		// defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI48PREDIV)

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSIDIV2)
	enablePll(PllClockSource::hsiDiv2, DISTORTOS_CHIP_RCC_PLLMUL);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSIPREDIV)
	enablePll(PllClockSource::hsiPrediv, DISTORTOS_CHIP_RCC_PLLMUL);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSEPREDIV)
	enablePll(PllClockSource::hsePrediv, DISTORTOS_CHIP_RCC_PLLMUL);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI48PREDIV)
	enablePll(PllClockSource::hsi48Prediv, DISTORTOS_CHIP_RCC_PLLMUL);
#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI48PREDIV)

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

	configureAhbClockDivider(DISTORTOS_CHIP_RCC_HPRE);
	configureApbClockDivider(DISTORTOS_CHIP_RCC_PPRE);

	constexpr uint8_t flashLatency {(sysclkFrequency - 1) / 24000000};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI)
	switchSystemClock(SystemClockSource::hsi);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI48)
	switchSystemClock(SystemClockSource::hsi48);
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI48)

#endif	// def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->AHBENR |=
#ifdef DISTORTOS_CHIP_GPIOA_ENABLE
			RCC_AHBENR_GPIOAEN |
#endif	// def DISTORTOS_CHIP_GPIOA_ENABLE
#ifdef DISTORTOS_CHIP_GPIOB_ENABLE
			RCC_AHBENR_GPIOBEN |
#endif	// def DISTORTOS_CHIP_GPIOB_ENABLE
#ifdef DISTORTOS_CHIP_GPIOC_ENABLE
			RCC_AHBENR_GPIOCEN |
#endif	// def DISTORTOS_CHIP_GPIOC_ENABLE
#ifdef DISTORTOS_CHIP_GPIOD_ENABLE
			RCC_AHBENR_GPIODEN |
#endif	// def DISTORTOS_CHIP_GPIOD_ENABLE
#ifdef DISTORTOS_CHIP_GPIOE_ENABLE
			RCC_AHBENR_GPIOEEN |
#endif	// def DISTORTOS_CHIP_GPIOE_ENABLE
#ifdef DISTORTOS_CHIP_GPIOF_ENABLE
			RCC_AHBENR_GPIOFEN |
#endif	// def DISTORTOS_CHIP_GPIOF_ENABLE
#ifdef DISTORTOS_CHIP_GPIOG_ENABLE
			RCC_AHBENR_GPIOGEN |
#endif	// def DISTORTOS_CHIP_GPIOG_ENABLE
#ifdef DISTORTOS_CHIP_GPIOH_ENABLE
			RCC_AHBENR_GPIOHEN |
#endif	// def DISTORTOS_CHIP_GPIOH_ENABLE
#ifdef DISTORTOS_CHIP_GPIOI_ENABLE
			RCC_AHBENR_GPIOIEN |
#endif	// def DISTORTOS_CHIP_GPIOI_ENABLE
#ifdef DISTORTOS_CHIP_GPIOJ_ENABLE
			RCC_AHBENR_GPIOJEN |
#endif	// def DISTORTOS_CHIP_GPIOJ_ENABLE
#ifdef DISTORTOS_CHIP_GPIOK_ENABLE
			RCC_AHBENR_GPIOKEN |
#endif	// def DISTORTOS_CHIP_GPIOK_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
