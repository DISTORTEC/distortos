/**
 * \file
 * \brief STM32 SPIv1's SpiMasterLowLevelInterruptBased test cases
 *
 * This test checks whether STM32 SPIv1's SpiMasterLowLevelInterruptBased performs all h/w operations properly and in
 * correct order.
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/SpiMasterLowLevelInterruptBased.hpp"
#include "distortos/chip/STM32-SPIv1.hpp"
#include "distortos/chip/STM32-SPIv1-SpiPeripheral.hpp"

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
constexpr uint16_t initialCr2 {};

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

	SECTION("Stopping stopped driver should fail with EBADF")
	{
		REQUIRE(spi.stop() == EBADF);
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
	distortos::chip::Stm32Spiv1Spiv2Mock stm32Spiv1Spiv2Mock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SpiMasterLowLevelInterruptBased spi {peripheralMock};

	{
		REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
		REQUIRE(spi.start() == 0);
	}

	const distortos::devices::SpiMode modes[]
	{
			distortos::devices::SpiMode::_0,
			distortos::devices::SpiMode::_1,
			distortos::devices::SpiMode::_2,
			distortos::devices::SpiMode::_3,
	};
	const uint32_t clockFrequencies[]
	{
			0xc059f6bf,
			0xe7170695,
			0x947e8c38,
			0x193d5a74,
	};
	const uint8_t wordLengths[]
	{
			0xc7,
			0x6d,
			0x8a,
			0x88,
	};
	const bool lsbFirsts[]
	{
			false,
			true,
	};
	const uint32_t realClockFrequencies[]
	{
			0x08d543bf,
			0x1b8aa8ff,
			0x3092c5a1,
			0xd29abd55,
	};
	for (auto mode : modes)
		for (auto clockFrequency : clockFrequencies)
			for (auto wordLength : wordLengths)
				for (auto lsbFirst : lsbFirsts)
					for (auto realClockFrequency : realClockFrequencies)
					{
						REQUIRE_CALL(stm32Spiv1Spiv2Mock,
								configureSpi(_, mode, clockFrequency, wordLength, lsbFirst))
								.LR_WITH(&_1 == &peripheralMock).IN_SEQUENCE(sequence).RETURN(realClockFrequency);
						REQUIRE(spi.configure(mode, clockFrequency, wordLength, lsbFirst,
								{}) == std::make_pair(0, realClockFrequency));
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
	distortos::chip::Stm32Spiv1Spiv2Mock stm32Spiv1Spiv2Mock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SpiMasterLowLevelInterruptBased spi {peripheralMock};

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

	SECTION("Testing 8-bit transfers")
	{
		REQUIRE_CALL(stm32Spiv1Spiv2Mock, configureSpi(_, distortos::devices::SpiMode{}, uint32_t{}, 8,
				bool{})).LR_WITH(&_1 == &peripheralMock).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(spi.configure({}, {}, 8, {}, dummyData).first == 0);

		SECTION("Testing 8-bit transfer of 1 item")
		{
			const std::array<uint8_t, 1> rxData {{0x9f}};
			std::array<uint8_t, rxData.size()> rxBuffer {};

			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeDr(dummyData)).IN_SEQUENCE(sequence);
			REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == 0);

			// starting another transfer when the previous one is ongoing should fail with EBUSY
			REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

			// trying to configure the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.configure({}, {}, 8, {}, dummyData).first == EBUSY);

			// trying to stop the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.stop() == EBUSY);

			REQUIRE_CALL(peripheralMock, readDr()).IN_SEQUENCE(sequence).RETURN(rxData[0]);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2 | SPI_CR2_RXNEIE);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(masterMock, transferCompleteEvent(sizeof(rxBuffer))).IN_SEQUENCE(sequence);
			spi.interruptHandler();

			REQUIRE(rxData == rxBuffer);
		}
		SECTION("Testing 8-bit transfer of several items")
		{
			const std::array<uint8_t, 5> txBuffer {{0xe5, 0x74, 0xb0, 0xf7, 0x95}};
			const std::array<uint8_t, txBuffer.size()> rxData {{0x20, 0xf7, 0xdb, 0xbf, 0xe7}};
			std::array<uint8_t, rxData.size()> rxBuffer {};

			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeDr(txBuffer.front())).IN_SEQUENCE(sequence);
			REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == 0);

			// starting another transfer when the previous one is ongoing should fail with EBUSY
			REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

			// trying to configure the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.configure({}, {}, 8, {}, dummyData).first == EBUSY);

			// trying to stop the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.stop() == EBUSY);

			for (size_t i {}; i < txBuffer.size(); ++i)
			{
				std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

				REQUIRE_CALL(peripheralMock, readDr()).IN_SEQUENCE(sequence).RETURN(rxData[i]);
				if (i != txBuffer.size() - 1)
					expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
							writeDr(txBuffer[i + 1])).IN_SEQUENCE(sequence));
				else
				{
					expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
							readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2 | SPI_CR2_RXNEIE));
					expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
							writeCr2(initialCr2)).IN_SEQUENCE(sequence));
					expectations.emplace_back(NAMED_REQUIRE_CALL(masterMock,
							transferCompleteEvent(sizeof(rxBuffer))).IN_SEQUENCE(sequence));
				}
				spi.interruptHandler();
			}

			REQUIRE(rxData == rxBuffer);
		}
	}
	SECTION("Testing 16-bit transfers")
	{
		REQUIRE_CALL(stm32Spiv1Spiv2Mock, configureSpi(_, distortos::devices::SpiMode{}, uint32_t{}, 16,
				bool{})).LR_WITH(&_1 == &peripheralMock).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(spi.configure({}, {}, 16, {}, dummyData).first == 0);

		SECTION("Starting transfer with odd length when word length is 16 should fail with EINVAL")
		{
			REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 1) == EINVAL);
		}
		SECTION("Testing 16-bit transfer of 1 item")
		{
			const std::array<uint16_t, 1> rxData {{0xad74}};
			std::array<uint16_t, rxData.size()> rxBuffer {};

			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeDr(dummyData)).IN_SEQUENCE(sequence);
			REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == 0);

			// starting another transfer when the previous one is ongoing should fail with EBUSY
			REQUIRE(spi.startTransfer(masterMock, nullptr, rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

			// trying to configure the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.configure({}, {}, 16, {}, dummyData).first == EBUSY);

			// trying to stop the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.stop() == EBUSY);

			REQUIRE_CALL(peripheralMock, readDr()).IN_SEQUENCE(sequence).RETURN(rxData[0]);
			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2 | SPI_CR2_RXNEIE);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(masterMock, transferCompleteEvent(sizeof(rxData))).IN_SEQUENCE(sequence);
			spi.interruptHandler();

			REQUIRE(rxData == rxBuffer);
		}
		SECTION("Testing 16-bit transfer of several items")
		{
			const std::array<uint16_t, 5> txBuffer {{0xf2a0, 0x74ba, 0x5b22, 0xa49c, 0xa205}};
			const std::array<uint16_t, txBuffer.size()> rxData {{0x4939, 0x376a, 0x29fa, 0x6c4e, 0x7a87}};
			std::array<uint16_t, rxData.size()> rxBuffer {};

			REQUIRE_CALL(peripheralMock, readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2);
			REQUIRE_CALL(peripheralMock, writeCr2(initialCr2 | SPI_CR2_RXNEIE)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeDr(txBuffer.front())).IN_SEQUENCE(sequence);
			REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == 0);

			// starting another transfer when the previous one is ongoing should fail with EBUSY
			REQUIRE(spi.startTransfer(masterMock, txBuffer.begin(), rxBuffer.begin(), sizeof(rxBuffer)) == EBUSY);

			// trying to configure the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.configure({}, {}, 16, {}, dummyData).first == EBUSY);

			// trying to stop the driver when a transfer is ongoing should fail with EBUSY
			REQUIRE(spi.stop() == EBUSY);

			for (size_t i {}; i < txBuffer.size(); ++i)
			{
				std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

				REQUIRE_CALL(peripheralMock, readDr()).IN_SEQUENCE(sequence).RETURN(rxData[i]);
				if (i != txBuffer.size() - 1)
					expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
							writeDr(txBuffer[i + 1])).IN_SEQUENCE(sequence));
				else
				{
					expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
							readCr2()).IN_SEQUENCE(sequence).RETURN(initialCr2 | SPI_CR2_RXNEIE));
					expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
							writeCr2(initialCr2)).IN_SEQUENCE(sequence));
					expectations.emplace_back(NAMED_REQUIRE_CALL(masterMock,
							transferCompleteEvent(sizeof(rxBuffer))).IN_SEQUENCE(sequence));
				}
				spi.interruptHandler();
			}

			REQUIRE(rxData == rxBuffer);
		}
	}

	{
		REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
		REQUIRE(spi.stop() == 0);
	}
}
