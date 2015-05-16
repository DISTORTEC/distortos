/**
 * \file
 * \brief RawFifoQueueOperationsTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#ifndef TEST_RAWFIFOQUEUE_RAWFIFOQUEUEOPERATIONSTESTCASE_HPP_
#define TEST_RAWFIFOQUEUE_RAWFIFOQUEUEOPERATIONSTESTCASE_HPP_

#include "TestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests various RawFifoQueue operations.
 *
 * Tests pushing (push(), tryPush(), tryPushFor() and tryPushUntil()) and popping (pop(), tryPop(), tryPopFor() and
 * tryPopUntil()) to/from RawFifoQueue, both from thread and from interrupt context - these operations must return
 * expected result, cause expected number of context switches and finish within expected time frame.
 */

class RawFifoQueueOperationsTestCase : public TestCaseCommon
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

#endif	// TEST_RAWFIFOQUEUE_RAWFIFOQUEUEOPERATIONSTESTCASE_HPP_
