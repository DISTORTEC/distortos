/**
 * \file
 * \deprecated scheduled to be removed after v0.6.0, use `BIND_LOW_LEVEL_INITIALIZER(60, lowLevelInitialization)`
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_BOARD_LOWLEVELINITIALIZATION_HPP_
#define INCLUDE_DISTORTOS_BOARD_LOWLEVELINITIALIZATION_HPP_

#warning "'distortos/board/lowLevelInitialization.hpp' is not working anymore: \
Use 'BIND_LOW_LEVEL_INITIALIZER(60, lowLevelInitialization);' to have your function executed before constructors for \
global and static objects"

#endif	// INCLUDE_DISTORTOS_BOARD_LOWLEVELINITIALIZATION_HPP_
