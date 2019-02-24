/**
 * \file
 * \brief STM32 DMAv1's DmaChannel test cases
 *
 * This test checks whether STM32 DMAv1's DmaChannel performs all h/w operations properly and in correct order.
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/DmaChannel.hpp"
#include "distortos/chip/DmaChannelFunctor.hpp"
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
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr uint8_t channelId {6};
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
	SECTION("Reserving released driver with wrong request number should fail with EINVAL")
	{
		distortos::chip::DmaChannel::UniqueHandle handle;
		REQUIRE(handle.reserve(channel, maxRequest + 1, functorMock) == EINVAL);
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

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

			const auto newCselr = (initialCselr & ~(DMA_CSELR_C1S << channelShift)) | request1 << channelShift;
			const auto oldCselr = newCselr ^ DMA_CSELR_C1S << channelShift;
			REQUIRE_CALL(peripheralMock, readCselr()).IN_SEQUENCE(sequence).RETURN(oldCselr);
			REQUIRE_CALL(peripheralMock, writeCselr(newCselr)).IN_SEQUENCE(sequence);

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

			REQUIRE_CALL(interruptMaskingLockProxyMock, destruct()).IN_SEQUENCE(sequence);
			REQUIRE(handle.reserve(channel, request1, functorMock) == 0);
		}

		SECTION("Reserving reserved driver with the same handle should succeed")
		{
			const auto oldCcr = UINT32_MAX;
			REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(oldCcr);
			const auto newCcr = oldCcr & ~(DMA_CCR_TEIE | DMA_CCR_HTIE | DMA_CCR_TCIE | DMA_CCR_EN);
			REQUIRE_CALL(channelPeripheralMock, writeCcr(newCcr)).IN_SEQUENCE(sequence);
			REQUIRE_CALL(peripheralMock, writeIfcr(DMA_IFCR_CGIF1 << channelShift)).IN_SEQUENCE(sequence);

			REQUIRE_CALL(interruptMaskingLockProxyMock, construct()).IN_SEQUENCE(sequence);

#if DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

			const auto newCselr = (initialCselr & ~(DMA_CSELR_C1S << channelShift)) | request2 << channelShift;
			const auto oldCselr = newCselr ^ DMA_CSELR_C1S << channelShift;
			REQUIRE_CALL(peripheralMock, readCselr()).IN_SEQUENCE(sequence).RETURN(oldCselr);
			REQUIRE_CALL(peripheralMock, writeCselr(newCselr)).IN_SEQUENCE(sequence);

#endif	// DISTORTOS_CHIP_DMA_CHANNEL_REQUEST_BITS != 0

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

		const auto oldCcr = UINT32_MAX;
		expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock,
				readCcr()).IN_SEQUENCE(sequence).RETURN(oldCcr));
		const auto newCcr = oldCcr & ~(DMA_CCR_TEIE | DMA_CCR_HTIE | DMA_CCR_TCIE | DMA_CCR_EN);
		expectations.emplace_back(NAMED_REQUIRE_CALL(channelPeripheralMock, writeCcr(newCcr)).IN_SEQUENCE(sequence));
		expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
				writeIfcr(DMA_IFCR_CGIF1 << channelShift)).IN_SEQUENCE(sequence));
	}
}

TEST_CASE("Testing configureTransfer()", "[configureTransfer]")
{
	DmaChannelFunctor functorMock {};
	distortos::chip::DmaPeripheral peripheralMock {};
	distortos::chip::DmaChannelPeripheral channelPeripheralMock {};
	distortos::InterruptMaskingLock::Proxy interruptMaskingLockProxyMock {};
	trompeloeil::sequence sequence {};

	constexpr uint8_t channelId {5};
	const auto channelShift = getChannelShift(channelId);

	distortos::chip::DmaChannel channel {peripheralMock, channelPeripheralMock};

	distortos::chip::DmaChannel::UniqueHandle handle;

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
		const uint8_t dataSizes[]
		{
				2,
				4,
		};
		for (const auto dataSize : dataSizes)
			for (uintptr_t address {1}; address < dataSize; ++address)
			{
				DYNAMIC_SECTION("Trying to use address " << static_cast<int>(address) << " when data size is " <<
						static_cast<int>(dataSize) << " should fail with EINVAL")
				{
					const auto memoryDataSize = dataSize == 2 ? Flags::memoryDataSize2 : Flags::memoryDataSize4;
					REQUIRE(handle.configureTransfer(address, {}, 1, memoryDataSize) == EINVAL);
					const auto peripheralDataSize =
							dataSize == 2 ? Flags::peripheralDataSize2 : Flags::peripheralDataSize4;
					REQUIRE(handle.configureTransfer({}, address, 1, peripheralDataSize) == EINVAL);
					const auto dataSizeFlags =
							dataSize == 2 ? Flags::dataSize2 : Flags::dataSize4;
					REQUIRE(handle.configureTransfer(address, address, 1, dataSizeFlags) == EINVAL);
				}
			}
	}

	SECTION("Trying to use 0 transactions should fail with EINVAL")
	{
		REQUIRE(handle.configureTransfer({}, {}, 0, {}) == EINVAL);
	}

	SECTION("Trying to configure the driver when transfer is ongoing should fail with EBUSY")
	{
		REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(DMA_CCR_EN);
		REQUIRE(handle.configureTransfer({}, {}, 1, {}) == EBUSY);
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
					const auto ccr = static_cast<uint32_t>(flags) |
							static_cast<uint32_t>(peripheralDataSize) |
							static_cast<uint32_t>(memoryDataSize) |
							DMA_CCR_TEIE;
					REQUIRE_CALL(channelPeripheralMock, writeCcr(ccr)).IN_SEQUENCE(sequence);
					REQUIRE_CALL(channelPeripheralMock, writeCndtr(transactions)).IN_SEQUENCE(sequence);
					REQUIRE_CALL(channelPeripheralMock, writeCpar(peripheralAddress)).IN_SEQUENCE(sequence);
					REQUIRE_CALL(channelPeripheralMock, writeCmar(memoryAddress)).IN_SEQUENCE(sequence);
					REQUIRE(handle.configureTransfer(memoryAddress, peripheralAddress, transactions,
							flags | peripheralDataSize | memoryDataSize) == 0);
				}
	}

	const auto oldCcr = UINT32_MAX;
	REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(oldCcr);
	const auto newCcr = oldCcr & ~(DMA_CCR_TEIE | DMA_CCR_HTIE | DMA_CCR_TCIE | DMA_CCR_EN);
	REQUIRE_CALL(channelPeripheralMock, writeCcr(newCcr)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(peripheralMock, writeIfcr(DMA_IFCR_CGIF1 << channelShift)).IN_SEQUENCE(sequence);
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

	distortos::chip::DmaChannel::UniqueHandle handle;

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

	SECTION("Testing startTransfer()")
	{
		const auto newCcr = 0xaaaaaaaa | DMA_CCR_EN;
		const auto oldCcr = newCcr ^ DMA_CCR_EN;
		REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(oldCcr);
		REQUIRE_CALL(channelPeripheralMock, writeCcr(newCcr)).IN_SEQUENCE(sequence);
		REQUIRE(handle.startTransfer() == 0);
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

	const auto oldCcr = UINT32_MAX;
	REQUIRE_CALL(channelPeripheralMock, readCcr()).IN_SEQUENCE(sequence).RETURN(oldCcr);
	const auto newCcr = oldCcr & ~(DMA_CCR_TEIE | DMA_CCR_HTIE | DMA_CCR_TCIE | DMA_CCR_EN);
	REQUIRE_CALL(channelPeripheralMock, writeCcr(newCcr)).IN_SEQUENCE(sequence);
	REQUIRE_CALL(peripheralMock, writeIfcr(DMA_IFCR_CGIF1 << channelShift)).IN_SEQUENCE(sequence);
	handle.release();
}
