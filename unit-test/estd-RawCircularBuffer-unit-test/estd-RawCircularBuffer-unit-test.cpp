/**
 * \file
 * \brief BufferingBlockDevice test cases
 *
 * This test checks whether BufferingBlockDevice perform all operations properly and in correct order.
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "estd/RawCircularBuffer.hpp"

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing read/write RawCircularBuffer()", "[read/write]")
{
	uint8_t buffer[9];
	estd::RawCircularBuffer rcb {buffer, sizeof(buffer)};

	// empty buffer

	//  012345678
	// [---------]
	//  W
	//  R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == true);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == 0);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == sizeof(buffer));
	}

	// buffer filled at the beginning

	rcb.increaseWritePosition(1);

	//  012345678
	// [X--------]
	//   W
	//  R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == 1);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == 1);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer + 1);
		REQUIRE(size == sizeof(buffer) - 1);
	}

	// buffer full

	rcb.increaseWritePosition(sizeof(buffer) - 1);

	//  012345678
	// [XXXXXXXXX]
	//  W
	//  R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == true);
	REQUIRE(rcb.getSize() == sizeof(buffer));
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == sizeof(buffer));
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}

	// read from the beginning

	rcb.increaseReadPosition(3);

	//  012345678
	// [---XXXXXX]
	//  W
	//     R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == sizeof(buffer) - 3);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == sizeof(buffer) - 3);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == 3);
	}

	// full buffer

	rcb.increaseWritePosition(3);

	//  012345678
	// [XXXXXXXXX]
	//     W
	//     R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == true);
	REQUIRE(rcb.getSize() == sizeof(buffer));
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == sizeof(buffer) - 3);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}

	// read from the middle

	rcb.increaseReadPosition(4);

	//  012345678
	// [XXX----XX]
	//     W
	//         R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == sizeof(buffer) - 4);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer + 3 + 4);
		REQUIRE(size == sizeof(buffer) - 3 - 4);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == 4);
	}

	// read till the end

	rcb.increaseReadPosition(2);

	//  012345678
	// [XXX------]
	//     W
	//  R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == sizeof(buffer) - 4 - 2);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == sizeof(buffer) - 4 - 2);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == 4 + 2);
	}

	// read what is left, empty buffer

	rcb.increaseReadPosition(sizeof(buffer) - 4 - 2);

	//  012345678
	// [---------]
	//     W
	//     R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == true);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == 0);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == sizeof(buffer) - 3);
	}

	// write a few bytes in the middle

	rcb.increaseWritePosition(5);

	//  012345678
	// [---XXXXX-]
	//          W
	//     R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == 5);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == 5);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer + 3 + 5);
		REQUIRE(size == sizeof(buffer) - 3 - 5);
	}

	// write till the end

	rcb.increaseWritePosition(sizeof(buffer) - 3 - 5);

	//  012345678
	// [---XXXXXX]
	//  W
	//     R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == sizeof(buffer) - 3);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == sizeof(buffer) - 3);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == 3);
	}

	// clear buffer

	rcb.clear();

	//  012345678
	// [---------]
	//  W
	//  R

	REQUIRE(rcb.isReadOnly() == false);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == true);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == 0);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == sizeof(buffer));
	}
}

TEST_CASE("Testing read-only RawCircularBuffer()", "[read-only]")
{
	const uint8_t buffer[7] {};
	estd::RawCircularBuffer rcb {buffer, sizeof(buffer)};

	// empty buffer

	//  0123456
	// [-------]
	//  W
	//  R

	REQUIRE(rcb.isReadOnly() == true);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == true);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == 0);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}

	// make it full buffer

	rcb.increaseWritePosition(sizeof(buffer));

	//  0123456
	// [XXXXXXX]
	//  W
	//  R

	REQUIRE(rcb.isReadOnly() == true);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == true);
	REQUIRE(rcb.getSize() == sizeof(buffer));
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer);
		REQUIRE(size == sizeof(buffer));
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}

	// read a few bytes from the beginning

	rcb.increaseReadPosition(3);

	//  0123456
	// [---XXXX]
	//  W
	//     R

	REQUIRE(rcb.isReadOnly() == true);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == sizeof(buffer) - 3);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer + 3);
		REQUIRE(size == sizeof(buffer) - 3);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}

	// read a few bytes int the middle

	rcb.increaseReadPosition(2);

	//  0123456
	// [-----XX]
	//  W
	//       R

	REQUIRE(rcb.isReadOnly() == true);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == false);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == sizeof(buffer) - 3 - 2);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == buffer + 3 + 2);
		REQUIRE(size == sizeof(buffer) - 3 - 2);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}

	// read what is left, empty buffer

	rcb.increaseReadPosition(sizeof(buffer) - 3 - 2);

	//  0123456
	// [-------]
	//  W
	//  R

	REQUIRE(rcb.isReadOnly() == true);
	REQUIRE(rcb.getCapacity() == sizeof(buffer));
	REQUIRE(rcb.isEmpty() == true);
	REQUIRE(rcb.isFull() == false);
	REQUIRE(rcb.getSize() == 0);
	{
		const auto [address, size] = rcb.getReadBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}
	{
		const auto [address, size] = rcb.getWriteBlock();
		REQUIRE(address == nullptr);
		REQUIRE(size == 0);
	}
}
