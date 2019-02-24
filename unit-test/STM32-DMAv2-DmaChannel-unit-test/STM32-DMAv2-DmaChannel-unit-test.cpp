/**
 * \file
 * \brief STM32 DMAv2's DmaChannel test cases
 *
 * This test checks whether STM32 DMAv2's DmaChannel performs all h/w operations properly and in correct order.
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/DmaChannel.hpp"
#include "distortos/chip/DmaChannelFunctor.hpp"
#include "distortos/chip/STM32-DMAv2-DmaChannelPeripheral.hpp"
#include "distortos/chip/STM32-DMAv2-DmaPeripheral.hpp"

#include "distortos/InterruptMaskingLock.hpp"

using trompeloeil::_;
using Flags = distortos::chip::DmaChannel::Flags;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class DmaChannelFunctor : public distortos::chip::DmaChannelFunctor
{
public:

	MAKE_MOCK0(transferCompleteEvent, void());
	MAKE_MOCK1(transferErrorEvent, void(size_t));
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

uint8_t getChannelShift(const uint8_t channelId)
{
	static const std::array<uint8_t, 4> channelShifts
	{{
			0,
			6,
			16,
			22,
	}};
	return channelShifts[channelId % channelShifts.size()];
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr uint8_t maxRequest {DMA_SxCR_CHSEL_Msk >> DMA_SxCR_CHSEL_Pos};
constexpr uint8_t request1 {maxRequest};
constexpr uint8_t request2 {};
constexpr uint32_t allFlags {DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0 |
		DMA_LIFCR_CFEIF0};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing reserve() & release() interactions", "[reserve/release]")
{
	DmaChannelFunctor functorMock {};
	distortos::chip::DmaPeripheral peripheralMock {};
	distortos::chip::DmaChannelPeripheral channelPeripheralMock {};
	distortos::InterruptMaskingLock::Proxy interruptMaskingLockProxyMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr uint8_t channelId {4};
	const auto channelShift = getChannelShift(channelId);

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	SECTION("Configuring released driver should fail with EBADF")
	{
		distortos::chip::DmaChannel::UniqueHandle handle;
		REQUIRE(handle.configureTransfer({}, {}, {}, {}) == EBADF);
	}
	SECTION("Starting a transfer with released driver should fail with EBADF")
	{
		distortos::chip::DmaChannel::UniqueHandle handle;
		REQUIRE(handle.startTransfer() == EBADF);
	}
	SECTION("Stopping a transfer with released driver should fail with EBADF")
	{
		distortos::chip::DmaChannel::UniqueHandle handle;
		REQUIRE(handle.stopTransfer() == EBADF);
	}
	SECTION("Releasing released driver results in hardware accesses")
	{
		distortos::chip::DmaChannel::UniqueHandle handle;
		handle.release();
	}
	SECTION("Reserving released driver should succeed")
	{
		distortos::chip::DmaChannel::UniqueHandle handle;

		{
			REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
			REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
		}

		SECTION("Reserving reserved driver with the same handle should succeed")
		{
			const auto oldCr = UINT32_MAX;
			REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(oldCr);
			const auto newCr = oldCr & ~(DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | DMA_SxCR_EN);
			REQUIRE_CALL(channelPeripheralMock, writeCr(newCr)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(newCr);
			if (channelId <= 3)
				expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
						writeLifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
			else
				expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
						writeHifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));

			REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
			REQUIRE(handle.reserve(channel, request2, functorMock) == 0);
		}
		SECTION("Reserving reserved driver with a different handle should fail with EBUSY")
		{
			distortos::chip::DmaChannel::UniqueHandle anotherHandle;
			REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
			REQUIRE(anotherHandle.reserve(channel, 0, functorMock) == EBUSY);
		}

		const auto oldCr = UINT32_MAX;
		expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock,
				readCr()).IN_SEQUENCE(sequence).RETURN(oldCr));
		const auto newCr = oldCr & ~(DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | DMA_SxCR_EN);
		expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock, writeCr(newCr)).IN_SEQUENCE(sequence));
		expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock,
				readCr()).IN_SEQUENCE(sequence).RETURN(newCr));
		if (channelId <= 3)
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
					writeLifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
		else
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
					writeHifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
	}
}

TEST_CASE("Testing configureTransfer()", "[configureTransfer]")
{
	DmaChannelFunctor functorMock {};
	distortos::chip::DmaPeripheral peripheralMock {};
	distortos::chip::DmaChannelPeripheral channelPeripheralMock {};
	distortos::InterruptMaskingLock::Proxy interruptMaskingLockProxyMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr uint8_t channelId {6};
	const auto channelShift = getChannelShift(channelId);

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	distortos::chip::DmaChannel::UniqueHandle handle;

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	{
		REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
		REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
	}

	SECTION("Trying to use invalid data size should fail with EINVAL")
	{
		constexpr auto memoryDataSize = Flags::memoryDataSize2 | Flags::memoryDataSize4;
		REQUIRE(handle.configureTransfer({}, {}, 1, memoryDataSize) == EINVAL);
		constexpr auto peripheralDataSize = Flags::peripheralDataSize2 | Flags::peripheralDataSize4;
		REQUIRE(handle.configureTransfer({}, {}, 1, peripheralDataSize) == EINVAL);
		constexpr auto dataSize = Flags::dataSize2 | Flags::dataSize4;
		REQUIRE(handle.configureTransfer({}, {}, 1, dataSize) == EINVAL);
	}

	{
		const uint8_t burstSizes[]
		{
				1,
				4,
				8,
				16,
		};
		const uint8_t dataSizes[]
		{
				1,
				2,
				4,
		};
		for (const auto burstSize : burstSizes)
			for (const auto dataSize : dataSizes)
			{
				const auto memoryBurstSize = burstSize == 1 ? Flags::memoryBurstSize1 :
						burstSize == 4 ? Flags::memoryBurstSize4 :
						burstSize == 8 ? Flags::memoryBurstSize8 : Flags::memoryBurstSize16;
				const auto memoryDataSize = dataSize == 1 ? Flags::memoryDataSize1 :
						dataSize == 2 ? Flags::memoryDataSize2 : Flags::memoryDataSize4;
				if (burstSize * dataSize > 16)
				{
					DYNAMIC_SECTION("Trying to use memory burst size " << static_cast<int>(burstSize) <<
							" and memory data size " << static_cast<int>(dataSize) << " should fail with EINVAL")
					{
						REQUIRE(handle.configureTransfer({}, {}, 1, memoryBurstSize | memoryDataSize) == EINVAL);
					}
				}
				for (uintptr_t address {1}; address < (burstSize * dataSize); ++address)
				{
					DYNAMIC_SECTION("Trying to use address " << static_cast<int>(address) << " when burst size is " <<
							static_cast<int>(burstSize) << " and data size is " << static_cast<int>(dataSize) <<
							" should fail with EINVAL")
					{
						REQUIRE(handle.configureTransfer(address, {}, 1, memoryBurstSize | memoryDataSize) == EINVAL);
						const auto peripheralBurstSize = burstSize == 1 ? Flags::peripheralBurstSize1 :
								 burstSize == 4 ? Flags::peripheralBurstSize4 :
								 burstSize == 8 ? Flags::peripheralBurstSize8 : Flags::peripheralBurstSize16;
						const auto peripheralDataSize = dataSize == 1 ? Flags::peripheralDataSize1 :
								dataSize == 2 ? Flags::peripheralDataSize2 : Flags::peripheralDataSize4;
						REQUIRE(handle.configureTransfer({}, address, 1,
								peripheralBurstSize | peripheralDataSize) == EINVAL);
						const auto burstSizeFlags = burstSize == 1 ? Flags::burstSize1 :
								burstSize == 4 ? Flags::burstSize4 :
								burstSize == 8 ? Flags::burstSize8 : Flags::burstSize16;
						const auto dataSizeFlags = dataSize == 1 ? Flags::dataSize1 :
								dataSize == 2 ? Flags::dataSize2 : Flags::dataSize4;
						REQUIRE(handle.configureTransfer(address, address, 1,
								burstSizeFlags | dataSizeFlags) == EINVAL);
					}
				}
			}
	}

	SECTION("Trying to use 0 transactions should fail with EINVAL")
	{
		REQUIRE(handle.configureTransfer({}, {}, 0, {}) == EINVAL);
	}

	SECTION("Trying to configure the driver when transfer is ongoing should fail with EBUSY")
	{
		REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(DMA_SxCR_EN);
		REQUIRE(handle.configureTransfer({}, {}, 1, {}) == EBUSY);
	}

	SECTION("Trying to use valid configuration should succeed")
	{
		const Flags flagsArray[]
		{
			Flags::transferCompleteInterruptDisable | Flags::dmaFlowController | Flags::peripheralToMemory |
					Flags::peripheralFixed | Flags::memoryFixed | Flags::lowPriority,
			Flags::transferCompleteInterruptEnable | Flags::peripheralFlowController | Flags::memoryToPeripheral |
					Flags::peripheralIncrement | Flags::memoryIncrement | Flags::mediumPriority,
			Flags::transferCompleteInterruptDisable | Flags::dmaFlowController | Flags::peripheralToMemory |
					Flags::peripheralFixed | Flags::memoryFixed | Flags::highPriority,
			Flags::transferCompleteInterruptEnable | Flags::peripheralFlowController | Flags::memoryToPeripheral |
					Flags::peripheralIncrement | Flags::memoryIncrement | Flags::veryHighPriority,
		};
		const Flags peripheralDataSizes[]
		{
				Flags::peripheralDataSize1,
				Flags::peripheralDataSize2,
				Flags::peripheralDataSize4,
		};
		const Flags memoryBurstDataSizes[]
		{
				Flags::memoryBurstSize1 | Flags::memoryDataSize1,
				Flags::memoryBurstSize1 | Flags::memoryDataSize2,
				Flags::memoryBurstSize1 | Flags::memoryDataSize4,

				Flags::memoryBurstSize4 | Flags::memoryDataSize1,
				Flags::memoryBurstSize4 | Flags::memoryDataSize2,
				Flags::memoryBurstSize4 | Flags::memoryDataSize4,

				Flags::memoryBurstSize8 | Flags::memoryDataSize1,
				Flags::memoryBurstSize8 | Flags::memoryDataSize2,
				// Flags::memoryBurstSize8 | Flags::memoryDataSize4,	// invalid

				Flags::memoryBurstSize16 | Flags::memoryDataSize1,
				// Flags::memoryBurstSize16 | Flags::memoryDataSize2,	// invalid
				// Flags::memoryBurstSize16 | Flags::memoryDataSize4,	// invalid
		};
		const Flags peripheralBurstSizes[]
		{
				Flags::peripheralBurstSize1,
				Flags::peripheralBurstSize4,
				Flags::peripheralBurstSize8,
				Flags::peripheralBurstSize16,
		};
		for (const auto flags : flagsArray)
			for (const auto peripheralDataSize : peripheralDataSizes)
				for (const auto memoryBurstDataSize : memoryBurstDataSizes)
					for (const auto peripheralBurstSize : peripheralBurstSizes)
					{
						constexpr uintptr_t memoryAddress {0xdf5d76a0};
						constexpr uintptr_t peripheralAddress {0x1b932340};
						constexpr uint16_t transactions {0xd353};

						REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(0);
						const auto cr = request1 << DMA_SxCR_CHSEL_Pos |
								static_cast<uint32_t>(flags) |
								static_cast<uint32_t>(peripheralDataSize) |
								static_cast<uint32_t>(memoryBurstDataSize) |
								static_cast<uint32_t>(peripheralBurstSize) |
								DMA_SxCR_TEIE;
						REQUIRE_CALL(channelPeripheralMock, writeCr(cr)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(channelPeripheralMock, writeNdtr(transactions)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(channelPeripheralMock, writePar(peripheralAddress)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(channelPeripheralMock, writeM0ar(memoryAddress)).IN_SEQUENCE(sequence);
						const auto fcr = DMA_SxFCR_DMDIS | DMA_SxFCR_FTH;
						REQUIRE_CALL(channelPeripheralMock, writeFcr(fcr)).IN_SEQUENCE(sequence);
						REQUIRE(handle.configureTransfer(memoryAddress, peripheralAddress, transactions,
								flags | peripheralDataSize | memoryBurstDataSize | peripheralBurstSize) == 0);
					}
	}

	const auto oldCr = UINT32_MAX;
	REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(oldCr);
	const auto newCr = oldCr & ~(DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | DMA_SxCR_EN);
	REQUIRE_CALL(channelPeripheralMock, writeCr(newCr)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(newCr);
	if (channelId <= 3)
		expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
				writeLifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
	else
		expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
				writeHifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
	handle.release();
}

TEST_CASE("Testing transfers", "[transfers]")
{
	DmaChannelFunctor functorMock {};
	distortos::chip::DmaPeripheral peripheralMock {};
	distortos::chip::DmaChannelPeripheral channelPeripheralMock {};
	distortos::InterruptMaskingLock::Proxy interruptMaskingLockProxyMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr uint8_t channelId {3};
	const auto channelShift = getChannelShift(channelId);

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	distortos::chip::DmaChannel::UniqueHandle handle;

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	{
		REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
		REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
	}

	SECTION("Testing startTransfer()")
	{
		const auto newCr = 0xaaaaaaaa | DMA_SxCR_EN;
		const auto oldCr = newCr ^ DMA_SxCR_EN;
		REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(oldCr);
		REQUIRE_CALL(channelPeripheralMock, writeCr(newCr)).IN_SEQUENCE(sequence);
		REQUIRE(handle.startTransfer() == 0);
	}
	SECTION("Testing stopTransfer()")
	{
		const auto oldCr = UINT32_MAX;
		REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(oldCr);
		const auto newCr = oldCr & ~(DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | DMA_SxCR_EN);
		REQUIRE_CALL(channelPeripheralMock, writeCr(newCr)).IN_SEQUENCE(sequence);
		for (size_t i {}; i < 13; ++i)
		{
			expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock,
					readCr()).IN_SEQUENCE(sequence).RETURN(newCr | DMA_SxCR_EN));
		}
		REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(newCr);
		if (channelId <= 3)
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
					writeLifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
		else
			expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
					writeHifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
		handle.stopTransfer();
	}
	SECTION("Testing interruptHandler()")
	{
		{
			const auto isr = UINT32_MAX & ~((DMA_LISR_TCIF0 | DMA_LISR_TEIF0) << channelShift);
			if (channelId <= 3)
				expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
						readLisr()).IN_SEQUENCE(sequence).RETURN(isr));
			else
				expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
						readHisr()).IN_SEQUENCE(sequence).RETURN(isr));
			channel.interruptHandler();
		}
		{
			const bool falseTrue[]
			{
					false,
					true,
			};
			for (const auto tcif : falseTrue)
				for (const auto teif : falseTrue)
				{
					if (tcif == false && teif == false)
						continue;

					const auto isr = UINT32_MAX & ~((DMA_LISR_TCIF0 | DMA_LISR_TEIF0) << channelShift) |
							tcif << (DMA_LISR_TCIF0_Pos + channelShift) |
							teif << (DMA_LISR_TEIF0_Pos + channelShift);
					if (channelId <= 3)
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readLisr()).IN_SEQUENCE(sequence).RETURN(isr));
					else
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readHisr()).IN_SEQUENCE(sequence).RETURN(isr));
					const auto cr = UINT32_MAX & ~(DMA_SxCR_TCIE | DMA_SxCR_TEIE) |
							!tcif << DMA_SxCR_TCIE_Pos |
							!teif << DMA_SxCR_TEIE_Pos;
					REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(cr);
					channel.interruptHandler();
				}

			for (const auto tcif : falseTrue)
				for (const auto teif : falseTrue)
					for (const auto tcie : falseTrue)
						for (const auto teie : falseTrue)
						{
							if ((tcif && tcie) == false && (teif && teie) == false)
								continue;

							const auto isr = UINT32_MAX & ~((DMA_LISR_TCIF0 | DMA_LISR_TEIF0) << channelShift) |
									tcif << (DMA_LISR_TCIF0_Pos + channelShift) |
									teif << (DMA_LISR_TEIF0_Pos + channelShift);
							if (channelId <= 3)
								expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
										readLisr()).IN_SEQUENCE(sequence).RETURN(isr));
							else
								expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
										readHisr()).IN_SEQUENCE(sequence).RETURN(isr));
							const auto cr = UINT32_MAX & ~(DMA_SxCR_TCIE | DMA_SxCR_TEIE) |
									tcie << DMA_SxCR_TCIE_Pos |
									teie << DMA_SxCR_TEIE_Pos;
							REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(cr);
							const uint32_t ifcr = (tcif && tcie) << (DMA_LIFCR_CTCIF0_Pos + channelShift) |
									(teif && teie) << (DMA_LIFCR_CTEIF0_Pos + channelShift);
							if (channelId <= 3)
								expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
										writeLifcr(ifcr)).IN_SEQUENCE(sequence));
							else
								expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
										writeHifcr(ifcr)).IN_SEQUENCE(sequence));

							if ((tcif && tcie) == true)
							{
								expectations.emplace_back(NAMED_REQUIRE_CALL(functorMock,
										transferCompleteEvent()).IN_SEQUENCE(sequence));
							}
							if ((teif && teie) == true)
							{
								constexpr uint16_t transactionsLeft {0xba61};
								expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock,
										readNdtr()).IN_SEQUENCE(sequence).RETURN(transactionsLeft));
								expectations.emplace_back(NAMED_REQUIRE_CALL(functorMock,
										transferErrorEvent(transactionsLeft)).IN_SEQUENCE(sequence));
							}

							channel.interruptHandler();
						}
		}
	}

	const auto oldCr = UINT32_MAX;
	REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(oldCr);
	const auto newCr = oldCr & ~(DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE | DMA_SxCR_EN);
	REQUIRE_CALL(channelPeripheralMock, writeCr(newCr)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(newCr);
	if (channelId <= 3)
		expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
				writeLifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
	else
		expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
				writeHifcr(allFlags << channelShift)).IN_SEQUENCE(sequence));
	handle.release();
}
