/**
 * \file
 * \brief BlockDeviceToMemoryTechnologyDevice test cases
 *
 * This test checks whether BlockDeviceToMemoryTechnologyDevice perform all operations properly and in correct order.
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "distortos/devices/memory/BlockDevice.hpp"
#include "distortos/devices/memory/BlockDeviceToMemoryTechnologyDevice.hpp"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class BlockDevice : public distortos::devices::BlockDevice
{
public:

	MAKE_MOCK0(close, int());
	MAKE_MOCK2(erase, int(uint64_t, uint64_t));
	MAKE_CONST_MOCK0(getBlockSize, size_t());
	MAKE_CONST_MOCK0(getSize, uint64_t());
	MAKE_MOCK0(lock, int());
	MAKE_MOCK0(open, int());
	MAKE_MOCK3(read, int(uint64_t, void*, size_t));
	MAKE_MOCK0(synchronize, int());
	MAKE_MOCK0(unlock, int());
	MAKE_MOCK3(write, int(uint64_t, const void*, size_t));
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr size_t blockSize {1};
constexpr uint64_t deviceSize {UINT64_MAX};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing get*BlockSize()", "[get*BlockSize]")
{
	BlockDevice blockDeviceMock;
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::devices::BlockDeviceToMemoryTechnologyDevice bd2Mtd {blockDeviceMock};

	constexpr size_t anotherBlockSize {0x91961ea6};
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(anotherBlockSize);
	REQUIRE(bd2Mtd.getEraseBlockSize() == anotherBlockSize);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(anotherBlockSize);
	REQUIRE(bd2Mtd.getProgramBlockSize() == anotherBlockSize);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(anotherBlockSize);
	REQUIRE(bd2Mtd.getReadBlockSize() == anotherBlockSize);

	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0));
	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0));
}

TEST_CASE("Testing getSize()", "[getSize]")
{
	BlockDevice blockDeviceMock;
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::devices::BlockDeviceToMemoryTechnologyDevice bd2Mtd {blockDeviceMock};

	constexpr size_t size {0x028660de};
	REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(size);
	REQUIRE(bd2Mtd.getSize() == size);

	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0));
	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0));
}

TEST_CASE("Testing open() & close()", "[open/close]")
{
	BlockDevice blockDeviceMock;
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::devices::BlockDeviceToMemoryTechnologyDevice bd2Mtd {blockDeviceMock};

	SECTION("Block device open error should propagate error code to caller")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		constexpr int ret {0x79500842};
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.open() == ret);
	}
	SECTION("Opening closed device should succeed")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.open() == 0);

		SECTION("Block device close error should propagate error code to caller")
		{
			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			constexpr int ret {0x58516981};
			REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(ret);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.close() == ret);
		}
		SECTION("Opening device too many times should fail with EMFILE")
		{
			size_t openCount {1};
			while (openCount < UINT8_MAX)
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(bd2Mtd.open() == 0);
				++openCount;
			}

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.open() == EMFILE);

			while (openCount > 1)
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(bd2Mtd.close() == 0);
				--openCount;
			}
		}

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.close() == 0);
	}
	SECTION("Last close of the device should flush any pending erase")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.open() == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.open() == 0);

		constexpr uint64_t address {0xb03505570b02d81d};
		constexpr uint64_t size {0x3399deb8};

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.erase(address, size) == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.close() == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		constexpr int ret {0x591eb333};
		REQUIRE_CALL(blockDeviceMock, erase(address, size)).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.close() == ret);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, erase(address, size)).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.close() == 0);
	}

	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0));
	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0));
}

TEST_CASE("Testing synchronize()", "[synchronize]")
{
	BlockDevice blockDeviceMock;
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::devices::BlockDeviceToMemoryTechnologyDevice bd2Mtd {blockDeviceMock};

	constexpr int ret {0x79910589};
	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(ret);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE(bd2Mtd.synchronize() == ret);

	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0));
	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0));
}

TEST_CASE("Testing erase(), program() & read()", "[erase/program/read]")
{
	BlockDevice blockDeviceMock;
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	distortos::devices::BlockDeviceToMemoryTechnologyDevice bd2Mtd {blockDeviceMock};

	SECTION("Erasing/programming/reading closed device should fail with EBADF")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

		SECTION("Erasing closed device should fail with EBADF")
		{
			REQUIRE(bd2Mtd.erase({}, 1) == EBADF);
		}
		SECTION("Programming closed device should fail with EBADF")
		{
			REQUIRE(bd2Mtd.program({}, {}, 1) == EBADF);
		}
		SECTION("Reading closed device should fail with EBADF")
		{
			REQUIRE(bd2Mtd.read({}, {}, 1) == EBADF);
		}
	}
	SECTION("Testing erase/program/read")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.open() == 0);

		uint8_t buffer[1] {};

		SECTION("Erasing/programming/reading zero bytes should succeed")
		{
			constexpr uint64_t address {0x536b92a6a5f7b5f9};

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

			SECTION("Erasing zero bytes should succeed")
			{
				REQUIRE(bd2Mtd.erase(address, {}) == 0);
			}
			SECTION("Programming zero bytes should succeed")
			{
				REQUIRE(bd2Mtd.program(address, buffer, {}) == 0);
			}
			SECTION("Reading zero bytes should succeed")
			{
				REQUIRE(bd2Mtd.read(address, buffer, {}) == 0);
			}
		}
		SECTION("Providing nullptr as program/read buffer should fail with EINVAL")
		{
			constexpr uint64_t address {0x2ee0d68164108f7c};
			constexpr size_t size {0xd2d3630e};

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

			SECTION("Providing nullptr as program buffer should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.program(address, {}, size) == EINVAL);
			}
			SECTION("Providing nullptr as read buffer should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.read(address, {}, size) == EINVAL);
			}
		}
		SECTION("Unaligned erase/program/read should fail with EINVAL")
		{
			constexpr size_t anotherBlockSize {10};

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(anotherBlockSize);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

			SECTION("Unaligned erase address should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.erase(anotherBlockSize + 1, anotherBlockSize) == EINVAL);
			}
			SECTION("Unaligned erase size should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.erase(anotherBlockSize, anotherBlockSize + 1) == EINVAL);
			}
			SECTION("Unaligned program address should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.program(anotherBlockSize + 1, buffer, anotherBlockSize) == EINVAL);
			}
			SECTION("Unaligned program size should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.program(anotherBlockSize, buffer, anotherBlockSize + 1) == EINVAL);
			}
			SECTION("Unaligned read address should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.read(anotherBlockSize + 1, buffer, anotherBlockSize) == EINVAL);
			}
			SECTION("Unaligned read size should fail with EINVAL")
			{
				REQUIRE(bd2Mtd.read(anotherBlockSize, buffer, anotherBlockSize + 1) == EINVAL);
			}
		}
		SECTION("Erase/program/read range greater than device size should fail with ENOSPC")
		{
			constexpr size_t anotherBlockSize {10};
			constexpr uint64_t anotherDeviceSize {anotherBlockSize * 10};

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(anotherBlockSize);
			REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(anotherDeviceSize);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);

			SECTION("Erase range greater than device size should fail with ENOSPC")
			{
				REQUIRE(bd2Mtd.erase(anotherBlockSize, anotherDeviceSize) == ENOSPC);
			}
			SECTION("Program range greater than device size should fail with ENOSPC")
			{
				REQUIRE(bd2Mtd.program(anotherBlockSize, buffer, anotherDeviceSize) == ENOSPC);
			}
			SECTION("Read range greater than device size should fail with ENOSPC")
			{
				REQUIRE(bd2Mtd.read(anotherBlockSize, buffer, anotherDeviceSize) == ENOSPC);
			}
		}
		SECTION("Overlaping and adjacent erases should be merged")
		{
			struct Step
			{
				uint64_t address;
				uint64_t size;
				uint64_t mergedAddress;
				uint64_t mergedSize;
			};
			const Step steps[]
			{
					{2400, 200, 2400, 200},
					{2600, 200, 2400, 400},		// adjacent, extends above
					{2200, 200, 2200, 600},		// adjacent, extends below
					{2700, 200, 2200, 700},		// partially overlapping, extends above
					{2100, 200, 2100, 800},		// partially overlapping, extends below
					{2050, 900, 2050, 900},		// overlapping, extends above and below
					{2050, 950, 2050, 950},		// overlapping, extends above
					{2000, 1000, 2000, 1000},	// overlapping, extends below
					{2000, 1000, 2000, 1000},	// overlapping
					{2001, 999, 2000, 1000},	// partially overlapping
					{2000, 999, 2000, 1000},	// partially overlapping
					{2001, 998, 2000, 1000},	// partially overlapping
			};
			for (size_t step {}; step < sizeof(steps) / sizeof(*steps); ++step)
			{
				DYNAMIC_SECTION("Testing erase with " << step + 1 << " steps")
				{
					for (size_t i {}; i <= step; ++i)
					{
						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
						REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.erase(steps[i].address, steps[i].size) == 0);
					}

					SECTION("Synchronize should flush any pending erase")
					{
						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, erase(steps[step].mergedAddress,
								steps[step].mergedSize)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.synchronize() == 0);
					}
					SECTION("Non-overlapping and non-adjacent erase should flush any pending erase")
					{
						constexpr uint64_t address {0xc10dad9c08fbaddc};
						constexpr uint64_t size {0x379d562e};

						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
						REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
						REQUIRE_CALL(blockDeviceMock, erase(steps[step].mergedAddress,
								steps[step].mergedSize)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.erase(address, size) == 0);

						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, erase(address, size)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.synchronize() == 0);
					}
				}
			}
		}
		SECTION("Test program operations overlapping pending erase region")
		{
			struct Step
			{
				uint64_t eraseAddress;
				uint64_t eraseSize;
				uint64_t writeAddress;
				size_t writeSize;
				uint64_t erasedAddress0;
				uint64_t erasedSize0;
				uint64_t erasedAddress1;
				uint64_t erasedSize1;
			};
			const Step steps[]
			{
					{2000, 1000, 1991, 8, 0, 0, 2000, 1000},	// non-overlapping, non-adjacent, below
					{0, 0, 3001, 9, 0, 0, 2000, 1000},			// non-overlapping, non-adjacent, above
					{0, 0, 1990, 10, 0, 0, 2000, 1000},			// non-overlapping, adjacent to beginning
					{0, 0, 3000, 11, 0, 0, 2000, 1000},			// non-overlapping, adjacent to end
					{0, 0, 2000, 100, 0, 0, 2100, 900},			// overlapping, adjacent to beginning
					{0, 0, 2000, 200, 0, 0, 2200, 800},			// partially overlapping, adjacent to beginning
					{0, 0, 2300, 200, 2200, 100, 2500, 500},	// partially overlapping, non-adjacent
					{0, 0, 2900, 100, 0, 0, 2500, 400},			// overlapping, adjacent to end
					{0, 0, 2800, 200, 0, 0, 2500, 300},			// partially overlapping, adjacent to end
					{0, 0, 2500, 300, 0, 0, 0, 0},				// overlapping, adjacent to beginning and end
					{500, 100, 500, 200, 0, 0, 0, 0},			// partially overlapping, adjacent to beginning
					{500, 100, 400, 200, 0, 0, 0, 0},			// partially overlapping, adjacent to end
					{500, 100, 400, 300, 0, 0, 0, 0},			// partially overlapping, non-adjacent
			};
			for (size_t step {}; step < sizeof(steps) / sizeof(*steps); ++step)
			{
				DYNAMIC_SECTION("Testing program with " << step + 1 << " steps")
				{
					for (size_t i {}; i <= step; ++i)
					{
						if (steps[i].eraseSize != 0)
						{
							REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
							REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
							REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE(bd2Mtd.erase(steps[i].eraseAddress, steps[i].eraseSize) == 0);
						}

						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
						REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);

						if (steps[i].erasedSize0 != 0)
						{
							expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, erase(steps[i].erasedAddress0,
									steps[i].erasedSize0)).IN_SEQUENCE(sequence).RETURN(0));
						}

						REQUIRE_CALL(blockDeviceMock, write(steps[i].writeAddress, buffer,
								steps[i].writeSize)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.program(steps[i].writeAddress, buffer, steps[i].writeSize) == 0);
					}

					if (steps[step].erasedSize1 != 0)
					{
						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, erase(steps[step].erasedAddress1,
								steps[step].erasedSize1)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.synchronize() == 0);
					}
				}
			}
		}
		SECTION("Test read operations overlapping pending erase region")
		{
			struct Step
			{
				uint64_t eraseAddress;
				uint64_t eraseSize;
				uint64_t readAddress;
				size_t readSize;
				uint64_t erasedAddress0;
				uint64_t erasedSize0;
				uint64_t erasedAddress1;
				uint64_t erasedSize1;
			};
			const Step steps[]
			{
					{2000, 1000, 1991, 8, 0, 0, 2000, 1000},	// non-overlapping, non-adjacent, below
					{0, 0, 3001, 9, 0, 0, 2000, 1000},			// non-overlapping, non-adjacent, above
					{0, 0, 1990, 10, 0, 0, 2000, 1000},			// non-overlapping, adjacent to beginning
					{0, 0, 3000, 11, 0, 0, 2000, 1000},			// non-overlapping, adjacent to end
					{0, 0, 2000, 100, 2000, 100, 2100, 900},	// overlapping, adjacent to beginning
					{0, 0, 2000, 200, 2100, 100, 2200, 800},	// partially overlapping, adjacent to beginning
					{0, 0, 2300, 200, 2200, 300, 2500, 500},	// partially overlapping, non-adjacent
					{0, 0, 2900, 100, 2900, 100, 2500, 400},	// overlapping, adjacent to end
					{0, 0, 2800, 200, 2800, 100, 2500, 300},	// partially overlapping, adjacent to end
					{0, 0, 2500, 300, 2500, 300, 0, 0},			// overlapping, adjacent to beginning and end
					{500, 100, 500, 200, 500, 100, 0, 0},		// partially overlapping, adjacent to beginning
					{500, 100, 400, 200, 500, 100, 0, 0},		// partially overlapping, adjacent to end
					{500, 100, 400, 300, 500, 100, 0, 0},		// partially overlapping, non-adjacent
			};
			for (size_t step {}; step < sizeof(steps) / sizeof(*steps); ++step)
			{
				DYNAMIC_SECTION("Testing read with " << step + 1 << " steps")
				{
					for (size_t i {}; i <= step; ++i)
					{
						if (steps[i].eraseSize != 0)
						{
							REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
							REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
							REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
							REQUIRE(bd2Mtd.erase(steps[i].eraseAddress, steps[i].eraseSize) == 0);
						}

						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
						REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);

						if (steps[i].erasedSize0 != 0)
						{
							expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, erase(steps[i].erasedAddress0,
									steps[i].erasedSize0)).IN_SEQUENCE(sequence).RETURN(0));
						}

						REQUIRE_CALL(blockDeviceMock,
								read(steps[i].readAddress, buffer, steps[i].readSize)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.read(steps[i].readAddress, buffer, steps[i].readSize) == 0);
					}

					if (steps[step].erasedSize1 != 0)
					{
						REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, erase(steps[step].erasedAddress1,
								steps[step].erasedSize1)).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
						REQUIRE(bd2Mtd.synchronize() == 0);
					}
				}
			}
		}
		SECTION("Block device erase error should propagate error code to caller")
		{
			constexpr uint64_t address {0x6db03e9f7b1b51aa};
			constexpr uint64_t size {0x70733f10};

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
			REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.erase(address, size) == 0);

			SECTION("Test via synchronize()")
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				constexpr int ret {0x4ce2b62c};
				REQUIRE_CALL(blockDeviceMock, erase(address, size)).IN_SEQUENCE(sequence).RETURN(ret);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(bd2Mtd.synchronize() == ret);
			}
			SECTION("Test via non-overlapping and non-adjacent erase()")
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				constexpr int ret {0x7ad1c3b0};
				REQUIRE_CALL(blockDeviceMock, erase(address, size)).IN_SEQUENCE(sequence).RETURN(ret);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(bd2Mtd.erase(address + size + blockSize, size) == ret);
			}
			SECTION("Test via overlapping and non-adjacent program()")
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				constexpr int ret {0x61e1b3e5};
				REQUIRE_CALL(blockDeviceMock, erase(address, blockSize)).IN_SEQUENCE(sequence).RETURN(ret);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(bd2Mtd.program(address + blockSize, buffer, size - 2 * blockSize) == ret);
			}
			SECTION("Test via overlapping and non-adjacent read()")
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				constexpr int ret {0x2250ef2c};
				REQUIRE_CALL(blockDeviceMock, erase(address, size - blockSize)).IN_SEQUENCE(sequence).RETURN(ret);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE(bd2Mtd.read(address + blockSize, buffer, size - 2 * blockSize) == ret);
			}

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, erase(address, size)).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.synchronize() == 0);
		}
		SECTION("Block device write error should propagate error code to caller")
		{
			constexpr uint64_t address {0x9f2e113999b4b49e};
			constexpr uint64_t size {0x3621031f};

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
			REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.erase(address, size) == 0);

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
			REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
			REQUIRE_CALL(blockDeviceMock, erase(address, blockSize)).IN_SEQUENCE(sequence).RETURN(0);
			constexpr int ret {0x44645a96};
			REQUIRE_CALL(blockDeviceMock,
					write(address + blockSize, buffer, size - 2 * blockSize)).IN_SEQUENCE(sequence).RETURN(ret);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.program(address + blockSize, buffer, size - 2 * blockSize) == ret);

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, erase(address + blockSize, size - blockSize)).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.synchronize() == 0);
		}
		SECTION("Block device read error should propagate error code to caller")
		{
			constexpr uint64_t address {0x5eeb2dc07ac36679};

			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
			REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
			constexpr int ret {0x51d4894f};
			REQUIRE_CALL(blockDeviceMock, read(address, buffer, sizeof(buffer))).IN_SEQUENCE(sequence).RETURN(ret);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE(bd2Mtd.read(address, buffer, sizeof(buffer)) == ret);
		}

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE(bd2Mtd.close() == 0);
	}

	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence).RETURN(0));
	expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence).RETURN(0));
}
