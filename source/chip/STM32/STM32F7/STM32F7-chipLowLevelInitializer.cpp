/**
 * \file
 * \brief Low-level chip initializer for STM32F7
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32F7-FLASH.hpp"
#include "distortos/chip/STM32F7-PWR.hpp"
#include "distortos/chip/STM32F7-RCC.hpp"

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
 * \brief Low-level chip initializer for STM32F7
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

#ifdef DISTORTOS_CHIP_FLASH_ART_ACCELERATOR_ENABLE
	enableArtAccelerator();
#else	// !def DISTORTOS_CHIP_FLASH_ART_ACCELERATOR_ENABLE
	disableArtAccelerator();
#endif	// !def DISTORTOS_CHIP_FLASH_ART_ACCELERATOR_ENABLE

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	configureVoltageScaling(DISTORTOS_CHIP_PWR_VOLTAGE_SCALE_MODE);

#if defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

	enableOverDriveMode();

#endif	// defined(DISTORTOS_CHIP_PWR_OVER_DRIVE_ENABLE)

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def DISTORTOS_CHIP_RCC_HSE_CLOCK_BYPASS

#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLLS_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI)
	configurePllClockSource(false);
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
	configurePllClockSource(true);
#endif

	configurePllInputClockDivider(DISTORTOS_CHIP_RCC_PLLM);

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

#if defined(DISTORTOS_CHIP_STM32F76) || defined(DISTORTOS_CHIP_STM32F77)

	enablePll(DISTORTOS_CHIP_RCC_PLLN, DISTORTOS_CHIP_RCC_PLLP, DISTORTOS_CHIP_RCC_PLLQ, DISTORTOS_CHIP_RCC_PLLR);

#else	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)

	enablePll(DISTORTOS_CHIP_RCC_PLLN, DISTORTOS_CHIP_RCC_PLLP, DISTORTOS_CHIP_RCC_PLLQ);

#endif	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)

#endif	// def DISTORTOS_CHIP_RCC_PLL_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLLI2S_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLLI2SP

	enablePlli2s(DISTORTOS_CHIP_RCC_PLLI2SN, DISTORTOS_CHIP_RCC_PLLI2SP, DISTORTOS_CHIP_RCC_PLLI2SQ,
			DISTORTOS_CHIP_RCC_PLLI2SR);

#else	// !def DISTORTOS_CHIP_RCC_PLLI2SP

	enablePlli2s(DISTORTOS_CHIP_RCC_PLLI2SN, DISTORTOS_CHIP_RCC_PLLI2SQ, DISTORTOS_CHIP_RCC_PLLI2SR);

#endif	// !def DISTORTOS_CHIP_RCC_PLLI2SP

#endif	// def DISTORTOS_CHIP_RCC_PLLI2S_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLLSAI_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLLSAIR

	enablePllsai(DISTORTOS_CHIP_RCC_PLLSAIN, DISTORTOS_CHIP_RCC_PLLSAIP, DISTORTOS_CHIP_RCC_PLLSAIQ,
			DISTORTOS_CHIP_RCC_PLLSAIR);

#else	// !def DISTORTOS_CHIP_RCC_PLLSAIR

	enablePllsai(DISTORTOS_CHIP_RCC_PLLSAIN, DISTORTOS_CHIP_RCC_PLLSAIP, DISTORTOS_CHIP_RCC_PLLSAIQ);

#endif	// !def DISTORTOS_CHIP_RCC_PLLSAIR

#endif	// def DISTORTOS_CHIP_RCC_PLLSAI_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLL48CLK_PLLQ)
	configurePll48ClockSource(false);
#elif defined(DISTORTOS_CHIP_RCC_PLL48CLK_PLLSAIP)
	configurePll48ClockSource(true);
#endif	// defined(DISTORTOS_CHIP_RCC_PLL48CLK_PLLSAIP)

#endif	// def DISTORTOS_CHIP_RCC_PLLS_ENABLE

	configureAhbClockDivider(DISTORTOS_CHIP_RCC_HPRE);
	configureApbClockDivider(false, DISTORTOS_CHIP_RCC_PPRE1);
	configureApbClockDivider(true, DISTORTOS_CHIP_RCC_PPRE2);

#if DISTORTOS_CHIP_VDD_MV < 2100
	constexpr uint32_t frequencyThreshold {20000000};
#elif DISTORTOS_CHIP_VDD_MV < 2400
	constexpr uint32_t frequencyThreshold {22000000};
#elif DISTORTOS_CHIP_VDD_MV < 2700
	constexpr uint32_t frequencyThreshold {24000000};
#else
	constexpr uint32_t frequencyThreshold {30000000};
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
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLL)

	auto rccDckcfgr2 = RCC->DCKCFGR2;

#if defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_PLL48CLK)
	rccDckcfgr2 &= ~RCC_DCKCFGR2_SDMMC1SEL;
#elif defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_SYSCLK)
	rccDckcfgr2 |= RCC_DCKCFGR2_SDMMC1SEL;
#endif	// defined(DISTORTOS_CHIP_RCC_SDMMC1CLK_SYSCLK)

#if defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_PLL48CLK)
	rccDckcfgr2 &= ~RCC_DCKCFGR2_SDMMC2SEL;
#elif defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_SYSCLK)
	rccDckcfgr2 |= RCC_DCKCFGR2_SDMMC2SEL;
#endif	// defined(DISTORTOS_CHIP_RCC_SDMMC2CLK_SYSCLK)

	RCC->DCKCFGR2 = rccDckcfgr2;

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
