/**
 * \file
 * \brief Header for FLASH-related functions for STM32F4
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-09-17
 */

#ifndef SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_HPP_
#define SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_HPP_

namespace distortos
{

namespace chip
{

/**
 * \brief Disables data cache.
 */

void disableDataCache();

/**
 * \brief Disables instruction cache.
 */

void disableInstructionCache();

/**
 * \brief Enables data cache.
 *
 * The cache is first disabled, then reset and finally enabled.
 */

void enableDataCache();

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STMICROELECTRONICS_STM32F4_INCLUDE_DISTORTOS_CHIP_STM32F4_FLASH_HPP_
