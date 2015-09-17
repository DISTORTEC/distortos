/**
 * \file
 * \brief Header for RCC-related functions for STM32F4
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-17
 */

#ifndef SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_RCC_HPP_
#define SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_RCC_HPP_

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for PLLM
constexpr uint8_t minPllm {2};

/// maximum allowed value for PLLM
constexpr uint8_t maxPllm {63};

/// minimum allowed value for PLLN
constexpr uint16_t minPlln {192};

/// maximum allowed value for PLLN
constexpr uint16_t maxPlln {432};

/// minimum allowed value for PLLQ
constexpr uint8_t minPllq {2};

/// maximum allowed value for PLLQ
constexpr uint8_t maxPllq {15};

/// first allowed value for PLLP - 2
constexpr uint8_t pllpDiv2 {2};

/// second allowed value for PLLP - 4
constexpr uint8_t pllpDiv4 {4};

/// third allowed value for PLLP - 6
constexpr uint8_t pllpDiv6 {6};

/// fourth allowed value for PLLP - 8
constexpr uint8_t pllpDiv8 {8};

/// HSI clock frequency, Hz
constexpr uint32_t hsiHz {16000000};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures clock source of main PLL and audio PLLI2S.
 *
 * \warning Before changing configuration of main PLL and/or audio PLLI2S make sure that they are not used in any way
 * (as core clock or as source of peripheral clocks) and that they are disabled.
 *
 * \param [in] hse selects whether HSI (false) or HSE (true) is used as clock source of main PLL and audio PLLI2S
 */

void configurePllClockSource(bool hse);

/**
 * \brief Configures divider of PLL input clock (PLLM value) for main PLL and audio PLLI2S.
 *
 * \warning Before changing configuration of main PLL and/or audio PLLI2S make sure that they are not used in any way
 * (as core clock or as source of peripheral clocks) and that they are disabled.
 *
 * \param [in] pllm is the PLLM value for main PLL and audio PLLI2S, [2; 63] or [minPllm; maxPllm]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllm value is invalid;
 */

int configurePllInputClockDivider(uint8_t pllm);

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
 * \param [in] plln is the PLLN value for main PLL, [192; 432] or [minPlln; maxPlln]
 * \param [in] pllp is the PLLP value for main PLL, {2, 4, 6, 8} or {pllpDiv2, pllpDiv4, pllpDiv6, pllpDiv8}
 * \param [in] pllq is the PLLQ value for main PLL, [2; 15] or [minPllq; maxPllq]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a plln or \a pllp or \a pllq value is invalid;
 */

int enablePll(uint16_t plln, uint8_t pllp, uint8_t pllq);

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

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_RCC_HPP_
