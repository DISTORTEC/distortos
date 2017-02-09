/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F4
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32F4-FLASH.hpp"
#include "distortos/chip/STM32F4-PWR.hpp"
#include "distortos/chip/STM32F4-RCC.hpp"
#include "distortos/chip/STM32F4-RCC-bits.h"

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

#if (defined(CONFIG_CHIP_STM32F427) || defined(CONFIG_CHIP_STM32F429) || defined(CONFIG_CHIP_STM32F43) || \
		defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) && \
		defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

	enableOverDriveMode();

#endif	// (defined(CONFIG_CHIP_STM32F427) || defined(CONFIG_CHIP_STM32F429) || defined(CONFIG_CHIP_STM32F43) ||
		// defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) &&
		// defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

#ifdef CONFIG_CHIP_STM32F4_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F4_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def CONFIG_CHIP_STM32F4_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def CONFIG_CHIP_STM32F4_RCC_HSE_CLOCK_BYPASS

#endif	// def CONFIG_CHIP_STM32F4_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F4_RCC_PLLSRC_HSI)
	configurePllClockSource(false);
#elif defined(CONFIG_CHIP_STM32F4_RCC_PLLSRC_HSE)
	configurePllClockSource(true);
#endif

	configurePllInputClockDivider(CONFIG_CHIP_STM32F4_RCC_PLLM);

#if defined(CONFIG_CHIP_STM32F412) || defined(CONFIG_CHIP_STM32F413) || defined(CONFIG_CHIP_STM32F423) || \
		defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

	enablePll(CONFIG_CHIP_STM32F4_RCC_PLLN, CONFIG_CHIP_STM32F4_RCC_PLLP, CONFIG_CHIP_STM32F4_RCC_PLLQ,
			CONFIG_CHIP_STM32F4_RCC_PLLR);

#else	// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423) &&
		// !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

	enablePll(CONFIG_CHIP_STM32F4_RCC_PLLN, CONFIG_CHIP_STM32F4_RCC_PLLP, CONFIG_CHIP_STM32F4_RCC_PLLQ);

#endif	// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423) &&
		// !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

#endif	// def CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

	configureAhbClockDivider(CONFIG_CHIP_STM32F4_RCC_HPRE);
	configureApbClockDivider(false, CONFIG_CHIP_STM32F4_RCC_PPRE1);
	configureApbClockDivider(true, CONFIG_CHIP_STM32F4_RCC_PPRE2);

#if CONFIG_CHIP_STM32F4_VDD_MV < 2100
#	if defined(CONFIG_CHIP_STM32F401) || defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411) || \
		defined(CONFIG_CHIP_STM32F412) || defined(CONFIG_CHIP_STM32F413) || defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {16000000};
#	else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411) &&
		// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {20000000};
#	endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411) &&
		// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
#elif CONFIG_CHIP_STM32F4_VDD_MV < 2400
#	if defined(CONFIG_CHIP_STM32F401) || defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411) || \
		defined(CONFIG_CHIP_STM32F412) || defined(CONFIG_CHIP_STM32F413) || defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {18000000};
#	else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411) &&
		// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {22000000};
#	endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411) &&
		// !defined(CONFIG_CHIP_STM32F412) && !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
#elif CONFIG_CHIP_STM32F4_VDD_MV < 2700
#	if defined(CONFIG_CHIP_STM32F413) || defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {20000000};
#	else	// !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {24000000};
#	endif	// !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
#else
#	if defined(CONFIG_CHIP_STM32F413) || defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {25000000};
#	else	// !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {30000000};
#	endif	// !defined(CONFIG_CHIP_STM32F413) && !defined(CONFIG_CHIP_STM32F423)
#endif

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#if defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSI)
	switchSystemClock(SystemClockSource::hsi);
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)
	switchSystemClock(SystemClockSource::pllr);
#endif	// defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)

#endif	// def CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE
}

}	// namespace chip

}	// namespace distortos
