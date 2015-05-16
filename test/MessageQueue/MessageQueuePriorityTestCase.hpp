/**
 * \file
 * \brief MessageQueuePriorityTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#ifndef TEST_MESSAGEQUEUE_MESSAGEQUEUEPRIORITYTESTCASE_HPP_
#define TEST_MESSAGEQUEUE_MESSAGEQUEUEPRIORITYTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests priority scheduling of message queue.
 *
 * Starts 10 small threads (in various order) with varying priorities which wait either for a message from the message
 * queue or for free space in the message queue, asserting that they start and finish in the expected order, using exact
 * number of context switches, that the data received from the message queue matches what was transferred and is
 * delivered in order which respects the priority of individual messages.
 */

class MessageQueuePriorityTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {1};

public:

	/// internal implementation of MessageQueuePriorityTestCase
	class Implementation : public TestCaseCommon
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
	 * \brief MessageQueuePriorityTestCase's constructor
	 *
	 * \param [in] implementation is a reference to MessageQueuePriorityTestCase::Implementation object used by this
	 * instance
	 */

	constexpr explicit MessageQueuePriorityTestCase(const Implementation& implementation) :
			PrioritizedTestCase{implementation, testCasePriority_}
	{

	}
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_MESSAGEQUEUE_MESSAGEQUEUEPRIORITYTESTCASE_HPP_
