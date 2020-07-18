/**
 * \file
 * \brief STM32 DMAv1's DmaChannel test cases
 *
 * This test checks whether STM32 DMAv1's DmaChannel performs all h/w operations properly and in correct order.
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/DmaChannelFunctor.hpp"
#include "distortos/chip/DmaChannelHandle.hpp"
#include "distortos/chip/STM32-DMAv1-DmaChannelPeripheral.hpp"
#include "distortos/chip/STM32-DMAv1-DmaPeripheral.hpp"

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
	return (channelId - 1) * 4;
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr uint8_t maxRequest {(1 << DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS) - 1};
constexpr uint8_t request1 {maxRequest};
constexpr uint8_t request2 {};
constexpr uint32_t initialCselr {0x55555555};

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

	constexpr uint8_t channelId {6};

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	const auto channelShift = getChannelShift(channelId);

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	SECTION("Releasing released driver causes no hardware accesses")
	{
		distortos::chip::DmaChannelHandle handle;
		handle.release();
	}
	SECTION("Reserving released driver should succeed")
	{
		distortos::chip::DmaChannelHandle handle;

		{
			REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

			const auto newCselr = (initialCselr & ~(DMA_CSELR_C1S << channelShift)) | request1 << channelShift;
			const auto oldCselr = newCselr ^ DMA_CSELR_C1S << channelShift;
			REQUIRE_CALL(peripheralMock, readCselr()).IN_SEQUENCE(sequence).RETURN(oldCselr);
			REQUIRE_CALL(peripheralMock, writeCselr(newCselr)).IN_SEQUENCE(sequence);

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

			REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
			REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
		}

		SECTION("Reserving reserved driver with a different handle should fail with EBUSY")
		{
			distortos::chip::DmaChannelHandle anotherHandle;
			REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
			REQUIRE(anotherHandle.reserve(channel, 0, functorMock) == EBUSY);
		}

		REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(0);
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

	constexpr uint8_t channelId {5};

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	distortos::chip::DmaChannelHandle handle;

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	{
		REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

		const auto channelShift = getChannelShift(channelId);

		const auto newCselr = (initialCselr & ~(DMA_CSELR_C1S << channelShift)) | request1 << channelShift;
		const auto oldCselr = newCselr ^ DMA_CSELR_C1S << channelShift;
		REQUIRE_CALL(peripheralMock, readCselr()).IN_SEQUENCE(sequence).RETURN(oldCselr);
		REQUIRE_CALL(peripheralMock, writeCselr(newCselr)).IN_SEQUENCE(sequence);

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

		REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
		REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
	}

	SECTION("Trying to use valid configuration should succeed")
	{
		const Flags flagsArray[]
		{
			Flags::transferCompleteInterruptDisable | Flags::peripheralToMemory | Flags::peripheralFixed |
					Flags::memoryFixed | Flags::lowPriority,
			Flags::transferCompleteInterruptEnable | Flags::memoryToPeripheral | Flags::peripheralIncrement |
					Flags::memoryIncrement | Flags::mediumPriority,
			Flags::transferCompleteInterruptDisable | Flags::peripheralToMemory | Flags::peripheralFixed |
					Flags::memoryFixed | Flags::highPriority,
			Flags::transferCompleteInterruptEnable | Flags::memoryToPeripheral | Flags::peripheralIncrement |
					Flags::memoryIncrement | Flags::veryHighPriority,
		};
		const Flags peripheralDataSizes[]
		{
				Flags::peripheralDataSize1,
				Flags::peripheralDataSize2,
				Flags::peripheralDataSize4
		};
		const Flags memoryDataSizes[]
		{
				Flags::memoryDataSize1,
				Flags::memoryDataSize2,
				Flags::memoryDataSize4
		};
		for (const auto flags : flagsArray)
			for (const auto peripheralDataSize : peripheralDataSizes)
				for (const auto memoryDataSize : memoryDataSizes)
				{
					constexpr uintptr_t memoryAddress {0xdf5d76a8};
					constexpr uintptr_t peripheralAddress {0x1b932358};
					constexpr uint16_t transactions {0xd353};

					REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(channelPeripheralMock, writeCndtr(transactions)).IN_SEQUENCE(sequence);
					REQUIRE_CALL(channelPeripheralMock, writeCpar(peripheralAddress)).IN_SEQUENCE(sequence);
					REQUIRE_CALL(channelPeripheralMock, writeCmar(memoryAddress)).IN_SEQUENCE(sequence);
					const auto ccr = static_cast<uint32_t>(flags) |
							static_cast<uint32_t>(peripheralDataSize) |
							static_cast<uint32_t>(memoryDataSize) |
							DMA_CCR_TEIE |
							DMA_CCR_EN;
					REQUIRE_CALL(channelPeripheralMock, writeCcr(ccr)).IN_SEQUENCE(sequence);
					handle.startTransfer(memoryAddress, peripheralAddress, transactions,
							flags | peripheralDataSize | memoryDataSize);
				}
	}

	REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(0);
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

	constexpr uint8_t channelId {0};
	const auto channelShift = getChannelShift(channelId);

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	distortos::chip::DmaChannelHandle handle;

	ALLOW_CALL(channelPeripheralMock, getChannelId()).RETURN(channelId);

	{
		REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

		const auto newCselr = (initialCselr & ~(DMA_CSELR_C1S << channelShift)) | request1 << channelShift;
		const auto oldCselr = newCselr ^ DMA_CSELR_C1S << channelShift;
		REQUIRE_CALL(peripheralMock, readCselr()).IN_SEQUENCE(sequence).RETURN(oldCselr);
		REQUIRE_CALL(peripheralMock, writeCselr(newCselr)).IN_SEQUENCE(sequence);

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

		REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
		REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
	}

	SECTION("Testing stopTransfer()")
	{
		const auto oldCcr = UINT32_MAX;
		REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(oldCcr);
		const auto newCcr = oldCcr & ~(DMA_CCR_TEIE | DMA_CCR_HTIE | DMA_CCR_TCIE | DMA_CCR_EN);
		REQUIRE_CALL(channelPeripheralMock, writeCcr(newCcr)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeIfcr(DMA_IFCR_CGIF1 << channelShift)).IN_SEQUENCE(sequence);
		handle.stopTransfer();
	}
	SECTION("Testing interruptHandler()")
	{
		{
			const auto isr = UINT32_MAX & ~((DMA_ISR_TEIF1 | DMA_ISR_TCIF1) << channelShift);
			REQUIRE_CALL(peripheralMock, readIsr()).IN_SEQUENCE(sequence).RETURN(isr);
			channel.interruptHandler();
		}
		{
			const bool falseTrue[]
			{
					false,
					true,
			};
			for (const auto teif : falseTrue)
				for (const auto tcif : falseTrue)
				{
					if (teif == false && tcif == false)
						continue;

					const auto isr = UINT32_MAX & ~((DMA_ISR_TEIF1 | DMA_ISR_TCIF1) << channelShift) |
							teif << (DMA_ISR_TEIF1_Pos + channelShift) |
							tcif << (DMA_ISR_TCIF1_Pos + channelShift);
					REQUIRE_CALL(peripheralMock, readIsr()).IN_SEQUENCE(sequence).RETURN(isr);
					const auto ccr = UINT32_MAX & ~(DMA_CCR_TEIE | DMA_CCR_TCIE) |
							!teif << DMA_CCR_TEIE_Pos |
							!tcif << DMA_CCR_TCIE_Pos;
					REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(ccr);
					channel.interruptHandler();
				}

			for (const auto teif : falseTrue)
				for (const auto tcif : falseTrue)
					for (const auto teie : falseTrue)
						for (const auto tcie : falseTrue)
						{
							if ((teif && teie) == false && (tcif && tcie) == false)
								continue;

							const auto isr = UINT32_MAX & ~((DMA_ISR_TEIF1 | DMA_ISR_TCIF1) << channelShift) |
									teif << (DMA_ISR_TEIF1_Pos + channelShift) |
									tcif << (DMA_ISR_TCIF1_Pos + channelShift);
							REQUIRE_CALL(peripheralMock, readIsr()).IN_SEQUENCE(sequence).RETURN(isr);
							const auto ccr = UINT32_MAX & ~(DMA_CCR_TEIE | DMA_CCR_TCIE) |
									teie << DMA_CCR_TEIE_Pos |
									tcie << DMA_CCR_TCIE_Pos;
							REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(ccr);
							const uint32_t ifcr = (teif && teie) << (DMA_IFCR_CTEIF1_Pos + channelShift) |
									(tcif && tcie) << (DMA_IFCR_CTCIF1_Pos + channelShift);
							REQUIRE_CALL(peripheralMock, writeIfcr(ifcr)).IN_SEQUENCE(sequence);

							if ((teif && teie) == true)
							{
								constexpr uint16_t transactionsLeft {0x6e95};
								expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock,
										readCndtr()).IN_SEQUENCE(sequence).RETURN(transactionsLeft));
								expectations.emplace_back(NAMED_REQUIRE_CALL(functorMock,
										transferErrorEvent(transactionsLeft)).IN_SEQUENCE(sequence));
							}
							if ((tcif && tcie) == true)
							{
								expectations.emplace_back(NAMED_REQUIRE_CALL(functorMock,
										transferCompleteEvent()).IN_SEQUENCE(sequence));
							}

							channel.interruptHandler();
						}
		}
	}

	REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(0);
	handle.release();
}
