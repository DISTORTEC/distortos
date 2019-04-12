/**
 * \file
 * \brief Header for SPIv2 functions for STM32
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_HPP_

#include "distortos/devices/communication/SpiMode.hpp"

namespace distortos
{

namespace chip
{

class SpiPeripheral;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for SPI word length
constexpr static uint8_t minSpiWordLength {4};

/// maximum allowed value for SPI word length
constexpr static uint8_t maxSpiWordLength {16};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures parameters of raw SPI peripheral.
 *
 * \pre \a clockFrequency is greater than or equal to `spiPeripheral.getPeripheralFrequency() / 256`.
 * \pre \a wordLength is valid.
 *
 * \param [in] spiPeripheral is a reference to raw SPI peripheral
 * \param [in] mode is the desired SPI mode
 * \param [in] clockFrequency is the desired clock frequency, Hz
 * \param [in] wordLength selects word length, bits, [4; 16] or [minSpiWordLength; maxSpiWordLength]
 * \param [in] lsbFirst selects whether MSB (false) or LSB (true) is transmitted first
 */

void configureSpi(const SpiPeripheral& spiPeripheral, devices::SpiMode mode, uint32_t clockFrequency,
		uint8_t wordLength, bool lsbFirst);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SPIV2_INCLUDE_DISTORTOS_CHIP_STM32_SPIV2_HPP_
