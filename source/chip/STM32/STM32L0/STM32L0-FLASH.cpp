/**
 * \file
 * \brief Implementation of FLASH-related functions for STM32L0
 *
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32L0-FLASH.hpp"

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
	return 0;
}

void configurePrefetchBuffer(const bool enable)
{
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_PRFTEN) | enable << FLASH_ACR_PRFTEN_Pos;
}

}	// namespace chip

}	// namespace distortos
