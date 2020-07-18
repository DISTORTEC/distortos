/**
 * \file
 * \brief SignalsTestCaseCommon class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALSTESTCASECOMMON_HPP_
#define TEST_SIGNALS_SIGNALSTESTCASECOMMON_HPP_

#include "PrioritizedTestCase.hpp"

#include "distortos/SignalAction.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief SignalsTestCaseCommon class is a TestCase with initialization and finalization steps required for signals.
 *
 * During initialization:
 * - configured priority is set;
 * - configured signal action is set for all signal numbers;
 * - configured signal mask is set;
 *
 * During finalization:
 * - empty signal mask is restored;
 * - default signal actions are restored for all signal numbers;
 * - original priority of main test thread is restored;
 */

class SignalsTestCaseCommon : public PrioritizedTestCase
{
public:

	/**
	 * \brief SignalsTestCaseCommon's constructor
	 *
	 * \param [in] priority is the priority at which test case will be executed
	 * \param [in] signalMask is the SignalSet with signal mask for test case
	 * \param [in] signalAction is the SignalAction that will be associated with all signal numbers
	 */

	constexpr SignalsTestCaseCommon(const uint8_t priority, const SignalSet signalMask,
			const SignalAction signalAction) :
			PrioritizedTestCase{priority},
			signalAction_{signalAction},
			signalMask_{signalMask}
	{

	}

protected:

	/**
	 * \brief Finalizes the test case.
	 *
	 * Restores empty signal mask, restores default signal actions for all signal numbers and calls
	 * PrioritizedTestCase::finalize().
	 *
	 * \return true if finalization succeeded, false otherwise
	 */

	bool finalize() const override;

	/**
	 * \brief Initializes the test case.
	 *
	 * Calls PrioritizedTestCase::initialize(), sets configured signal action for all signal numbers and sets configured
	 * signal mask.
	 *
	 * \return true if initialization succeeded, false otherwise
	 */

	bool initialize() const override;

private:

	/// SignalAction that will be associated with all signal numbers
	SignalAction signalAction_;

	/// SignalSet with signal mask for test case
	SignalSet signalMask_;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSTESTCASECOMMON_HPP_
