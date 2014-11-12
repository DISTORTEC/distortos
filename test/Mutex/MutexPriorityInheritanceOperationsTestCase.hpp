/**
 * \file
 * \brief MutexPriorityInheritanceOperationsTestCase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-11-12
 */

#ifndef TEST_MUTEX_MUTEXPRIORITYINHERITANCEOPERATIONSTESTCASE_HPP_
#define TEST_MUTEX_MUTEXPRIORITYINHERITANCEOPERATIONSTESTCASE_HPP_

#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations in scenarios specific for PriorityInheritance protocol of mutex.
 *
 * Tests basic priority inheritance mechanism of mutexes with PriorityInheritance protocol.
 */

class MutexPriorityInheritanceOperationsTestCase : public TestCase
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

#endif	// TEST_MUTEX_MUTEXPRIORITYINHERITANCEOPERATIONSTESTCASE_HPP_
