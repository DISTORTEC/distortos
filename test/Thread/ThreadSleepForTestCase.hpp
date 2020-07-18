/**
 * \file
 * \brief ThreadSleepForTestCase class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_THREAD_THREADSLEEPFORTESTCASE_HPP_
#define TEST_THREAD_THREADSLEEPFORTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests "sleep for" functionality of threads.
 *
 * Starts 8 small threads (in varying order) which sleep for varying amount of time, asserting that they finish in the
 * right order and sleep for requested duration.
 */

class ThreadSleepForTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX};

public:

	/**
	 * \brief ThreadSleepForTestCase's constructor
	 */

	constexpr ThreadSleepForTestCase() :
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

#endif	// TEST_THREAD_THREADSLEEPFORTESTCASE_HPP_
