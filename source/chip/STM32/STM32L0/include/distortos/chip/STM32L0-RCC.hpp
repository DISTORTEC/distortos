/**
 * \file
 * \brief Header for RCC-related functions for STM32L0
 *
 * \author Copyright (C) 2017-2019 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_STM32L0_RCC_HPP_
#define SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_STM32L0_RCC_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

#if defined(DISTORTOS_CHIP_STM32L052) || defined(DISTORTOS_CHIP_STM32L053) || defined(DISTORTOS_CHIP_STM32L062) || \
		defined(DISTORTOS_CHIP_STM32L063) || defined(DISTORTOS_CHIP_STM32L072) || defined(DISTORTOS_CHIP_STM32L073) || \
		defined(DISTORTOS_CHIP_STM32L082) || defined(DISTORTOS_CHIP_STM32L083)

#define STM32L0X2_STM32L0X3_RCC_FEATURES

#endif	// defined(DISTORTOS_CHIP_STM32L052) || defined(DISTORTOS_CHIP_STM32L053) ||
		// defined(DISTORTOS_CHIP_STM32L062) || defined(DISTORTOS_CHIP_STM32L063) ||
		// defined(DISTORTOS_CHIP_STM32L072) || defined(DISTORTOS_CHIP_STM32L073) ||
		// defined(DISTORTOS_CHIP_STM32L082) || defined(DISTORTOS_CHIP_STM32L083)

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// system clock source
enum class SystemClockSource : uint8_t
{
	/// MSI oscillator selected as system clock
	msi,
	/// HSI16 oscillator selected as system clock
	hsi16,
	/// HSE oscillator selected as system clock
	hse,
	/// main PLL selected as system clock
	pll,
};

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for MSI range
constexpr uint8_t minMsi {0};

/// maximum allowed value for MSI range
constexpr uint8_t maxMsi {6};

/// minimum allowed value for PLLDIV
constexpr uint8_t minPlldiv {2};

/// maximum allowed value for PLLDIV
constexpr uint8_t maxPlldiv {4};

/// first allowed value for PLLMUL - 3
constexpr uint8_t pllMul3 {3};

/// second allowed value for PLLMUL - 4
constexpr uint8_t pllMul4 {4};

/// third allowed value for PLLMUL - 6
constexpr uint8_t pllMul6 {6};

/// fourth allowed value for PLLMUL - 8
constexpr uint8_t pllMul8 {8};

/// fifth allowed value for PLLMUL - 12
constexpr uint8_t pllMul12 {12};

/// sixth allowed value for PLLMUL - 16
constexpr uint8_t pllMul16 {16};

/// seventh allowed value for PLLMUL - 24
constexpr uint8_t pllMul24 {24};

/// eighth allowed value for PLLMUL - 32
constexpr uint8_t pllMul32 {32};

/// ninth allowed value for PLLMUL - 48
constexpr uint8_t pllMul48 {48};

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
 * \brief Configures divider of APB1 or APB2 clock (PPRE1 or PPRE2 value).
 *
 * \param [in] ppre2 selects whether PPRE1 (false) or PPRE2 (true) is configured
 * \param [in] ppre is the PPRE value, {1, 2, 4, 8, 16} or {ppreDiv1, ppreDiv2, ppreDiv4, ppreDiv8, ppreDiv16}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a ppre value is invalid;
 */

int configureApbClockDivider(bool ppre2, uint8_t ppre);

/**
 * \brief Configures clock source of PLL.
 *
 * \warning Before changing configuration of any PLL make sure that they are not used in any way (as core clock or as
 * source of peripheral clocks) and that they are disabled.
 *
 * \param [in] hse selects whether HSI16 (false) or HSE (true) is used as clock source of PLL
 */

void configurePllClockSource(bool hse);

/**
 * \brief Disables MSI clock.
 *
 * \warning Before changing configuration of MSI clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void disableMsi();

/**
 * \brief Disables HSE clock.
 *
 * \warning Before changing configuration of HSE clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void disableHse();

/**
 * \brief Disables HSI16 clock.
 *
 * \warning Before changing configuration of HSI16 clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void disableHsi16();

#ifdef STM32L0X2_STM32L0X3_RCC_FEATURES

/**
 * \brief Disables HSI48 clock.
 *
 * \warning Before changing configuration of HSI48 clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of USB clock).
 */

void disableHsi48();

#endif	// def STM32L0X2_STM32L0X3_RCC_FEATURES

/**
 * \brief Disables main PLL.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks).
 */

void disablePll();

/**
 * \brief Enables MSI clock.
 *
 * Enables HSE clock using MSI clock generated from internal RC oscillator. This
 * function waits until the MSI oscillator is stable after enabling the clock.
 *
 * \warning Before changing configuration of MSI clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 *
 * \param [in] range is the MSIRANGE value for MSI [0; 6] or [minMsi; maxMsi]
 */

int enableMsi(const uint8_t range);

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

/**
 * \brief Enables HSI16 clock.
 *
 * Enables HSI16 clock. This function waits until the HSI16 oscillator is stable after enabling the clock.
 *
 * \warning Before changing configuration of HSI16 clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void enableHsi16();

#ifdef STM32L0X2_STM32L0X3_RCC_FEATURES

/**
 * \brief Enables HSI48 clock.
 *
 * This function waits until the HSI48 clock is stable after enabling it.
 *
 * \warning Before changing configuration of HSI48 clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of USB clock).
 */

void enableHsi48();

#endif	// def STM32L0X2_STM32L0X3_RCC_FEATURES

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 * \param [in] pllmul is the PLLMUL value for main PLL, {3, 4, 6, 8, 12, 16, 24, 32, 48} or {pllMul3, pllMul4, pllMul6,
 * pllMul8, pllMul12, pllMul16, pllMul24, pllMul32, pllMul48}
 * \param [in] plldiv is the PLLDIV value for main PLL, [2; 4] or [minPlldiv; maxPlldiv]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllmul or \a plldiv value is invalid;
 */

int enablePll(const uint8_t pllmul, const uint8_t plldiv);

/**
 * \brief Switches system clock.
 *
 * \param [in] source is the new source of system clock
 */

void switchSystemClock(SystemClockSource source);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_STM32L0_RCC_HPP_
