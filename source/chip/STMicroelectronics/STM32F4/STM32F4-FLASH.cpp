/**
 * \file
 * \brief Implementation of FLASH-related functions for STM32F4
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-17
 */

#include "distortos/chip/STM32F4-FLASH.hpp"

#include "distortos/chip/STM32F4-FLASH-bits.h"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void configureInstructionPrefetch(const bool enable)
{
	FLASH_ACR_PRFTEN_bb = enable;
}

void disableDataCache()
{
	FLASH_ACR_DCEN_bb = 0;
}

void disableInstructionCache()
{
	FLASH_ACR_ICEN_bb = 0;
}

void enableDataCache()
{
	disableDataCache();
	FLASH_ACR_DCRST_bb = 1;	// reset data cache
	FLASH_ACR_DCRST_bb = 0;
	FLASH_ACR_DCEN_bb = 1;
}

void enableInstructionCache()
{
	disableInstructionCache();
	FLASH_ACR_ICRST_bb = 1;	// reset instruction cache
	FLASH_ACR_ICRST_bb = 0;
	FLASH_ACR_ICEN_bb = 1;
}

}	// namespace chip

}	// namespace distortos
