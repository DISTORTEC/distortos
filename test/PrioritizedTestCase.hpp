/**
 * \file
 * \brief PrioritizedTestCase class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
 * \brief PrioritizedTestCase class is a TestCase which runs at specific priority, restoring original priority of main
 * test thread afterwards.
 */

class PrioritizedTestCase : public TestCaseCommon
{
public:

	/**
	 * \brief PrioritizedTestCase's constructor
	 *
	 * \param [in] priority is the priority at which test case will be executed
	 */

	constexpr explicit PrioritizedTestCase(const uint8_t priority) :
			priority_{priority}
	{

	}

protected:

	/**
	 * \brief Finalizes the test case.
	 *
	 * Restores original priority of main test thread.
	 *
	 * \return true if finalization succeeded, false otherwise
	 */

	bool finalize() const override;

	/**
	 * \brief Initializes the test case.
	 *
	 * Sets configured priority.
	 *
	 * \return true if initialization succeeded, false otherwise
	 */

	bool initialize() const override;

private:

	/// priority at which testCase_ will be executed
	const uint8_t priority_;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_PRIORITIZEDTESTCASE_HPP_
