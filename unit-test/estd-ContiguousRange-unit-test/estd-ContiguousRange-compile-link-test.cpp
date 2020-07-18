/**
 * \file
 * \brief estd::ContiguousRange compile/link test
 *
 * The only purpose of this test is to ensure estd::ContiguousRange can be used with all possible kinds of arrays. It
 * just uses all constructors from the tested header.
 *
 * \author Copyright (C) 2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "estd/ContiguousRange.hpp"

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void compileLinkTest()
{
	using ConstRange = estd::ContiguousRange<const int>;
	using Range = estd::ContiguousRange<int>;

	int value {};
	const int constValue {};
	int array[10] {};
	const int constArray[10] {};
	std::array<int, 10> stdArray {};
	std::array<const int, 10> stdArrayConst {};
	const std::array<int, 10> constStdArray {};
	const std::array<const int, 10> constStdArrayConst {};

	// constructor taking two iterators
	{
		ConstRange constRangeWithValue {&value, &value + 1};
		ConstRange constRangeWithConstValue {&constValue, &constValue + 1};
		ConstRange constRangeWithArray {std::begin(array), std::end(array)};
		ConstRange constRangeWithConstArray {std::begin(constArray), std::end(constArray)};
		ConstRange constRangeWithStdArray {stdArray.begin(), stdArray.end()};
		ConstRange constRangeWithStdArrayConst {stdArrayConst.begin(), stdArrayConst.end()};
		ConstRange constRangeWithConstStdArray {constStdArray.begin(), constStdArray.end()};
		ConstRange constRangeWithConstStdArrayConst {constStdArrayConst.begin(), constStdArrayConst.end()};

		Range rangeWithValue {&value, &value + 1};
		Range rangeWithArray {std::begin(array), std::end(array)};
		Range rangeWithStdArray {stdArray.begin(), stdArray.end()};
	}
	// constructor of empty range
	{
		ConstRange constRange {};
		Range range {};
	}
	// constructor taking iterator and size
	{
		ConstRange constRangeWithValue {&value, 1};
		ConstRange constRangeWithConstValue {&constValue, 1};
		ConstRange constRangeWithArray {std::begin(array), std::size(array)};
		ConstRange constRangeWithConstArray {std::begin(constArray), std::size(constArray)};
		ConstRange constRangeWithStdArray {stdArray.begin(), stdArray.size()};
		ConstRange constRangeWithStdArrayConst {stdArrayConst.begin(), stdArrayConst.size()};
		ConstRange constRangeWithConstStdArray {constStdArray.begin(), constStdArray.size()};
		ConstRange constRangeWithConstStdArrayConst {constStdArrayConst.begin(), constStdArrayConst.size()};

		Range rangeWithValue {&value, 1};
		Range rangeWithArray {std::begin(array), std::size(array)};
		Range rangeWithStdArray {stdArray.begin(), stdArray.size()};
	}
	// constructor taking C-style array
	{
		ConstRange constRangeWithArray {array};
		ConstRange constRangeWithConstArray {constArray};

		Range rangeWithArray {array};
	}
	// constructor taking std::array
	{
		ConstRange constRangeWithStdArray {stdArray};
		ConstRange constRangeWithStdArrayConst {stdArrayConst};
		ConstRange constRangeWithConstStdArray {constStdArray};
		ConstRange constRangeWithConstStdArrayConst {constStdArrayConst};

		Range rangeWithStdArray {stdArray};
	}
	// constructor taking single value
	{
		ConstRange constRangeWithValue {value};
		ConstRange constRangeWithConstValue {constValue};

		Range rangeWithValue {value};
	}
	// converting copy constructor
	{
		const ConstRange constRange {array};
		const Range range {array};

		ConstRange constRangeFromConstRange {constRange};
		ConstRange constRangeFromRange {range};
		Range rangeFromRange {range};
	}
}
