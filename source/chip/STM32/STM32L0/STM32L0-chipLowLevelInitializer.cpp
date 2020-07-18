/**
 * \file
 * \brief Low-level chip initializer for STM32L0
 *
 * \author Copyright (C) 2017-2019 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32L0-FLASH.hpp"
#include "distortos/chip/STM32L0-PWR.hpp"
#include "distortos/chip/STM32L0-RCC.hpp"

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
 * \brief Low-level chip initializer for STM32L0
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

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	configureVoltageScaling(DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE);

#ifdef DISTORTOS_CHIP_RCC_MSI_ENABLE
	enableMsi(DISTORTOS_CHIP_RCC_MSIRANGE);
#endif

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS

#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSI16_ENABLE
	enableHsi16();
#endif	// def DISTORTOS_CHIP_RCC_HSI16_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSI48_ENABLE
	enableHsi48();
#endif	// def DISTORTOS_CHIP_RCC_HSI48_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI16)
	configurePllClockSource(false);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
	configurePllClockSource(true);
#endif

	enablePll(DISTORTOS_CHIP_RCC_PLLMUL, DISTORTOS_CHIP_RCC_PLLDIV);

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

	configureAhbClockDivider(DISTORTOS_CHIP_RCC_HPRE);
	configureApbClockDivider(false, DISTORTOS_CHIP_RCC_PPRE1);
	configureApbClockDivider(true, DISTORTOS_CHIP_RCC_PPRE2);

#if DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE == 3
	constexpr uint32_t frequencyThreshold {4200000};
#elif DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE == 2
	constexpr uint32_t frequencyThreshold {8000000};
#else
	constexpr uint32_t frequencyThreshold {16000000};
#endif

	constexpr uint8_t flashLatency {(sysclkFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#if defined(DISTORTOS_CHIP_RCC_SYSCLK_MSI)
	switchSystemClock(SystemClockSource::msi);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI16)
	switchSystemClock(SystemClockSource::hsi16);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)

#endif	// def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->IOPENR |=
#ifdef DISTORTOS_CHIP_GPIOA_ENABLE
			RCC_IOPENR_GPIOAEN |
#endif	// def DISTORTOS_CHIP_GPIOA_ENABLE
#ifdef DISTORTOS_CHIP_GPIOB_ENABLE
			RCC_IOPENR_GPIOBEN |
#endif	// def DISTORTOS_CHIP_GPIOB_ENABLE
#ifdef DISTORTOS_CHIP_GPIOC_ENABLE
			RCC_IOPENR_GPIOCEN |
#endif	// def DISTORTOS_CHIP_GPIOC_ENABLE
#ifdef DISTORTOS_CHIP_GPIOD_ENABLE
			RCC_IOPENR_GPIODEN |
#endif	// def DISTORTOS_CHIP_GPIOD_ENABLE
#ifdef DISTORTOS_CHIP_GPIOE_ENABLE
			RCC_IOPENR_GPIOEEN |
#endif	// def DISTORTOS_CHIP_GPIOE_ENABLE
#ifdef DISTORTOS_CHIP_GPIOH_ENABLE
			RCC_IOPENR_GPIOHEN |
#endif	// def DISTORTOS_CHIP_GPIOH_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
