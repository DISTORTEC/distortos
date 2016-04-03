/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F4
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/STM32F4-FLASH.hpp"
#include "distortos/chip/STM32F4-PWR.hpp"
#include "distortos/chip/STM32F4-RCC.hpp"
#include "distortos/chip/STM32F4-RCC-bits.h"

#include "distortos/architecture/ARMv6-M-ARMv7-M-configureSysTick.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32F4_FLASH_PREFETCH_ENABLE
	static_assert(CONFIG_CHIP_STM32F4_VDD_MV >= 2100,
			"Instruction prefetch must not be enabled when supply voltage is below 2.1V!");
	configureInstructionPrefetch(true);
#else	// !def CONFIG_CHIP_STM32F4_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(false);
#endif	// !def CONFIG_CHIP_STM32F4_FLASH_PREFETCH_ENABLE

#ifdef CONFIG_CHIP_STM32F4_FLASH_DATA_CACHE_ENABLE
	enableDataCache();
#else	// !def CONFIG_CHIP_STM32F4_FLASH_DATA_CACHE_ENABLE
	disableDataCache();
#endif	// !def CONFIG_CHIP_STM32F4_FLASH_DATA_CACHE_ENABLE

#ifdef CONFIG_CHIP_STM32F4_FLASH_INSTRUCTION_CACHE_ENABLE
	enableInstructionCache();
#else	// !def CONFIG_CHIP_STM32F4_FLASH_INSTRUCTION_CACHE_ENABLE
	disableInstructionCache();
#endif	// !def CONFIG_CHIP_STM32F4_FLASH_INSTRUCTION_CACHE_ENABLE

#ifdef CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC_APB1ENR_PWREN_bb = 1;

	configureVoltageScaling(CONFIG_CHIP_STM32F4_PWR_VOLTAGE_SCALE_MODE);

#if (defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) || \
		defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) && \
		defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

	enableOverDriveMode();

#endif	// (defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) ||
		// defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) &&
		// defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

#ifdef CONFIG_CHIP_STM32F4_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F4_RCC_HSE_CLOCK_BYPASS
	constexpr bool hseClockBypass {true};
#else	// !def CONFIG_CHIP_STM32F4_RCC_HSE_CLOCK_BYPASS
	constexpr bool hseClockBypass {};
#endif	// !def CONFIG_CHIP_STM32F4_RCC_HSE_CLOCK_BYPASS
	enableHse(hseClockBypass);

#endif	// def CONFIG_CHIP_STM32F4_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F4_RCC_PLLSRC_HSI)
	constexpr bool pllClockSourceHse {};
#elif defined(CONFIG_CHIP_STM32F4_RCC_PLLSRC_HSE)
	constexpr bool pllClockSourceHse {true};
#endif
	configurePllClockSource(pllClockSourceHse);
	configurePllInputClockDivider(CONFIG_CHIP_STM32F4_RCC_PLLM);

#if defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

	enablePll(CONFIG_CHIP_STM32F4_RCC_PLLN, CONFIG_CHIP_STM32F4_RCC_PLLP, CONFIG_CHIP_STM32F4_RCC_PLLQ,
			CONFIG_CHIP_STM32F4_RCC_PLLR);

#else // !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

	enablePll(CONFIG_CHIP_STM32F4_RCC_PLLN, CONFIG_CHIP_STM32F4_RCC_PLLP, CONFIG_CHIP_STM32F4_RCC_PLLQ);

#endif // !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

#endif	// def CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

	configureAhbClockDivider(CONFIG_CHIP_STM32F4_RCC_HPRE);
	configureApbClockDivider(false, CONFIG_CHIP_STM32F4_RCC_PPRE1);
	configureApbClockDivider(true, CONFIG_CHIP_STM32F4_RCC_PPRE2);

#if CONFIG_CHIP_STM32F4_VDD_MV < 2100
#	if defined(CONFIG_CHIP_STM32F401) || defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411)
	constexpr uint32_t frequencyThreshold {16000000};
#	else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411)
	constexpr uint32_t frequencyThreshold {20000000};
#	endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411)
#elif CONFIG_CHIP_STM32F4_VDD_MV < 2400
#	if defined(CONFIG_CHIP_STM32F401) || defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411)
	constexpr uint32_t frequencyThreshold {18000000};
#	else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411)
	constexpr uint32_t frequencyThreshold {22000000};
#	endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411)
#elif CONFIG_CHIP_STM32F4_VDD_MV < 2700
	constexpr uint32_t frequencyThreshold {24000000};
#else
	constexpr uint32_t frequencyThreshold {30000000};
#endif

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#if defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSI)
	constexpr SystemClockSource systemClockSource {SystemClockSource::hsi};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSE)
	constexpr SystemClockSource systemClockSource {SystemClockSource::hse};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLL)
	constexpr SystemClockSource systemClockSource {SystemClockSource::pll};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)
	constexpr SystemClockSource systemClockSource {SystemClockSource::pllr};
#endif	// defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)

	switchSystemClock(systemClockSource);

#endif	// def CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

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
