/**
 * \file
 * \brief disableInterruptMasking() declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_DISABLEINTERRUPTMASKING_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_DISABLEINTERRUPTMASKING_HPP_

#include "distortos/architecture/InterruptMask.hpp"

namespace distortos
{

namespace architecture
{

/**
 * \brief Disables interrupt masking.
 *
 * Enables normal-priority interrupts.
 *
 * \return previous value of interrupts' mask, must be used for matched restoreInterruptMasking() call
 */

InterruptMask disableInterruptMasking();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_DISABLEINTERRUPTMASKING_HPP_
