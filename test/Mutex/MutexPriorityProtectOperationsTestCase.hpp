/**
 * \file
 * \brief MutexPriorityProtectOperationsTestCase class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_MUTEX_MUTEXPRIORITYPROTECTOPERATIONSTESTCASE_HPP_
#define TEST_MUTEX_MUTEXPRIORITYPROTECTOPERATIONSTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations in scenarios specific for priorityProtect protocol of mutex.
 *
 * Tests detection of lock attempts (lock(), tryLock(), tryLockFor() and tryLockUntil()) from thread with priority above
 * priority ceiling and changes of priority resulting from locking mutexes with this protocol.
 */

class MutexPriorityProtectOperationsTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {1};

public:

	/**
	 * \return priority at which this test case should be executed
	 */

	constexpr static uint8_t getTestCasePriority()
	{
		return testCasePriority_;
	}

	/**
	 * \brief MutexPriorityProtectOperationsTestCase's constructor
	 */

	constexpr MutexPriorityProtectOperationsTestCase() :
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

#endif	// TEST_MUTEX_MUTEXPRIORITYPROTECTOPERATIONSTESTCASE_HPP_
