/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32L0
 *
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/STM32L0-FLASH.hpp"
#include "distortos/chip/STM32L0-PWR.hpp"
#include "distortos/chip/STM32L0-RCC.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32L0_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(true);
#else	// !def CONFIG_CHIP_STM32L0_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(false);
#endif	// !def CONFIG_CHIP_STM32L0_FLASH_PREFETCH_ENABLE

#ifdef CONFIG_CHIP_STM32L0_STANDARD_CLOCK_CONFIGURATION_ENABLE

	configureVoltageScaling(CONFIG_CHIP_STM32L0_PWR_VOLTAGE_SCALE_MODE);

#ifdef CONFIG_CHIP_STM32L0_RCC_MSI_ENABLE
	enableMsi(CONFIG_CHIP_STM32L0_RCC_MSIRANGE);
#endif

#ifdef CONFIG_CHIP_STM32L0_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32L0_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def CONFIG_CHIP_STM32L0_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def CONFIG_CHIP_STM32L0_RCC_HSE_CLOCK_BYPASS

#endif	// def CONFIG_CHIP_STM32L0_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32L0_RCC_HSI16_ENABLE
	enableHsi16();
#endif	// def CONFIG_CHIP_STM32L0_RCC_HSI16_ENABLE

#ifdef CONFIG_CHIP_STM32L0_RCC_HSI48_ENABLE
	enableHsi48();
#endif	// def CONFIG_CHIP_STM32L0_RCC_HSI48_ENABLE

#ifdef CONFIG_CHIP_STM32L0_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32L0_RCC_PLLSRC_HSI16)
	configurePllClockSource(false);
#elif defined(CONFIG_CHIP_STM32L0_RCC_PLLSRC_HSE)
	configurePllClockSource(true);
#endif

	enablePll(CONFIG_CHIP_STM32L0_RCC_PLLMUL, CONFIG_CHIP_STM32L0_RCC_PLLDIV);

#endif	// def CONFIG_CHIP_STM32L0_RCC_PLL_ENABLE

	configureAhbClockDivider(CONFIG_CHIP_STM32L0_RCC_HPRE);
	configureApbClockDivider(false, CONFIG_CHIP_STM32L0_RCC_PPRE1);
	configureApbClockDivider(true, CONFIG_CHIP_STM32L0_RCC_PPRE2);

#if CONFIG_CHIP_STM32L0_PWR_VOLTAGE_SCALE_MODE == 3
	constexpr uint32_t frequencyThreshold {4200000};
#elif CONFIG_CHIP_STM32L0_PWR_VOLTAGE_SCALE_MODE == 2
	constexpr uint32_t frequencyThreshold {8000000};
#else
	constexpr uint32_t frequencyThreshold {16000000};
#endif

	constexpr uint8_t flashLatency {(sysclkFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#if defined(CONFIG_CHIP_STM32L0_RCC_SYSCLK_MSI)
	switchSystemClock(SystemClockSource::msi);
#elif defined(CONFIG_CHIP_STM32L0_RCC_SYSCLK_HSI16)
	switchSystemClock(SystemClockSource::hsi16);
#elif defined(CONFIG_CHIP_STM32L0_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(CONFIG_CHIP_STM32L0_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#endif	// defined(CONFIG_CHIP_STM32L0_RCC_SYSCLK_PLL)

#endif	// def CONFIG_CHIP_STM32L0_STANDARD_CLOCK_CONFIGURATION_ENABLE
}

}	// namespace chip

}	// namespace distortos
