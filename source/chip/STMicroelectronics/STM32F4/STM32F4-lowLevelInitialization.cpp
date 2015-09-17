/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32F4
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-17
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/STM32F4-FLASH.hpp"
#include "distortos/chip/STM32F4-RCC.hpp"

#include "distortos/architecture/configureSysTick.hpp"

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
	configureInstructionPrefetch(true);
#else	// !def CONFIG_CHIP_STM32F4_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(false);
#endif	// !def CONFIG_CHIP_STM32F4_FLASH_PREFETCH_ENABLE
	enableInstructionCache();
	enableDataCache();

#ifdef CONFIG_CHIP_STM32F4_RCC_STANDARD_CLOCK_CONFIGURATION_ENABLE

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

	constexpr uint32_t pllInHz {pllClockSourceHse == false ? hsiHz : CONFIG_CHIP_STM32F4_RCC_HSE_HZ};
	constexpr uint32_t vcoInHz {pllInHz / CONFIG_CHIP_STM32F4_RCC_PLLM};
	static_assert(minVcoInHz <= vcoInHz && vcoInHz <= maxVcoInHz, "Invalid VCO input frequency!");
	configurePllInputClockDivider(CONFIG_CHIP_STM32F4_RCC_PLLM);

	constexpr uint32_t vcoOutHz {vcoInHz * CONFIG_CHIP_STM32F4_RCC_PLLN};
	static_assert(minVcoOutHz <= vcoOutHz && vcoOutHz <= maxVcoOutHz, "Invalid VCO output frequency!");

#if defined(CONFIG_CHIP_STM32F4_RCC_PLLP_DIV2)
	constexpr uint8_t pllp {pllpDiv2};
#elif defined(CONFIG_CHIP_STM32F4_RCC_PLLP_DIV4)
	constexpr uint8_t pllp {pllpDiv4};
#elif defined(CONFIG_CHIP_STM32F4_RCC_PLLP_DIV6)
	constexpr uint8_t pllp {pllpDiv6};
#elif defined(CONFIG_CHIP_STM32F4_RCC_PLLP_DIV8)
	constexpr uint8_t pllp {pllpDiv8};
#endif

	constexpr uint32_t pllOutHz {vcoOutHz / pllp};
	static_assert(pllOutHz <= maxPllOutHz, "Invalid PLL output frequency!");

	constexpr uint32_t pllqOutHz {vcoOutHz / CONFIG_CHIP_STM32F4_RCC_PLLQ};
	static_assert(pllqOutHz <= maxPllqOutHz, "Invalid PLL \"Q\" output frequency!");

	enablePll(CONFIG_CHIP_STM32F4_RCC_PLLN, pllp, CONFIG_CHIP_STM32F4_RCC_PLLQ);

#endif	// def CONFIG_CHIP_STM32F4_RCC_PLL_ENABLE

#endif	// def CONFIG_CHIP_STM32F4_RCC_STANDARD_CLOCK_CONFIGURATION_ENABLE

	constexpr uint32_t period {hsiHz / CONFIG_TICK_RATE_HZ};
	constexpr uint32_t periodDividedBy8 {period / 8};
	constexpr bool divideBy8 {period > architecture::maxSysTickPeriod};
	// at least one of the periods must be valid
	static_assert(period <= architecture::maxSysTickPeriod || periodDividedBy8 <= architecture::maxSysTickPeriod,
			"Invalid SysTick configuration!");
	architecture::configureSysTick(divideBy8 == false ? period : periodDividedBy8, divideBy8);
}

}	// namespace chip

}	// namespace distortos
