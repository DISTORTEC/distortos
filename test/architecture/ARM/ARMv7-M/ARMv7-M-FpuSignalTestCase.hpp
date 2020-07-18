/**
 * \file
 * \brief FpuSignalTestCase class header for ARMv7-M
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_FPUSIGNALTESTCASE_HPP_
#define TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_FPUSIGNALTESTCASE_HPP_

#include "Signals/SignalsTestCaseCommon.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests saving of FPU context between thread, signal sender (interrupt or another thread) and signal handler.
 *
 * Various combinations of FPU usage (in main thread only, in signal sender (interrupt or another thread) only, in
 * signal handler only and all possible combinations) are tested.
 */

class FpuSignalTestCase : public SignalsTestCaseCommon
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
	 * \brief SignalsCatchingTestCase's constructor
	 */

	constexpr FpuSignalTestCase() :
			SignalsTestCaseCommon{testCasePriority_, SignalSet{SignalSet::empty}, {}}
	{

	}

protected:

	/**
	 * \brief Finalizes the test case.
	 *
	 * Disables FPU context for main thread and calls SignalsTestCaseCommon::finalize().
	 *
	 * \return true if finalization succeeded, false otherwise
	 */

	bool finalize() const override;

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

#endif	// TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_FPUSIGNALTESTCASE_HPP_
