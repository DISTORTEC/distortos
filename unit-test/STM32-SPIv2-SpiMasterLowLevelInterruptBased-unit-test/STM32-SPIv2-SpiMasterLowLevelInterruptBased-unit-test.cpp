/**
 * \file
 * \brief STM32 SPIv2's SpiMasterLowLevelInterruptBased test cases
 *
 * This test checks whether STM32 SPIv2's SpiMasterLowLevelInterruptBased performs all h/w operations properly and in
 * correct order.
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/SpiMasterLowLevelInterruptBased.hpp"
#include "distortos/chip/STM32-SPIv2-SpiPeripheral.hpp"

#include "distortos/devices/communication/SpiMasterBase.hpp"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class SpiMaster : public distortos::devices::SpiMasterBase
{
public:

	MAKE_MOCK1(transferCompleteEvent, void(size_t));
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr uint16_t initialCr1 {SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR};
constexpr uint16_t initialCr2 {SPI_CR2_FRXTH | (8 - 1) << SPI_CR2_DS_Pos};
constexpr uint32_t peripheralFrequency {256000000};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing start() & stop() interactions", "[start/stop]")
{
	SpiMaster masterMock {};
	distortos::chip::SpiPeripheral peripheralMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::chip::SpiMasterLowLevelInterruptBased spi {peripheralMock};

	ALLOW_CALL(peripheralMock, getPeripheralFrequency()).RETURN(peripheralFrequency);

	SECTION("Stopping stopped driver should fail with EBADF")
	{
		REQUIRE(spi.stop() == EBADF);
	}
	SECTION("Configuring stopped driver should fail with EBADF")
	{
		REQUIRE(spi.configure({}, {}, 8, {}, {}).first == EBADF);
	}
	SECTION("Starting transfer with stopped driver should fail with EBADF")
	{
		REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 1) == EBADF);
	}
	SECTION("Starting stopped driver should succeed")
	{
		REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
		REQUIRE(spi.start() == 0);

		SECTION("Starting started driver should fail with EBADF")
		{
			REQUIRE(spi.start() == EBADF);
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence));
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence));
		}
		SECTION("Stopping started driver should succeed")
		{
			REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
			REQUIRE(spi.stop() == 0);
		}
	}
}

TEST_CASE("Testing configure()", "[configure]")
{
	distortos::chip::SpiPeripheral peripheralMock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SpiMasterLowLevelInterruptBased spi {peripheralMock};

	ALLOW_CALL(peripheralMock, getPeripheralFrequency()).RETURN(peripheralFrequency);

	{
		REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
		REQUIRE(spi.start() == 0);
	}

	SECTION("Trying to use clock frequency lower than `peripheral frequency / 256` should fail with EINVAL")
	{
		REQUIRE(spi.configure({}, peripheralFrequency / 256 - 1, 8, {}, {}).first == EINVAL);
	}

	for (uint8_t wordLength {}; wordLength <= 32; ++wordLength)
	{
		if (wordLength >= distortos::chip::SpiMasterLowLevelInterruptBased::minWordLength &&
				wordLength <= distortos::chip::SpiMasterLowLevelInterruptBased::maxWordLength)
			continue;

		DYNAMIC_SECTION("Trying to use word length " << wordLength << " should fail with EINVAL")
		{
			REQUIRE(spi.configure({}, {}, wordLength, {}, {}).first == EINVAL);
		}
	}

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
			REQUIRE_CALL(peripheralMock, writeCr1(newCr1)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
			REQUIRE(spi.configure(mode, peripheralFrequency / 256, 8, {}, {}).first == 0);
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
			const auto br = divider <= 2 ? 0 : 31 - __builtin_clz(divider - 1);
			const auto newCr1 = (initialCr1 & ~SPI_CR1_BR) | br << SPI_CR1_BR_Pos;
			const auto oldCr1 = newCr1 ^ SPI_CR1_BR;
			const auto realClockFrequency = peripheralFrequency / (1 << (br + 1));

			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(oldCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(newCr1)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
			REQUIRE(spi.configure({}, clockFrequency, 8, {}, {}) == std::make_pair(0, realClockFrequency));
		}
	}

	SECTION("Trying to change word length to valid value should succeed")
	{
		for (auto wordLength {distortos::chip::SpiMasterLowLevelInterruptBased::minWordLength};
				wordLength <= distortos::chip::SpiMasterLowLevelInterruptBased::minWordLength;
				++wordLength)
		{
			const auto newCr2 = (initialCr2 & ~(SPI_CR2_FRXTH | SPI_CR2_DS)) |
					(wordLength <= 8) << SPI_CR2_FRXTH_Pos |
					(wordLength - 1) << SPI_CR2_DS_Pos;
			const auto oldCr2 = newCr2 ^ (SPI_CR2_FRXTH | SPI_CR2_DS);

			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(initialCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(oldCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(newCr2)).IN_SEQUENCE(sequence);
			REQUIRE(spi.configure({}, peripheralFrequency / 256, wordLength, {}, {}).first == 0);
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
			REQUIRE_CALL(peripheralMock, writeCr1(newCr1)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
			REQUIRE(spi.configure({}, peripheralFrequency / 256, 8, lsbFirst, {}).first == 0);
		}
	}

	{
		REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
		REQUIRE(spi.stop() == 0);
	}
}

TEST_CASE("Testing startTransfer()", "[startTransfer]")
{
	SpiMaster masterMock {};
	distortos::chip::SpiPeripheral peripheralMock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SpiMasterLowLevelInterruptBased spi {peripheralMock};

	ALLOW_CALL(peripheralMock, getPeripheralFrequency()).RETURN(peripheralFrequency);

	{
		REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
		REQUIRE(spi.start() == 0);
	}

	SECTION("Starting transfer with zero length should fail with EINVAL")
	{
		REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 0) == EINVAL);
	}

	constexpr uint16_t dummyData {0xd515};

	for (uint8_t wordLength {distortos::chip::SpiMasterLowLevelInterruptBased::minWordLength}; wordLength <= 8;
			++wordLength)
	{
		const auto cr2 = (initialCr2 & ~(SPI_CR2_FRXTH | SPI_CR2_DS)) |
				(wordLength <= 8) << SPI_CR2_FRXTH_Pos |
				(wordLength - 1) << SPI_CR2_DS_Pos;

		DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfers")
		{
			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(initialCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(cr2)).IN_SEQUENCE(sequence);
			REQUIRE(spi.configure({}, peripheralFrequency / 256, wordLength, {}, dummyData).first == 0);

			DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfer of 1 item")
			{
				const std::array<uint8_t, 1> rxData {{0x9f}};
				std::array<uint8_t, rxData.size()> rxBuffer {};

				REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(cr2);
				REQUIRE_CALL(peripheralMock, writeCr2(cr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(peripheralMock, writeDr(wordLength, dummyData)).IN_SEQUENCE(sequence);
				REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == 0);

				// starting another transfer when the previous one is ongoing should fail with EBUSY
				REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

				// trying to configure the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.configure({}, peripheralFrequency / 256, wordLength, {}, dummyData).first == EBUSY);

				// trying to stop the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.stop() == EBUSY);

				REQUIRE_CALL(peripheralMock, readDr(wordLength)).IN_SEQUENCE(sequence).RETURN(rxData[0]);
				REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(cr2 | SPI_CR2_RXNEIE);
				REQUIRE_CALL(peripheralMock, writeCr2(cr2)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(masterMock, transferCompleteEvent(sizeof(rxBuffer))).IN_SEQUENCE(sequence);
				spi.interruptHandler();

				REQUIRE(rxData == rxBuffer);
			}
			DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfer of several items")
			{
				const std::array<uint8_t, 5> txBuffer {{0xe5, 0x74, 0xb0, 0xf7, 0x95}};
				const std::array<uint8_t, txBuffer.size()> rxData {{0x20, 0xf7, 0xdb, 0xbf, 0xe7}};
				std::array<uint8_t, rxData.size()> rxBuffer {};

				REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(cr2);
				REQUIRE_CALL(peripheralMock, writeCr2(cr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(peripheralMock, writeDr(wordLength, txBuffer.front())).IN_SEQUENCE(sequence);
				REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == 0);

				// starting another transfer when the previous one is ongoing should fail with EBUSY
				REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

				// trying to configure the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.configure({}, peripheralFrequency / 256, wordLength, {}, dummyData).first == EBUSY);

				// trying to stop the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.stop() == EBUSY);

				for (size_t i {}; i < txBuffer.size(); ++i)
				{
					std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

					REQUIRE_CALL(peripheralMock, readDr(wordLength)).IN_SEQUENCE(sequence).RETURN(rxData[i]);
					if (i != txBuffer.size() - 1)
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								writeDr(wordLength, txBuffer[i + 1])).IN_SEQUENCE(sequence));
					else
					{
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readCr2()).IN_SEQUENCE(sequence).RETURN(cr2 | SPI_CR2_RXNEIE));
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								writeCr2(cr2)).IN_SEQUENCE(sequence));
						expectations.emplace_back(NAMED_REQUIRE_CALL(masterMock,
								transferCompleteEvent(sizeof(rxBuffer))).IN_SEQUENCE(sequence));
					}
					spi.interruptHandler();
				}

				REQUIRE(rxData == rxBuffer);
			}
		}
	}
	for (uint8_t wordLength {9}; wordLength <= distortos::chip::SpiMasterLowLevelInterruptBased::maxWordLength;
			++wordLength)
	{
		const auto cr2 = (initialCr2 & ~(SPI_CR2_FRXTH | SPI_CR2_DS)) |
				(wordLength <= 8) << SPI_CR2_FRXTH_Pos |
				(wordLength - 1) << SPI_CR2_DS_Pos;

		DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfers")
		{
			REQUIRE_CALL(peripheralMock, readCr1()).IN_SEQUENCE(sequence).RETURN(initialCr1);
			REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(cr2)).IN_SEQUENCE(sequence);
			REQUIRE(spi.configure({}, peripheralFrequency / 256, wordLength, {}, dummyData).first == 0);

			DYNAMIC_SECTION("Starting transfer with odd length when word length is " << static_cast<int>(wordLength) <<
					" bits should fail with EINVAL")
			{
				REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 1) == EINVAL);
			}
			DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfer of 1 item")
			{
				const std::array<uint16_t, 1> rxData {{0xad74}};
				std::array<uint16_t, rxData.size()> rxBuffer {};

				REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(cr2);
				REQUIRE_CALL(peripheralMock, writeCr2(cr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(peripheralMock, writeDr(wordLength, dummyData)).IN_SEQUENCE(sequence);
				REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == 0);

				// starting another transfer when the previous one is ongoing should fail with EBUSY
				REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

				// trying to configure the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.configure({}, peripheralFrequency / 256, wordLength, {}, dummyData).first == EBUSY);

				// trying to stop the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.stop() == EBUSY);

				REQUIRE_CALL(peripheralMock, readDr(wordLength)).IN_SEQUENCE(sequence).RETURN(rxData[0]);
				REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(cr2 | SPI_CR2_RXNEIE);
				REQUIRE_CALL(peripheralMock, writeCr2(cr2)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(masterMock, transferCompleteEvent(sizeof(rxData))).IN_SEQUENCE(sequence);
				spi.interruptHandler();

				REQUIRE(rxData == rxBuffer);
			}
			DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfer of several items")
			{
				const std::array<uint16_t, 5> txBuffer {{0xf2a0, 0x74ba, 0x5b22, 0xa49c, 0xa205}};
				const std::array<uint16_t, txBuffer.size()> rxData {{0x4939, 0x376a, 0x29fa, 0x6c4e, 0x7a87}};
				std::array<uint16_t, rxData.size()> rxBuffer {};

				REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(cr2);
				REQUIRE_CALL(peripheralMock, writeCr2(cr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
				REQUIRE_CALL(peripheralMock, writeDr(wordLength, txBuffer.front())).IN_SEQUENCE(sequence);
				REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == 0);

				// starting another transfer when the previous one is ongoing should fail with EBUSY
				REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

				// trying to configure the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.configure({}, peripheralFrequency / 256, wordLength, {}, dummyData).first == EBUSY);

				// trying to stop the driver when a transfer is ongoing should fail with EBUSY
				REQUIRE(spi.stop() == EBUSY);

				for (size_t i {}; i < txBuffer.size(); ++i)
				{
					std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

					REQUIRE_CALL(peripheralMock, readDr(wordLength)).IN_SEQUENCE(sequence).RETURN(rxData[i]);
					if (i != txBuffer.size() - 1)
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								writeDr(wordLength, txBuffer[i + 1])).IN_SEQUENCE(sequence));
					else
					{
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readCr2()).IN_SEQUENCE(sequence).RETURN(cr2 | SPI_CR2_RXNEIE));
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								writeCr2(cr2)).IN_SEQUENCE(sequence));
						expectations.emplace_back(NAMED_REQUIRE_CALL(masterMock,
								transferCompleteEvent(sizeof(rxBuffer))).IN_SEQUENCE(sequence));
					}
					spi.interruptHandler();
				}

				REQUIRE(rxData == rxBuffer);
			}
		}
	}

	{
		REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
		REQUIRE(spi.stop() == 0);
	}
}
