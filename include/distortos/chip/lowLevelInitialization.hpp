/**
 * \file
 * \brief chip::lowLevelInitialization() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_CHIP_LOWLEVELINITIALIZATION_HPP_
#define INCLUDE_DISTORTOS_CHIP_LOWLEVELINITIALIZATION_HPP_

namespace distortos
{

namespace chip
{

/**
 * \brief Low level chip initialization.
 *
 * This function is called before constructors for global and static objects from __libc_init_array() via address in
 * distortosPreinitArray[].
 */

void lowLevelInitialization();

}	// namespace chip

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_CHIP_LOWLEVELINITIALIZATION_HPP_
