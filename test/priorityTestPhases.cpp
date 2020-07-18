/**
 * \file
 * \brief priorityTestPhases object definition
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		{maxPhasePriority - 0, 0},
		{maxPhasePriority - 1, 1},
		{maxPhasePriority - 2, 2},
		{maxPhasePriority - 3, 3},
		{maxPhasePriority - 4, 4},
		{maxPhasePriority - 5, 5},
		{maxPhasePriority - 6, 6},
		{maxPhasePriority - 7, 7},
}};

/// array with non-unique priorities and expected sequence points
const std::array<ThreadParameters, totalThreads> nonUniquePriorities
{{
		{maxPhasePriority - 0, 0},
		{maxPhasePriority - 0, 1},
		{maxPhasePriority - 0, 2},
		{maxPhasePriority - 0, 3},
		{maxPhasePriority - 1, 4},
		{maxPhasePriority - 1, 5},
		{maxPhasePriority - 1, 6},
		{maxPhasePriority - 1, 7},
}};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

const std::array<TestPhase, totalPhases> priorityTestPhases
{{
	// phase 0 - threads added with decreasing unique priority
	{uniquePriorities, {0, 1, 2, 3, 4, 5, 6, 7}},
	// phase 1 - threads added with increasing unique priority
	{uniquePriorities, {7, 6, 5, 4, 3, 2, 1, 0}},
	// phase 2 - threads added with random unique priority
	{uniquePriorities, {6, 3, 5, 0, 2, 1, 7, 4}},
	// phase 3 - multiple threads with identical priority (block 1)
	{nonUniquePriorities, {0, 1, 2, 3, 4, 5, 6, 7}},
	// phase 4 - multiple threads with identical priority (block 2)
	{nonUniquePriorities, {4, 5, 6, 7, 0, 1, 2, 3}},
	// phase 5 - multiple threads with identical priority (alternating 1)
	{nonUniquePriorities, {0, 4, 1, 5, 2, 6, 3, 7}},
	// phase 6 - multiple threads with identical priority (alternating 2)
	{nonUniquePriorities, {4, 0, 5, 1, 6, 2, 7, 3}},
	// phase 7 - multiple threads with identical priority (sort-of-random)
	{nonUniquePriorities, {4, 5, 6, 0, 1, 7, 2, 3}},
}};

}	// namespace test

}	// namespace distortos
