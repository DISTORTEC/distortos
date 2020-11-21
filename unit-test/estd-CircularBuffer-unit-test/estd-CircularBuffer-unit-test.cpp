/**
 * \file
 * \brief CircularBuffer test cases
 *
 * This test checks whether CircularBuffer perform all operations properly and in correct order.
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "estd/DynamicCircularBuffer.hpp"
#include "estd/StaticCircularBuffer.hpp"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class Object
{
public:

	Object()
	{
		auto& instance = getInstanceInternal();
		REQUIRE(instance == nullptr);
		instance = this;
	}

	Object(const uint32_t value) :
			value_{value}
	{
		getInstance().constructed(this, value);
	}

	Object(const Object& other) :
			value_{other.value_}
	{
		getInstance().copyConstructed(this, &other);
	}

	Object(Object&& other) :
			value_(other.value_)
	{
		getInstance().moveConstructed(this, &other);
	}

	virtual ~Object()
	{
		auto& instance = getInstanceInternal();
		if (instance == this)
			instance = {};
		else
		{
			REQUIRE(instance != nullptr);
			instance->destructed(this);
		}
	}

	uint32_t getValue() const
	{
		return value_;
	}

	MAKE_MOCK2(constructed, void(const Object*, uint32_t));
	MAKE_MOCK2(copyConstructed, void(const Object*, const Object*));
	MAKE_MOCK1(destructed, void(const Object*));
	MAKE_MOCK2(moveConstructed, void(const Object*, const Object*));

	static Object& getInstance()
	{
		const auto instance = getInstanceInternal();
		REQUIRE(instance != nullptr);
		return *instance;
	}

private:

	static Object*& getInstanceInternal()
	{
		static Object* instance;
		return instance;
	}

	uint32_t value_;
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr size_t capacity {7};

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

template<typename CircularBuffer>
void testCircularBuffer(std::unique_ptr<CircularBuffer>&& cb)
{
	constexpr uint32_t randomValues[]
	{
			0xb041da4b,
			0x05121408,
			0x02759be6,
			0xaec840b9,

			0xdc689d8e,
			0x88a20fa3,
			0x9d87adf9,
			0xb9717450,

			0x27a3e451,
			0x916abe91,
			0xb327521d,
			0x67298b68,
	};

	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};
	Object objectMock {};

	const Object* begin;

	{
		INFO("Buffer is initially empty");

		//  0123456
		// [-------]
		//  W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == true);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 0);
	}
	{
		INFO("Copy-construct an element into the buffer");

		REQUIRE_CALL(objectMock, constructed(_, randomValues[0])).IN_SEQUENCE(sequence);
		const Object other {randomValues[0]};
		REQUIRE_CALL(objectMock, copyConstructed(_, &other)).IN_SEQUENCE(sequence).LR_SIDE_EFFECT(begin = _1);
		cb->push(other);
		expectations.emplace_back(NAMED_REQUIRE_CALL(objectMock, destructed(&other)).IN_SEQUENCE(sequence));

		//  0123456
		// [X------]
		//   W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 1);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[0]);
	}
	{
		INFO("Move-construct an element into the buffer");

		REQUIRE_CALL(objectMock, constructed(_, randomValues[1])).IN_SEQUENCE(sequence);
		Object other {randomValues[1]};
		REQUIRE_CALL(objectMock, moveConstructed(begin + 1, &other)).IN_SEQUENCE(sequence);
		cb->push(std::move(other));
		expectations.emplace_back(NAMED_REQUIRE_CALL(objectMock, destructed(&other)).IN_SEQUENCE(sequence));

		//  0123456
		// [XX-----]
		//    W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 2);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[0]);
	}
	{
		INFO("Emplace an element in the buffer");

		REQUIRE_CALL(objectMock, constructed(begin + 2, randomValues[2])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[2]);

		//  0123456
		// [XXX----]
		//     W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 3);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[0]);
	}
	{
		INFO("Emplace elements in the buffer until it is full");

		REQUIRE_CALL(objectMock, constructed(begin + 3, randomValues[3])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[3]);

		//  0123456
		// [XXXX---]
		//      W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 4);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[0]);

		REQUIRE_CALL(objectMock, constructed(begin + 4, randomValues[4])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[4]);

		//  0123456
		// [XXXXX--]
		//       W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 5);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[0]);

		REQUIRE_CALL(objectMock, constructed(begin + 5, randomValues[5])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[5]);

		//  0123456
		// [XXXXXX-]
		//        W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 6);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[0]);

		REQUIRE_CALL(objectMock, constructed(begin + 6, randomValues[6])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[6]);

		//  0123456
		// [XXXXXXX]
		//  W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == true);
		REQUIRE(cb->getSize() == capacity);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[0]);
	}
	{
		INFO("Pop two elements from the buffer");

		REQUIRE_CALL(objectMock, destructed(begin + 0)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [-XXXXXX]
		//  W
		//   R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 1);
		REQUIRE(&cb->front() == begin + 1);
		REQUIRE(cb->front().getValue() == randomValues[1]);

		REQUIRE_CALL(objectMock, destructed(begin + 1)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [--XXXXX]
		//  W
		//    R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 2);
		REQUIRE(&cb->front() == begin + 2);
		REQUIRE(cb->front().getValue() == randomValues[2]);
	}
	{
		INFO("Emplace elements in the buffer until it is full again");

		REQUIRE_CALL(objectMock, constructed(begin + 0, randomValues[7])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[7]);

		//  0123456
		// [X-XXXXX]
		//   W
		//    R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 1);
		REQUIRE(&cb->front() == begin + 2);
		REQUIRE(cb->front().getValue() == randomValues[2]);

		REQUIRE_CALL(objectMock, constructed(begin + 1, randomValues[8])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[8]);

		//  0123456
		// [XXXXXXX]
		//    W
		//    R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == true);
		REQUIRE(cb->getSize() == capacity);
		REQUIRE(&cb->front() == begin + 2);
		REQUIRE(cb->front().getValue() == randomValues[2]);
	}
	{
		INFO("Pop all available elements from the buffer");

		REQUIRE_CALL(objectMock, destructed(begin + 2)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [XX-XXXX]
		//    W
		//     R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 1);
		REQUIRE(&cb->front() == begin + 3);
		REQUIRE(cb->front().getValue() == randomValues[3]);

		REQUIRE_CALL(objectMock, destructed(begin + 3)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [XX--XXX]
		//    W
		//      R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 2);
		REQUIRE(&cb->front() == begin + 4);
		REQUIRE(cb->front().getValue() == randomValues[4]);

		REQUIRE_CALL(objectMock, destructed(begin + 4)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [XX---XX]
		//    W
		//       R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 3);
		REQUIRE(&cb->front() == begin + 5);
		REQUIRE(cb->front().getValue() == randomValues[5]);

		REQUIRE_CALL(objectMock, destructed(begin + 5)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [XX----X]
		//    W
		//        R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 4);
		REQUIRE(&cb->front() == begin + 6);
		REQUIRE(cb->front().getValue() == randomValues[6]);

		REQUIRE_CALL(objectMock, destructed(begin + 6)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [XX-----]
		//    W
		//  R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 5);
		REQUIRE(&cb->front() == begin + 0);
		REQUIRE(cb->front().getValue() == randomValues[7]);

		REQUIRE_CALL(objectMock, destructed(begin + 0)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [-X-----]
		//    W
		//   R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == capacity - 6);
		REQUIRE(&cb->front() == begin + 1);
		REQUIRE(cb->front().getValue() == randomValues[8]);

		REQUIRE_CALL(objectMock, destructed(begin + 1)).IN_SEQUENCE(sequence);
		cb->pop();

		//  0123456
		// [-------]
		//    W
		//    R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == true);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 0);
	}
	{
		INFO("Emplace three elements in the buffer");

		REQUIRE_CALL(objectMock, constructed(begin + 2, randomValues[9])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[9]);

		//  0123456
		// [--X----]
		//     W
		//    R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 1);
		REQUIRE(&cb->front() == begin + 2);
		REQUIRE(cb->front().getValue() == randomValues[9]);

		REQUIRE_CALL(objectMock, constructed(begin + 3, randomValues[10])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[10]);

		//  0123456
		// [--XX---]
		//      W
		//    R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 2);
		REQUIRE(&cb->front() == begin + 2);
		REQUIRE(cb->front().getValue() == randomValues[9]);

		REQUIRE_CALL(objectMock, constructed(begin + 4, randomValues[11])).IN_SEQUENCE(sequence);
		cb->emplace(randomValues[11]);

		//  0123456
		// [--XXX--]
		//       W
		//    R

		REQUIRE(cb->getCapacity() == capacity);
		REQUIRE(cb->isEmpty() == false);
		REQUIRE(cb->isFull() == false);
		REQUIRE(cb->getSize() == 3);
		REQUIRE(&cb->front() == begin + 2);
		REQUIRE(cb->front().getValue() == randomValues[9]);
	}
	{
		INFO("Buffer's destructor should destruct all elements remaining in the buffer");

		REQUIRE_CALL(objectMock, destructed(begin + 2)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(objectMock, destructed(begin + 3)).IN_SEQUENCE(sequence);
		REQUIRE_CALL(objectMock, destructed(begin + 4)).IN_SEQUENCE(sequence);
		cb.reset();
	}
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing DynamicCircularBuffer", "[dynamic]")
{
	testCircularBuffer(std::make_unique<estd::DynamicCircularBuffer<Object>>(capacity));
}

TEST_CASE("Testing StaticCircularBuffer", "[static]")
{
	testCircularBuffer(std::make_unique<estd::StaticCircularBuffer<Object, capacity>>());
}
