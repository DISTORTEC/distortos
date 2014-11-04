/**
 * \file
 * \brief MutexPriorityProtocolTestCase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-04
 */

#ifndef TEST_MUTEX_MUTEXPRIORITYPROTOCOLTESTCASE_HPP_
#define TEST_MUTEX_MUTEXPRIORITYPROTOCOLTESTCASE_HPP_

#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Executes typical "priority inversion" scenario using mutexes with PriorityProtect protocol.
 *
 * 5 threads are executed and they are expected to execute and preempt each other in exact sequence.
 */

class MutexPriorityProtocolTestCase : public TestCase
{
private:

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	virtual bool run_() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_MUTEX_MUTEXPRIORITYPROTOCOLTESTCASE_HPP_
