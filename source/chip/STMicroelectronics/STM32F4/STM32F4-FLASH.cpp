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

void disableDataCache()
{
	FLASH_ACR_DCEN_bb = 0;
}

}	// namespace chip

}	// namespace distortos
