/**
 * \file
 * \brief Header for RCC-related functions for STM32F1
 *
 * This file covers devices as described in following places:
 * - RM0008 reference manual (STM32F101, STM32F102, STM32F103, STM32F105 and STM32F107), Revision 16, 2015-11-26
 * - RM0041 reference manual (STM32F100), Revision 4, 2011-07-21
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_HPP_
#define SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_HPP_

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Disables HSE clock.
 *
 * \warning Before changing configuration of HSE clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void disableHse();

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F1_INCLUDE_DISTORTOS_CHIP_STM32F1_RCC_HPP_
