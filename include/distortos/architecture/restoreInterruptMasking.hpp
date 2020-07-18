/**
 * \file
 * \brief restoreInterruptMasking() declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_

#include "distortos/architecture/InterruptMask.hpp"

namespace distortos
{

namespace architecture
{

/**
 * \brief Restores interrupt masking.
 *
 * Restores previous interrupt masking state (before matching enableInterruptMasking() or disableInterruptMasking() was
 * called), enabling some (maybe all, maybe none) interrupts.
 *
 * \param [in] interruptMask is the value of interrupts' mask, must come from previous call to enableInterruptMasking()
 * or disableInterruptMasking()
 */

void restoreInterruptMasking(InterruptMask interruptMask);

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_RESTOREINTERRUPTMASKING_HPP_
