/**
 * \file
 * \brief SdCard test cases
 *
 * This test checks whether SdCard performs all operations properly and in correct order.
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "distortos/devices/memory/SdCard.hpp"

#include "distortos/ThisThread.hpp"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

using ShortResponse = std::array<uint32_t, 1>;

using LongResponse = std::array<uint32_t, 4>;

using BusMode = distortos::devices::SdMmcCardLowLevel::BusMode;

using SdMmcCardBase = distortos::devices::SdMmcCardBase;

using Response = distortos::devices::SdMmcCardLowLevel::Response;

using Result = SdMmcCardBase::Result;

using Transfer = distortos::devices::SdMmcCardLowLevel::Transfer;

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

template<size_t size>
void copy(const std::array<uint32_t, size> source, Response& destination)
{
	REQUIRE(source.size() == destination.size());
	memcpy(destination.begin(), source.cbegin(), destination.size() * sizeof(destination[0]));
}

template<size_t size>
void copy(const std::array<uint8_t, size> source, Transfer& destination)
{
	REQUIRE(source.size() == destination.getSize());
	memcpy(destination.getReadBuffer(), source.begin(), destination.getSize());
}

inline auto responseMatcher(const size_t size)
{
	return trompeloeil::make_matcher<Response>(
			// predicate lambda that checks the condition
			[](const Response& response, const size_t storedSize)
			{
				return response.size() == storedSize;
			},
			// print lambda for error message
			[](std::ostream& stream, const size_t storedSize)
			{
				stream << ".size() == " << storedSize;
			},
			// stored values
			size);
}

inline auto transferMatcher(const bool writeTransfer, const size_t size, const size_t blockSize,
		const uint16_t timeoutMs)
{
	return trompeloeil::make_matcher<Transfer>(
			// predicate lambda that checks the condition
			[](const Transfer& transfer, const bool storedWriteTransfer, const size_t storedSize,
					const size_t storedBlockSize, const uint16_t storedTimeoutMs)
			{
				return transfer.isWriteTransfer() == storedWriteTransfer &&
						transfer.getSize() == storedSize &&
						transfer.getBlockSize() == storedBlockSize &&
						transfer.getTimeoutMs() == storedTimeoutMs;
			},
			// print lambda for error message
			[](std::ostream& stream, const bool storedWriteTransfer, const size_t storedSize,
					const size_t storedBlockSize, const uint16_t storedTimeoutMs)
			{
				stream << " == " << (storedWriteTransfer == false ? "Read" : "Write") << "Transfer{..., " <<
						storedSize << ", " << storedBlockSize << ", " << storedTimeoutMs << "}";
			},
			// stored values
			writeTransfer,
			size,
			blockSize,
			timeoutMs);
}

inline auto transferMatcher()
{
	return transferMatcher({}, {}, {}, {});
}

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr size_t blockSize {512};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing get*BlockSize()", "[get*BlockSize]")
{
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	SdMmcCardLowLevel sdCardLowLevelMock;
	distortos::devices::mock::SynchronousSdMmcCardLowLevel synchronousSdCardLowLevelMock;
	distortos::ThisThreadMock thisThreadMock;
	distortos::TickClock tickClockMock;
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::devices::SdCard sdCard {sdCardLowLevelMock};

	SECTION("Locking SD card should lock the mutex")
	{
		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		sdCard.lock();
	}
	SECTION("Unlocking SD card should unlock the mutex")
	{
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		sdCard.unlock();
	}
	SECTION("Low-level SD/MMC card driver start error should propagate error code to caller")
	{
		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		constexpr int ret {0x2abbc7db};
		REQUIRE_CALL(synchronousSdCardLowLevelMock, start()).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(sdCard.open() == ret);
	}
	SECTION("Testing initialized SD card")
	{
		constexpr size_t auSize {4194304};
		// constexpr uint16_t eraseTimeoutMs {2082};
		constexpr uint16_t readTimeoutMs {100};
		constexpr uint16_t writeTimeoutMs {250};

		const ShortResponse cmd8Response {0x1aa};
		const ShortResponse cmd55Response0 {0x120};
		const ShortResponse acmd41Response0 {0xff8000};
		const ShortResponse cmd7Response {0x700};
		const ShortResponse cmd55Response {0x920};
		const ShortResponse acmd6Response {0x920};
		const ShortResponse cmd16Response {0x900};
		const ShortResponse acmd13Response {0x920};
		const ShortResponse cmd13Response {0x900};

		const ShortResponse cmd12AfterCmd18Response {0xb00};
		const ShortResponse cmd12AfterCmd25Response {0xc00};
		const ShortResponse cmd17Response {0x900};
		const ShortResponse cmd18Response {0x900};
		const ShortResponse cmd24Response {0x900};
		const ShortResponse cmd25Response {0x900};
		const ShortResponse cmd32Response {0x900};
		const ShortResponse cmd33Response {0x900};
		const ShortResponse cmd38Response {0x800};
		const ShortResponse acmd23Response {0x920};

		struct CardParameters
		{
			size_t blocksCount;
			bool blockAddressing;
			ShortResponse acmd41Response1;
			LongResponse cmd2Response;
			ShortResponse cmd3Response;
			LongResponse cmd9Response;
			std::array<uint8_t, 64> acmd13Transfer;
		};
		const CardParameters cardParametersArray[]
		{
				// 2 GB SDSC Kingston microSD
				{
						3932160, false,
						{0x80ff8000},
						{0x9100832e, 0x32a9ac43, 0x44303247, 0x2544d53},
						{0xad1b0520},
						{0x16800066, 0xffffff80, 0x5b5a83bf, 0x5d0032},
						{0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x28, 0x2, 0x2, 0x90, 0x2, 0x0, 0xaa},
				},
				// 32 GB SDHC Kingston microSD
				{
						60751872, true,
						{0xc0ff8000},
						{0x4a00f9ea, 0x307cb154, 0x44333247, 0x27504853},
						{0x70500},
						{0xa40008c, 0xe7bf7f80, 0x5b590000, 0x400e0032},
						{0x80, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 0x4, 0x0, 0x90, 0x2, 0x0, 0xaa, 0x1f},
				},
		};
		for (auto& cardParameters : cardParametersArray)
		{
			const auto blockAddressing = cardParameters.blockAddressing;
			const uint32_t shiftedRca {cardParameters.cmd3Response[0] & 0xffff0000};
			DYNAMIC_SECTION("Card parameters: RCA " << (shiftedRca >> 16) << ", " << cardParameters.blocksCount <<
					" blocks, " << (blockAddressing == false ? "byte" : "block") << " addressing")
			{
				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(synchronousSdCardLowLevelMock, start()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(synchronousSdCardLowLevelMock, configure(BusMode::_1Bit, 400000u))
						.IN_SEQUENCE(sequence);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(0u, 0u, responseMatcher(0), transferMatcher())).IN_SEQUENCE(sequence)
						.RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(8u, 0x1aau, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(cmd8Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence).RETURN(distortos::TickClock::time_point{});
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(55u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(cmd55Response0, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(41u, 0x50ff8000u, responseMatcher(1), transferMatcher()))
						.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(acmd41Response0, _3))
						.RETURN(Result::responseCrcMismatch);
				REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence).RETURN(distortos::TickClock::time_point{});
				REQUIRE_CALL(thisThreadMock, sleepFor(distortos::TickClock::duration{})).IN_SEQUENCE(sequence)
						.RETURN(0);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(55u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(cmd55Response0, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(41u, 0x50ff8000u, responseMatcher(1), transferMatcher()))
						.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cardParameters.acmd41Response1, _3))
						.RETURN(Result::responseCrcMismatch);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(2u, 0u, responseMatcher(4), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(cardParameters.cmd2Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(3u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(cardParameters.cmd3Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(9u, shiftedRca, responseMatcher(4), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(cardParameters.cmd9Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(7u, shiftedRca, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(cmd7Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
						.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd55Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(6u, 2u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(acmd6Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock, configure(BusMode::_4Bit, 25000000u)).IN_SEQUENCE(sequence);
				if (blockAddressing == false)
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(16u, blockSize, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd16Response, _3)).RETURN(Result::success));
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
						.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd55Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(13u, 0u, responseMatcher(1), transferMatcher(false,
						sizeof(cardParameters.acmd13Transfer), sizeof(cardParameters.acmd13Transfer), readTimeoutMs)))
						.IN_SEQUENCE(sequence)
						.SIDE_EFFECT(copy(acmd13Response, _3); copy(cardParameters.acmd13Transfer, _4))
						.RETURN(Result::success);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

				REQUIRE(sdCard.open() == 0);

				SECTION("Additional open()/close() calls should perform no action")
				{
					constexpr size_t count {17};
					for (size_t i {}; i < count; ++i)
					{
						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(sdCard.open() == 0);
					}
					for (size_t i {}; i < count; ++i)
					{
						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(sdCard.close() == 0);
					}
				}
				SECTION("Testing getBlockSize()")
				{
					REQUIRE(sdCard.getBlockSize() == blockSize);
				}
				SECTION("Testing getSize()")
				{
					REQUIRE(sdCard.getSize() == cardParameters.blocksCount * blockSize);
				}
				SECTION("Testing synchronize()")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.synchronize() == 0);
				}
				SECTION("Testing synchronize() failure: CMD13 response timeout")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).RETURN(Result::responseTimeout);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.synchronize() == ETIMEDOUT);
				}
				SECTION("Testing synchronize() failure: CMD13 response CRC mismatch")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).RETURN(Result::responseCrcMismatch);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.synchronize() == EIO);
				}
				SECTION("Testing synchronize() failure: error bit set in CMD13 R1 response")
				{
					const uint8_t errorBits[]
					{
							3,
							15,
							16,
							19,
							20,
							21,
							22,
							23,
							24,
							26,
							27,
							28,
							29,
							30,
							31,
					};
					for (const auto errorBit : errorBits)
					{
						const ShortResponse r1Response {static_cast<uint32_t>(1u << errorBit)};

						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(synchronousSdCardLowLevelMock,
								executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
								.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(r1Response, _3)).RETURN(Result::success);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

						REQUIRE(sdCard.synchronize() == EIO);
					}
				}
				SECTION("Testing synchronize() failure: invalid card state in CMD13 R1 response")
				{
					const uint8_t states[]
					{
							0,		// idle
							1,		// ready
							2,		// identification (ident)
							3,		// standby (stby)
							8,		// disconnect (dis)
							9,		// reserved
							10,		// reserved
							11,		// reserved
							12,		// reserved
							13,		// reserved
							14,		// reserved
							15,		// reserved for I/O mode
					};
					for (const auto state : states)
					{
						const ShortResponse r1Response {static_cast<uint32_t>(state) << 9};

						REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(synchronousSdCardLowLevelMock,
								executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
								.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(r1Response, _3)).RETURN(Result::success);
						REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

						REQUIRE(sdCard.synchronize() == EIO);
					}
				}
				SECTION("Testing read() of 0 bytes")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					uint8_t dummy;
					REQUIRE(sdCard.read(0, &dummy, 0) == 0);
				}
				SECTION("Testing read() of 1 block")
				{
					uint8_t buffer[blockSize];
					constexpr uint16_t block {0xfb0c};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(17u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(false, sizeof(buffer), blockSize, readTimeoutMs)))
							.LR_WITH(_4.getReadBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd17Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == 0);

					SECTION("Testing synchronize() failure: timeout while waiting for transfer card state")
					{
						const uint32_t states[]
						{
								5,		// sending data (data)
								6,		// receive data (rcv)
								7,		// programming (prg)
						};
						for (const auto state : states)
						{
							const ShortResponse r1Response {state << 9};

							REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(synchronousSdCardLowLevelMock,
									executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
									.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(r1Response, _3)).RETURN(Result::success);
							REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
									.RETURN(distortos::TickClock::time_point{});
							REQUIRE_CALL(thisThreadMock, sleepFor(distortos::TickClock::duration{}))
									.IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(synchronousSdCardLowLevelMock,
									executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
									.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(r1Response, _3)).RETURN(Result::success);
							REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
									.RETURN(distortos::TickClock::time_point{std::chrono::milliseconds{readTimeoutMs}});
							REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

							REQUIRE(sdCard.synchronize() == ETIMEDOUT);
						}
					}
				}
				SECTION("Testing read() of 3 blocks")
				{
					uint8_t buffer[blockSize * 3];
					constexpr uint16_t block {0x45ee};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(18u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(false, sizeof(buffer), blockSize, readTimeoutMs)))
							.LR_WITH(_4.getReadBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd18Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd12AfterCmd18Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == 0);
				}
				SECTION("Testing whether a multi block read which includes the last block of the card ignores an "
					"OUT_OF_RANGE error in CMD12 R1 response")
				{
					constexpr size_t blocks {3};
					uint8_t buffer[blockSize * blocks];
					const uint32_t block = cardParameters.blocksCount - blocks;
					const uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(18u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(false, sizeof(buffer), blockSize, readTimeoutMs)))
							.LR_WITH(_4.getReadBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd18Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(ShortResponse{1u << 31}, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == 0);
				}
				SECTION("Testing read() failure: CMD13 response CRC mismatch")
				{
					uint8_t buffer[blockSize];
					constexpr uint16_t block {0x4def};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).RETURN(Result::responseCrcMismatch);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == EIO);
				}
				SECTION("Testing read() failure: CMD17 data timeout")
				{
					uint8_t buffer[blockSize];
					constexpr uint16_t block {0x16ec};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(17u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(false, sizeof(buffer), blockSize, readTimeoutMs)))
							.LR_WITH(_4.getReadBuffer() == buffer).IN_SEQUENCE(sequence).RETURN(Result::dataTimeout);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseCrcMismatch);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == ETIMEDOUT);
				}
				SECTION("Testing read() failure: error bit set in CMD17 R1 response")
				{
					uint8_t buffer[blockSize];
					constexpr uint16_t block {0xc78c};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(17u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(false, sizeof(buffer), blockSize, readTimeoutMs)))
							.LR_WITH(_4.getReadBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(ShortResponse{1u << 30}, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseTimeout);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == EIO);
				}
				SECTION("Testing read() failure: CMD12 response timeout")
				{
					uint8_t buffer[blockSize * 3];
					constexpr uint16_t block {0xc732};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(18u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(false, sizeof(buffer), blockSize, readTimeoutMs)))
							.LR_WITH(_4.getReadBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd18Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseTimeout);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == ETIMEDOUT);
				}
				SECTION("Testing read() failure: error bit set in CMD12 R1 response")
				{
					uint8_t buffer[blockSize * 3];
					constexpr uint16_t block {0x2073};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(18u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(false, sizeof(buffer), blockSize, readTimeoutMs)))
							.LR_WITH(_4.getReadBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd18Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(ShortResponse{1u << 29}, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.read(address, buffer, sizeof(buffer)) == EIO);
				}
				SECTION("Testing write() of 0 bytes")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					const uint8_t dummy {};
					REQUIRE(sdCard.write(0, &dummy, 0) == 0);
				}
				SECTION("Testing write() of 1 block")
				{
					const uint8_t buffer[blockSize] {};
					constexpr uint16_t block {0x7d0e};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(24u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(true, sizeof(buffer), blockSize, writeTimeoutMs)))
							.LR_WITH(_4.getWriteBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd24Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == 0);
				}
				SECTION("Testing write() of 5 blocks")
				{
					const uint8_t buffer[blockSize * 5] {};
					constexpr uint16_t block {0x2475};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd55Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(23u, sizeof(buffer) / blockSize, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(acmd23Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(25u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(true, sizeof(buffer), blockSize, writeTimeoutMs)))
							.LR_WITH(_4.getWriteBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd25Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd12AfterCmd25Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == 0);
				}
				SECTION("Testing write() failure: CMD13 response timeout")
				{
					const uint8_t buffer[blockSize] {};
					constexpr uint16_t block {0xe76a};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).RETURN(Result::responseTimeout);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == ETIMEDOUT);
				}
				SECTION("Testing write() failure: ACMD23 response timeout")
				{
					const uint8_t buffer[blockSize * 5] {};
					constexpr uint16_t block {0xcdca};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd55Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(23u, sizeof(buffer) / blockSize, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).RETURN(Result::responseTimeout);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == ETIMEDOUT);
				}
				SECTION("Testing write() failure: error bit set in ACMD23 R1 response")
				{
					const uint8_t buffer[blockSize * 5] {};
					constexpr uint16_t block {0x9e9b};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd55Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(23u, sizeof(buffer) / blockSize, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(ShortResponse{1u << 28}, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == EIO);
				}
				SECTION("Testing write() failure: CMD24 response timeout")
				{
					const uint8_t buffer[blockSize] {};
					constexpr uint16_t block {0xaf81};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(24u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(true, sizeof(buffer), blockSize, writeTimeoutMs)))
							.LR_WITH(_4.getWriteBuffer() == buffer).IN_SEQUENCE(sequence)
							.RETURN(Result::responseTimeout);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseCrcMismatch);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == ETIMEDOUT);
				}
				SECTION("Testing write() failure: error bit set in CMD24 R1 response")
				{
					const uint8_t buffer[blockSize] {};
					constexpr uint16_t block {0xa97a};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(24u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(true, sizeof(buffer), blockSize, writeTimeoutMs)))
							.LR_WITH(_4.getWriteBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(ShortResponse{1u << 27}, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseTimeout);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == EIO);
				}
				SECTION("Testing write() failure: CMD12 response timeout")
				{
					const uint8_t buffer[blockSize * 5] {};
					constexpr uint16_t block {0x7d2f};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd55Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(23u, sizeof(buffer) / blockSize, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(acmd23Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(25u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(true, sizeof(buffer), blockSize, writeTimeoutMs)))
							.LR_WITH(_4.getWriteBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd25Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseTimeout);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == ETIMEDOUT);
				}
				SECTION("Testing write() failure: error bit set in CMD12 R1 response")
				{
					const uint8_t buffer[blockSize * 5] {};
					constexpr uint16_t block {0x8951};
					constexpr uint64_t address {block * blockSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd55Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(23u, sizeof(buffer) / blockSize, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(acmd23Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(25u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher(true, sizeof(buffer), blockSize, writeTimeoutMs)))
							.LR_WITH(_4.getWriteBuffer() == buffer).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd25Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(12u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(ShortResponse{1u << 26}, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.write(address, buffer, sizeof(buffer)) == EIO);
				}
				SECTION("Testing erase() of 0 bytes")
				{
					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE(sdCard.erase(0x5b17 * blockSize, 0) == 0);
				}
				SECTION("Testing erase() of 1 AU")
				{
					constexpr uint64_t address {0x7aull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd32Response, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(33u,
							(address + size - blockSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd33Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(38u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd38Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == 0);
				}
				SECTION("Testing erase() of 2 AUs")
				{
					constexpr uint64_t address {0x20ull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize * 2};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd32Response, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(33u,
							(address + auSize - blockSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd33Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(38u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd38Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, (address + auSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd32Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(33u,
							(address + size - blockSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd33Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(38u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd38Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{});
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == 0);
				}
				SECTION("Testing erase() failure: CMD13 response CRC mismatch")
				{
					constexpr uint64_t address {0xcfull * auSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).RETURN(Result::responseCrcMismatch);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == EIO);
				}
				SECTION("Testing erase() failure: CMD32 response timeout")
				{
					constexpr uint64_t address {0xcaull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).RETURN(Result::responseTimeout);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == ETIMEDOUT);
				}
				SECTION("Testing erase() failure: error bit set in CMD32 R1 response")
				{
					constexpr uint64_t address {0x69ull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).SIDE_EFFECT(copy(ShortResponse{1u << 24}, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == EIO);
				}
				SECTION("Testing erase() failure: CMD33 response timeout")
				{
					constexpr uint64_t address {0xb3ull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd32Response, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(33u,
							(address + size - blockSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseTimeout);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == ETIMEDOUT);
				}
				SECTION("Testing erase() failure: error bit set in CMD33 R1 response")
				{
					constexpr uint64_t address {0x33ull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd32Response, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(33u,
							(address + size - blockSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(ShortResponse{1u << 23}, _3)).RETURN(Result::success);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == EIO);
				}
				SECTION("Testing erase() failure: CMD38 response timeout")
				{
					constexpr uint64_t address {0xffull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd32Response, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(33u,
							(address + size - blockSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd33Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(38u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::responseTimeout);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == ETIMEDOUT);
				}
				SECTION("Testing erase() failure: error bit set in CMD38 R1 response")
				{
					constexpr uint64_t address {0x90ull * auSize};
					constexpr uint32_t block {address / blockSize};
					constexpr uint64_t size {auSize};

					REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(32u, blockAddressing == false ? address : block, responseMatcher(1),
							transferMatcher())).IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd32Response, _3))
							.RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(33u,
							(address + size - blockSize) / (blockAddressing == false ? 1 : blockSize),
							responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(cmd33Response, _3)).RETURN(Result::success);
					REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(38u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(ShortResponse{1u << 22}, _3)).RETURN(Result::success);
					REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

					REQUIRE(sdCard.erase(address, size) == EIO);
				}

				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(synchronousSdCardLowLevelMock,
						executeTransaction(13u, shiftedRca, responseMatcher(1), transferMatcher()))
						.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(cmd13Response, _3)).RETURN(Result::success);
				REQUIRE_CALL(synchronousSdCardLowLevelMock, stop()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

				REQUIRE(sdCard.close() == 0);
			}
		}
	}
	{
		const ShortResponse cmd8Response {0x1aa};
		const ShortResponse cmd55Response0 {0x120};
		const ShortResponse acmd41Response0 {0xff8000};
		const ShortResponse cmd3Response {0xad1b0520};
		const LongResponse cmd9Response {0x16800066, 0xffffff80, 0x5b5a83bf, 0x5d0032};
		const ShortResponse cmd7Response {0x700};
		const ShortResponse cmd55Response {0x920};
		const ShortResponse acmd6Response {0x920};
		const ShortResponse cmd16Response {0x900};
		const ShortResponse acmd13Response {0x920};

		struct Step
		{
			const char* description;
			int ret;
			size_t loops;
			Result cmd8Result;
			ShortResponse cmd8Response;
			bool continueAfterCmd8;

			Result cmd55Result0 {};
			ShortResponse cmd55Response0 {};
			bool continueAfterCmd55_0 {};

			Result acmd41Result0 {};
			ShortResponse acmd41Response0 {};
			bool continueAfterAcmd41_0 {};

			ShortResponse acmd41Response1 {};
			distortos::TickClock::time_point timePoint1 {};
			bool continueAfterAcmd41_1 {};

			Result cmd2Result {};
			bool continueAfterCmd2 {};

			Result cmd3Result {};
			ShortResponse cmd3Response {};
			bool continueAfterCmd3 {};

			Result cmd9Result {};
			LongResponse cmd9Response {};
			bool continueAfterCmd9 {};

			Result cmd7Result {};
			ShortResponse cmd7Response {};
			bool continueAfterCmd7 {};

			Result cmd55Result1 {};
			ShortResponse cmd55Response1 {};
			bool continueAfterCmd55_1 {};

			Result acmd6Result {};
			ShortResponse acmd6Response {};
			bool continueAfterAcmd6 {};

			Result cmd16Result {};
			ShortResponse cmd16Response {};
			bool continueAfterCmd16 {};

			Result acmd13Result {};
			ShortResponse acmd13Response {};
			std::array<uint8_t, 64> acmd13Transfer {};
		};
		const Step steps[]
		{
				{"CMD8 response timeout",
						ETIMEDOUT, 100,
						Result::responseTimeout, {}, true},
				{"\"no match\" (first variant) CMD8 response",
 						ENOTSUP, 1,
						Result::success, {0x0}, true},
				{"\"no match\" (second variant) CMD8 response",
 						ENOTSUP, 1,
						Result::success, {0xaa}, true},
				{"\"no match\" (third variant) CMD8 response",
 						ENOTSUP, 1,
						Result::success, {0x100}, true},
				{"first CMD55 response timeout",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::responseTimeout, {}, true},
				{"error bit set in first CMD55 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, {1u << 21}, true},
				{"ACMD41 response timeout",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseTimeout, {}, true},
				{"timeout while waiting for card to finish ACMD41 initialization",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, distortos::TickClock::time_point{std::chrono::seconds{1}}, true},
				{"CMD2 response CRC mismatch",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::responseCrcMismatch, true},
				{"CMD3 response timeout",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::responseTimeout, {}, true},
				{"error bit set in CMD3 R6 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, {1u << 20}, true},
				{"invalid card state in CMD3 R6 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, {}, true},
				{"CMD9 response timeout",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::responseTimeout, {}, true},
				{"invalid CSD_STRUCTURE bit field value in CMD9 R2 response",
 						ENOTSUP, 1,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, {0x16800066, 0xffffff80, 0x5b5a83bf, 0xd05d0032}, true},
				{"unsupported ERASE_BLK_EN bit field value in CMD9 R2 response",
 						ENOTSUP, 1,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, {0x16800066, 0xffffbf80, 0x5b5a83bf, 0x5d0032}, true},
				{"CMD7 response CRC mismatch",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::responseCrcMismatch, {}, true},
				{"error bit set in CMD7 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, {1u << 19}, true},
				{"invalid card state in CMD7 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, {2 << 9}, true},
				{"second CMD55 response timeout",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::responseTimeout, {}, true},
				{"error bit set in second CMD55 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, {1u << 16}, true},
				{"ACMD6 response timeout",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::responseTimeout, {}, true},
				{"error bit set in ACMD6 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, {1u << 15}, true},
				{"invalid card state in ACMD6 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, {3 << 9}, true},
				{"CMD16 response CRC mismatch",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::responseCrcMismatch, {}, true},
				{"error bit set in CMD16 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, {1u << 3}, true},
				{"invalid card state in CMD16 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, {5 << 9}, true},
				{"ACMD13 data timeout",
 						ETIMEDOUT, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, cmd16Response, false,
						Result::dataTimeout, {}, {}},
				{"error bit set in ACMD13 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, cmd16Response, false,
						Result::success, {1u << 31}, {}},
				{"invalid card state in ACMD13 R1 response",
 						EIO, 100,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, cmd16Response, false,
						Result::success, {6 << 9}, {}},
				{"unsupported AU_SIZE bit field value in SD status",
 						ENOTSUP, 1,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, cmd16Response, false,
						Result::success, acmd13Response,
						{0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x28, 0x2, 0x2, 0x00, 0x2, 0x0, 0xaa}},
				{"unsupported ERASE_SIZE bit field value in SD status",
 						ENOTSUP, 1,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, cmd16Response, false,
						Result::success, acmd13Response,
						{0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x28, 0x2, 0x2, 0x90, 0x0, 0x0, 0xaa}},
				{"unsupported ERASE_TIMEOUT bit field value in SD status",
 						ENOTSUP, 1,
						Result::success, cmd8Response, false,
						Result::success, cmd55Response0, false,
						Result::responseCrcMismatch, acmd41Response0, false,
						acmd41Response0, {}, false,
						Result::success, false,
						Result::success, cmd3Response, false,
						Result::success, cmd9Response, false,
						Result::success, cmd7Response, false,
						Result::success, cmd55Response, false,
						Result::success, acmd6Response, false,
						Result::success, cmd16Response, false,
						Result::success, acmd13Response,
						{0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x28, 0x2, 0x2, 0x90, 0x2, 0x0, 0x2}},
		};
		for (auto& step : steps)
		{
			DYNAMIC_SECTION("Failure during initialization: " << step.description)
			{
				REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(synchronousSdCardLowLevelMock, start()).IN_SEQUENCE(sequence).RETURN(0);

				for (size_t i {}; i < step.loops; ++i)
				{
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							configure(BusMode::_1Bit, 400000u)).IN_SEQUENCE(sequence));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(0u, 0u, responseMatcher(0), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(Result::success));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(8u, 0x1aau, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(step.cmd8Response, _3)).RETURN(step.cmd8Result));

					if (step.continueAfterCmd8 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{}));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(step.cmd55Response0, _3)).RETURN(step.cmd55Result0));

					if (step.continueAfterCmd55_0 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(41u, 0x50ff8000u, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(step.acmd41Response0, _3))
							.RETURN(step.acmd41Result0));

					if (step.continueAfterAcmd41_0 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(distortos::TickClock::time_point{}));
					expectations.emplace_back(NAMED_REQUIRE_CALL(thisThreadMock,
							sleepFor(distortos::TickClock::duration{})).IN_SEQUENCE(sequence).RETURN(0));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(step.cmd55Response0, _3)).RETURN(step.cmd55Result0));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(41u, 0x50ff8000u, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(step.acmd41Response1, _3))
							.RETURN(Result::responseCrcMismatch));
					expectations.emplace_back(NAMED_REQUIRE_CALL(tickClockMock, nowMock()).IN_SEQUENCE(sequence)
							.RETURN(step.timePoint1));

					if (step.continueAfterAcmd41_1 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(thisThreadMock,
							sleepFor(distortos::TickClock::duration{})).IN_SEQUENCE(sequence).RETURN(0));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(step.cmd55Response0, _3)).RETURN(step.cmd55Result0));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(41u, 0x50ff8000u, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(ShortResponse{0x80ff8000}, _3))
							.RETURN(Result::responseCrcMismatch));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(2u, 0u, responseMatcher(4), transferMatcher())).IN_SEQUENCE(sequence)
							.RETURN(step.cmd2Result));

					if (step.continueAfterCmd2 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(3u, 0u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(step.cmd3Response, _3)).RETURN(step.cmd3Result));

					if (step.continueAfterCmd3 == true)
						continue;

					const auto shiftedRca = step.cmd3Response[0] & 0xffff0000;
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(9u, shiftedRca, responseMatcher(4), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(step.cmd9Response, _3)).RETURN(step.cmd9Result));

					if (step.continueAfterCmd9 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(7u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(step.cmd7Response, _3)).RETURN(step.cmd7Result));

					if (step.continueAfterCmd7 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(step.cmd55Response1, _3))
							.RETURN(step.cmd55Result1));

					if (step.continueAfterCmd55_1 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(6u, 2u, responseMatcher(1), transferMatcher())).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(step.acmd6Response, _3)).RETURN(step.acmd6Result));

					if (step.continueAfterAcmd6 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							configure(BusMode::_4Bit, 25000000u)).IN_SEQUENCE(sequence));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(16u, blockSize, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(step.cmd16Response, _3)).RETURN(step.cmd16Result));

					if (step.continueAfterCmd16 == true)
						continue;

					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(55u, shiftedRca, responseMatcher(1), transferMatcher()))
							.IN_SEQUENCE(sequence).SIDE_EFFECT(copy(step.cmd55Response1, _3))
							.RETURN(step.cmd55Result1));
					expectations.emplace_back(NAMED_REQUIRE_CALL(synchronousSdCardLowLevelMock,
							executeTransaction(13u, 0u, responseMatcher(1),
							transferMatcher(false, sizeof(step.acmd13Transfer), sizeof(step.acmd13Transfer), 100)))
							.IN_SEQUENCE(sequence)
							.SIDE_EFFECT(copy(step.acmd13Response, _3); copy(step.acmd13Transfer, _4))
							.RETURN(step.acmd13Result));
				}

				REQUIRE_CALL(synchronousSdCardLowLevelMock, stop()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

				REQUIRE(sdCard.open() == step.ret);
			}
		}
	}
}
