/**
 * \file
 * \brief InterruptUnmaskingLock class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-06
 */

#ifndef INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTUNMASKINGLOCK_HPP_
#define INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTUNMASKINGLOCK_HPP_

#include "distortos/architecture/InterruptMaskingUnmaskingLock.hpp"

namespace distortos
{

namespace architecture
{

/// InterruptUnmaskingLock class is a RAII wrapper for disableInterruptMasking() / restoreInterruptMasking()
using InterruptUnmaskingLock = InterruptMaskingUnmaskingLock<disableInterruptMasking>;

}	// namespace architecture

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ARCHITECTURE_INTERRUPTUNMASKINGLOCK_HPP_
