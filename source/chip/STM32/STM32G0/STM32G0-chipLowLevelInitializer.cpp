/**
 * \file
 * \brief Low-level chip initializer for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/clocks.hpp"
#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32G0-FLASH.hpp"
#include "distortos/chip/STM32G0-PWR.hpp"
#include "distortos/chip/STM32G0-RCC.hpp"

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
 * \brief Low-level chip initializer for STM32G0
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

#ifdef DISTORTOS_CHIP_FLASH_INSTRUCTION_CACHE_ENABLE
	enableInstructionCache();
#else	// !def DISTORTOS_CHIP_FLASH_INSTRUCTION_CACHE_ENABLE
	disableInstructionCache();
#endif	// !def DISTORTOS_CHIP_FLASH_INSTRUCTION_CACHE_ENABLE

#ifdef DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->APBENR1 |= RCC_APBENR1_PWREN;

	configureVoltageScaling(DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE);

#ifdef DISTORTOS_CHIP_RCC_HSE_ENABLE
	enableHse(hseClockBypass);
#endif	// def DISTORTOS_CHIP_RCC_HSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_HSI16_ENABLE
	enableHsi16();
#endif	// def DISTORTOS_CHIP_RCC_HSI16_ENABLE

#ifdef DISTORTOS_CHIP_RCC_LSE_ENABLE

#if defined(DISTORTOS_CHIP_RCC_LSE_DRIVE_CAPABILITY_LOW) || defined(DISTORTOS_CHIP_RCC_LSE_CLOCK_BYPASS)
	constexpr LseDriveCapability lseDriveCapability {LseDriveCapability::low};
#elif defined(DISTORTOS_CHIP_RCC_LSE_DRIVE_CAPABILITY_MEDIUMLOW)
	constexpr LseDriveCapability lseDriveCapability {LseDriveCapability::mediumLow};
#elif defined(DISTORTOS_CHIP_RCC_LSE_DRIVE_CAPABILITY_MEDIUMHIGH)
	constexpr LseDriveCapability lseDriveCapability {LseDriveCapability::mediumHigh};
#elif defined(DISTORTOS_CHIP_RCC_LSE_DRIVE_CAPABILITY_HIGH)
	constexpr LseDriveCapability lseDriveCapability {LseDriveCapability::high};
#else
	#error "LSE drive capability not configured!"
#endif

	disableRtcDomainWriteProtection();
	enableLse(lseClockBypass, lseDriveCapability);
	enableRtcDomainWriteProtection();

#endif	// def DISTORTOS_CHIP_RCC_LSE_ENABLE

#ifdef DISTORTOS_CHIP_RCC_PLL_ENABLE

#if defined(DISTORTOS_CHIP_RCC_PLLSRC_HSE)
	constexpr PllClockSource pllClockSource {PllClockSource::hse};
#elif defined(DISTORTOS_CHIP_RCC_PLLSRC_HSI16)
	constexpr PllClockSource pllClockSource {PllClockSource::hsi16};
#endif	// defined(DISTORTOS_CHIP_RCC_PLLSRC_MSI)

#ifdef STM32G0_RCC_HAS_PLLQ

	enablePll(pllClockSource, DISTORTOS_CHIP_RCC_PLLM, DISTORTOS_CHIP_RCC_PLLN, DISTORTOS_CHIP_RCC_PLLP,
			DISTORTOS_CHIP_RCC_PLLQ, DISTORTOS_CHIP_RCC_PLLR);

#else	// !def STM32G0_RCC_HAS_PLLQ

	enablePll(pllClockSource, DISTORTOS_CHIP_RCC_PLLM, DISTORTOS_CHIP_RCC_PLLN, DISTORTOS_CHIP_RCC_PLLP,
			DISTORTOS_CHIP_RCC_PLLR);

#endif	// !def STM32G0_RCC_HAS_PLLQ

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
	configureApbClockDivider(DISTORTOS_CHIP_RCC_PPRE);

#if DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE == 1
	constexpr uint32_t frequencyThreshold {24000000};
	constexpr uint8_t flashLatencyUpperBound {2};
#elif DISTORTOS_CHIP_PWR_VOLTAGE_SCALING_RANGE == 2
	constexpr uint32_t frequencyThreshold {8000000};
	constexpr uint8_t flashLatencyUpperBound {1};
#endif

	constexpr uint8_t flashLatency {(ahbFrequency - 1) / frequencyThreshold};
	static_assert(flashLatency <= maxFlashLatency, "Invalid flash latency!");
	configureFlashLatency(std::min(flashLatency, flashLatencyUpperBound));

#if defined(DISTORTOS_CHIP_RCC_SYSCLK_HSE)
	switchSystemClock(SystemClockSource::hse);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_HSISYS)
	switchSystemClock(SystemClockSource::hsisys);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_LSE)
	switchSystemClock(SystemClockSource::lse);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_LSI)
	switchSystemClock(SystemClockSource::lsi);
#elif defined(DISTORTOS_CHIP_RCC_SYSCLK_PLLR)
	switchSystemClock(SystemClockSource::pllr);
#endif	// defined(DISTORTOS_CHIP_RCC_SYSCLK_PLLR)

#endif	// def DISTORTOS_CHIP_STANDARD_CLOCK_CONFIGURATION_ENABLE

	RCC->IOPENR |=
#ifdef DISTORTOS_CHIP_GPIOA_ENABLE
			RCC_IOPENR_GPIOAEN |
#endif	// def DISTORTOS_CHIP_GPIOA_ENABLE
#ifdef DISTORTOS_CHIP_GPIOB_ENABLE
			RCC_IOPENR_GPIOBEN |
#endif	// def DISTORTOS_CHIP_GPIOB_ENABLE
#ifdef DISTORTOS_CHIP_GPIOC_ENABLE
			RCC_IOPENR_GPIOCEN |
#endif	// def DISTORTOS_CHIP_GPIOC_ENABLE
#ifdef DISTORTOS_CHIP_GPIOD_ENABLE
			RCC_IOPENR_GPIODEN |
#endif	// def DISTORTOS_CHIP_GPIOD_ENABLE
#ifdef DISTORTOS_CHIP_GPIOE_ENABLE
			RCC_IOPENR_GPIOEEN |
#endif	// def DISTORTOS_CHIP_GPIOE_ENABLE
#ifdef DISTORTOS_CHIP_GPIOF_ENABLE
			RCC_IOPENR_GPIOFEN |
#endif	// def DISTORTOS_CHIP_GPIOF_ENABLE
			0;
}

BIND_LOW_LEVEL_INITIALIZER(40, chipLowLevelInitializer);

}	// namespace

}	// namespace chip

}	// namespace distortos
