/**
 * \file
 * \brief Header for PWR-related functions for STM32F4
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_HPP_
#define SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value of voltage scale mode
#if defined(DISTORTOS_CHIP_STM32F401)
constexpr uint8_t minVoltageScale {2};
#else	// !defined(DISTORTOS_CHIP_STM32F401)
constexpr uint8_t minVoltageScale {1};
#endif	// !defined(DISTORTOS_CHIP_STM32F401)

/// maximum allowed value of voltage scale mode
#if defined(DISTORTOS_CHIP_STM32F405) || defined(DISTORTOS_CHIP_STM32F407) || defined(DISTORTOS_CHIP_STM32F415) || \
		defined(DISTORTOS_CHIP_STM32F417)
constexpr uint8_t maxVoltageScale {2};
#else	// !defined(DISTORTOS_CHIP_STM32F405) && !defined(DISTORTOS_CHIP_STM32F407) &&
		// !defined(DISTORTOS_CHIP_STM32F415) && !defined(DISTORTOS_CHIP_STM32F417)
constexpr uint8_t maxVoltageScale {3};
#endif	// !defined(DISTORTOS_CHIP_STM32F405) && !defined(DISTORTOS_CHIP_STM32F407) &&
		// !defined(DISTORTOS_CHIP_STM32F415) && !defined(DISTORTOS_CHIP_STM32F417)

/// default voltage scale mode after reset
#if defined(DISTORTOS_CHIP_STM32F401) || defined(DISTORTOS_CHIP_STM32F410) || defined(DISTORTOS_CHIP_STM32F411) || \
		defined(DISTORTOS_CHIP_STM32F412) || defined(DISTORTOS_CHIP_STM32F413) || defined(DISTORTOS_CHIP_STM32F423)
constexpr uint8_t defaultVoltageScale {2};
#else	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)
constexpr uint8_t defaultVoltageScale {1};
#endif	// !defined(DISTORTOS_CHIP_STM32F401) && !defined(DISTORTOS_CHIP_STM32F410) &&
		// !defined(DISTORTOS_CHIP_STM32F411) && !defined(DISTORTOS_CHIP_STM32F412) &&
		// !defined(DISTORTOS_CHIP_STM32F413) && !defined(DISTORTOS_CHIP_STM32F423)

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures voltage scaling.
 *
 * \warning Voltage scaling can be configured only when PLL is disabled.
 *
 * \param [in] voltageScale is the new voltage scale mode, [minVoltageScale; maxVoltageScale]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a voltageScale value is invalid;
 */

int configureVoltageScaling(uint8_t voltageScale);

#if defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) || defined(DISTORTOS_CHIP_STM32F43) || \
		defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || defined(DISTORTOS_CHIP_STM32F479)

/**
 * \brief Disables over-drive mode.
 *
 * \warning Over-drive mode can be enabled/disabled only when HSI or HSE is selected as source of system clock
 */

void disableOverDriveMode();

/**
 * \brief Enables over-drive mode.
 *
 * \warning Over-drive mode can be enabled/disabled only when HSI or HSE is selected as source of system clock
 */

void enableOverDriveMode();

#endif	// defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) || defined(DISTORTOS_CHIP_STM32F43) ||
		// defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || defined(DISTORTOS_CHIP_STM32F479)

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_HPP_
