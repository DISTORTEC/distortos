/**
 * \file
 * \brief Header for RCC-related functions for STM32F1
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_HPP_
#define SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

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
	/// HSI oscillator selected as system clock
	hsi,
	/// HSE oscillator selected as system clock
	hse,
	/// main PLL selected as system clock
	pll,
};

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for PLLMUL
#if defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)
constexpr uint8_t minPllmul {4};
#else	// !defined(DISTORTOS_CHIP_STM32F105) && !defined(DISTORTOS_CHIP_STM32F107)
constexpr uint8_t minPllmul {2};
#endif	// !defined(DISTORTOS_CHIP_STM32F105) && !defined(DISTORTOS_CHIP_STM32F107)

/// maximum allowed value for PLLMUL
#if defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)
constexpr uint8_t maxPllmul {9};
#else	// !defined(DISTORTOS_CHIP_STM32F105) && !defined(DISTORTOS_CHIP_STM32F107)
constexpr uint8_t maxPllmul {16};
#endif	// !defined(DISTORTOS_CHIP_STM32F105) && !defined(DISTORTOS_CHIP_STM32F107)

#if defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

/// additional allowed value for PLLMUL - 6.5 (note: numeric value is not equal to logical value!)
constexpr uint16_t pllmul6_5 {15};

/// minimum allowed value for PLL2MUL and PLL3MUL
constexpr uint8_t minPll23Mul {8};

/// maximum allowed value for PLL2MUL and PLL3MUL
constexpr uint8_t maxPll23Mul {14};

/// first additional allowed value for PLL2MUL and PLL3MUL - 16
constexpr uint16_t pll23Mul16 {16};

/// second additional allowed value for PLL2MUL and PLL3MUL - 20
constexpr uint16_t pll23Mul20 {20};

#endif	// defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

/// minimum allowed value for PREDIV1 and PREDIV2
constexpr uint8_t minPrediv {1};

/// maximum allowed value for PREDIV1 and PREDIV2
#if defined(DISTORTOS_CHIP_STM32F100) || defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)
constexpr uint8_t maxPrediv {16};
#else	// !defined(DISTORTOS_CHIP_STM32F100) && !defined(DISTORTOS_CHIP_STM32F105) &&
		// !defined(DISTORTOS_CHIP_STM32F107)
constexpr uint8_t maxPrediv {2};
#endif	// !defined(DISTORTOS_CHIP_STM32F100) && !defined(DISTORTOS_CHIP_STM32F105) &&
		// !defined(DISTORTOS_CHIP_STM32F107)

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

/// first allowed value for APB1 and APB2 dividers - 1
constexpr uint8_t ppreDiv1 {1};

/// second allowed value for APB1 and APB2 dividers - 2
constexpr uint8_t ppreDiv2 {2};

/// third allowed value for APB1 and APB2 dividers - 4
constexpr uint8_t ppreDiv4 {4};

/// fourth allowed value for APB1 and APB2 dividers - 8
constexpr uint8_t ppreDiv8 {8};

/// fifth allowed value for APB1 and APB2 dividers - 16
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
 * \brief Configures PREDIV1 division factor.
 *
 * \warning Before changing configuration of PLL make sure that it is not used in any way (as core clock or as source of
 * peripheral clocks) and that it is disabled.
 *
 * \param [in] prediv1 is the PREDIV1 division factor, [minPrediv; maxPrediv]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a prediv1 value is invalid;
 */

int configurePrediv1(uint8_t prediv1);

#if defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

/**
 * \brief Configures PREDIV1 clock source.
 *
 * \warning Before changing configuration of PLL make sure that it is not used in any way (as core clock or as source of
 * peripheral clocks) and that it is disabled.
 *
 * \param [in] pll2 selects HSE (false) or PLL2 (true) as PREDIV1 clock source
 */

void configurePrediv1ClockSource(bool pll2);

/**
 * \brief Configures PREDIV2 division factor.
 *
 * \warning Before changing configuration of any PLL make sure that they are not used in any way (as core clock or as
 * source of peripheral clocks) and that they are disabled.
 *
 * \param [in] prediv2 is the PREDIV2 division factor, [minPrediv; maxPrediv]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a prediv2 value is invalid;
 */

int configurePrediv2(uint8_t prediv2);

#endif	// defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

/**
 * \brief Disables HSE clock.
 *
 * \warning Before changing configuration of HSE clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void disableHse();

/**
 * \brief Disables main PLL.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks).
 */

void disablePll();

#if defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

/**
 * \brief Disables PLL2.
 *
 * \warning Before changing configuration of PLL2 make sure that it is not used in any way (as source of main PLL).
 */

void disablePll2();

/**
 * \brief Disables PLL3.
 *
 * \warning Before changing configuration of PLL3 make sure that it is not used in any way (as source of peripheral
 * clocks).
 */

void disablePll3();

#endif	// defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

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
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] prediv1 selects whether HSI / 2 (false) or PREDIV1 (true) is used as clock source of main PLL
 * \param [in] pllmul is the PLLMUL value for main PLL, [minPllmul; maxPllmul] or (only for STM32F105 and STM32F107)
 * [minPllmul; maxPllmul] and pllmul6_5
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllmul value is invalid;
 */

int enablePll(bool prediv1, uint8_t pllmul);

#if defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

/**
 * \brief Enables PLL2.
 *
 * Enables PLL2 using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of PLL2 make sure that it is not used in any way (as source of main PLL).
 *
 * \param [in] pll2Mul is the PLL2MUL value for PLL2, [minPll23Mul; maxPll23Mul] and {pll23Mul16, pll23Mul20}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pll2Mul value is invalid;
 */

int enablePll2(uint8_t pll2Mul);

/**
 * \brief Enables PLL3.
 *
 * Enables PLL3 using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of PLL3 make sure that it is not used in any way (as source of peripheral
 * clocks).
 *
 * \param [in] pll3Mul is the PLL3MUL value for PLL3, [minPll23Mul; maxPll23Mul] and {pll23Mul16, pll23Mul20}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pll3Mul value is invalid;
 */

int enablePll3(uint8_t pll3Mul);

#endif	// defined(DISTORTOS_CHIP_STM32F105) || defined(DISTORTOS_CHIP_STM32F107)

/**
 * \brief Switches system clock.
 *
 * \param [in] source is the new source of system clock, SystemClockSource::{hsi, hse, pll}
 */

void switchSystemClock(SystemClockSource source);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_HPP_
