/**
 * \file
 * \brief Implementation of FLASH-related functions for STM32F4
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F4-FLASH.hpp"

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

int configureFlashLatency(const uint8_t latency)
{
	if (latency > maxFlashLatency)
		return EINVAL;

	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | latency << FLASH_ACR_LATENCY_Pos;
	return 0;
}

void configureInstructionPrefetch(const bool enable)
{
	STM32_BITBAND_FLASH(ACR, PRFTEN) = enable;
}

void disableDataCache()
{
	STM32_BITBAND_FLASH(ACR, DCEN) = 0;
}

void disableInstructionCache()
{
	STM32_BITBAND_FLASH(ACR, ICEN) = 0;
}

void enableDataCache()
{
	disableDataCache();
	STM32_BITBAND_FLASH(ACR, DCRST) = 1;	// reset data cache
	STM32_BITBAND_FLASH(ACR, DCRST) = 0;
	STM32_BITBAND_FLASH(ACR, DCEN) = 1;
}

void enableInstructionCache()
{
	disableInstructionCache();
	STM32_BITBAND_FLASH(ACR, ICRST) = 1;	// reset instruction cache
	STM32_BITBAND_FLASH(ACR, ICRST) = 0;
	STM32_BITBAND_FLASH(ACR, ICEN) = 1;
}

}	// namespace chip

}	// namespace distortos
