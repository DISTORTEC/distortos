/**
 * \file
 * \brief CallOnceOperationsTestCase class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_CALLONCE_CALLONCEOPERATIONSTESTCASE_HPP_
#define TEST_CALLONCE_CALLONCEOPERATIONSTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests callOnce() functionality.
 *
 * Starts 8 small threads which pass the same function and OnceFlag to callOnce(), asserting that they start and finish
 * in the expected order, using exact number of context switches, taking expected amount of time and that the function
 * passed to callOnce() is executed only once.
 */

class CallOnceOperationsTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX};

public:

	/**
	 * \brief CallOnceOperationsTestCase's constructor
	 */

	constexpr CallOnceOperationsTestCase() :
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

#endif	// TEST_CALLONCE_CALLONCEOPERATIONSTESTCASE_HPP_
