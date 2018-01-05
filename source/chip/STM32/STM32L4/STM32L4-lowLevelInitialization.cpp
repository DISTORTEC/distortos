/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32L4
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32-bit-banding.h"
#include "distortos/chip/STM32L4-FLASH.hpp"
#include "distortos/chip/STM32L4-PWR.hpp"
#include "distortos/chip/STM32L4-RCC.hpp"

#include <algorithm>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32L4_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(true);
#else	// !def CONFIG_CHIP_STM32L4_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(false);
#endif	// !def CONFIG_CHIP_STM32L4_FLASH_PREFETCH_ENABLE

#ifdef CONFIG_CHIP_STM32L4_FLASH_DATA_CACHE_ENABLE
	enableDataCache();
#else	// !def CONFIG_CHIP_STM32L4_FLASH_DATA_CACHE_ENABLE
	disableDataCache();
#endif	// !def CONFIG_CHIP_STM32L4_FLASH_DATA_CACHE_ENABLE

#ifdef CONFIG_CHIP_STM32L4_FLASH_INSTRUCTION_CACHE_ENABLE
	enableInstructionCache();
#else	// !def CONFIG_CHIP_STM32L4_FLASH_INSTRUCTION_CACHE_ENABLE
	disableInstructionCache();
#endif	// !def CONFIG_CHIP_STM32L4_FLASH_INSTRUCTION_CACHE_ENABLE

#ifdef CONFIG_CHIP_STM32L4_STANDARD_CLOCK_CONFIGURATION_ENABLE

	STM32_BITBAND(RCC, APB1ENR1, PWREN) = 1;

	configureVoltageScaling(CONFIG_CHIP_STM32L4_PWR_VOLTAGE_SCALING_RANGE);

#ifdef CONFIG_CHIP_STM32L4_RCC_HSE_ENABLE
	enableHse(hseClockBypass);
#endif	// def CONFIG_CHIP_STM32L4_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32L4_RCC_HSI16_ENABLE
	enableHsi16();
#endif	// def CONFIG_CHIP_STM32L4_RCC_HSI16_ENABLE

#ifdef CONFIG_CHIP_STM32L4_RCC_MSI_ENABLE
	enableMsi(CONFIG_CHIP_STM32L4_RCC_MSI_RANGE);
#endif	// def CONFIG_CHIP_STM32L4_RCC_MSI_ENABLE

#ifdef CONFIG_CHIP_STM32L4_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32L4_RCC_PLLSRC_HSE)
	configurePllClockSource(PllClockSource::hse);
#elif defined(CONFIG_CHIP_STM32L4_RCC_PLLSRC_HSI16)
	configurePllClockSource(PllClockSource::hsi16);
#elif defined(CONFIG_CHIP_STM32L4_RCC_PLLSRC_MSI)
	configurePllClockSource(PllClockSource::msi);
#endif	// defined(CONFIG_CHIP_STM32L4_RCC_PLLSRC_MSI)

	configurePllInputClockDivider(CONFIG_CHIP_STM32L4_RCC_PLLM);
	enablePll(CONFIG_CHIP_STM32L4_RCC_PLLN, CONFIG_CHIP_STM32L4_RCC_PLLP, CONFIG_CHIP_STM32L4_RCC_PLLQ,
			CONFIG_CHIP_STM32L4_RCC_PLLR);

#ifdef CONFIG_CHIP_STM32L4_RCC_PLLP_ENABLE
	enablePllOutput(PllOutput::p, true);
#endif	// def CONFIG_CHIP_STM32L4_RCC_PLLP_ENABLE
#ifdef CONFIG_CHIP_STM32L4_RCC_PLLQ_ENABLE
	enablePllOutput(PllOutput::q, true);
#endif	// def CONFIG_CHIP_STM32L4_RCC_PLLQ_ENABLE
#ifdef CONFIG_CHIP_STM32L4_RCC_PLLR_ENABLE
	enablePllOutput(PllOutput::r, true);
#endif	// def CONFIG_CHIP_STM32L4_RCC_PLLR_ENABLE

#endif	// def CONFIG_CHIP_STM32L4_RCC_PLL_ENABLE

	configureAhbClockDivider(CONFIG_CHIP_STM32L4_RCC_HPRE);
	configureApbClockDivider(false, CONFIG_CHIP_STM32L4_RCC_PPRE1);
	configureApbClockDivider(true, CONFIG_CHIP_STM32L4_RCC_PPRE2);

#if CONFIG_CHIP_STM32L4_PWR_VOLTAGE_SCALING_RANGE == 1
	constexpr uint32_t frequencyThreshold {16000000};
	constexpr uint8_t flashLatencyUpperBound {4};
#elif CONFIG_CHIP_STM32L4_PWR_VOLTAGE_SCALING_RANGE == 2
	constexpr uint32_t frequencyThreshold {6000000};
	constexpr uint8_t flashLatencyUpperBound {3};
#endif

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(std::min(flashLatency, flashLatencyUpperBound));

#if defined(CONFIG_CHIP_STM32L4_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(CONFIG_CHIP_STM32L4_RCC_SYSCLK_HSI16)
	switchSystemClock(SystemClockSource::hsi16);
#elif defined(CONFIG_CHIP_STM32L4_RCC_SYSCLK_MSI)
	switchSystemClock(SystemClockSource::msi);
#elif defined(CONFIG_CHIP_STM32L4_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#endif	// defined(CONFIG_CHIP_STM32L4_RCC_SYSCLK_PLL)

#endif	// def CONFIG_CHIP_STM32L4_STANDARD_CLOCK_CONFIGURATION_ENABLE
}

}	// namespace chip

}	// namespace distortos
