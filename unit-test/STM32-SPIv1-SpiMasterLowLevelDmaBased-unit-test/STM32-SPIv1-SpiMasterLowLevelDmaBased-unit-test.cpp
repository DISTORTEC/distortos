/**
 * \file
 * \brief STM32 SPIv1's SpiMasterLowLevelDmaBased test cases
 *
 * This test checks whether STM32 SPIv1's SpiMasterLowLevelDmaBased performs all h/w operations properly and in correct
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

constexpr uintptr_t drAddress {0xd596c5f4};
constexpr uint16_t initialCr1 {SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE | SPI_CR1_BR | SPI_CR1_MSTR};
constexpr uint16_t initialCr2 {SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN};
constexpr uint8_t rxDmaRequest {0x9e};
constexpr auto rxPriority = distortos::chip::DmaChannel::Priority::veryHigh;
constexpr uint8_t txDmaRequest {0x65};
constexpr auto txPriority = distortos::chip::DmaChannel::Priority::low;

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
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::chip::SpiMasterLowLevelDmaBased spi {peripheralMock, rxDmaChannelMock, rxDmaRequest, txDmaChannelMock,
			txDmaRequest};

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

		SECTION("Starting started driver should fail with EBADF")
		{
			REQUIRE(spi.start() == EBADF);
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence));
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence));
			expectations.emplace_back(NAMED_REQUIRE_CALL(txDmaChannelMock, release()).IN_SEQUENCE(sequence));
			expectations.emplace_back(NAMED_REQUIRE_CALL(rxDmaChannelMock, release()).IN_SEQUENCE(sequence));
		}
		SECTION("Stopping started driver should succeed")
		{
			REQUIRE_CALL(rxDmaChannelMock, release()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(txDmaChannelMock, release()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
			REQUIRE(spi.stop() == 0);
		}
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
			0x8ef07b4d,
			0x88cc94e0,
			0xd4332d59,
			0xc8e44b00,
	};
	const uint8_t wordLengths[]
	{
			0xda,
			0x9a,
			0x95,
			0xd0,
	};
	const bool lsbFirsts[]
	{
			false,
			true,
	};
	const std::pair<int, uint32_t> rets[]
	{
			{0x1b200951, 0x9c0432b1},
			{0x5b260efd, 0x1c24f633},
			{0x6accba86, 0xa6eeadb6},
			{0x49b4f5e0, 0x2e6dde22},
	};
	for (auto mode : modes)
		for (auto clockFrequency : clockFrequencies)
			for (auto wordLength : wordLengths)
				for (auto lsbFirst : lsbFirsts)
					for (auto ret : rets)
					{
						REQUIRE_CALL(stm32Spiv1Spiv2Mock, configureSpi(_, mode, clockFrequency, wordLength,
								lsbFirst)).LR_WITH(&_1 == &peripheralMock).IN_SEQUENCE(sequence).RETURN(ret);
						REQUIRE(spi.configure(mode, clockFrequency, wordLength, lsbFirst, {}) == ret);
					}

	{
		REQUIRE_CALL(rxDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(txDmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr1(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCr2(0u)).IN_SEQUENCE(sequence);
		REQUIRE(spi.stop() == 0);
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
	SECTION("RX DMA configuration error should propagate error code to caller")
	{
		REQUIRE_CALL(peripheralMock, getDrAddress()).IN_SEQUENCE(sequence).RETURN(drAddress);
		constexpr int ret {0x58cf6045};
		REQUIRE_CALL(rxDmaChannelMock, configureTransfer(_, _, _, _, _, _, _, _, _)).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 2) == ret);
	}
	SECTION("TX DMA configuration error should propagate error code to caller")
	{
		REQUIRE_CALL(peripheralMock, getDrAddress()).IN_SEQUENCE(sequence).RETURN(drAddress);
		REQUIRE_CALL(rxDmaChannelMock, configureTransfer(_, _, _, _, _, _, _, _, _)).IN_SEQUENCE(sequence).RETURN(0);
		constexpr int ret {0x17e727e9};
		REQUIRE_CALL(peripheralMock, getDrAddress()).IN_SEQUENCE(sequence).RETURN(drAddress);
		REQUIRE_CALL(txDmaChannelMock, configureTransfer(_, _, _, _, _, _, _, _, _)).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE(spi.startTransfer(masterMock, nullptr, nullptr, 2) == ret);
	}

	const uint8_t wordLengths[]
	{
			8,
			16,
	};
	for (const auto wordLength : wordLengths)
		DYNAMIC_SECTION("Testing " << static_cast<int>(wordLength) << "-bit transfers")
		{
			constexpr uint16_t dummyData {0xfac5};
			REQUIRE_CALL(stm32Spiv1Spiv2Mock, configureSpi(_, distortos::devices::SpiMode{}, uint32_t{}, wordLength,
					bool{})).LR_WITH(&_1 == &peripheralMock).IN_SEQUENCE(sequence).RETURN(std::make_pair(0, 0));
			REQUIRE(spi.configure({}, {}, wordLength, {}, dummyData).first == 0);

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
						const auto dataSize = wordLength / 8u;

						REQUIRE_CALL(peripheralMock, getDrAddress()).IN_SEQUENCE(sequence).RETURN(drAddress);
						const auto rxAddressMatcher = [rxBuffer](const uintptr_t address)
								{
									return rxBuffer != nullptr ?
											address == reinterpret_cast<uintptr_t>(rxBuffer) :
											true;
								};
						REQUIRE_CALL(rxDmaChannelMock, configureTransfer(_, dataSize, rxBuffer != nullptr,
								drAddress, dataSize, false, transferSize / dataSize, false,
								rxPriority)).WITH(rxAddressMatcher(_1)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(peripheralMock, getDrAddress()).IN_SEQUENCE(sequence).RETURN(drAddress);
						const auto txAddressMatcher = [dataSize, dummyData, txBuffer](const uintptr_t address)
								{
									return txBuffer != nullptr ?
											address == reinterpret_cast<uintptr_t>(txBuffer) :
											memcmp(reinterpret_cast<const void*>(address), &dummyData, dataSize) == 0;
								};
						REQUIRE_CALL(txDmaChannelMock, configureTransfer(_, dataSize, txBuffer != nullptr,
								drAddress, dataSize, false, transferSize / dataSize, true,
								txPriority)).WITH(txAddressMatcher(_1)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(rxDmaChannelMock, startTransfer()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(txDmaChannelMock, startTransfer()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(spi.startTransfer(masterMock, txBuffer, rxBuffer, transferSize) == 0);

						// starting another transfer when the previous one is ongoing should fail with EBUSY
						REQUIRE(spi.startTransfer(masterMock, txBuffer, rxBuffer, transferSize) == EBUSY);

						// trying to configure the driver when a transfer is ongoing should fail with EBUSY
						REQUIRE(spi.configure({}, {}, 8, {}, dummyData).first == EBUSY);

						// trying to stop the driver when a transfer is ongoing should fail with EBUSY
						REQUIRE(spi.stop() == EBUSY);

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
		REQUIRE(spi.stop() == 0);
	}
}
