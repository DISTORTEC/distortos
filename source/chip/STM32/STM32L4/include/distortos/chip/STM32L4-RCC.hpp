/**
 * \file
 * \brief Header for RCC-related functions for STM32L4
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_STM32L4_RCC_HPP_
#define SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_STM32L4_RCC_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// clock source of PLLs
enum class PllClockSource : uint8_t
{
	/// no clock sent to PLLs
	none,
	/// MSI oscillator selected as clock source of PLLs
	msi,
	/// HSI16 oscillator selected as clock source of PLLs
	hsi16,
	/// HSE oscillator selected as clock source of PLLs
	hse,
};

/// outputs of main PLL
enum class PllOutput : uint8_t
{
	/// PLLP output of main PLL
	p,
	/// PLLQ output of main PLL
	q,
	/// PLLR output of main PLL
	r,
};

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
constexpr uint8_t maxMsi {11};

/// minimum allowed value for PLLM
constexpr uint8_t minPllm {1};

/// maximum allowed value for PLLM
constexpr uint8_t maxPllm {8};

/// minimum allowed value for PLLN
constexpr uint16_t minPlln {8};

/// maximum allowed value for PLLN
constexpr uint16_t maxPlln {86};

#if defined(DISTORTOS_CHIP_STM32L43) || defined(DISTORTOS_CHIP_STM32L44) || defined(DISTORTOS_CHIP_STM32L45) || \
		defined(DISTORTOS_CHIP_STM32L46) || defined(DISTORTOS_CHIP_STM32L496) || defined(DISTORTOS_CHIP_STM32L4A6)

/// minimum allowed value for PLLP
constexpr uint16_t minPllp {2};

/// maximum allowed value for PLLP
constexpr uint16_t maxPllp {31};

#else	// !defined(DISTORTOS_CHIP_STM32L43) && !defined(DISTORTOS_CHIP_STM32L44) &&
		// !defined(DISTORTOS_CHIP_STM32L45) && !defined(DISTORTOS_CHIP_STM32L46) &&
		// !defined(DISTORTOS_CHIP_STM32L496) && !defined(DISTORTOS_CHIP_STM32L4A6)

/// first allowed value for PLLP - 7
constexpr uint8_t pllpDiv7 {7};

/// second allowed value for PLLP - 17
constexpr uint8_t pllpDiv17 {17};

#endif	// !defined(DISTORTOS_CHIP_STM32L43) && !defined(DISTORTOS_CHIP_STM32L44) &&
		// !defined(DISTORTOS_CHIP_STM32L45) && !defined(DISTORTOS_CHIP_STM32L46) &&
		// !defined(DISTORTOS_CHIP_STM32L496) && !defined(DISTORTOS_CHIP_STM32L4A6)

/// first allowed value for PLLQ - 2
constexpr uint8_t pllqDiv2 {2};

/// second allowed value for PLLQ - 4
constexpr uint8_t pllqDiv4 {4};

/// third allowed value for PLLQ - 6
constexpr uint8_t pllqDiv6 {6};

/// fourth allowed value for PLLQ - 8
constexpr uint8_t pllqDiv8 {8};

/// first allowed value for PLLR - 2
constexpr uint8_t pllrDiv2 {2};

/// second allowed value for PLLR - 4
constexpr uint8_t pllrDiv4 {4};

/// third allowed value for PLLR - 6
constexpr uint8_t pllrDiv6 {6};

/// fourth allowed value for PLLR - 8
constexpr uint8_t pllrDiv8 {8};

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
 * \brief Configures clock source of PLLs.
 *
 * \warning Before changing configuration of any PLL make sure that they are not used in any way (as core clock or as
 * source of peripheral clocks) and that they are disabled.
 *
 * \param [in] pllClockSource selects clock source of PLLs
 */

void configurePllClockSource(PllClockSource pllClockSource);

/**
 * \brief Configures divider of PLL input clock (PLLM value) for PLLs.
 *
 * \warning Before changing configuration of any PLL make sure that they are not used in any way (as core clock or as
 * source of peripheral clocks) and that they are disabled.
 *
 * \param [in] pllm is the PLLM value for PLLs, [1; 8] or [minPllm; maxPllm]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllm value is invalid;
 */

