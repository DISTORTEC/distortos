/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32L4
 *
 * \author Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/STM32L4-FLASH.hpp"

#include "distortos/distortosConfiguration.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32L4_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(true);
#else	// !def CONFIG_CHIP_STM32L4_FLASH_PREFETCH_ENABLE
	configureInstructionPrefetch(false);
#endif	// !def CONFIG_CHIP_STM32L4_FLASH_PREFETCH_ENABLE

#ifdef CONFIG_CHIP_STM32L4_FLASH_DATA_CACHE_ENABLE
	enableDataCache();
#else	// !def CONFIG_CHIP_STM32L4_FLASH_DATA_CACHE_ENABLE
	disableDataCache();
#endif	// !def CONFIG_CHIP_STM32L4_FLASH_DATA_CACHE_ENABLE

#ifdef CONFIG_CHIP_STM32L4_FLASH_INSTRUCTION_CACHE_ENABLE
	enableInstructionCache();
#else	// !def CONFIG_CHIP_STM32L4_FLASH_INSTRUCTION_CACHE_ENABLE
	disableInstructionCache();
#endif	// !def CONFIG_CHIP_STM32L4_FLASH_INSTRUCTION_CACHE_ENABLE
}

}	// namespace chip

}	// namespace distortos
