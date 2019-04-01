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

#include "distortos/chip/DmaChannelFunctor.hpp"
#include "distortos/chip/DmaChannelUniqueHandle.hpp"
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

uint8_t getNextChannelId()
{
	static uint8_t channelId {};
	const auto nextChannelId = channelId;
	channelId = (channelId + 1) % 8;
	return nextChannelId;
}

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

	const auto channelId = getNextChannelId();

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	SECTION("Releasing released driver causes no hardware accesses")
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

		SECTION("Reserving reserved driver with a different handle should fail with EBUSY")
		{
			distortos::chip::DmaChannel::UniqueHandle anotherHandle;
			REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
			REQUIRE(anotherHandle.reserve(channel, 0, functorMock) == EBUSY);
		}

		REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(0);
		handle.release();
	}
}

TEST_CASE("Testing transfer configuration", "[configuration]")
{
	DmaChannelFunctor functorMock {};
	distortos::chip::DmaPeripheral peripheralMock {};
	distortos::chip::DmaChannelPeripheral channelPeripheralMock {};
	distortos::InterruptMaskingLock::Proxy interruptMaskingLockProxyMock {};
	trompeloeil::sequence sequence {};

	const auto channelId = getNextChannelId();

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	distortos::chip::DmaChannel::UniqueHandle handle;

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	{
		REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
		REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
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
						REQUIRE_CALL(channelPeripheralMock, writeNdtr(transactions)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(channelPeripheralMock, writePar(peripheralAddress)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(channelPeripheralMock, writeM0ar(memoryAddress)).IN_SEQUENCE(sequence);
						const auto fcr = DMA_SxFCR_DMDIS | DMA_SxFCR_FTH;
						REQUIRE_CALL(channelPeripheralMock, writeFcr(fcr)).IN_SEQUENCE(sequence);
						const auto cr = request1 << DMA_SxCR_CHSEL_Pos |
								static_cast<uint32_t>(flags) |
								static_cast<uint32_t>(peripheralDataSize) |
								static_cast<uint32_t>(memoryBurstDataSize) |
								static_cast<uint32_t>(peripheralBurstSize) |
								DMA_SxCR_TEIE |
								DMA_SxCR_EN;
						REQUIRE_CALL(channelPeripheralMock, writeCr(cr)).IN_SEQUENCE(sequence);
						handle.startTransfer(memoryAddress, peripheralAddress, transactions,
								flags | peripheralDataSize | memoryBurstDataSize | peripheralBurstSize);
					}
	}

	REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(0);
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

	const auto channelId = getNextChannelId();
	const auto channelShift = getChannelShift(channelId);

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	distortos::chip::DmaChannel::UniqueHandle handle;

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	{
		REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
		REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
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

	SECTION("Testing interruptHandler() - both TCIF and TEIF flags cleared")
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
	const bool falseTrue[]
	{
			false,
			true,
	};
	SECTION("Testing interruptHandler() - TCIF and/or TEIF flags set but corresponding interrupts disabled")
	{
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
	}
	SECTION("Testing interruptHandler() - TCIF and/or TEIF flags set and corresponding interrupts enabled")
	{
		for (const auto tcif : falseTrue)
			for (const auto teif : falseTrue)
				for (const auto tcie : falseTrue)
					for (const auto teie : falseTrue)
					{
						if ((tcif && tcie) == false && (teif && teie) == false)
							continue;

						DYNAMIC_SECTION("Testing interruptHandler() - "
								"TCIF flag " << (tcif == false ? "cleared" : "set") <<
								", TEIF flag " << (teif == false ? "cleared" : "set") <<
								", TC interrupt " << (tcie == false ? "disabled" : "enabled") <<
								" and TE interrupt " << (teie == false ? "disabled" : "enabled"))
						{
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

	REQUIRE_CALL(channelPeripheralMock, readCr()).IN_SEQUENCE(sequence).RETURN(0);
	handle.release();
}
