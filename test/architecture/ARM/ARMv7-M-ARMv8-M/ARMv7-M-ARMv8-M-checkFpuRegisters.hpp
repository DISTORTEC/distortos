/**
 * \file
 * \brief checkFpuRegisters() declaration for ARMv7-M
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV8_M_ARMV7_M_ARMV8_M_CHECKFPUREGISTERS_HPP_
#define TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV8_M_ARMV7_M_ARMV8_M_CHECKFPUREGISTERS_HPP_

#include <cstdint>

namespace distortos
{

namespace test
{

/**
 * \brief Checks whether all FPU registers contain given value.
 *
 * \param [in] value is the expected value for s0-s31 registers
 * \param [in] fpscr is the expected value for fpscr register
 *
 * \return true if values of all FPU registers are equal to expected values, false otherwise
 */

bool checkFpuRegisters(uint32_t value, uint32_t fpscr);

}	// namespace test

}	// namespace distortos

#endif	// TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV8_M_ARMV7_M_ARMV8_M_CHECKFPUREGISTERS_HPP_
