/**
 * \file
 * \brief FifoQueuePriorityTestCase class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_QUEUE_FIFOQUEUEPRIORITYTESTCASE_HPP_
#define TEST_QUEUE_FIFOQUEUEPRIORITYTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests priority scheduling of FIFO queue (raw and non-raw).
 *
 * Starts 8 small threads (in various order) with varying priorities which wait either for a message from the FIFO queue
 * or for free space in the FIFO queue, asserting that they start and finish in the expected order, using exact number
 * of context switches, that the data received from the FIFO queue matches what was transferred and that no memory was
 * leaked (in case of "dynamic" queue).
 */

class FifoQueuePriorityTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {1};

public:

	/**
	 * \brief FifoQueuePriorityTestCase's constructor
	 */

	constexpr FifoQueuePriorityTestCase() :
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

#endif	// TEST_QUEUE_FIFOQUEUEPRIORITYTESTCASE_HPP_
