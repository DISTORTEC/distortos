/**
 * \file
 * \brief SignalsWaitTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-02-18
 */

#ifndef TEST_SIGNALS_SIGNALSWAITTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSWAITTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests waiting for signals
 *
 * Starts 10 small threads which wait for any signal, asserting that they receive the same signal that was generated,
 * start and finish in the expected order, using exact number of context switches.
 */

class SignalsWaitTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {1};

public:

	/// internal implementation of SignalsWaitTestCase
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
	 * \brief SignalsWaitTestCase's constructor
	 *
	 * \param [in] implementation is a reference to SignalsWaitTestCase::Implementation object used by this
	 * instance
	 */

	constexpr explicit SignalsWaitTestCase(const Implementation& implementation) :
			PrioritizedTestCase{implementation, testCasePriority_}
	{

	}
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSWAITTESTCASE_HPP_
