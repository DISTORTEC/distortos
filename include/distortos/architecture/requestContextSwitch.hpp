/**
 * \file
 * \brief requestContextSwitch() declaration
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_REQUESTCONTEXTSWITCH_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_REQUESTCONTEXTSWITCH_HPP_

namespace distortos
{

namespace architecture
{

/**
 * \brief Architecture-specific request for context switch.
 *
 * Causes the architecture to do context save, call internal::getScheduler().switchContext() and do context restore. The
 * call to internal::getScheduler().switchContext() must be done from the context in which such call is valid (usually
 * system interrupt).
 */

void requestContextSwitch();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_REQUESTCONTEXTSWITCH_HPP_
