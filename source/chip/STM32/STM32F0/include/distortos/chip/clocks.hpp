/**
 * \file
 * \brief Definitions of clocks for STM32F0
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
#define SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// AHB frequency, Hz
constexpr uint32_t ahbFrequency {CONFIG_CHIP_STM32F0_RCC_AHB_FREQUENCY};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_CLOCKS_HPP_
