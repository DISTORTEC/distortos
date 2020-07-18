/**
 * \file
 * \brief Header for FLASH-related functions for STM32F0
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_FLASH_HPP_
#define SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_FLASH_HPP_

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// maximum allowed value of flash latency
constexpr uint8_t maxFlashLatency {1};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures flash latency.
 *
 * \param [in] latency is the new flash latency, [0; maxFlashLatency]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a latency value is invalid;
 */

int configureFlashLatency(uint8_t latency);

/**
 * \brief Configures prefetch buffer.
 *
 * \param [in] enable selects whether prefetch buffer will be disabled (false) or enabled (true)
 */

void configurePrefetchBuffer(bool enable);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F0_INCLUDE_DISTORTOS_CHIP_STM32F0_FLASH_HPP_
