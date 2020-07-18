/**
 * \file
 * \brief Header for FLASH-related functions for STM32F7
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_STM32F7_FLASH_HPP_
#define SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_STM32F7_FLASH_HPP_

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// maximum allowed value of flash latency
constexpr uint8_t maxFlashLatency {15};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures flash latency.
 *
 * \param [in] latency is the new flash latency, [0; 15] or [0; maxFlashLatency]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a latency value is invalid;
 */

int configureFlashLatency(uint8_t latency);

/**
 * \brief Configures instruction prefetch.
 *
 * \param [in] enable selects whether instruction prefetch will be disabled (false) or enabled (true)
 */

void configureInstructionPrefetch(bool enable);

/**
 * \brief Disables ART accelerator.
 */

void disableArtAccelerator();

/**
 * \brief Enables ART accelerator.
 *
 * The accelerator is first disabled, then reset and finally enabled.
 */

void enableArtAccelerator();

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_STM32F7_FLASH_HPP_
