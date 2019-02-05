/**
 * \file
 * \brief Implementation of SPIv2 functions for STM32
 *
 * \author Copyright (C) 2016-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32-SPIv2.hpp"

#include "distortos/chip/STM32-SPIv2-SpiPeripheral.hpp"

#include "estd/log2u.hpp"

#include <cerrno>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

std::pair<int, uint32_t> configureSpi(const SpiPeripheral& spiPeripheral, const devices::SpiMode mode,
		const uint32_t clockFrequency, const uint8_t wordLength, const bool lsbFirst)
{
	if (wordLength < minSpiWordLength || wordLength > maxSpiWordLength)
		return {EINVAL, {}};

	const auto peripheralFrequency = spiPeripheral.getPeripheralFrequency();
	const auto divider = (peripheralFrequency + clockFrequency - 1) / clockFrequency;
	if (divider > 256)
		return {EINVAL, {}};

	const uint32_t br = divider <= 2 ? 0 : estd::log2u(divider - 1);
	const auto cr1 = spiPeripheral.readCr1();
	spiPeripheral.writeCr1((cr1 & ~(SPI_CR1_LSBFIRST | SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA)) |
			lsbFirst << SPI_CR1_LSBFIRST_Pos | br << SPI_CR1_BR_Pos |
			(mode == devices::SpiMode::cpol1cpha0 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPOL_Pos |
			(mode == devices::SpiMode::cpol0cpha1 || mode == devices::SpiMode::cpol1cpha1) << SPI_CR1_CPHA_Pos);
	const auto cr2 = spiPeripheral.readCr2();
	spiPeripheral.writeCr2((cr2 & ~(SPI_CR2_FRXTH | SPI_CR2_DS)) |
			(wordLength <= 8) << SPI_CR2_FRXTH_Pos |
			(wordLength - 1) << SPI_CR2_DS_Pos);

	return {{}, peripheralFrequency / (1 << (br + 1))};
}

}	// namespace chip

}	// namespace distortos
