/**
 * \file
 * \brief priorityTestPhases object definition
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-27
 */

#include "priorityTestPhases.hpp"

namespace distortos
{

namespace test
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with unique priorities and expected sequence points
const std::array<ThreadParameters, totalThreads> uniquePriorities
{{
		{UINT8_MAX - 0, 0},
		{UINT8_MAX - 1, 1},
		{UINT8_MAX - 2, 2},
		{UINT8_MAX - 3, 3},
		{UINT8_MAX - 4, 4},
		{UINT8_MAX - 5, 5},
		{UINT8_MAX - 6, 6},
		{UINT8_MAX - 7, 7},
		{UINT8_MAX - 8, 8},
		{UINT8_MAX - 9, 9},
}};

/// array with non-unique priorities and expected sequence points
const std::array<ThreadParameters, totalThreads> nonUniquePriorities
{{
		{UINT8_MAX - 0, 0},
		{UINT8_MAX - 0, 1},
		{UINT8_MAX - 0, 2},
		{UINT8_MAX - 0, 3},
		{UINT8_MAX - 0, 4},
		{UINT8_MAX - 1, 5},
		{UINT8_MAX - 1, 6},
		{UINT8_MAX - 1, 7},
		{UINT8_MAX - 1, 8},
		{UINT8_MAX - 1, 9},
}};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const std::array<TestPhase, totalPhases> priorityTestPhases
{{
	// phase 0 - threads added with decreasing unique priority
	{uniquePriorities, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	// phase 1 - threads added with increasing unique priority
	{uniquePriorities, {9, 8, 7, 6, 5, 4, 3, 2, 1, 0}},
	// phase 2 - threads added with random unique priority
	{uniquePriorities, {7, 2, 6, 3, 0, 8, 5, 9, 1, 4}},
	// phase 3 - multiple threads with identical priority (block 1)
	{nonUniquePriorities, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	// phase 4 - multiple threads with identical priority (block 2)
	{nonUniquePriorities, {5, 6, 7, 8, 9, 0, 1, 2, 3, 4}},
	// phase 5 - multiple threads with identical priority (alternating 1)
	{nonUniquePriorities, {0, 5, 1, 6, 2, 7, 3, 8, 4, 9}},
	// phase 6 - multiple threads with identical priority (alternating 2)
	{nonUniquePriorities, {5, 0, 6, 1, 7, 2, 8, 3, 9, 4}},
	// phase 7 - multiple threads with identical priority (sort-of-random)
	{nonUniquePriorities, {5, 6, 0, 7, 1, 8, 9, 2, 3, 4}},
}};

}	// namespace test

}	// namespace distortos
