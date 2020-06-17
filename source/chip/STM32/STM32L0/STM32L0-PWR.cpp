/**
 * \file
 * \brief Implementation of PWR-related functions for STM32L0
 *
 * \author Copyright (C) 2017-2018 Cezary Gapinski cezary.gapinski@gmail.com
 * \author Copyright (C) 2020 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32L0-PWR.hpp"

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

	PWR->CR = (PWR->CR & ~PWR_CR_VOS) | ((voltageScale << PWR_CR_VOS_Pos) & PWR_CR_VOS);

	while ((PWR->CSR & PWR_CSR_VOSF) != 0);	// wait until VOSF bit is reset to 0

	return 0;
}

}	// namespace chip

}	// namespace distortos
