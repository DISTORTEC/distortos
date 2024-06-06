/**
 * \file
 * \brief Implementation of PWR-related functions for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32G0-PWR.hpp"

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

	PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS) | ((voltageScale << PWR_CR1_VOS_Pos) & PWR_CR1_VOS);
	while ((PWR->SR2 & PWR_SR2_VOSF) != 0);
	return 0;
}

void disableRtcDomainWriteProtection()
{
	PWR->CR1 |= PWR_CR1_DBP;
	while ((PWR->CR1 & PWR_CR1_DBP) == 0);
}

void enableRtcDomainWriteProtection()
{
	PWR->CR1 &= ~PWR_CR1_DBP;
}

}	// namespace chip

}	// namespace distortos
