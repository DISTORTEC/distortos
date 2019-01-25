/**
 * \file
 * \brief Low-level chip initializer for STM32F7
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
#ifdef CONFIG_CHIP_STM32F7_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(true);
#else	// !def CONFIG_CHIP_STM32F7_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(false);
#endif	// !def CONFIG_CHIP_STM32F7_FLASH_PREFETCH_ENABLE

#ifdef CONFIG_CHIP_STM32F7_FLASH_ART_ACCELERATOR_ENABLE
	enableArtAccelerator();
#else	// !def CONFIG_CHIP_STM32F7_FLASH_ART_ACCELERATOR_ENABLE
	disableArtAccelerator();
#endif	// !def CONFIG_CHIP_STM32F7_FLASH_ART_ACCELERATOR_ENABLE

#ifdef CONFIG_CHIP_STM32F7_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	configureVoltageScaling(CONFIG_CHIP_STM32F7_PWR_VOLTAGE_SCALE_MODE);

#if defined(CONFIG_CHIP_STM32F7_PWR_OVER_DRIVE_ENABLE)

	enableOverDriveMode();

#endif	// defined(CONFIG_CHIP_STM32F7_PWR_OVER_DRIVE_ENABLE)

#ifdef CONFIG_CHIP_STM32F7_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F7_RCC_HSE_CLOCK_BYPASS
	enableHse(true);
#else	// !def CONFIG_CHIP_STM32F7_RCC_HSE_CLOCK_BYPASS
	enableHse(false);
#endif	// !def CONFIG_CHIP_STM32F7_RCC_HSE_CLOCK_BYPASS

#endif	// def CONFIG_CHIP_STM32F7_RCC_HSE_ENABLE

#ifdef CONFIG_CHIP_STM32F7_RCC_PLLS_ENABLE

#if defined(CONFIG_CHIP_STM32F7_RCC_PLLSRC_HSI)
	configurePllClockSource(false);
#elif defined(CONFIG_CHIP_STM32F7_RCC_PLLSRC_HSE)
	configurePllClockSource(true);
#endif

	configurePllInputClockDivider(CONFIG_CHIP_STM32F7_RCC_PLLM);

#ifdef CONFIG_CHIP_STM32F7_RCC_PLL_ENABLE

#if defined(CONFIG_CHIP_STM32F76) || defined(CONFIG_CHIP_STM32F77)

	enablePll(CONFIG_CHIP_STM32F7_RCC_PLLN, CONFIG_CHIP_STM32F7_RCC_PLLP, CONFIG_CHIP_STM32F7_RCC_PLLQ,
			CONFIG_CHIP_STM32F7_RCC_PLLR);

#else	// !defined(CONFIG_CHIP_STM32F76) && !defined(CONFIG_CHIP_STM32F77)

	enablePll(CONFIG_CHIP_STM32F7_RCC_PLLN, CONFIG_CHIP_STM32F7_RCC_PLLP, CONFIG_CHIP_STM32F7_RCC_PLLQ);

#endif	// !defined(CONFIG_CHIP_STM32F76) && !defined(CONFIG_CHIP_STM32F77)

#endif	// def CONFIG_CHIP_STM32F7_RCC_PLL_ENABLE

#ifdef CONFIG_CHIP_STM32F7_RCC_PLLI2S_ENABLE

#ifdef CONFIG_CHIP_STM32F7_RCC_PLLI2SP

	enablePlli2s(CONFIG_CHIP_STM32F7_RCC_PLLI2SN, CONFIG_CHIP_STM32F7_RCC_PLLI2SP, CONFIG_CHIP_STM32F7_RCC_PLLI2SQ,
			CONFIG_CHIP_STM32F7_RCC_PLLI2SR);

#else	// !def CONFIG_CHIP_STM32F7_RCC_PLLI2SP

	enablePlli2s(CONFIG_CHIP_STM32F7_RCC_PLLI2SN, CONFIG_CHIP_STM32F7_RCC_PLLI2SQ, CONFIG_CHIP_STM32F7_RCC_PLLI2SR);

#endif	// !def CONFIG_CHIP_STM32F7_RCC_PLLI2SP

#endif	// def CONFIG_CHIP_STM32F7_RCC_PLLI2S_ENABLE

#ifdef CONFIG_CHIP_STM32F7_RCC_PLLSAI_ENABLE

#ifdef CONFIG_CHIP_STM32F7_RCC_PLLSAIR

	enablePllsai(CONFIG_CHIP_STM32F7_RCC_PLLSAIN, CONFIG_CHIP_STM32F7_RCC_PLLSAIP, CONFIG_CHIP_STM32F7_RCC_PLLSAIQ,
			CONFIG_CHIP_STM32F7_RCC_PLLSAIR);

#else	// !def CONFIG_CHIP_STM32F7_RCC_PLLSAIR

	enablePllsai(CONFIG_CHIP_STM32F7_RCC_PLLSAIN, CONFIG_CHIP_STM32F7_RCC_PLLSAIP, CONFIG_CHIP_STM32F7_RCC_PLLSAIQ);

#endif	// !def CONFIG_CHIP_STM32F7_RCC_PLLSAIR

#endif	// def CONFIG_CHIP_STM32F7_RCC_PLLSAI_ENABLE

#if defined(CONFIG_CHIP_STM32F7_RCC_PLL48CLK_PLLQ)
	configurePll48ClockSource(false);
#elif defined(CONFIG_CHIP_STM32F7_RCC_PLL48CLK_PLLSAIP)
	configurePll48ClockSource(true);
#endif	// defined(CONFIG_CHIP_STM32F7_RCC_PLL48CLK_PLLSAIP)

#endif	// def CONFIG_CHIP_STM32F7_RCC_PLLS_ENABLE

	configureAhbClockDivider(CONFIG_CHIP_STM32F7_RCC_HPRE);
	configureApbClockDivider(false, CONFIG_CHIP_STM32F7_RCC_PPRE1);
	configureApbClockDivider(true, CONFIG_CHIP_STM32F7_RCC_PPRE2);

#if CONFIG_CHIP_STM32F7_VDD_MV < 2100
	constexpr uint32_t frequencyThreshold {20000000};
#elif CONFIG_CHIP_STM32F7_VDD_MV < 2400
	constexpr uint32_t frequencyThreshold {22000000};
#elif CONFIG_CHIP_STM32F7_VDD_MV < 2700
	constexpr uint32_t frequencyThreshold {24000000};
#else
	constexpr uint32_t frequencyThreshold {30000000};
#endif

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(flashLatency);

#if defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_HSI)
	switchSystemClock(SystemClockSource::hsi);
#elif defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_PLL)
	switchSystemClock(SystemClockSource::pll);
#endif	// defined(CONFIG_CHIP_STM32F7_RCC_SYSCLK_PLL)

#endif	// def CONFIG_CHIP_STM32F7_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->AHB1ENR |=
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOA_ENABLE
			RCC_AHB1ENR_GPIOAEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOA_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOB_ENABLE
			RCC_AHB1ENR_GPIOBEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOB_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOC_ENABLE
			RCC_AHB1ENR_GPIOCEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOC_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOD_ENABLE
			RCC_AHB1ENR_GPIODEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOD_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOE_ENABLE
			RCC_AHB1ENR_GPIOEEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOE_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOF_ENABLE
			RCC_AHB1ENR_GPIOFEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOF_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOG_ENABLE
			RCC_AHB1ENR_GPIOGEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOG_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOH_ENABLE
			RCC_AHB1ENR_GPIOHEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOH_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOI_ENABLE
			RCC_AHB1ENR_GPIOIEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOI_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOJ_ENABLE
			RCC_AHB1ENR_GPIOJEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOJ_ENABLE
#ifdef CONFIG_CHIP_STM32_GPIO_V2_GPIOK_ENABLE
			RCC_AHB1ENR_GPIOKEN |
#endif	// def CONFIG_CHIP_STM32_GPIO_V2_GPIOK_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
