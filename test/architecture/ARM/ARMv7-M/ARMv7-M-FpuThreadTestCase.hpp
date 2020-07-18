/**
 * \file
 * \brief FpuThreadTestCase class header for ARMv7-M
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_FPUTHREADTESTCASE_HPP_
#define TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_FPUTHREADTESTCASE_HPP_

#include "PrioritizedTestCase.hpp"

namespace distortos
{

namespace test
{

/**
 * \brief Tests saving of FPU context between threads.
 *
 * Starts 8 small threads which modify all FPU registers (including fpscr), cause context switch to next test thread
 * and - after return - check whether all modified registers still have the same value. This sequence is repeated
 * predefined number of times (or until first failure).
 */

class FpuThreadTestCase : public PrioritizedTestCase
{
	/// priority at which this test case should be executed
	constexpr static uint8_t testCasePriority_ {UINT8_MAX};

public:

	/**
	 * \brief FpuThreadTestCase's constructor
	 */

	constexpr FpuThreadTestCase() :
			PrioritizedTestCase{testCasePriority_}
	{

	}

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

#endif	// TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_FPUTHREADTESTCASE_HPP_
