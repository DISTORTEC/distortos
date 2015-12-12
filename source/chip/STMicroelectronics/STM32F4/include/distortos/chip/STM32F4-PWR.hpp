/**
 * \file
 * \brief Header for PWR-related functions for STM32F4
 *
 * This file covers devices as described in following places:
 * - RM0368 reference manual (STM32F401xB/C and STM32F401xD/E), Revision 4, 2015-05-04
 * - RM0090 reference manual (STM32F405/415, STM32F407/417, STM32F427/437 and STM32F429/439), Revision 11, 2015-10-20
 * - RM0401 reference manual (STM32F410), Revision 2, 2015-10-26
 * - RM0383 reference manual (STM32F411xC/E), Revision 1, 2014-07-24
 * - RM0390 reference manual (STM32F446xx), Revision 1, 2015-03-17
 * - RM0386 reference manual (STM32F469xx and STM32F479xx), Revision 2, 2015-11-19
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-12
 */

#ifndef SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_HPP_
#define SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_HPP_

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
#if defined(CONFIG_CHIP_STM32F401)
constexpr uint8_t minVoltageScale {2};
#else	// !defined(CONFIG_CHIP_STM32F401)
constexpr uint8_t minVoltageScale {1};
#endif	// !defined(CONFIG_CHIP_STM32F401)

/// maximum allowed value of voltage scale mode
#if defined(CONFIG_CHIP_STM32F405) || defined(CONFIG_CHIP_STM32F407) || defined(CONFIG_CHIP_STM32F415) || \
		defined(CONFIG_CHIP_STM32F417)
constexpr uint8_t maxVoltageScale {2};
#else	// !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) && !defined(CONFIG_CHIP_STM32F415) &&
		// !defined(CONFIG_CHIP_STM32F417)
constexpr uint8_t maxVoltageScale {3};
#endif	// !defined(CONFIG_CHIP_STM32F405) && !defined(CONFIG_CHIP_STM32F407) && !defined(CONFIG_CHIP_STM32F415) &&
		// !defined(CONFIG_CHIP_STM32F417)

/// default voltage scale mode after reset
#if defined(CONFIG_CHIP_STM32F401) || defined(CONFIG_CHIP_STM32F410) || defined(CONFIG_CHIP_STM32F411)
constexpr uint8_t defaultVoltageScale {2};
#else	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411)
constexpr uint8_t defaultVoltageScale {1};
#endif	// !defined(CONFIG_CHIP_STM32F401) && !defined(CONFIG_CHIP_STM32F410) && !defined(CONFIG_CHIP_STM32F411)

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

#if defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) || \
		defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

/**
 * \brief Disables over-drive mode.
 *
 * \warning Over-drive mode can be enabied/disabled only when HSI or HSE is selected as source of system clock
 */

void disableOverDriveMode();

/**
 * \brief Enables over-drive mode.
 *
 * \warning Over-drive mode can be enabied/disabled only when HSI or HSE is selected as source of system clock
 */

void enableOverDriveMode();

#endif	// defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) ||
		// defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_PWR_HPP_
