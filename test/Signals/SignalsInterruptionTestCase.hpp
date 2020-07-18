/**
 * \file
 * \brief SignalsInterruptionTestCase class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_SIGNALS_SIGNALSINTERRUPTIONTESTCASE_HPP_
#define TEST_SIGNALS_SIGNALSINTERRUPTIONTESTCASE_HPP_

#include "SignalsTestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests interruption of various "blocking" functions by an unmasked, caught signal.
 *
 * Functions like ConditionVariable::wait(), Mutex::lock(), Semaphore::wait(), ThisThread::sleepFor(),
 * ThisThread::Signals::wait() and Thread::join() (including the variants with timeout) are tested.
 */

class SignalsInterruptionTestCase : public SignalsTestCaseCommon
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX / 2};

public:

	/**
	 * \return priority at which this test case should be executed
	 */

	constexpr static uint8_t getTestCasePriority()
	{
		return testCasePriority_;
	}

	/**
	 * \brief SignalsInterruptionTestCase's constructor
	 */

	constexpr SignalsInterruptionTestCase() :
			SignalsTestCaseCommon{testCasePriority_, SignalSet{SignalSet::empty},
					{signalHandler, SignalSet{SignalSet::empty}}}
	{

	}

private:

	/**
	 * \brief Signal handler
	 *
	 * Marks sequence point (signal number) in shared SequenceAsserter (address from sigval).
	 *
	 * \param [in] signalInformation is a reference to received SignalInformation object
	 */

	static void signalHandler(const SignalInformation& signalInformation);

	/**
	 * \brief Runs the test case.
	 *
	 * \return true if the test case succeeded, false otherwise
	 */

	bool run_() const override;
};

}	// namespace test

}	// namespace distortos

#endif	// TEST_SIGNALS_SIGNALSINTERRUPTIONTESTCASE_HPP_
