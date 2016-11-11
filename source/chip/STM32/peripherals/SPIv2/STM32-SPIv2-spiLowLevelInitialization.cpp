/**
 * \file
 * \brief chip::spiLowLevelInitialization() definition for SPIv2 in STM32
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "STM32-SPIv2-spiLowLevelInitialization.hpp"

#include "distortos/chip/CMSIS-proxy.h"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void spiLowLevelInitialization()
{
#ifdef CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE
	NVIC_SetPriority(SPI1_IRQn, {});
	NVIC_EnableIRQ(SPI1_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI1_ENABLE
#ifdef CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE
	NVIC_SetPriority(SPI2_IRQn, {});
	NVIC_EnableIRQ(SPI2_IRQn);
#endif	// def CONFIG_CHIP_STM32_SPIV2_SPI2_ENABLE
}

}	// namespace chip

}	// namespace distortos
