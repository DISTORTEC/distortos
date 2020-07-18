/**
 * \file
 * \brief Implementation of FLASH-related functions for STM32F7
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32F7-FLASH.hpp"

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

void configureInstructionPrefetch(const bool enable)
{
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_PRFTEN) | enable << FLASH_ACR_PRFTEN_Pos;
}

void disableArtAccelerator()
{
	FLASH->ACR &= ~FLASH_ACR_ARTEN;
}

void enableArtAccelerator()
{
	disableArtAccelerator();
	FLASH->ACR |= FLASH_ACR_ARTRST;	// reset ART accelerator
	FLASH->ACR &= ~FLASH_ACR_ARTRST;
	FLASH->ACR |= FLASH_ACR_ARTEN;
}

}	// namespace chip

}	// namespace distortos
