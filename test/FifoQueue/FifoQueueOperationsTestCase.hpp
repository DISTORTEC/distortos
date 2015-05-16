/**
 * \file
 * \brief FifoQueueOperationsTestCase class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#ifndef TEST_FIFOQUEUE_FIFOQUEUEOPERATIONSTESTCASE_HPP_
#define TEST_FIFOQUEUE_FIFOQUEUEOPERATIONSTESTCASE_HPP_

#include "TestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests various FifoQueue operations.
 *
 * Tests emplacing (emplace(), tryEmplace(), tryEmplaceFor() and tryEmplaceUntil()), pushing (push(), tryPush(),
 * tryPushFor() and tryPushUntil()) and popping (pop(), tryPop(), tryPopFor() and tryPopUntil()) to/from FifoQueue, both
 * from thread and from interrupt context - these operations must return expected result, cause expected number of
 * context switches, finish within expected time frame and execute expected actions on transferred object (various
 * constructor types, destructor, swap, ...).
 */

class FifoQueueOperationsTestCase : public TestCaseCommon
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

#endif	// TEST_FIFOQUEUE_FIFOQUEUEOPERATIONSTESTCASE_HPP_
