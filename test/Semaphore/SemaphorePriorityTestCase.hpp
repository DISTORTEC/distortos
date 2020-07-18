/**
 * \file
 * \brief SemaphorePriorityTestCase class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SEMAPHORE_SEMAPHOREPRIORITYTESTCASE_HPP_
#define TEST_SEMAPHORE_SEMAPHOREPRIORITYTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests priority scheduling of semaphores.
 *
 * Starts 8 small threads (in various order) with varying priorities which wait on the same locked semaphore, asserting
 * that they start and finish in the expected order, using exact number of context switches.
 */

class SemaphorePriorityTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {1};

public:

	/**
	 * \brief SemaphorePriorityTestCase's constructor
	 */

	constexpr SemaphorePriorityTestCase() :
			PrioritizedTestCase{testCasePriority_}
	{

	}

private:

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	bool run_() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SEMAPHORE_SEMAPHOREPRIORITYTESTCASE_HPP_
