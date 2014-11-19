/**
 * \file
 * \brief ThreadPriorityChangeTestCase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-19
 */

#ifndef TEST_THREAD_THREADPRIORITYCHANGETESTCASE_HPP_
#define TEST_THREAD_THREADPRIORITYCHANGETESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests changing of thread priority.
 *
 * Creates 10 threads with the same priority. Their priority is changed and the order in which they execute is checked.
 * Change of the main test thread priority is also tested - it is used as a kind of "semaphore" which allows other
 * threads to run.
 */

class ThreadPriorityChangeTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX / 2};

public:

	/// internal implementation of ThreadPriorityChangeTestCase
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
	 * \brief ThreadPriorityChangeTestCase's constructor
	 *
	 * \param [in] implementation is a reference to ThreadPriorityChangeTestCase::Implementation object used by this
	 * instance
	 */

	constexpr explicit ThreadPriorityChangeTestCase(const Implementation& implementation) :
			PrioritizedTestCase{implementation, testCasePriority_}
	{

	}
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_THREAD_THREADPRIORITYCHANGETESTCASE_HPP_
