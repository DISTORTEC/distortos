/**
 * \file
 * \brief Header for RCC-related functions for STM32F1
 *
 * This file covers devices as described in following places:
 * - RM0008 reference manual (STM32F101, STM32F102, STM32F103, STM32F105 and STM32F107), Revision 16, 2015-11-26
 * - RM0041 reference manual (STM32F100), Revision 4, 2011-07-21
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for PREDIV1 and PREDIV2
constexpr uint8_t minPrediv {1};

/// maximum allowed value for PREDIV1 and PREDIV2
#if defined(CONFIG_CHIP_STM32F100) || defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)
constexpr uint8_t maxPrediv {16};
#else	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)
constexpr uint8_t maxPrediv {2};
#endif	// !defined(CONFIG_CHIP_STM32F100) && !defined(CONFIG_CHIP_STM32F105) && !defined(CONFIG_CHIP_STM32F107)

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures PREDIV1 division factor.
 *
 * \warning Before changing configuration of PLL make sure that it is not used in any way (as core clock or as source of
 * peripheral clocks) and that it is disabled.
 *
 * \param [in] prediv1 is the PREDIV1 division factor, [minPrediv1; maxPrediv1]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a prediv1 value is invalid;
 */

int configurePrediv1(uint8_t prediv1);

#if defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

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

#endif	// defined(CONFIG_CHIP_STM32F105) || defined(CONFIG_CHIP_STM32F107)

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

#endif	// SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_HPP_
