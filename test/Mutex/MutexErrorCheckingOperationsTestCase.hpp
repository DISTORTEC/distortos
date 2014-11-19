/**
 * \file
 * \brief MutexErrorCheckingOperationsTestCase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-19
 */

#ifndef TEST_MUTEX_MUTEXERRORCHECKINGOPERATIONSTESTCASE_HPP_
#define TEST_MUTEX_MUTEXERRORCHECKINGOPERATIONSTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations in scenarios specific for ErrorChecking type of mutex.
 *
 * Tests locking (lock(), tryLock(), tryLockFor() and tryLockUntil()) and unlocking.
 */

class MutexErrorCheckingOperationsTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX - 1};

public:

	/// internal implementation of MutexErrorCheckingOperationsTestCase
	class Implementation : public TestCase
	{
	private:

		/**
		 * \brief Runs the test case.
		 *
		 * \return true if the test case succeeded, false otherwise
		 */

		virtual bool run_() const override;
	};

	/**
	 * \brief MutexErrorCheckingOperationsTestCase's constructor
	 *
	 * \param [in] implementation is a reference to MutexErrorCheckingOperationsTestCase::Implementation object used by
	 * this instance
	 */

	constexpr explicit MutexErrorCheckingOperationsTestCase(const Implementation& implementation) :
			PrioritizedTestCase{implementation, testCasePriority_}
	{

	}
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_MUTEX_MUTEXERRORCHECKINGOPERATIONSTESTCASE_HPP_
