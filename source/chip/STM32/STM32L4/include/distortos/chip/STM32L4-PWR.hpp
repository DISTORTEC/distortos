/**
 * \file
 * \brief Header for PWR-related functions for STM32L4
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_STM32L4_PWR_HPP_
#define SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_STM32L4_PWR_HPP_

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
constexpr uint8_t maxVoltageScale {2};

/// default voltage scale mode after reset
constexpr uint8_t defaultVoltageScale {1};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures voltage scaling.
 *
 * This function waits until selected voltage level is reached.
 *
 * \param [in] voltageScale is the new voltage scale mode, [minVoltageScale; maxVoltageScale]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a voltageScale value is invalid;
 */

int configureVoltageScaling(uint8_t voltageScale);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32L4_INCLUDE_DISTORTOS_CHIP_STM32L4_PWR_HPP_
