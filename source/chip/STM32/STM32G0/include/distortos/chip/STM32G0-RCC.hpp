/**
 * \file
 * \brief Header for RCC-related functions for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_STM32G0_RCC_HPP_
#define SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_STM32G0_RCC_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

#if !defined(DISTORTOS_CHIP_STM32G030) && !defined(DISTORTOS_CHIP_STM32G050) && !defined(DISTORTOS_CHIP_STM32G070)

#define STM32G0_RCC_HAS_PLLQ

#endif	// !defined(DISTORTOS_CHIP_STM32G030) && !defined(DISTORTOS_CHIP_STM32G050) &&
		// !defined(DISTORTOS_CHIP_STM32G070)

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// LSE oscillator drive capability
enum class LseDriveCapability : uint8_t
{
	/// low driving capability of LSE oscillator
	low,
	/// medium-low driving capability of LSE oscillator
	mediumLow,
	/// medium-high driving capability of LSE oscillator
	mediumHigh,
	/// high driving capability of LSE oscillator
	high
};

/// clock source of PLL
enum class PllClockSource : uint8_t
{
	/// no clock sent to PLL
	none = 0,
	/// HSI16 oscillator selected as clock source of PLL
	hsi16 = 2,
	/// HSE oscillator selected as clock source of PLL
	hse = 3,
};

/// outputs of main PLL
enum class PllOutput : uint8_t
{
	/// PLLP output of main PLL
	p,
#ifdef STM32G0_RCC_HAS_PLLQ
	/// PLLQ output of main PLL
	q,
#endif	// def STM32G0_RCC_HAS_PLLQ
	/// PLLR output of main PLL
	r,
};

/// system clock source
enum class SystemClockSource : uint8_t
{
	/// HSISYS selected as system clock
	hsisys,
	/// HSE oscillator selected as system clock
	hse,
	/// main PLL's "/R" output selected as system clock
	pllr,
	/// LSI oscillator selected as system clock
	lsi,
	/// LSE oscillator selected as system clock
	lse,
};

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for PLLM
constexpr uint8_t minPllm {1};

/// maximum allowed value for PLLM
constexpr uint8_t maxPllm {8};

/// minimum allowed value for PLLN
constexpr uint16_t minPlln {8};

/// maximum allowed value for PLLN
constexpr uint16_t maxPlln {86};

/// minimum allowed value for PLLP
constexpr uint16_t minPllp {2};

/// maximum allowed value for PLLP
constexpr uint16_t maxPllp {32};

#ifdef STM32G0_RCC_HAS_PLLQ

/// minimum allowed value for PLLQ
constexpr uint16_t minPllq {2};

/// maximum allowed value for PLLQ
constexpr uint16_t maxPllq {8};

#endif	// def STM32G0_RCC_HAS_PLLQ

/// minimum allowed value for PLLR
constexpr uint16_t minPllr {2};

/// maximum allowed value for PLLR
constexpr uint16_t maxPllr {8};

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

/// first allowed value for HSI16 divider - 1
constexpr uint8_t hsi16Div1 {1};

/// second allowed value for HSI16 divider - 2
constexpr uint8_t hsi16Div2 {2};

/// third allowed value for HSI16 divider - 4
constexpr uint8_t hsi16Div4 {4};

/// fourth allowed value for HSI16 divider - 8
constexpr uint8_t hsi16Div8 {8};

/// fifth allowed value for HSI16 divider - 16
constexpr uint8_t hsi16Div16 {16};

/// sixth allowed value for HSI16 divider - 32
constexpr uint8_t hsi16Div32 {32};

/// seventh allowed value for HSI16 divider - 64
constexpr uint8_t hsi16Div64 {64};

/// eighth allowed value for HSI16 divider - 128
constexpr uint8_t hsi16Div128 {128};

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
 * \brief Configures divider of HSI16 clock (HSIDIV value).
 *
 * \param [in] hsidiv is the HSIDIV value, {1, 2, 4, 8, 16, 32, 64, 128} or {hsi16Div1, hsi16Div2, hsi16Div4, hsi16Div8,
 * hsi16Div16, hsi16Div32, hsi16Div64, hsi16Div128}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a hsidiv value is invalid;
 */

int configureHsi16ClockDivider(uint8_t hsidiv);

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
 * \brief Disables LSE clock.
 *
 * \note RTC domain write protection must be disabled during execution of this function.
 *
 * \warning Before changing configuration of LSE clock make sure that it is not used in any way (as core clock, as
 * source of peripheral clocks or as source of RTC clock).
 */

void disableLse();

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
 * \brief Enables LSE clock.
 *
 * Enables LSE clock using crystal/ceramic resonator (bypass disabled) or external user clock (bypass enabled). This
 * function waits until the LSE oscillator is stable after enabling the clock.
 *
 * \note RTC domain write protection must be disabled during execution of this function.
 *
 * \warning Before changing configuration of LSE clock make sure that it is not used in any way (as core clock, as
 * source of peripheral clocks or as source of RTC clock).
 *
 * \param [in] bypass selects whether crystal/ceramic resonator (false) or external user clock (true) is used
 * \param [in] lseDriveCapability selects LSE oscillator drive capability, applicable only when crystal/ceramic
 * resonator is used (bypass disabled, `bypass == false`)
 */

void enableLse(bool bypass, LseDriveCapability lseDriveCapability);

#ifdef STM32G0_RCC_HAS_PLLQ

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] pllClockSource selects clock source of PLL
 * \param [in] pllm is the PLLM value for main PLL, [1; 8] or [minPllm; maxPllm]
 * \param [in] plln is the PLLN value for main PLL, [8; 86] or [minPlln; maxPlln]
 * \param [in] pllp is the PLLP value for main PLL, [2; 32] or [minPllp; maxPllp]
 * \param [in] pllq is the PLLQ value for main PLL, [2; 8] or [minPllq; maxPllq]
 * \param [in] pllr is the PLLR value for main PLL, [2; 8] or [minPllr; maxPllr]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllm or \a plln or \a pllp or \a pllq or \a pllr value is invalid;
 */

int enablePll(PllClockSource pllClockSource, uint8_t pllm, uint16_t plln, uint8_t pllp, uint8_t pllq, uint8_t pllr);

#else	// !def STM32G0_RCC_HAS_PLLQ

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] pllClockSource selects clock source of PLL
 * \param [in] pllm is the PLLM value for main PLL, [1; 8] or [minPllm; maxPllm]
 * \param [in] plln is the PLLN value for main PLL, [8; 86] or [minPlln; maxPlln]
 * \param [in] pllp is the PLLP value for main PLL, [2; 32] or [minPllp; maxPllp]
 * \param [in] pllr is the PLLR value for main PLL, [2; 8] or [minPllr; maxPllr]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllm or \a plln or \a pllp or \a pllr value is invalid;
 */

int enablePll(PllClockSource pllClockSource, uint8_t pllm, uint16_t plln, uint8_t pllp, uint8_t pllr);

#endif	// !def STM32G0_RCC_HAS_PLLQ

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

#endif	// SOURCE_CHIP_STM32_STM32G0_INCLUDE_DISTORTOS_CHIP_STM32G0_RCC_HPP_
