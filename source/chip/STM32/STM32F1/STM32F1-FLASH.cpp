/**
 * \file
 * \brief Implementation of FLASH-related functions for STM32F1
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F1-FLASH.hpp"

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

void configureFlashHalfCycleAccess(const bool enable)
{
	STM32_BITBAND_FLASH(ACR, HLFCYA) = enable;
}

#ifndef DISTORTOS_CHIP_STM32F100

int configureFlashLatency(const uint8_t latency)
{
	if (latency > maxFlashLatency)
		return EINVAL;

	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | latency << FLASH_ACR_LATENCY_Pos;
	return 0;
}

void configurePrefetchBuffer(const bool enable)
{
	STM32_BITBAND_FLASH(ACR, PRFTBE) = enable;
}

#endif	// !def DISTORTOS_CHIP_STM32F100

}	// namespace chip

}	// namespace distortos
