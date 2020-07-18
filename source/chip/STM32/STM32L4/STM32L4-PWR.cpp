/**
 * \file
 * \brief Implementation of PWR-related functions for STM32L4
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32L4-PWR.hpp"

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

	PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS) | ((voltageScale << PWR_CR1_VOS_Pos) & PWR_CR1_VOS);
	while (STM32_BITBAND(PWR, SR2, VOSF) != 0);
	return 0;
}

}	// namespace chip

}	// namespace distortos
