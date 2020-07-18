/**
 * \file
 * \brief priorityTestPhases object declaration
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_PRIORITYTESTPHASES_HPP_
#define TEST_PRIORITYTESTPHASES_HPP_

#include <array>

#include <cstddef>
#include <cstdint>

namespace distortos
{

namespace test
{

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// number of test threads
constexpr size_t totalThreads {8};

/// number of test phases
constexpr size_t totalPhases {8};

/// max priority used in test phases
constexpr uint8_t maxPhasePriority {UINT8_MAX - 1};

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// parameters of test thread - priority, sequence point
using ThreadParameters = std::pair<uint8_t, uint8_t>;

/// description of test phase - reference to array with ThreadParameters, sequence of indexes for this array
using TestPhase = std::pair<const std::array<ThreadParameters, totalThreads>&, std::array<uint8_t, totalThreads>>;

/*---------------------------------------------------------------------------------------------------------------------+
| global objects
+---------------------------------------------------------------------------------------------------------------------*/

/// array with test phases
extern const std::array<TestPhase, totalPhases> priorityTestPhases;

}	// namespace test

}	// namespace distortos

#endif	// TEST_PRIORITYTESTPHASES_HPP_
