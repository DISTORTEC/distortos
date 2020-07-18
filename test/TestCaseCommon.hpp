/**
 * \file
 * \brief TestCaseCommon class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_TESTCASECOMMON_HPP_
#define TEST_TESTCASECOMMON_HPP_

#include "TestCase.hpp"

namespace distortos
{

namespace test
{

/// TestCaseCommon class implements common parts of TestCase
class TestCaseCommon : public TestCase
{
protected:

	/**
	 * \brief Finalizes the test case.
	 *
	 * Empty stub.
	 *
	 * \return true if finalization succeeded, false otherwise
	 */

	bool finalize() const override;

	/**
	 * \brief Initializes the test case.
	 *
	 * Empty stub.
	 *
	 * \return true if initialization succeeded, false otherwise
	 */

	bool initialize() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_TESTCASECOMMON_HPP_
