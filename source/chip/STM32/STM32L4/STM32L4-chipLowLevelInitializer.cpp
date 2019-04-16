/**
 * \file
 * \brief Low-level chip initializer for STM32L4
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32-bit-banding.h"
#include "distortos/chip/STM32L4-FLASH.hpp"
#include "distortos/chip/STM32L4-PWR.hpp"
#include "distortos/chip/STM32L4-RCC.hpp"

#include "distortos/BIND_LOW_LEVEL_INITIALIZER.h"

#include <algorithm>

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
 * \brief Low-level chip initializer for STM32L4
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void chipLowLevelInitializer()
{
#ifdef DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(true);
#else	// !def DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(false);
#endif	// !def DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE

#ifdef DISTORTOS_CHIP_FLASH_DATA_CACHE_ENABLE
	enableDataCache();
#else	// !def DISTORTOS_CHIP_FLASH_DATA_CACHE_ENABLE
	disableDataCache();
#endif	// !def DISTORTOS_CHIP_FLASH_DATA_CACHE_ENABLE

#ifdef DISTORTOS_CHIP_FLASH_INSTRUCTION_CACHE_ENABLE
	enableInstructionCache();
#else	// !def DISTORTOS_CHIP_FLASH_INSTRUCTION_CACHE_ENABLE
	disableInstructionCache();
#endif	// !def DISTORTOS_CHIP_FLASH_INSTRUCTION_CACHE_ENABLE

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	STM32_BITBAND(RCC, APB1ENR1, PWREN) = 1;

	configureVoltageScaling(DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE);

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE
	enableHse(hseClockBypass);
#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSI16_ENABLE
	enableHsi16();
#endif	// def DISTORTOS_CHIP_RCC_HSI16_ENABLE

#ifdef DISTORTOS_CHIP_RCC_MSI_ENABLE
	enableMsi(DISTORTOS_CHIP_RCC_MSI_RANGE);
#endif	// def DISTORTOS_CHIP_RCC_MSI_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
	configurePllClockSource(PllClockSource::hse);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI16)
	configurePllClockSource(PllClockSource::hsi16);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_MSI)
	configurePllClockSource(PllClockSource::msi);
#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_MSI)

	configurePllInputClockDivider(DISTORTOS_CHIP_RCC_PLLM);
	enablePll(DISTORTOS_CHIP_RCC_PLLN, DISTORTOS_CHIP_RCC_PLLP, DISTORTOS_CHIP_RCC_PLLQ, DISTORTOS_CHIP_RCC_PLLR);

#ifdef DISTORTOS_CHIP_RCC_PLLP_ENABLE
	enablePllOutput(PllOutput::p, true);
#endif	// def DISTORTOS_CHIP_RCC_PLLP_ENABLE
#ifdef DISTORTOS_CHIP_RCC_PLLQ_ENABLE
	enablePllOutput(PllOutput::q, true);
#endif	// def DISTORTOS_CHIP_RCC_PLLQ_ENABLE
#ifdef DISTORTOS_CHIP_RCC_PLLR_ENABLE
	enablePllOutput(PllOutput::r, true);
#endif	// def DISTORTOS_CHIP_RCC_PLLR_ENABLE

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

	configureAhbClockDivider(DISTORTOS_CHIP_RCC_HPRE);
	configureApbClockDivider(false, DISTORTOS_CHIP_RCC_PPRE1);
	configureApbClockDivider(true, DISTORTOS_CHIP_RCC_PPRE2);

#if DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE == 1
	constexpr uint32_t frequencyThreshold {16000000};
	constexpr uint8_t flashLatencyUpperBound {4};
#elif DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE == 2
	constexpr uint32_t frequencyThreshold {6000000};
	constexpr uint8_t flashLatencyUpperBound {3};
#endif

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(std::min(flashLatency, flashLatencyUpperBound));

#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI16)
	switchSystemClock(SystemClockSource::hsi16);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_MSI)
	switchSystemClock(SystemClockSource::msi);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)

#endif	// def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->AHB2ENR |=
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOA_ENABLE
			RCC_AHB2ENR_GPIOAEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOA_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOB_ENABLE
			RCC_AHB2ENR_GPIOBEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOB_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOC_ENABLE
			RCC_AHB2ENR_GPIOCEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOC_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOD_ENABLE
			RCC_AHB2ENR_GPIODEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOD_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOE_ENABLE
			RCC_AHB2ENR_GPIOEEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOE_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOF_ENABLE
			RCC_AHB2ENR_GPIOFEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOF_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOG_ENABLE
			RCC_AHB2ENR_GPIOGEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOG_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOH_ENABLE
			RCC_AHB2ENR_GPIOHEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOH_ENABLE
#ifdef DISTORTOS_CHIP_STM32_GPIO_V2_GPIOI_ENABLE
			RCC_AHB2ENR_GPIOIEN |
#endif	// def DISTORTOS_CHIP_STM32_GPIO_V2_GPIOI_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