int configurePllInputClockDivider(uint8_t pllm);

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
 * source for any PLL or as source of peripheral clocks).
 */

void disableHsi16();

/**
 * \brief Disables MSI clock.
 *
 * \warning Before changing configuration of MSI clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of peripheral clocks).
 */

void disableMsi();

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

/**
 * \brief Enables HSI16 clock.
 *
 * Enables HSI16 clock. This function waits until the HSI16 oscillator is stable after enabling the clock.
 *
 * \warning Before changing configuration of HSI16 clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of peripheral clocks).
 */

void enableHsi16();

/**
 * \brief Enables MSI clock.
 *
 * Enables HSE clock using MSI clock generated from internal RC oscillator. This function waits until the MSI oscillator
 * is stable after enabling the clock.
 *
 * \warning Before changing configuration of MSI clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of peripheral clocks).
 *
 * \param [in] range is the MSIRANGE value for MSI [0; 11] or [minMsi; maxMsi]
 */

int enableMsi(const uint8_t range);

#if defined(DISTORTOS_CHIP_STM32L43) || defined(DISTORTOS_CHIP_STM32L44) || defined(DISTORTOS_CHIP_STM32L45) || \
		defined(DISTORTOS_CHIP_STM32L46) || defined(DISTORTOS_CHIP_STM32L496) || defined(DISTORTOS_CHIP_STM32L4A6)

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] plln is the PLLN value for main PLL, [8; 86] or [minPlln; maxPlln]
 * \param [in] pllp is the PLLP value for main PLL, [2; 31] or [minPllp; maxPllp]
 * \param [in] pllq is the PLLQ value for main PLL, {2, 4, 6, 8} or {pllqDiv2, pllqDiv4, pllqDiv6, pllqDiv8}
 * \param [in] pllr is the PLLR value for main PLL, {2, 4, 6, 8} or {pllrDiv2, pllrDiv4, pllrDiv6, pllrDiv8}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a plln or \a pllp or \a pllq or \a pllr value is invalid;
 */

#else	// !defined(DISTORTOS_CHIP_STM32L43) && !defined(DISTORTOS_CHIP_STM32L44) &&
		// !defined(DISTORTOS_CHIP_STM32L45) && !defined(DISTORTOS_CHIP_STM32L46) &&
		// !defined(DISTORTOS_CHIP_STM32L496) && !defined(DISTORTOS_CHIP_STM32L4A6)

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] plln is the PLLN value for main PLL, [8; 86] or [minPlln; maxPlln]
 * \param [in] pllp is the PLLP value for main PLL, {7, 17} or {pllpDiv7, pllpDiv17}
 * \param [in] pllq is the PLLQ value for main PLL, {2, 4, 6, 8} or {pllqDiv2, pllqDiv4, pllqDiv6, pllqDiv8}
 * \param [in] pllr is the PLLR value for main PLL, {2, 4, 6, 8} or {pllrDiv2, pllrDiv4, pllrDiv6, pllrDiv8}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a plln or \a pllp or \a pllq or \a pllr value is invalid;
 */

#endif	// !defined(DISTORTOS_CHIP_STM32L43) && !defined(DISTORTOS_CHIP_STM32L44) &&
		// !defined(DISTORTOS_CHIP_STM32L45) && !defined(DISTORTOS_CHIP_STM32L46) &&
		// !defined(DISTORTOS_CHIP_STM32L496) && !defined(DISTORTOS_CHIP_STM32L4A6)

int enablePll(uint16_t plln, uint8_t pllp, uint8_t pllq, uint8_t pllr);

/**
 * \brief Enables or disables selected output of main PLL.
 *
 * \warning Before disabling selected output of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks).
 *
 * \param [in] pllOutput selects the output which will be enabled or disabled
 * \param [in] enable selects whether the output will be enabled (true) or disabled (false)
 */

void enablePllOutput(PllOutput pllOutput, bool enable);

/**
 * \brief Switches system clock.
 *
 * \param [in] source is the new source of system clock
 */

void switchSystemClock(SystemClockSource source);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_STM32L4_RCC_HPP_
