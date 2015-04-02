/**
 * \file
 * \brief SignalsQueuedOperationsTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-02
 */

#ifndef TEST_SIGNALS_SIGNALSQUEUEDOPERATIONSTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSQUEUEDOPERATIONSTESTCASE_HPP_

#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests operations specific to queued signals.
 *
 * Tests queuing of signals and error cases specific to queued signals.
 */

class SignalsQueuedOperationsTestCase : public TestCase
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

#endif	// TEST_SIGNALS_SIGNALSQUEUEDOPERATIONSTESTCASE_HPP_
