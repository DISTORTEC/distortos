/**
 * \file
 * \brief Header for RCC-related functions for STM32F0
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_RCC_HPP_
#define SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_RCC_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

#if defined(DISTORTOS_CHIP_STM32F04) || defined(DISTORTOS_CHIP_STM32F071) || defined(DISTORTOS_CHIP_STM32F072) || \
	defined(DISTORTOS_CHIP_STM32F078) || defined(DISTORTOS_CHIP_STM32F09)

#define STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

#endif	// defined(DISTORTOS_CHIP_STM32F04) || defined(DISTORTOS_CHIP_STM32F071) || defined(DISTORTOS_CHIP_STM32F072) ||
		// defined(DISTORTOS_CHIP_STM32F078) || defined(DISTORTOS_CHIP_STM32F09)

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// PLL clock source
enum class PllClockSource : uint8_t
{
	/// HSI oscillator divided by 2 selected as PLL clock source
	hsiDiv2 = 0,

#ifdef STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

	/// HSI oscillator divided by PREDIV selected as PLL clock source
	hsiPrediv = 1,

#endif	// def STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

	/// HSE oscillator divided by PREDIV selected as PLL clock source
	hsePrediv = 2,

#ifdef STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

	/// HSI48 oscillator divided by PREDIV selected as PLL clock source
	hsi48Prediv = 3,

#endif	// def STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES
};

/// system clock source
enum class SystemClockSource : uint8_t
{
	/// HSI oscillator selected as system clock
	hsi,
	/// HSE oscillator selected as system clock
	hse,
	/// main PLL selected as system clock
	pll,

#ifdef STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

	/// HSI48 oscillator selected as system clock
	hsi48,

#endif	// def STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES
};

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for PLLMUL
constexpr uint8_t minPllmul {2};

/// maximum allowed value for PLLMUL
constexpr uint8_t maxPllmul {16};

/// minimum allowed value for PREDIV
constexpr uint8_t minPrediv {1};

/// maximum allowed value for PREDIV
constexpr uint8_t maxPrediv {16};

/// first allowed value for AHB divider - 1
constexpr uint16_t hpreDiv1 {1};

/// second allowed value for AHB divider - 2
constexpr uint16_t hpreDiv2 {2};

/// third allowed value for AHB divider - 4
constexpr uint16_t hpreDiv4 {4};

/// fourth allowed value for AHB divider - 8
constexpr uint16_t hpreDiv8 {8};

/// fifth allowed value for AHB divider - 16
constexpr uint16_t hpreDiv16 {16};

/// sixth allowed value for AHB divider - 64
constexpr uint16_t hpreDiv64 {64};

/// seventh allowed value for AHB divider - 128
constexpr uint16_t hpreDiv128 {128};

/// eighth allowed value for AHB divider - 256
constexpr uint16_t hpreDiv256 {256};

/// ninth allowed value for AHB divider - 512
constexpr uint16_t hpreDiv512 {512};

/// first allowed value for APB divider - 1
constexpr uint8_t ppreDiv1 {1};

/// second allowed value for APB divider - 2
constexpr uint8_t ppreDiv2 {2};

/// third allowed value for APB divider - 4
constexpr uint8_t ppreDiv4 {4};

/// fourth allowed value for APB divider - 8
constexpr uint8_t ppreDiv8 {8};

/// fifth allowed value for APB divider - 16
constexpr uint8_t ppreDiv16 {16};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures divider of AHB clock (HPRE value).
 *
 * \param [in] hpre is the HPRE value, {1, 2, 4, 8, 16, 64, 128, 256, 512} or {hpreDiv1, hpreDiv2, hpreDiv4, hpreDiv8,
 * hpreDiv16, hpreDiv64, hpreDiv128, hpreDiv256, hpreDiv512}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a hpre value is invalid;
 */

int configureAhbClockDivider(uint16_t hpre);

/**
 * \brief Configures divider of APB clock (PPRE value).
 *
 * \param [in] ppre is the PPRE value, {1, 2, 4, 8, 16} or {ppreDiv1, ppreDiv2, ppreDiv4, ppreDiv8, ppreDiv16}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a ppre value is invalid;
 */

int configureApbClockDivider(uint8_t ppre);

/**
 * \brief Configures PREDIV division factor.
 *
 * \warning Before changing configuration of PLL make sure that it is not used in any way (as core clock or as source of
 * peripheral clocks) and that it is disabled.
 *
 * \param [in] prediv is the PREDIV division factor, [1; 16] or [minPrediv; maxPrediv]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a prediv value is invalid;
 */

int configurePrediv(uint8_t prediv);

/**
 * \brief Disables HSE clock.
 *
 * \warning Before changing configuration of HSE clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void disableHse();

#ifdef STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

/**
 * \brief Disables HSI48 clock.
 *
 * \warning Before changing configuration of HSI48 clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of USB clock).
 */

void disableHsi48();

#endif	// def STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

/**
 * \brief Disables main PLL.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks).
 */

void disablePll();

/**
 * \brief Enables HSE clock.
 *
 * Enables HSE clock using crystal/ceramic resonator (bypass disabled) or external user clock (bypass enabled). This
 * function waits until the HSE oscillator is stable after enabling the clock.
 *
 * \warning Before changing configuration of HSE clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 *
 * \param [in] bypass selects whether crystal/ceramic resonator (false) or external user clock (true) is used
 */

void enableHse(bool bypass);

#ifdef STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

/**
 * \brief Enables HSI48 clock.
 *
 * This function waits until the HSI48 clock is stable after enabling it.
 *
 * \warning Before changing configuration of HSI48 clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of USB clock).
 */

void enableHsi48();

#endif	// def STM32F04_STM32F071_STM32F072_STM32F078_STM32F09_RCC_FEATURES

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] pllClockSource selects PLL clock source
 * \param [in] pllmul is the PLLMUL value for main PLL, [2; 16] or [minPllmul; maxPllmul]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllmul value is invalid;
 */

int enablePll(PllClockSource pllClockSource, uint8_t pllmul);

/**
 * \brief Switches system clock.
 *
 * \param [in] source is the new source of system clock
 */

void switchSystemClock(SystemClockSource source);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_RCC_HPP_
