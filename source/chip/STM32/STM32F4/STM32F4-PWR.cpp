/**
 * \file
 * \brief Implementation of PWR-related functions for STM32F4
 *
 * \author Copyright (C) 2015-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F4-PWR.hpp"

#include "distortos/chip/CMSIS-proxy.h"
#include "distortos/chip/STM32-bit-banding.h"

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

	PWR->CR = (PWR->CR & ~PWR_CR_VOS) | (((4 - voltageScale) << PWR_CR_VOS_Pos) & PWR_CR_VOS);
	return 0;
}

#if defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) || defined(DISTORTOS_CHIP_STM32F43) || \
		defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || defined(DISTORTOS_CHIP_STM32F479)

void disableOverDriveMode()
{
	PWR->CR &= ~(PWR_CR_ODEN | PWR_CR_ODSWEN);
	while (STM32_BITBAND(PWR, CSR, ODSWRDY) != 0);
}

void enableOverDriveMode()
{
	STM32_BITBAND(PWR, CR, ODEN) = 1;
	while (STM32_BITBAND(PWR, CSR, ODRDY) == 0);
	STM32_BITBAND(PWR, CR, ODSWEN) = 1;
	while (STM32_BITBAND(PWR, CSR, ODSWRDY) == 0);
}

#endif	// defined(DISTORTOS_CHIP_STM32F427) || defined(DISTORTOS_CHIP_STM32F429) || defined(DISTORTOS_CHIP_STM32F43) ||
		// defined(DISTORTOS_CHIP_STM32F446) || defined(DISTORTOS_CHIP_STM32F469) || defined(DISTORTOS_CHIP_STM32F479)

}	// namespace chip

}	// namespace distortos
