/**
 * \file
 * \brief STM32 SPIv2's SpiMasterLowLevelDmaBased test cases
 *
 * This test checks whether STM32 SPIv2's SpiMasterLowLevelDmaBased performs all h/w operations properly and in correct
 * order.
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/DmaChannel.hpp"
#include "distortos/chip/SpiMasterLowLevelDmaBased.hpp"
#include "distortos/chip/STM32-SPIv2.hpp"
#include "distortos/chip/STM32-SPIv2-SpiPeripheral.hpp"

#include "distortos/devices/communication/SpiMasterBase.hpp"

using trompeloeil::_;
using Flags = distortos::chip::DmaChannel::Flags;

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

constexpr uintptr_t drAddress {0xe0105554};
constexpr uint16_t initialCr1 {SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR};
constexpr uint16_t initialCr2 {SPI_CR2_FRXTH | (8 - 1) << SPI_CR2_DS_Pos | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN};
constexpr uint8_t rxDmaRequest {0xb9};
constexpr uint8_t txDmaRequest {0x0c};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing start() & stop() interactions", "[start/stop]")
{
	SpiMaster masterMock {};
	distortos::chip::SpiPeripheral peripheralMock {};
	distortos::chip::DmaChannel rxDmaChannelMock {};
	distortos::chip::DmaChannel txDmaChannelMock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SpiMasterLowLevelDmaBased spi {peripheralMock, rxDmaChannelMock, rxDmaRequest, txDmaChannelMock,
			txDmaRequest};

	SECTION("Starting stopped driver when RX DMA channel is busy should fail with EBUSY")
	{
		REQUIRE_CALL(rxDmaChannelMock, reserve(rxDmaRequest, _)).IN_SEQUENCE(sequence).RETURN(EBUSY);
		REQUIRE(spi.start() == EBUSY);
	}
	SECTION("Starting stopped driver when TX DMA channel is busy should fail with EBUSY")
	{
		REQUIRE_CALL(rxDmaChannelMock, reserve(rxDmaRequest, _)).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(txDmaChannelMock, reserve(txDmaRequest, _)).IN_SEQUENCE(sequence).RETURN(EBUSY);
		REQUIRE_CALL(rxDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE(spi.start() == EBUSY);
	}
	SECTION("Starting stopped driver should succeed")
	{
		REQUIRE_CALL(rxDmaChannelMock, reserve(rxDmaRequest, _)).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(txDmaChannelMock, reserve(txDmaRequest, _)).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
		REQUIRE(spi.start() == 0);

		// stopping started driver should succeed
		REQUIRE_CALL(rxDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(txDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
		spi.stop();
	}
}

TEST_CASE("Testing configure()", "[configure]")
{
	distortos::chip::SpiPeripheral peripheralMock {};
	distortos::chip::DmaChannel rxDmaChannelMock {};
	distortos::chip::DmaChannel txDmaChannelMock {};
	distortos::chip::Stm32Spiv1Spiv2Mock stm32Spiv1Spiv2Mock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SpiMasterLowLevelDmaBased spi {peripheralMock, rxDmaChannelMock, rxDmaRequest, txDmaChannelMock,
			txDmaRequest};

	{
		REQUIRE_CALL(rxDmaChannelMock, reserve(rxDmaRequest, _)).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(txDmaChannelMock, reserve(txDmaRequest, _)).IN_SEQUENCE(sequence).RETURN(0);
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
			0x5170fd22,
			0xe63e3a74,
			0x6c71e06c,
			0xd1feaee6,
	};
	const uint8_t wordLengths[]
	{
			0x23,
			0xb3,
			0xaa,
			0x27,
	};
	const bool lsbFirsts[]
	{
			false,
			true,
	};
	const uint32_t realClockFrequencies[]
	{
			0x80bbaed9,
			0x9f0c692e,
			0x080afd7a,
			0xa1671330,
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
						REQUIRE(spi.configure(mode, clockFrequency, wordLength, lsbFirst, {}) == realClockFrequency);
					}

	{
		REQUIRE_CALL(rxDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(txDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
		spi.stop();
	}
}

TEST_CASE("Testing startTransfer()", "[startTransfer]")
{
	SpiMaster masterMock {};
	distortos::chip::SpiPeripheral peripheralMock {};
	distortos::chip::DmaChannel rxDmaChannelMock {};
	distortos::chip::DmaChannel txDmaChannelMock {};
	distortos::chip::Stm32Spiv1Spiv2Mock stm32Spiv1Spiv2Mock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SpiMasterLowLevelDmaBased spi {peripheralMock, rxDmaChannelMock, rxDmaRequest, txDmaChannelMock,
			txDmaRequest};

	distortos::chip::DmaChannelFunctor* rxDmaChannelFunctor {};
	distortos::chip::DmaChannelFunctor* txDmaChannelFunctor {};

	{
		REQUIRE_CALL(rxDmaChannelMock,
				reserve(rxDmaRequest, _)).IN_SEQUENCE(sequence).LR_SIDE_EFFECT(rxDmaChannelFunctor = &_2).RETURN(0);
		REQUIRE_CALL(txDmaChannelMock,
				reserve(txDmaRequest, _)).IN_SEQUENCE(sequence).LR_SIDE_EFFECT(txDmaChannelFunctor = &_2).RETURN(0);
		REQUIRE_CALL(peripheralMock, writeCr1(initialCr1)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(initialCr2)).IN_SEQUENCE(sequence);
		REQUIRE(spi.start() == 0);
	}

	SECTION("Starting transfer with zero length should fail with EINVAL")
	{
		REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 0) == EINVAL);
	}

	for (auto wordLength {distortos::chip::minSpiWordLength}; wordLength <= distortos::chip::maxSpiWordLength;
			++wordLength)
		DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfers")
		{
			constexpr uint16_t dummyData {0xaf5a};
			REQUIRE_CALL(stm32Spiv1Spiv2Mock, configureSpi(_, distortos::devices::SpiMode{}, uint32_t{}, wordLength,
					bool{})).LR_WITH(&_1 == &peripheralMock).IN_SEQUENCE(sequence).RETURN(0);
			spi.configure({}, {}, wordLength, {}, dummyData);

			if (wordLength == 16)
				SECTION("Starting transfer with odd length when word length is 16 should fail with EINVAL")
				{
					REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 1) == EINVAL);
				}

			constexpr size_t transferSize {6};
			const uint8_t constBuffer[transferSize] {};
			uint8_t nonConstBuffer[transferSize] {};
			const void* const txBuffers[]
			{
					nullptr,
					constBuffer,
			};
			void* const rxBuffers[]
			{
					nullptr,
					nonConstBuffer,
			};
			for (const auto txBuffer : txBuffers)
				for (const auto rxBuffer : rxBuffers)
					DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfers, " <<
							(txBuffer != nullptr ? "non-" : "") << "null source, " <<
							(rxBuffer != nullptr ? "non-" : "") << "null destination")
					{
						const auto dataSize = (wordLength + 8 - 1) / 8u;
						const auto commonDmaFlags = Flags::peripheralFixed |
								(dataSize == 1 ? Flags::dataSize1 : Flags::dataSize2);

						REQUIRE_CALL(peripheralMock, getDrAddress()).IN_SEQUENCE(sequence).RETURN(drAddress);
						const auto rxDmaFlags = commonDmaFlags |
								Flags::transferCompleteInterruptEnable |
								Flags::peripheralToMemory |
								Flags::peripheralFixed |
								(rxBuffer != nullptr ? Flags::memoryIncrement : Flags::memoryFixed) |
								Flags::veryHighPriority;
						const auto rxAddressMatcher = [rxBuffer](const uintptr_t address)
								{
									return rxBuffer != nullptr ?
											address == reinterpret_cast<uintptr_t>(rxBuffer) :
											true;
								};
						REQUIRE_CALL(rxDmaChannelMock, startTransfer(_, drAddress, transferSize / dataSize,
								rxDmaFlags)).WITH(rxAddressMatcher(_1)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, getDrAddress()).IN_SEQUENCE(sequence).RETURN(drAddress);
						const auto txDmaFlags = commonDmaFlags |
								Flags::transferCompleteInterruptDisable |
								Flags::memoryToPeripheral |
								Flags::peripheralFixed |
								(txBuffer != nullptr ? Flags::memoryIncrement : Flags::memoryFixed) |
								Flags::lowPriority;
						const auto txAddressMatcher = [dataSize, dummyData, txBuffer](const uintptr_t address)
								{
									return txBuffer != nullptr ?
											address == reinterpret_cast<uintptr_t>(txBuffer) :
											memcmp(reinterpret_cast<const void*>(address), &dummyData, dataSize) == 0;
								};
						REQUIRE_CALL(txDmaChannelMock, startTransfer(_, drAddress, transferSize / dataSize,
								txDmaFlags)).WITH(txAddressMatcher(_1)).IN_SEQUENCE(sequence);
						REQUIRE(spi.startTransfer(masterMock, txBuffer, rxBuffer, transferSize) == 0);

						// starting another transfer when the previous one is ongoing should fail with EBUSY
						REQUIRE(spi.startTransfer(masterMock, txBuffer, rxBuffer, transferSize) == EBUSY);

						SECTION("Testing DMA TX error during transfer")
						{
							REQUIRE_CALL(txDmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
							REQUIRE_CALL(rxDmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
							REQUIRE_CALL(masterMock,
									transferCompleteEvent(transferSize - dataSize)).IN_SEQUENCE(sequence);
							txDmaChannelFunctor->transferErrorEvent(1);
						}
						SECTION("Testing DMA RX error during transfer")
						{
							txDmaChannelFunctor->transferCompleteEvent();

							REQUIRE_CALL(txDmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
							REQUIRE_CALL(rxDmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
							REQUIRE_CALL(masterMock,
									transferCompleteEvent(transferSize - dataSize)).IN_SEQUENCE(sequence);
							rxDmaChannelFunctor->transferErrorEvent(1);
						}
						SECTION("Testing successfully completed transfer")
						{
							txDmaChannelFunctor->transferCompleteEvent();

							REQUIRE_CALL(txDmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
							REQUIRE_CALL(rxDmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
							REQUIRE_CALL(masterMock, transferCompleteEvent(transferSize)).IN_SEQUENCE(sequence);
							rxDmaChannelFunctor->transferCompleteEvent();
						}
					}
		}

	{
		REQUIRE_CALL(rxDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(txDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
		spi.stop();
	}
}
