/**
 * \file
 * \brief TestCase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-24
 */

#ifndef TEST_TESTCASE_HPP_
#define TEST_TESTCASE_HPP_

namespace distortos
{

/// test namespace - functional tests
namespace test
{

/**
 * \brief TestCase class is an interface class for test cases
 *
 * \note This class deliberately has neither public virtual destructor nor protected non-virtual destructor - this way
 * objects derived from this class may be ROMable. These objects should not be deleted via pointer of TestCase type.
 */

class TestCase
{
public:

	/**
	 * \brief Public function to start the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	bool run() const
	{
		return run_();
	}

private:

	/**
	 * \brief Runs the test case.
	 *
	 * \note this should be provided by derived classes
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	virtual bool run_() const = 0;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_TESTCASE_HPP_
