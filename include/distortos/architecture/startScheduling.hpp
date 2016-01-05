/**
 * \file
 * \brief startScheduling() declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_STARTSCHEDULING_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_STARTSCHEDULING_HPP_

namespace distortos
{

namespace architecture
{

/**
 * \brief Architecture-specific start of scheduling.
 *
 * Initializes all required hardware/software to perform context switching and starts the scheduling.
 */

void startScheduling();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_STARTSCHEDULING_HPP_
