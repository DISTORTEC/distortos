/**
 * \file
 * \brief board::lowLevelInitialization() declaration
 *
 * \author Copyright (C) 2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_BOARD_LOWLEVELINITIALIZATION_HPP_
#define INCLUDE_DISTORTOS_BOARD_LOWLEVELINITIALIZATION_HPP_

namespace distortos
{

namespace board
{

/**
 * \brief Low level board initialization.
 *
 * This function is called before constructors for global and static objects from __libc_init_array() via address in
 * distortosPreinitArray[].
 *
 * \note Use of this function is optional - it may be left undefined, in which case it will not be called.
 */

void lowLevelInitialization() __attribute__ ((weak));

}	// namespace board

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_BOARD_LOWLEVELINITIALIZATION_HPP_
