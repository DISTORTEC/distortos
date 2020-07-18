/**
 * \file
 * \brief STM32 SPIv1's functions' test cases
 *
 * This test checks whether STM32 SPIv1's functions perform all h/w operations properly and in correct order.
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/STM32-SPIv1.hpp"
#include "distortos/chip/STM32-SPIv1-SpiPeripheral.hpp"

#include "estd/log2u.hpp"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr uint16_t initialCr1 {SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR};
constexpr uint32_t peripheralFrequency {256000000};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing distortos::chip::configureSpi()", "[configureSpi]")
{
	distortos::chip::SpiPeripheral peripheralMock {};
	trompeloeil::sequence sequence {};

	ALLOW_CALL(peripheralMock, getPeripheralFrequency()).RETURN(peripheralFrequency);

	const distortos::devices::SpiMode modes[]
	{
			distortos::devices::SpiMode::_0,
			distortos::devices::SpiMode::_1,
			distortos::devices::SpiMode::_2,
			distortos::devices::SpiMode::_3,
	};
	for (auto mode : modes)
	{
		DYNAMIC_SECTION("Trying to use mode " << static_cast<int>(mode) << " should succeed")
		{
			const auto cpol =
					mode == distortos::devices::SpiMode::cpol1cpha0 || mode == distortos::devices::SpiMode::cpol1cpha1;
			const auto cpha =
					mode == distortos::devices::SpiMode::cpol0cpha1 || mode == distortos::devices::SpiMode::cpol1cpha1;
			const auto newCr1 = (initialCr1 & ~(SPI_CR1_CPOL | SPI_CR1_CPHA)) |
					cpol << SPI_CR1_CPOL_Pos |
					cpha << SPI_CR1_CPHA_Pos;
			const auto oldCr1 = newCr1 ^ (SPI_CR1_CPOL | SPI_CR1_CPHA);

			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(oldCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(oldCr1 & ~SPI_CR1_SPE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeCr1(newCr1)).IN_SEQUENCE(sequence);
			distortos::chip::configureSpi(peripheralMock, mode, peripheralFrequency / 256, 8, {});
		}
	}

	const uint32_t clockFrequencies[]
	{
			peripheralFrequency + 1,
			peripheralFrequency,
			peripheralFrequency - 1,

			peripheralFrequency / 2 + 1,
			peripheralFrequency / 2,
			peripheralFrequency / 2 - 1,

			peripheralFrequency / 4 + 1,
			peripheralFrequency / 4,
			peripheralFrequency / 4 - 1,

			peripheralFrequency / 8 + 1,
			peripheralFrequency / 8,
			peripheralFrequency / 8 - 1,

			peripheralFrequency / 16 + 1,
			peripheralFrequency / 16,
			peripheralFrequency / 16 - 1,

			peripheralFrequency / 32 + 1,
			peripheralFrequency / 32,
			peripheralFrequency / 32 - 1,

			peripheralFrequency / 64 + 1,
			peripheralFrequency / 64,
			peripheralFrequency / 64 - 1,

			peripheralFrequency / 128 + 1,
			peripheralFrequency / 128,
			peripheralFrequency / 128 - 1,

			peripheralFrequency / 256 + 1,
			peripheralFrequency / 256,
	};
	for (auto clockFrequency : clockFrequencies)
	{
		DYNAMIC_SECTION("Trying to use clock frequency " << clockFrequency << " should succeed")
		{
			const auto divider = (peripheralFrequency + clockFrequency - 1) / clockFrequency;
			const auto br = divider <= 2 ? 0 : estd::log2u(divider - 1);
			const auto newCr1 = (initialCr1 & ~SPI_CR1_BR) | br << SPI_CR1_BR_Pos;
			const auto oldCr1 = newCr1 ^ SPI_CR1_BR;

			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(oldCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(oldCr1 & ~SPI_CR1_SPE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeCr1(newCr1)).IN_SEQUENCE(sequence);
			distortos::chip::configureSpi(peripheralMock, {}, clockFrequency, 8, {});
		}
	}

	SECTION("Trying to change word length to valid value should succeed")
	{
		const uint8_t wordLengths[]
		{
				16,
				8,
		};
		for (auto wordLength : wordLengths)
		{
			const auto newCr1 = (initialCr1 & ~SPI_CR1_DFF) | (wordLength == 16) << SPI_CR1_DFF_Pos;
			const auto oldCr1 = newCr1 ^ SPI_CR1_DFF;

			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(oldCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(oldCr1 & ~SPI_CR1_SPE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeCr1(newCr1)).IN_SEQUENCE(sequence);
			distortos::chip::configureSpi(peripheralMock, {}, peripheralFrequency / 256, wordLength, {});
		}
	}

	const bool lsbFirsts[]
	{
			false,
			true,
	};
	for (auto lsbFirst : lsbFirsts)
	{
		DYNAMIC_SECTION("Trying to use LSB first " << lsbFirst << " should succeed")
		{
			const auto newCr1 = (initialCr1 & ~SPI_CR1_LSBFIRST) | lsbFirst << SPI_CR1_LSBFIRST_Pos;
			const auto oldCr1 = newCr1 ^ SPI_CR1_LSBFIRST;

			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(oldCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(oldCr1 & ~SPI_CR1_SPE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeCr1(newCr1)).IN_SEQUENCE(sequence);
			distortos::chip::configureSpi(peripheralMock, {}, peripheralFrequency / 256, 8, lsbFirst);
		}
	}
}
