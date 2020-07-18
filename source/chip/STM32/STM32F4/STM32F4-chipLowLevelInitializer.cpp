/**
 * \file
 * \brief Low-level chip initializer for STM32F4
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32-bit-banding.h"
#include "distortos/chip/STM32F4-FLASH.hpp"
#include "distortos/chip/STM32F4-PWR.hpp"
#include "distortos/chip/STM32F4-RCC.hpp"

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
 * \brief Low-level chip initializer for STM32F4
 *
 * This function is called before constructors for global and static objects via BIND_LOW_LEVEL_INITIALIZER().
 */

void chipLowLevelInitializer()
{
#ifdef DISTORTOS_CHIP_FLASH_PREFETCH_ENABLE
	static_assert(DISTORTOS_CHIP_VDD_MV >= 2100,
			"Instruction prefetch must not be enabled when supply voltage is below 2.1 V!");
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

	STM32_BITBAND(RCC, APB1ENR, PWREN) = 1;

	configureVoltageScaling(DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE);

#if (defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) || defined(DISTORTOS_CHIP_STM32F43) || \
		defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || \
		defined(DISTORTOS_CHIP_STM32F479)) && defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

	enableOverDriveMode();

#endif	// (defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) ||
		// defined(DISTORTOS_CHIP_STM32F43) || defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) ||
		// defined(DISTORTOS_CHIP_STM32F479)) && defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS

#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI)
	configurePllClockSource(false);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
	configurePllClockSource(true);
#endif

	configurePllInputClockDivider(DISTORTOS_CHIP_RCC_PLLM);

#if defined(DISTORTOS_CHIP_STM32F412) || defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423) || \
		defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || defined(DISTORTOS_CHIP_STM32F479)

	enablePll(DISTORTOS_CHIP_RCC_PLLN, DISTORTOS_CHIP_RCC_PLLP, DISTORTOS_CHIP_RCC_PLLQ, DISTORTOS_CHIP_RCC_PLLR);

#else	// !defined(DISTORTOS_CHIP_STM32F412) && !defined(DISTORTOS_CHIP_STM32F413) &&
		// !defined(DISTORTOS_CHIP_STM32F423) && !defined(DISTORTOS_CHIP_STM32F446) &&
		// !defined(DISTORTOS_CHIP_STM32F469) && !defined(DISTORTOS_CHIP_STM32F479)

	enablePll(DISTORTOS_CHIP_RCC_PLLN, DISTORTOS_CHIP_RCC_PLLP, DISTORTOS_CHIP_RCC_PLLQ);

#endif	// !defined(DISTORTOS_CHIP_STM32F412) && !defined(DISTORTOS_CHIP_STM32F413) &&
		// !defined(DISTORTOS_CHIP_STM32F423) && !defined(DISTORTOS_CHIP_STM32F446) &&
		// !defined(DISTORTOS_CHIP_STM32F469) && !defined(DISTORTOS_CHIP_STM32F479)

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

	configureAhbClockDivider(DISTORTOS_CHIP_RCC_HPRE);
	configureApbClockDivider(false, DISTORTOS_CHIP_RCC_PPRE1);
	configureApbClockDivider(true, DISTORTOS_CHIP_RCC_PPRE2);

#if DISTORTOS_CHIP_VDD_MV < 2100
#	if defined(DISTORTOS_CHIP_STM32F401) || defined(DISTORTOS_CHIP_STM32F410) || defined(DISTORTOS_CHIP_STM32F411) || \
		defined(DISTORTOS_CHIP_STM32F412) || defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {16000000};
#	else	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {20000000};
#	endif	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
#elif DISTORTOS_CHIP_VDD_MV < 2400
#	if defined(DISTORTOS_CHIP_STM32F401) || defined(DISTORTOS_CHIP_STM32F410) || defined(DISTORTOS_CHIP_STM32F411) || \
		defined(DISTORTOS_CHIP_STM32F412) || defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {18000000};
#	else	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {22000000};
#	endif	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
#elif DISTORTOS_CHIP_VDD_MV < 2700
#	if defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {20000000};
#	else	// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {24000000};
#	endif	// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
#else
#	if defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {25000000};
#	else	// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
	constexpr uint32_t frequencyThreshold {30000000};
#	endif	// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
#endif

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSI)
	switchSystemClock(SystemClockSource::hsi);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLLR)
	switchSystemClock(SystemClockSource::pllr);
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLLR)

#endif	// def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->AHB1ENR |=
#ifdef DISTORTOS_CHIP_GPIOA_ENABLE
			RCC_AHB1ENR_GPIOAEN |
#endif	// def DISTORTOS_CHIP_GPIOA_ENABLE
#ifdef DISTORTOS_CHIP_GPIOB_ENABLE
			RCC_AHB1ENR_GPIOBEN |
#endif	// def DISTORTOS_CHIP_GPIOB_ENABLE
#ifdef DISTORTOS_CHIP_GPIOC_ENABLE
			RCC_AHB1ENR_GPIOCEN |
#endif	// def DISTORTOS_CHIP_GPIOC_ENABLE
#ifdef DISTORTOS_CHIP_GPIOD_ENABLE
			RCC_AHB1ENR_GPIODEN |
#endif	// def DISTORTOS_CHIP_GPIOD_ENABLE
#ifdef DISTORTOS_CHIP_GPIOE_ENABLE
			RCC_AHB1ENR_GPIOEEN |
#endif	// def DISTORTOS_CHIP_GPIOE_ENABLE
#ifdef DISTORTOS_CHIP_GPIOF_ENABLE
			RCC_AHB1ENR_GPIOFEN |
#endif	// def DISTORTOS_CHIP_GPIOF_ENABLE
#ifdef DISTORTOS_CHIP_GPIOG_ENABLE
			RCC_AHB1ENR_GPIOGEN |
#endif	// def DISTORTOS_CHIP_GPIOG_ENABLE
#ifdef DISTORTOS_CHIP_GPIOH_ENABLE
			RCC_AHB1ENR_GPIOHEN |
#endif	// def DISTORTOS_CHIP_GPIOH_ENABLE
#ifdef DISTORTOS_CHIP_GPIOI_ENABLE
			RCC_AHB1ENR_GPIOIEN |
#endif	// def DISTORTOS_CHIP_GPIOI_ENABLE
#ifdef DISTORTOS_CHIP_GPIOJ_ENABLE
			RCC_AHB1ENR_GPIOJEN |
#endif	// def DISTORTOS_CHIP_GPIOJ_ENABLE
#ifdef DISTORTOS_CHIP_GPIOK_ENABLE
			RCC_AHB1ENR_GPIOKEN |
#endif	// def DISTORTOS_CHIP_GPIOK_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
