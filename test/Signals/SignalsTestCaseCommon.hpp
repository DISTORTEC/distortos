/**
 * \file
 * \brief SignalsTestCaseCommon class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-16
 */

#ifndef TEST_SIGNALS_SIGNALSTESTCASECOMMON_HPP_
#define TEST_SIGNALS_SIGNALSTESTCASECOMMON_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief SignalsTestCaseCommon class is a TestCase with initialization and finalization steps required for signals.
 *
 * During initialization:
 * - configured priority is set;
 *
 * During finalization:
 * - empty signal mask is restored;
 * - original priority of main test thread is restored;
 */

class SignalsTestCaseCommon : public PrioritizedTestCase
{
public:

	/**
	 * \brief SignalsTestCaseCommon's constructor
	 *
	 * \param [in] priority is the priority at which test case will be executed
	 */

	constexpr explicit SignalsTestCaseCommon(const uint8_t priority) :
			PrioritizedTestCase{priority}
	{

	}

protected:

	/**
	 * \brief Finalizes the test case.
	 *
	 * Restores empty signal mask and calls PrioritizedTestCase::finalize().
	 *
	 * \return true if finalization succeeded, false otherwise
	 */

	virtual bool finalize() const override;

	/**
	 * \brief Initializes the test case.
	 *
	 * Calls PrioritizedTestCase::initialize().
	 *
	 * \return true if initialization succeeded, false otherwise
	 */

	virtual bool initialize() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSTESTCASECOMMON_HPP_
