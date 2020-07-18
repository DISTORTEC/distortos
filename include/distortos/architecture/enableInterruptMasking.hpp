/**
 * \file
 * \brief enableInterruptMasking() declaration
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_

#include "distortos/architecture/InterruptMask.hpp"

namespace distortos
{

namespace architecture
{

/**
 * \brief Enables interrupt masking.
 *
 * Disables normal-priority interrupts.
 *
 * \note High-priority interrupts are not controlled by distortos, so they may be left enabled. Support for that feature
 * is architecture-dependent.
 *
 * \return previous value of interrupts' mask, must be used for matched restoreInterruptMasking() call
 */

InterruptMask enableInterruptMasking();

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_ENABLEINTERRUPTMASKING_HPP_
