/**
 * \file
 * \brief Implementation of PWR-related functions for STM32F4
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-12-12
 */

#include "distortos/chip/STM32F4-PWR.hpp"

#include "distortos/chip/STM32F4-PWR-bits.h"

#include "distortos/chip/CMSIS-proxy.h"

#include <cerrno>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

int configureVoltageScaling(const uint8_t voltageScale)
{
	if (voltageScale < minVoltageScale || voltageScale > maxVoltageScale)
		return EINVAL;

	PWR->CR = (PWR->CR & ~PWR_CR_VOS) | (((4 - voltageScale) << PWR_CR_VOS_bit) & PWR_CR_VOS);
	return 0;
}

#if defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) || \
		defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

void disableOverDriveMode()
{
	PWR->CR &= ~(PWR_CR_ODEN | PWR_CR_ODSWEN);
}

void enableOverDriveMode()
{
	PWR_CR_ODEN_bb = 1;
	while (PWR_CSR_ODRDY_bb == 0);
	PWR_CR_ODSWEN_bb = 1;
	while (PWR_CSR_ODSWRDY_bb == 0);
}

#endif	// defined(CONFIG_CHIP_STM32F42) || defined(CONFIG_CHIP_STM32F43) || defined(CONFIG_CHIP_STM32F446) ||
		// defined(CONFIG_CHIP_STM32F469) || defined(CONFIG_CHIP_STM32F479)

}	// namespace chip

}	// namespace distortos
