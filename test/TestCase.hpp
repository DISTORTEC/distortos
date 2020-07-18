/**
 * \file
 * \brief TestCase class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_TESTCASE_HPP_
#define TEST_TESTCASE_HPP_

namespace distortos
{

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

	bool run() const;

protected:

	/**
	 * \brief Finalizes the test case.
	 *
	 * \note this should be provided by derived classes
	 *
	 * \return true if finalization succeeded, false otherwise
	 */

	virtual bool finalize() const = 0;

	/**
	 * \brief Initializes the test case.
	 *
	 * \note this should be provided by derived classes
	 *
	 * \return true if initialization succeeded, false otherwise
	 */

	virtual bool initialize() const = 0;

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
