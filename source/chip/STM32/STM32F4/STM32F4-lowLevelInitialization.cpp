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

	static_assert(CONFIG_CHIP_STM32F4_PWR_VOLTAGE_SCALE_MODE == 1, "Over-drive mode requires voltage scale 1 mode!");
	static_assert(CONFIG_CHIP_STM32F4_VDD_MV >= 2100,
			"Over-drive mode must not be enabled when supply voltage is below 2.1V!");
	enableOverDriveMode();
	constexpr uint8_t voltageScaleIndex {0};

#else	// !(defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) ||
		// defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) ||
		// !defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

	constexpr uint8_t voltageScaleIndex {CONFIG_CHIP_STM32F4_PWR_VOLTAGE_SCALE_MODE};

#endif	// !(defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) ||
		// defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)) ||
		// !defined(CONFIG_CHIP_STM32F4_PWR_OVER_DRIVE_ENABLE)

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
	constexpr uint32_t pllInFrequency {hsiFrequency};
#elif defined(CONFIG_CHIP_STM32F4_RCC_PLLSRC_HSE)
	constexpr bool pllClockSourceHse {true};
	constexpr uint32_t pllInFrequency {CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY};
#endif
	configurePllClockSource(pllClockSourceHse);

	constexpr uint32_t vcoInFrequency {pllInFrequency / CONFIG_CHIP_STM32F4_RCC_PLLM};
	static_assert(minVcoInFrequency <= vcoInFrequency && vcoInFrequency <= maxVcoInFrequency,
			"Invalid VCO input frequency!");
	configurePllInputClockDivider(CONFIG_CHIP_STM32F4_RCC_PLLM);

	constexpr uint32_t vcoOutFrequency {vcoInFrequency * CONFIG_CHIP_STM32F4_RCC_PLLN};
	static_assert(minVcoOutFrequency <= vcoOutFrequency && vcoOutFrequency <= maxVcoOutFrequency,
			"Invalid VCO output frequency!");

	constexpr uint32_t pllOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F4_RCC_PLLP};
	static_assert(pllOutFrequency <= maxPllOutFrequency[voltageScaleIndex], "Invalid PLL output frequency!");

	constexpr uint32_t pllqOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F4_RCC_PLLQ};
	static_assert(pllqOutFrequency <= maxPllqOutFrequency, "Invalid PLL \"/Q\" output frequency!");

#if defined(CONFIG_CHIP_STM32F446) || defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

	constexpr uint32_t pllrOutFrequency {vcoOutFrequency / CONFIG_CHIP_STM32F4_RCC_PLLR};

	enablePll(CONFIG_CHIP_STM32F4_RCC_PLLN, CONFIG_CHIP_STM32F4_RCC_PLLP, CONFIG_CHIP_STM32F4_RCC_PLLQ,
			CONFIG_CHIP_STM32F4_RCC_PLLR);

#else // !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

	enablePll(CONFIG_CHIP_STM32F4_RCC_PLLN, CONFIG_CHIP_STM32F4_RCC_PLLP, CONFIG_CHIP_STM32F4_RCC_PLLQ);

#endif // !defined(CONFIG_CHIP_STM32F446) && !defined(CONFIG_CHIP_STM32F469) && !defined(CONFIG_CHIP_STM32F479)

#endif	// def CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSI)
	constexpr uint32_t sysclkFrequency {hsiFrequency};
	constexpr SystemClockSource systemClockSource {SystemClockSource::hsi};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_HSE)
	constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F4_RCC_HSE_FREQUENCY};
	constexpr SystemClockSource systemClockSource {SystemClockSource::hse};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLL)
	constexpr uint32_t sysclkFrequency {pllOutFrequency};
	constexpr SystemClockSource systemClockSource {SystemClockSource::pll};
#elif defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)
	constexpr uint32_t sysclkFrequency {pllrOutFrequency};
	constexpr SystemClockSource systemClockSource {SystemClockSource::pllr};
#endif	// defined(CONFIG_CHIP_STM32F4_RCC_SYSCLK_PLLR)

#else	// !def CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

	constexpr uint32_t sysclkFrequency {CONFIG_CHIP_STM32F4_RCC_SYSCLK_FREQUENCY};

#endif	// !def CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

	constexpr uint32_t ahbFrequency {sysclkFrequency / CONFIG_CHIP_STM32F4_RCC_HPRE};
	constexpr uint32_t apb1Frequency {ahbFrequency / CONFIG_CHIP_STM32F4_RCC_PPRE1};
	static_assert(apb1Frequency <= maxApb1Frequency, "Invalid APB1 (low speed) frequency!");
	constexpr uint32_t apb2Frequency {ahbFrequency / CONFIG_CHIP_STM32F4_RCC_PPRE2};
	static_assert(apb2Frequency <= maxApb2Frequency, "Invalid APB2 (high speed) frequency!");

#ifdef CONFIG_CHIP_STM32F4_STANDARD_CLOCK_CONFIGURATION_ENABLE

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
