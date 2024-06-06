/**
 * \file
 * \brief Implementation of FLASH-related functions for STM32G0
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32G0-FLASH.hpp"

#include "distortos/chip/CMSIS-proxy.h"

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
	// wait for new latency value to become effective
	while ((FLASH->ACR & FLASH_ACR_LATENCY) != latency << FLASH_ACR_LATENCY_Pos);
	return 0;
}

void configureInstructionPrefetch(const bool enable)
{
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_PRFTEN) | enable << FLASH_ACR_PRFTEN_Pos;
}

void disableInstructionCache()
{
	FLASH->ACR &= ~FLASH_ACR_ICEN;
}

void enableInstructionCache()
{
	disableInstructionCache();
	FLASH->ACR |= FLASH_ACR_ICRST;	// reset instruction cache
	FLASH->ACR &= ~FLASH_ACR_ICRST;
	FLASH->ACR |= FLASH_ACR_ICEN;
}

}	// namespace chip

}	// namespace distortos
