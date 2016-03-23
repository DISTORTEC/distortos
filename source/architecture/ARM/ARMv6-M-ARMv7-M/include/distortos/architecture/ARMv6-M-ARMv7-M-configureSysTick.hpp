/**
 * \file
 * \brief configureSysTick() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_ARMV6_M_ARMV7_M_CONFIGURESYSTICK_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_ARMV6_M_ARMV7_M_CONFIGURESYSTICK_HPP_

#include <cstdint>

namespace distortos
{

namespace architecture
{

/// max possible SysTick's period
constexpr uint32_t maxSysTickPeriod {1 << 24};

/**
 * \brief Configures SysTick timer in ARMv6-M and ARMv7-M.
 *
 * \param [in] period is period of SysTick timer, [1; maxSysTickPeriod]
 * \param [in] divideBy8 selects whether the SysTick timer will use core clock "directly" (false) or divided by 8 (true)
 */

void configureSysTick(uint32_t period, bool divideBy8);

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_INCLUDE_DISTORTOS_ARCHITECTURE_ARMV6_M_ARMV7_M_CONFIGURESYSTICK_HPP_
