/**
 * \file
 * \brief Implementation of RCC-related functions for STM32F1
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F1-RCC.hpp"

#include "distortos/chip/STM32F1-RCC-bits.h"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void enableHse(const bool bypass)
{
	RCC_CR_HSEBYP_bb = bypass;
	RCC_CR_HSEON_bb = 1;
	while (RCC_CR_HSERDY_bb == 0);	// wait until HSE oscillator is stable
}

void disableHse()
{
	RCC_CR_HSEON_bb = 0;
}

}	// namespace chip

}	// namespace distortos
