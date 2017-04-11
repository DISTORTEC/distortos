/**
 * \file
 * \brief chip::lowLevelInitialization() implementation for STM32L0
 *
 * \author Copyright (C) 2017 Cezary Gapinski cezary.gapinski@gmail.com
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/lowLevelInitialization.hpp"

#include "distortos/chip/STM32L0-FLASH.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void lowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32L0_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(true);
#else	// !def CONFIG_CHIP_STM32L0_FLASH_PREFETCH_ENABLE
	configurePrefetchBuffer(false);
#endif	// !def CONFIG_CHIP_STM32L0_FLASH_PREFETCH_ENABLE
}

}	// namespace chip

}	// namespace distortos
