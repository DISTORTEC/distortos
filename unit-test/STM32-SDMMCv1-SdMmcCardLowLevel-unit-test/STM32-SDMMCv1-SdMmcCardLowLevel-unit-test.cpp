/**
 * \file
 * \brief STM32 SDMMCv1's SdMmcCardLowLevel test cases
 *
 * This test checks whether STM32 SDMMCv1's SdMmcCardLowLevel performs all h/w operations properly and in correct order.
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/chip/DmaChannel.hpp"
#include "distortos/chip/SdMmcCardLowLevel.hpp"
#include "distortos/chip/STM32-SDMMCv1-SdmmcPeripheral.hpp"

#include "distortos/devices/memory/SdMmcCardBase.hpp"

using trompeloeil::_;
using Flags = distortos::chip::DmaChannel::Flags;
using ReadTransfer = distortos::devices::SdMmcCardLowLevel::ReadTransfer;
using Response = distortos::devices::SdMmcCardLowLevel::Response;
using Result = distortos::devices::SdMmcCardBase::Result;
using Transfer = distortos::devices::SdMmcCardLowLevel::Transfer;
using WriteTransfer = distortos::devices::SdMmcCardLowLevel::WriteTransfer;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class SdMmcCard : public distortos::devices::SdMmcCardBase
{
public:

	MAKE_MOCK1(transactionCompleteEvent, void(Result), override);
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr uint32_t adapterFrequency {257000000};
constexpr uint32_t allIcrBits {SDMMC_ICR_SDIOITC | SDMMC_ICR_DBCKENDC | SDMMC_ICR_DATAENDC | SDMMC_ICR_CMDSENTC |
		SDMMC_ICR_CMDRENDC | SDMMC_ICR_RXOVERRC | SDMMC_ICR_TXUNDERRC | SDMMC_ICR_DTIMEOUTC | SDMMC_ICR_CTIMEOUTC |
		SDMMC_ICR_DCRCFAILC | SDMMC_ICR_CCRCFAILC};
constexpr uint8_t dmaRequest {0xac};
constexpr uintptr_t fifoAddress {0xff69811d};
constexpr uint16_t initialClkcr {SDMMC_CLKCR_CLKEN | SDMMC_CLKCR_CLKDIV};
constexpr uint16_t initialPower {SDMMC_POWER_PWRCTRL};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing start() & stop() interactions", "[start/stop]")
{
	SdMmcCard cardMock {};
	distortos::chip::SdmmcPeripheral peripheralMock {};
	distortos::chip::DmaChannel dmaChannelMock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SdMmcCardLowLevel sdMmc {peripheralMock, dmaChannelMock, dmaRequest};

	ALLOW_CALL(peripheralMock, getAdapterFrequency()).RETURN(adapterFrequency);

	SECTION("Stopping stopped driver should fail with EBADF")
	{
		REQUIRE(sdMmc.stop() == EBADF);
	}
	SECTION("Starting stopped driver when DMA channel is busy should fail with EBUSY")
	{
		REQUIRE_CALL(dmaChannelMock, reserve(dmaRequest, _)).IN_SEQUENCE(sequence).RETURN(EBUSY);
		REQUIRE(sdMmc.start() == EBUSY);
	}
	SECTION("Starting stopped driver should succeed")
	{
		REQUIRE_CALL(dmaChannelMock, reserve(dmaRequest, _)).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(peripheralMock, writeClkcr(initialClkcr)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writePower(initialPower)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.start() == 0);

		// stopping started driver should succeed
		REQUIRE_CALL(dmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeMask(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeDctrl(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCmd(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeClkcr(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writePower(0u)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.stop() == 0);
	}
}

TEST_CASE("Testing configure()", "[configure]")
{
	SdMmcCard cardMock {};
	distortos::chip::SdmmcPeripheral peripheralMock {};
	distortos::chip::DmaChannel dmaChannelMock {};
	trompeloeil::sequence sequence {};

	distortos::chip::SdMmcCardLowLevel sdMmc {peripheralMock, dmaChannelMock, dmaRequest};

	ALLOW_CALL(peripheralMock, getAdapterFrequency()).RETURN(adapterFrequency);

	{
		REQUIRE_CALL(dmaChannelMock, reserve(dmaRequest, _)).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(peripheralMock, writeClkcr(initialClkcr)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writePower(initialPower)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.start() == 0);
	}

	SECTION("Trying to use clock frequency lower than `adapter frequency / 257` should fail with EINVAL")
	{
		REQUIRE(sdMmc.configure({}, adapterFrequency / 257 - 1) == EINVAL);
	}

	const distortos::devices::SdMmcCardLowLevel::BusMode busModes[]
	{
			distortos::devices::SdMmcCardLowLevel::BusMode::_1Bit,
			distortos::devices::SdMmcCardLowLevel::BusMode::_4Bit,
			distortos::devices::SdMmcCardLowLevel::BusMode::_8Bit,
	};
	for (auto busMode : busModes)
	{
		DYNAMIC_SECTION("Trying to use bus mode " << static_cast<int>(busMode) << " should succeed")
		{
			const auto widbus = busMode == distortos::devices::SdMmcCardLowLevel::BusMode::_1Bit ? 0 :
					busMode == distortos::devices::SdMmcCardLowLevel::BusMode::_4Bit ? SDMMC_CLKCR_WIDBUS_0 :
					SDMMC_CLKCR_WIDBUS_1;
			const auto newClkcr = (initialClkcr & ~SDMMC_CLKCR_WIDBUS) | widbus;
			const auto oldClkcr = newClkcr ^ SDMMC_CLKCR_WIDBUS;

			REQUIRE_CALL(peripheralMock, readClkcr()).IN_SEQUENCE(sequence).RETURN(oldClkcr);
			REQUIRE_CALL(peripheralMock, writeClkcr(newClkcr)).IN_SEQUENCE(sequence);
			REQUIRE(sdMmc.configure(busMode, adapterFrequency / 257) == 0);
		}
	}

	const uint32_t clockFrequencies[]
	{
			adapterFrequency + 1,
			adapterFrequency,
			adapterFrequency - 1,

			adapterFrequency / 2 + 1,
			adapterFrequency / 2,
			adapterFrequency / 2 - 1,

			adapterFrequency / 3 + 1,
			adapterFrequency / 3,
			adapterFrequency / 3 - 1,

			adapterFrequency / 5 + 1,
			adapterFrequency / 5,
			adapterFrequency / 5 - 1,

			adapterFrequency / 8 + 1,
			adapterFrequency / 8,
			adapterFrequency / 8 - 1,

			adapterFrequency / 13 + 1,
			adapterFrequency / 13,
			adapterFrequency / 13 - 1,

			adapterFrequency / 21 + 1,
			adapterFrequency / 21,
			adapterFrequency / 21 - 1,

			adapterFrequency / 34 + 1,
			adapterFrequency / 34,
			adapterFrequency / 34 - 1,

			adapterFrequency / 55 + 1,
			adapterFrequency / 55,
			adapterFrequency / 55 - 1,

			adapterFrequency / 89 + 1,
			adapterFrequency / 89,
			adapterFrequency / 89 - 1,

			adapterFrequency / 144 + 1,
			adapterFrequency / 144,
			adapterFrequency / 144 - 1,

			adapterFrequency / 256 + 1,
			adapterFrequency / 256,
			adapterFrequency / 256 - 1,

			adapterFrequency / 257 + 1,
			adapterFrequency / 257,
	};
	for (auto clockFrequency : clockFrequencies)
	{
		DYNAMIC_SECTION("Trying to use clock frequency " << clockFrequency << " should succeed")
		{
			const auto divider = (adapterFrequency + clockFrequency - 1) / clockFrequency;
			const auto bypass = (divider <= 1) << SDMMC_CLKCR_BYPASS_Pos;
			const auto clkdiv = (bypass == false ? divider - 2 : 0) << SDMMC_CLKCR_CLKDIV_Pos;
			const auto newClkcr = (initialClkcr & ~(SDMMC_CLKCR_BYPASS | SDMMC_CLKCR_CLKDIV)) | bypass | clkdiv;
			const auto oldClkcr = newClkcr ^ (SDMMC_CLKCR_BYPASS | SDMMC_CLKCR_CLKDIV);

			REQUIRE_CALL(peripheralMock, readClkcr()).IN_SEQUENCE(sequence).RETURN(oldClkcr);
			REQUIRE_CALL(peripheralMock, writeClkcr(newClkcr)).IN_SEQUENCE(sequence);
			REQUIRE(sdMmc.configure({}, clockFrequency) == 0);
		}
	}

	{
		REQUIRE_CALL(dmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeMask(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeDctrl(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCmd(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeClkcr(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writePower(0u)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.stop() == 0);
	}
}

TEST_CASE("Testing startTransaction()", "[startTransaction]")
{
	SdMmcCard cardMock {};
	distortos::chip::SdmmcPeripheral peripheralMock {};
	distortos::chip::DmaChannel dmaChannelMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::chip::SdMmcCardLowLevel sdMmc {peripheralMock, dmaChannelMock, dmaRequest};

	distortos::chip::DmaChannelFunctor* dmaChannelFunctor {};

	ALLOW_CALL(peripheralMock, getAdapterFrequency()).RETURN(adapterFrequency);

	{
		REQUIRE_CALL(dmaChannelMock,
				reserve(dmaRequest, _)).IN_SEQUENCE(sequence).LR_SIDE_EFFECT(dmaChannelFunctor = &_2).RETURN(0);
		REQUIRE_CALL(peripheralMock, writeClkcr(initialClkcr)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writePower(initialPower)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.start() == 0);
	}

	SECTION("Starting transaction with invalid command should fail with EINVAL")
	{
		REQUIRE(sdMmc.startTransaction(cardMock,
				distortos::devices::SdMmcCardLowLevel::maxCommand + 1, {}, {}, {}) == EINVAL);
	}
	SECTION("Starting transaction with no response expected and valid transfer should fail with EINVAL")
	{
		constexpr size_t blockSize {512};

		uint8_t buffer[blockSize];
		REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, {},
				Transfer{buffer, sizeof(buffer), blockSize, 100}) == EINVAL);
	}
	SECTION("Starting transaction with nullptr as transfer's buffer should fail with EINVAL")
	{
		constexpr size_t blockSize {512};

		uint32_t response;
		REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
				ReadTransfer{{}, blockSize, blockSize, 100}) == EINVAL);
		REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
				WriteTransfer{{}, blockSize, blockSize, 100}) == EINVAL);
	}
	SECTION("Starting transaction with transfer's block size less than 4 should fail with EINVAL")
	{
		uint32_t response;
		uint8_t buffer[512];
		for (size_t blockSize {}; blockSize < 4; ++blockSize)
			REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
					Transfer{buffer, sizeof(buffer), blockSize, 100}) == EINVAL);
	}
	SECTION("Starting transaction when transfer's size is not a multiple of block size should fail with EINVAL")
	{
		constexpr size_t blockSize {512};

		uint32_t response;
		uint8_t buffer[blockSize * 4];

		const size_t sizes[]
		{
				blockSize * 0 + 1,
				blockSize * 1 / 2,
				blockSize * 1 - 1,
				blockSize * 1 + 1,
				blockSize * 3 / 2,
				blockSize * 2 - 1,
				blockSize * 2 + 1,
				blockSize * 5 / 2,
				blockSize * 3 - 1,
				blockSize * 3 + 1,
				blockSize * 7 / 2,
				blockSize * 4 - 1,
		};
		for (const auto size : sizes)
			REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
					Transfer{buffer, size, blockSize, 100}) == EINVAL);
	}
	SECTION("Starting transaction with invalid transfer's block size should fail with EINVAL")
	{
		uint32_t response;
		uint8_t buffer[16384];
		const size_t blockSizes[]
		{
				4 + 1,
				8 - 1,
				8 + 1,
				16 - 1,
				16 + 1,
				32 - 1,
				32 + 1,
				64 - 1,
				64 + 1,
				128 - 1,
				128 + 1,
				256 - 1,
				256 + 1,
				512 - 1,
				512 + 1,
				1024 - 1,
				1024 + 1,
				2048 - 1,
				2048 + 1,
				4096 - 1,
				4096 + 1,
				8192 - 1,
				8192 + 1,
				16384 - 1,
		};
		for (const auto blockSize : blockSizes)
			REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
					Transfer{buffer, blockSize, blockSize, 100}) == EINVAL);
	}
	SECTION("Starting transaction with transfer's block size greater than 16384 should fail with EINVAL")
	{
		constexpr size_t blockSize {32768};

		uint32_t response;
		uint8_t buffer[blockSize];
		REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
				Transfer{buffer, sizeof(buffer), blockSize, 100}) == EINVAL);
	}
	SECTION("Starting transaction with invalid transfer's timeout should fail with EINVAL")
	{
		constexpr size_t blockSize {512};
		constexpr uint32_t otherAdapterFrequency {131072000};
		constexpr uint16_t timeoutMs {(UINT32_MAX * 1000ull + otherAdapterFrequency - 1) / otherAdapterFrequency};

		REQUIRE_CALL(peripheralMock, getAdapterFrequency()).RETURN(otherAdapterFrequency);
		REQUIRE_CALL(peripheralMock, readClkcr()).IN_SEQUENCE(sequence).RETURN(initialClkcr);
		REQUIRE_CALL(peripheralMock, writeClkcr(_)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.configure({}, otherAdapterFrequency) == 0);

		uint32_t response;
		uint8_t buffer[blockSize];
		REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
				Transfer{buffer, sizeof(buffer), blockSize, timeoutMs}) == EINVAL);
	}
	SECTION("Starting transaction with invalid transfer's size should fail with EINVAL")
	{
		constexpr size_t blockSize {512};

		uint32_t response;
		uint8_t buffer[blockSize];
		REQUIRE(sdMmc.startTransaction(cardMock, {}, {}, Response{response},
				Transfer{buffer, 1 << 25, blockSize, 100}) == EINVAL);
	}

	SECTION("Testing transactions without response")
	{
		constexpr uint8_t command {0x2e};
		constexpr uint32_t argument {0x644f3198};

		REQUIRE_CALL(peripheralMock, writeArg(argument)).IN_SEQUENCE(sequence);
		const auto cmd = SDMMC_CMD_CPSMEN | command << SDMMC_CMD_CMDINDEX_Pos;
		REQUIRE_CALL(peripheralMock, writeCmd(cmd)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeMask(SDMMC_MASK_CMDSENTIE)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.startTransaction(cardMock, command, argument, {}, {}) == 0);

		// starting another transaction when the previous one is ongoing should fail with EBUSY
		REQUIRE(sdMmc.startTransaction(cardMock, command, argument, {}, {}) == EBUSY);

		// trying to stop the driver when a transaction is ongoing should fail with EBUSY
		REQUIRE(sdMmc.stop() == EBUSY);

		REQUIRE_CALL(peripheralMock, readSta()).IN_SEQUENCE(sequence).RETURN(SDMMC_STA_CMDSENT);
		REQUIRE_CALL(peripheralMock, writeIcr(allIcrBits)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, readDctrl()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(peripheralMock, writeMask(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(cardMock, transactionCompleteEvent(Result::success)).IN_SEQUENCE(sequence);
		sdMmc.interruptHandler();
	}

	SECTION("Testing transactions with response and without transfer")
	{
		constexpr uint8_t command {0x10};
		constexpr uint32_t argument {0x24dcab5b};
		constexpr uint32_t expectedResponse[4] {0x0405cd70, 0xe22d54c9, 0xb8dc7d90, 0x54402929};

		uint32_t shortResponseBuffer[1] {};
		uint32_t longResponseBuffer[4] {};
		const Response responses[]
		{
				Response{shortResponseBuffer},
				Response{longResponseBuffer},
		};
		struct Step
		{
			uint32_t sta;
			Result result;
		};
		const Step steps[]
		{
				{SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND, Result::success},
				{SDMMC_STA_CMDSENT | SDMMC_STA_CCRCFAIL, Result::responseCrcMismatch},
				{SDMMC_STA_CMDSENT | SDMMC_STA_CTIMEOUT, Result::responseTimeout},
		};
		for (auto& response : responses)
			for (auto& step : steps)
				DYNAMIC_SECTION("Response size " << response.size() << ", STA register value " << step.sta <<
						", expected result " << static_cast<int>(step.result))
				{
					REQUIRE_CALL(peripheralMock, writeArg(argument)).IN_SEQUENCE(sequence);
					const auto cmd = SDMMC_CMD_CPSMEN |
							(response.size() == 1 ? SDMMC_CMD_WAITRESP_0 : SDMMC_CMD_WAITRESP) |
							command << SDMMC_CMD_CMDINDEX_Pos;
					REQUIRE_CALL(peripheralMock, writeCmd(cmd)).IN_SEQUENCE(sequence);
					const auto mask = SDMMC_MASK_CMDRENDIE | SDMMC_MASK_CTIMEOUTIE | SDMMC_MASK_CCRCFAILIE;
					REQUIRE_CALL(peripheralMock, writeMask(mask)).IN_SEQUENCE(sequence);
					REQUIRE(sdMmc.startTransaction(cardMock, command, argument, response, {}) == 0);

					// starting another transaction when the previous one is ongoing should fail with EBUSY
					REQUIRE(sdMmc.startTransaction(cardMock, command, argument, response, {}) == EBUSY);

					// trying to stop the driver when a transaction is ongoing should fail with EBUSY
					REQUIRE(sdMmc.stop() == EBUSY);

					REQUIRE_CALL(peripheralMock, readSta()).IN_SEQUENCE(sequence).RETURN(step.sta);
					REQUIRE_CALL(peripheralMock, writeIcr(allIcrBits)).IN_SEQUENCE(sequence);
					REQUIRE_CALL(peripheralMock, readDctrl()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(peripheralMock, writeMask(0u)).IN_SEQUENCE(sequence);
					if (response.size() == 1)
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readResp1()).IN_SEQUENCE(sequence).RETURN(expectedResponse[0]));
					else
					{
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readResp4()).IN_SEQUENCE(sequence).RETURN(expectedResponse[0]));
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readResp3()).IN_SEQUENCE(sequence).RETURN(expectedResponse[1]));
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readResp2()).IN_SEQUENCE(sequence).RETURN(expectedResponse[2]));
						expectations.emplace_back(NAMED_REQUIRE_CALL(peripheralMock,
								readResp1()).IN_SEQUENCE(sequence).RETURN(expectedResponse[3]));
					}
					REQUIRE_CALL(cardMock, transactionCompleteEvent(step.result)).IN_SEQUENCE(sequence);
					sdMmc.interruptHandler();

					for (size_t i {}; i < response.size(); ++i)
						REQUIRE(response[i] == expectedResponse[i]);
				}
	}

	SECTION("Testing transactions with response and read transfer")
	{
		constexpr uint8_t command {0x2b};
		constexpr uint32_t argument {0x7b3ee1a1};
		constexpr std::array<uint32_t, 1> expectedResponse {0xe044b672};
		constexpr uint16_t timeoutMs {321};

		struct Step
		{
			uint32_t sta;
			Result result;
		};
		const Step steps[]
		{
				{SDMMC_STA_DBCKEND | SDMMC_STA_DATAEND | SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND, Result::success},
				{SDMMC_STA_DATAEND | SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND | SDMMC_STA_DCRCFAIL,
						Result::dataCrcMismatch},
				{SDMMC_STA_DATAEND | SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND | SDMMC_STA_DTIMEOUT, Result::dataTimeout},
				{SDMMC_STA_DATAEND | SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND | SDMMC_STA_RXOVERR, Result::receiveOverrun},
				{SDMMC_STA_CMDSENT | SDMMC_STA_CCRCFAIL, Result::responseCrcMismatch},
				{SDMMC_STA_CMDSENT | SDMMC_STA_CTIMEOUT, Result::responseTimeout},
		};
		const bool falseTrue[]
		{
				false,
				true,
		};
		for (uint8_t blockSizeLog2 {2}; blockSizeLog2 <= 14; ++blockSizeLog2)
		{
			const auto blockSize = 1u << blockSizeLog2;
			for (auto& step : steps)
				for (const auto dmaError : falseTrue)
				{
					const auto result = dmaError == false ? step.result : Result::receiveOverrun;
					DYNAMIC_SECTION("Block size " << blockSize << ", STA register value " << step.sta <<
							", with" << (dmaError == false ? "out" : "") << " DMA error, expected result " <<
							static_cast<int>(result))
					{
						std::array<uint32_t, 1> response {};
						uint8_t buffer[1 << 14];

						REQUIRE_CALL(peripheralMock, getFifoAddress()).IN_SEQUENCE(sequence).RETURN(fifoAddress);
						const auto dmaFlags = Flags::transferCompleteInterruptDisable |
								Flags::peripheralFlowController |
								Flags::peripheralToMemory |
								Flags::peripheralFixed |
								Flags::memoryIncrement |
								Flags::veryHighPriority |
								Flags::dataSize4 |
								Flags::burstSize4;
						const auto addressMatcher = [&buffer](const uintptr_t address)
								{
									return address == reinterpret_cast<uintptr_t>(buffer);
								};
						REQUIRE_CALL(dmaChannelMock, startTransfer(_, fifoAddress, sizeof(buffer) / 4,
								dmaFlags)).WITH(addressMatcher(_1)).IN_SEQUENCE(sequence);
						const auto dtimer = (adapterFrequency / 257 + 1000 - 1) / 1000 * timeoutMs;
						REQUIRE_CALL(peripheralMock, writeDtimer(dtimer)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, writeDlen(sizeof(buffer))).IN_SEQUENCE(sequence);
						const auto dctrl = blockSizeLog2 << SDMMC_DCTRL_DBLOCKSIZE_Pos | SDMMC_DCTRL_DMAEN |
								SDMMC_DCTRL_DTDIR | SDMMC_DCTRL_DTEN;
						REQUIRE_CALL(peripheralMock, writeDctrl(dctrl)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, writeArg(argument)).IN_SEQUENCE(sequence);
						const auto cmd = SDMMC_CMD_CPSMEN | SDMMC_CMD_WAITRESP_0 | command << SDMMC_CMD_CMDINDEX_Pos;
						REQUIRE_CALL(peripheralMock, writeCmd(cmd)).IN_SEQUENCE(sequence);
						const auto mask = SDMMC_MASK_DATAENDIE | SDMMC_MASK_RXOVERRIE | SDMMC_MASK_DTIMEOUTIE |
								SDMMC_MASK_CTIMEOUTIE | SDMMC_MASK_DCRCFAILIE | SDMMC_MASK_CCRCFAILIE;
						REQUIRE_CALL(peripheralMock, writeMask(mask)).IN_SEQUENCE(sequence);
						REQUIRE(sdMmc.startTransaction(cardMock, command, argument, Response{response},
								ReadTransfer{buffer, sizeof(buffer), blockSize, timeoutMs}) == 0);

						// starting another transaction when the previous one is ongoing should fail with EBUSY
						REQUIRE(sdMmc.startTransaction(cardMock, command, argument, Response{response},
								ReadTransfer{buffer, sizeof(buffer), blockSize, timeoutMs}) == EBUSY);

						// trying to stop the driver when a transaction is ongoing should fail with EBUSY
						REQUIRE(sdMmc.stop() == EBUSY);

						if (dmaError == true)
							dmaChannelFunctor->transferErrorEvent(1);

						REQUIRE_CALL(peripheralMock, readSta()).IN_SEQUENCE(sequence).RETURN(step.sta);
						REQUIRE_CALL(peripheralMock, writeIcr(allIcrBits)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, readDctrl()).IN_SEQUENCE(sequence).RETURN(dctrl);
						REQUIRE_CALL(peripheralMock, writeDctrl(0u)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(dmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, writeMask(0u)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, readResp1()).IN_SEQUENCE(sequence).RETURN(expectedResponse[0]);
						REQUIRE_CALL(cardMock, transactionCompleteEvent(result)).IN_SEQUENCE(sequence);
						sdMmc.interruptHandler();

						REQUIRE(response == expectedResponse);
					}
				}
		}
	}

	SECTION("Testing transactions with response and write transfer")
	{
		constexpr uint8_t command {0x29};
		constexpr uint32_t argument {0x718a52c8};
		constexpr std::array<uint32_t, 4> expectedResponse {0xc54eb961, 0x139282ba, 0xe0d6876d, 0xfd555733};
		constexpr uint16_t timeoutMs {123};

		struct Step
		{
			uint32_t sta0;
			uint32_t sta1;
			Result result;
		};
		const Step steps[]
		{
				{SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND, SDMMC_STA_DBCKEND | SDMMC_STA_DATAEND, Result::success},
				{SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND, SDMMC_STA_DATAEND | SDMMC_STA_DCRCFAIL,
						Result::dataCrcMismatch},
				{SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND, SDMMC_STA_DATAEND | SDMMC_STA_DTIMEOUT, Result::dataTimeout},
				{SDMMC_STA_CMDSENT | SDMMC_STA_CMDREND, SDMMC_STA_DATAEND | SDMMC_STA_TXUNDERR,
						Result::transmitUnderrun},
				{{}, SDMMC_STA_CMDSENT | SDMMC_STA_CCRCFAIL, Result::responseCrcMismatch},
				{{}, SDMMC_STA_CMDSENT | SDMMC_STA_CTIMEOUT, Result::responseTimeout},
		};
		for (uint8_t blockSizeLog2 {2}; blockSizeLog2 <= 14; ++blockSizeLog2)
		{
			const auto blockSize = 1u << blockSizeLog2;
			for (auto& step : steps)
				for (uint8_t dmaError {}; dmaError <= 2; ++dmaError)
				{
					const auto result = dmaError == 0 ? step.result : Result::transmitUnderrun;
					DYNAMIC_SECTION("Block size " << blockSize << ", STA register values " << step.sta0 << ", " <<
							step.sta1 << ", with" << (dmaError == 0 ? "out" : "") <<
							" DMA error (" << static_cast<int>(dmaError) << "), expected result " <<
							static_cast<int>(result))
					{
						std::array<uint32_t, 4> response {};
						const uint8_t buffer[1 << 14] {};

						REQUIRE_CALL(peripheralMock, getFifoAddress()).IN_SEQUENCE(sequence).RETURN(fifoAddress);
						const auto dmaFlags = Flags::transferCompleteInterruptDisable |
								Flags::peripheralFlowController |
								Flags::memoryToPeripheral |
								Flags::peripheralFixed |
								Flags::memoryIncrement |
								Flags::veryHighPriority |
								Flags::dataSize4 |
								Flags::burstSize4;
						const auto addressMatcher = [&buffer](const uintptr_t address)
								{
									return address == reinterpret_cast<uintptr_t>(buffer);
								};
						REQUIRE_CALL(dmaChannelMock, startTransfer(_, fifoAddress, sizeof(buffer) / 4,
								dmaFlags)).WITH(addressMatcher(_1)).IN_SEQUENCE(sequence);
						const auto dtimer = (adapterFrequency / 257 + 1000 - 1) / 1000 * timeoutMs;
						REQUIRE_CALL(peripheralMock, writeDtimer(dtimer)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, writeDlen(sizeof(buffer))).IN_SEQUENCE(sequence);
						auto dctrl = blockSizeLog2 << SDMMC_DCTRL_DBLOCKSIZE_Pos | SDMMC_DCTRL_DMAEN;
						REQUIRE_CALL(peripheralMock, writeDctrl(dctrl)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, writeArg(argument)).IN_SEQUENCE(sequence);
						const auto cmd = SDMMC_CMD_CPSMEN | SDMMC_CMD_WAITRESP | command << SDMMC_CMD_CMDINDEX_Pos;
						REQUIRE_CALL(peripheralMock, writeCmd(cmd)).IN_SEQUENCE(sequence);
						const auto mask0 = SDMMC_MASK_CMDRENDIE | SDMMC_MASK_CTIMEOUTIE | SDMMC_MASK_CCRCFAILIE;
						REQUIRE_CALL(peripheralMock, writeMask(mask0)).IN_SEQUENCE(sequence);
						REQUIRE(sdMmc.startTransaction(cardMock, command, argument, Response{response},
								WriteTransfer{buffer, sizeof(buffer), blockSize, timeoutMs}) == 0);

						// starting another transaction when the previous one is ongoing should fail with EBUSY
						REQUIRE(sdMmc.startTransaction(cardMock, command, argument, Response{response},
								WriteTransfer{buffer, sizeof(buffer), blockSize, timeoutMs}) == EBUSY);

						// trying to stop the driver when a transaction is ongoing should fail with EBUSY
						REQUIRE(sdMmc.stop() == EBUSY);

						if (dmaError == 1)
							dmaChannelFunctor->transferErrorEvent(1);

						if (step.sta0 != 0 && dmaError != 1)
						{
							REQUIRE_CALL(peripheralMock, readSta()).IN_SEQUENCE(sequence).RETURN(step.sta0);
							REQUIRE_CALL(peripheralMock, writeIcr(allIcrBits)).IN_SEQUENCE(sequence);
							REQUIRE_CALL(peripheralMock, readDctrl()).IN_SEQUENCE(sequence).RETURN(dctrl);
							dctrl |= SDMMC_DCTRL_DTEN;
							REQUIRE_CALL(peripheralMock, writeDctrl(dctrl)).IN_SEQUENCE(sequence);
							const auto mask1 = SDMMC_MASK_DATAENDIE | SDMMC_MASK_TXUNDERRIE | SDMMC_MASK_DTIMEOUTIE |
									SDMMC_MASK_DCRCFAILIE;
							REQUIRE_CALL(peripheralMock, writeMask(mask1)).IN_SEQUENCE(sequence);
							sdMmc.interruptHandler();
						}

						if (dmaError == 2)
							dmaChannelFunctor->transferErrorEvent(1);

						REQUIRE_CALL(peripheralMock, readSta()).IN_SEQUENCE(sequence).RETURN(step.sta1);
						REQUIRE_CALL(peripheralMock, writeIcr(allIcrBits)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, readDctrl()).IN_SEQUENCE(sequence).RETURN(dctrl);
						REQUIRE_CALL(peripheralMock, writeDctrl(0u)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(dmaChannelMock, stopTransfer()).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, writeMask(0u)).IN_SEQUENCE(sequence);
						REQUIRE_CALL(peripheralMock, readResp4()).IN_SEQUENCE(sequence).RETURN(expectedResponse[0]);
						REQUIRE_CALL(peripheralMock, readResp3()).IN_SEQUENCE(sequence).RETURN(expectedResponse[1]);
						REQUIRE_CALL(peripheralMock, readResp2()).IN_SEQUENCE(sequence).RETURN(expectedResponse[2]);
						REQUIRE_CALL(peripheralMock, readResp1()).IN_SEQUENCE(sequence).RETURN(expectedResponse[3]);
						REQUIRE_CALL(cardMock, transactionCompleteEvent(result)).IN_SEQUENCE(sequence);
						sdMmc.interruptHandler();

						REQUIRE(response == expectedResponse);
					}
				}
		}
	}

	{
		REQUIRE_CALL(dmaChannelMock, release()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeMask(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeDctrl(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeCmd(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writeClkcr(0u)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(peripheralMock, writePower(0u)).IN_SEQUENCE(sequence);
		REQUIRE(sdMmc.stop() == 0);
	}
}
