/**
 * \file
 * \brief chip::peripheralsLowLevelInitialization() implementation for STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/peripheralsLowLevelInitialization.hpp"

#include "distortos/distortosConfiguration.h"

#ifdef CONFIG_CHIP_STM32_SPIV1
#include "SPIv1/STM32-SPIv1-spiLowLevelInitialization.hpp"
#endif	// def CONFIG_CHIP_STM32_SPIV1

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void peripheralsLowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32_SPIV1
	spiLowLevelInitialization();
#endif	// CONFIG_CHIP_STM32_SPIV1
}

}	// namespace chip

}	// namespace distortos
