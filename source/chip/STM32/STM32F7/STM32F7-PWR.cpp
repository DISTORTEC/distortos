/**
 * \file
 * \brief Implementation of PWR-related functions for STM32F7
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F7-PWR.hpp"

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

	PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS) | (((4 - voltageScale) << PWR_CR1_VOS_Pos) & PWR_CR1_VOS);
	return 0;
}

void disableOverDriveMode()
{
	PWR->CR1 &= ~(PWR_CR1_ODEN | PWR_CR1_ODSWEN);
	while ((PWR->CSR1 & PWR_CSR1_ODSWRDY) != 0);
}

void enableOverDriveMode()
{
	PWR->CR1 |= PWR_CR1_ODEN;
	while ((PWR-> CSR1 & PWR_CSR1_ODRDY) == 0);
	PWR->CR1 |= PWR_CR1_ODSWEN;
	while ((PWR-> CSR1 & PWR_CSR1_ODSWRDY) == 0);
}

}	// namespace chip

}	// namespace distortos
