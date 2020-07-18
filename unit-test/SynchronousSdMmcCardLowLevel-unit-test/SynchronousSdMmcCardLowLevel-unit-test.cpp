/**
 * \file
 * \brief SynchronousSdMmcCardLowLevel test cases
 *
 * This test checks whether SynchronousSdMmcCardLowLevel perform all operations properly and in correct order.
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "distortos/devices/memory/SynchronousSdMmcCardLowLevel.hpp"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

using BusMode = distortos::devices::SdMmcCardLowLevel::BusMode;

using ReadTransfer = distortos::devices::SdMmcCardLowLevel::ReadTransfer;

using Response = distortos::devices::SdMmcCardLowLevel::Response;

using Result = distortos::devices::SdMmcCardBase::Result;

using SdMmcCardBase = distortos::devices::SdMmcCardBase;

using Transfer = distortos::devices::SdMmcCardLowLevel::Transfer;

using WriteTransfer = distortos::devices::SdMmcCardLowLevel::WriteTransfer;

class SdMmcCardLowLevel : public distortos::devices::SdMmcCardLowLevel
{
public:

	MAKE_MOCK2(configure, void(BusMode, uint32_t), override);
	MAKE_MOCK0(start, int(), override);
	MAKE_MOCK5(startTransaction, void(SdMmcCardBase&, uint8_t, uint32_t, Response, Transfer), override);
	MAKE_MOCK0(stop, void(), override);
};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

bool operator==(const Response& left, const Response& right)
{
	if (left.size() != right.size())
		return false;
	if (left.begin() != right.begin())
		return false;
	return true;
}

bool operator==(const Transfer& left, const Transfer& right)
{
	if (left.getSize() != right.getSize())
		return false;
	if (left.getBlockSize() != right.getBlockSize())
		return false;
	if (left.isWriteTransfer() != right.isWriteTransfer())
		return false;
	if (left.getTimeoutMs() != right.getTimeoutMs())
		return false;
	if (left.isWriteTransfer() == false && left.getReadBuffer() != right.getReadBuffer())
		return false;
	if (left.isWriteTransfer() == true && left.getWriteBuffer() != right.getWriteBuffer())
		return false;
	return true;
}

void notify(SdMmcCardBase& sdMmcCardBase, const Result result)
{
	sdMmcCardBase.transactionCompleteEvent(result);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing configure()", "[configure]")
{
	SdMmcCardLowLevel sdMmcCardLowLevelMock;
	trompeloeil::sequence sequence {};

	distortos::devices::SynchronousSdMmcCardLowLevel sdMmcCard {sdMmcCardLowLevelMock};

	const BusMode busModes[]
	{
			BusMode::_1Bit,
			BusMode::_4Bit,
	};
	const uint32_t clockFrequencies[]
	{
			0x0ccf9226,
			0x2e14b347,
			0x47e127e5,
			0xe5836d0d,
	};
	for (const auto busMode : busModes)
		for (const auto clockFrequency : clockFrequencies)
		{
			REQUIRE_CALL(sdMmcCardLowLevelMock, configure(busMode, clockFrequency)).IN_SEQUENCE(sequence);
			sdMmcCard.configure(busMode, clockFrequency);
		}
}

TEST_CASE("Testing executeTransaction()", "[executeTransaction]")
{
	SdMmcCardLowLevel sdMmcCardLowLevelMock;
	distortos::mock::Semaphore semaphoreMock {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::devices::SynchronousSdMmcCardLowLevel sdMmcCard {sdMmcCardLowLevelMock};

	const Result results[]
	{
			Result::success,
			Result::responseTimeout,
			Result::responseCrcMismatch,
			Result::dataTimeout,
			Result::dataCrcMismatch,
			Result::transmitUnderrun,
			Result::receiveOverrun,
	};
	std::array<uint32_t, 1> shortResponseBuffer;
	std::array<uint32_t, 4> longResponseBuffer;
	const Response responses[]
	{
			Response{},
			Response{shortResponseBuffer},
			Response{longResponseBuffer},
	};
	constexpr size_t blockSize {512};
	uint8_t readTransferBuffer[blockSize * 4];
	const uint8_t writeTransferBuffer[blockSize * 2] {};
	const Transfer transfers[]
	{
			Transfer{},
			ReadTransfer{readTransferBuffer, sizeof(readTransferBuffer), blockSize, 0xf510},
			WriteTransfer{writeTransferBuffer, sizeof(writeTransferBuffer), blockSize, 0x734d},
	};
	const size_t interruptCounts[]
	{
			0,
			2,
			7,
	};
	for (const auto result : results)
		for (auto& response : responses)
			for (auto& transfer : transfers)
				for (const auto interruptCount : interruptCounts)
				{
					constexpr uint8_t command {0x24};
					constexpr uint32_t argument {0x6be74670};

					REQUIRE_CALL(sdMmcCardLowLevelMock, startTransaction(_, command, argument, response,
							transfer)).IN_SEQUENCE(sequence).LR_SIDE_EFFECT(notify(_1, result));
					REQUIRE_CALL(semaphoreMock, post()).IN_SEQUENCE(sequence).RETURN(0);
					for (size_t i {}; i < interruptCount; ++i)
						expectations.emplace_back(NAMED_REQUIRE_CALL(semaphoreMock,
								wait()).IN_SEQUENCE(sequence).RETURN(EINTR));
					REQUIRE_CALL(semaphoreMock, wait()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdMmcCard.executeTransaction(command, argument, response, transfer) == result);
				}
}

TEST_CASE("Testing start()", "[start]")
{
	SdMmcCardLowLevel sdMmcCardLowLevelMock;
	trompeloeil::sequence sequence {};

	distortos::devices::SynchronousSdMmcCardLowLevel sdMmcCard {sdMmcCardLowLevelMock};

	const int rets[]
	{
			0x5005a406,
			0x423b8fcc,
			0x756618bd,
			0x088cc96d,
	};
	for (const auto ret : rets)
	{
		REQUIRE_CALL(sdMmcCardLowLevelMock, start()).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE(sdMmcCard.start() == ret);
	}
}

TEST_CASE("Testing stop()", "[stop]")
{
	SdMmcCardLowLevel sdMmcCardLowLevelMock;
	trompeloeil::sequence sequence {};

	distortos::devices::SynchronousSdMmcCardLowLevel sdMmcCard {sdMmcCardLowLevelMock};

	REQUIRE_CALL(sdMmcCardLowLevelMock, stop()).IN_SEQUENCE(sequence);
	sdMmcCard.stop();
}
