/**
 * \file
 * \brief setFpuRegisters() declaration for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-05-30
 */

#ifndef TEST_ARCHITECTURE_ARM_ARMV7_M_SETFPUREGISTERS_HPP_
#define TEST_ARCHITECTURE_ARM_ARMV7_M_SETFPUREGISTERS_HPP_

#include <cstdint>

namespace distortos
{

namespace test
{

/**
 * \brief Sets all FPU registers (including FPSCR) to given value.
 *
 * \param [in] value is the value that will be written to FPU registers
 *
 * \return value read from FPSCR register (not all fields of this register are writable)
 */

uint32_t setFpuRegisters(uint32_t value);

}	// namespace test

}	// namespace distortos

#endif	// TEST_ARCHITECTURE_ARM_ARMV7_M_SETFPUREGISTERS_HPP_
