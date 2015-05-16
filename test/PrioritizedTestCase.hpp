/**
 * \file
 * \brief PrioritizedTestCase class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#ifndef TEST_PRIORITIZEDTESTCASE_HPP_
#define TEST_PRIORITIZEDTESTCASE_HPP_

#include "TestCaseCommon.hpp"

#include <cstdint>

namespace distortos
{

namespace test
{

/**
 * \brief PrioritizedTestCase class is a wrapper for TestCase which runs the internal TestCase at specific priority,
 * restoring original priority afterwards.
 */

class PrioritizedTestCase : public TestCaseCommon
{
public:

	/**
	 * \brief PrioritizedTestCase's constructor
	 *
	 * \param [in] testCase is a reference to TestCase run by that object
	 * \param [in] priority is the priority at which testCase will be executed
	 */

	constexpr PrioritizedTestCase(const TestCase& testCase, const uint8_t priority) :
			testCase_(testCase),
			priority_{priority}
	{

	}

private:

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	virtual bool run_() const override;

	/// reference to TestCase run by that object
	const TestCase& testCase_;

	/// priority at which testCase_ will be executed
	const uint8_t priority_;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_PRIORITIZEDTESTCASE_HPP_
