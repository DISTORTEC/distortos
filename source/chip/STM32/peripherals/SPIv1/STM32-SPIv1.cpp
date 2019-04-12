/**
 * \file
 * \brief Implementation of SPIv1 functions for STM32
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32-SPIv1.hpp"

#include "distortos/chip/STM32-SPIv1-SpiPeripheral.hpp"

#include "distortos/assert.h"

#include "estd/log2u.hpp"

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void configureSpi(const SpiPeripheral& spiPeripheral, const devices::SpiMode mode, const uint32_t clockFrequency,
		const uint8_t wordLength, const bool lsbFirst)
{
	assert(wordLength == 8 || wordLength == 16);

	const auto peripheralFrequency = spiPeripheral.getPeripheralFrequency();
	const auto divider = (peripheralFrequency + clockFrequency - 1) / clockFrequency;
	assert(divider <= 256);

	const uint32_t br = divider <= 2 ? 0 : estd::log2u(divider - 1);

	auto cr1 = spiPeripheral.readCr1();

	// value of DFF bit (which determines word length) must be changed only when SPI peripheral is disabled
	cr1 &= ~SPI_CR1_SPE;
	spiPeripheral.writeCr1(cr1);

	spiPeripheral.writeCr1((cr1 & ~(SPI_CR1_DFF | SPI_CR1_LSBFIRST | SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA)) |
			(wordLength == 16) << SPI_CR1_DFF_Pos |
			lsbFirst << SPI_CR1_LSBFIRST_Pos |
			SPI_CR1_SPE |
			br << SPI_CR1_BR_Pos |
			(mode == devices::SpiMode::cpol1cpha0 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPOL_Pos |
			(mode == devices::SpiMode::cpol0cpha1 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPHA_Pos);
}

}	// namespace chip

}	// namespace distortos
