/**
 * \file
 * \brief supervisorCall() declaration
 *
 * \author Copyright (C) 2015-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV6_M_ARMV7_M_ARMV8_M_SUPERVISORCALL_HPP_
#define SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV6_M_ARMV7_M_ARMV8_M_SUPERVISORCALL_HPP_

namespace distortos
{

namespace architecture
{

/**
 * \brief Supervisor call.
 *
 * Requests execution of \a function with arguments from SVC_Handler().
 *
 * \param [in] function is a reference to function that will be executed
 * \param [in] argument1 is the first argument for \a function
 * \param [in] argument2 is the second argument for \a function
 * \param [in] argument3 is the third argument for \a function
 * \param [in] argument4 is the fourth argument for \a function
 *
 * \return value returned by \a function
 */

int supervisorCall(int (& function)(int, int, int, int), int argument1, int argument2, int argument3, int argument4);

}	// namespace architecture

}	// namespace distortos

#endif	// SOURCE_ARCHITECTURE_ARM_ARMV6_M_ARMV7_M_ARMV8_M_ARMV6_M_ARMV7_M_ARMV8_M_SUPERVISORCALL_HPP_
