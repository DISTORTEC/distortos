/**
 * \file
 * \brief setFpuRegisters() declaration for ARMv7-M
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_SETFPUREGISTERS_HPP_
#define TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_SETFPUREGISTERS_HPP_

#include <cstdint>

namespace distortos
{

namespace test
{

/**
 * \brief Sets FPU registers (including fpscr) to given value.
 *
 * \param [in] value is the value that will be written to FPU registers
 * \param [in] full selects whether all FPU registers will be modified (true) or only s0-s15 and fpscr (false)
 *
 * \return value read from fpscr register (not all fields of this register are writable)
 */

uint32_t setFpuRegisters(uint32_t value, bool full);

}	// namespace test

}	// namespace distortos

#endif	// TEST_ARCHITECTURE_ARM_ARMV7_M_ARMV7_M_SETFPUREGISTERS_HPP_
