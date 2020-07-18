/**
 * \file
 * \brief Header for PWR-related functions for STM32L0
 *
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_STM32L0_PWR_HPP_
#define SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_STM32L0_PWR_HPP_

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value of voltage scale mode
constexpr uint8_t minVoltageScale {1};

/// maximum allowed value of voltage scale mode
constexpr uint8_t maxVoltageScale {3};

/// default voltage scale mode after reset
constexpr uint8_t defaultVoltageScale {1};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures voltage scaling.
 *
 * \warning Voltage scaling can be configured only when PLL is disabled.
 *
 * \param [in] voltageScale is the new voltage scale mode, [1; 3] or [minVoltageScale; maxVoltageScale]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a voltageScale value is invalid;
 */

int configureVoltageScaling(uint8_t voltageScale);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32L0_INCLUDE_DISTORTOS_CHIP_STM32L0_PWR_HPP_
